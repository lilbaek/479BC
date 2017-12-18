#include "FigureAction_private.h"

#include "BuildingHouse.h"
#include "CityInfo.h"
#include "HousePopulation.h"
#include "Terrain.h"

#include "Data/CityInfo.h"

#include "building/building.h"
#include "building/model.h"
#include "figure/route.h"

static void updateDirectionAndGraphic(figure *f)
{
	int dir = FigureActionDirection(f);
	if (f->actionState == FigureActionState_149_Corpse) {
		f->graphicId = image_group(GROUP_FIGURE_MIGRANT) +
			FigureActionCorpseGraphicOffset(f) + 96;
	} else {
		f->graphicId = image_group(GROUP_FIGURE_MIGRANT) +
			dir + 8 * f->graphicOffset;
	}
	if (f->actionState == FigureActionState_2_ImmigrantArriving ||
        f->actionState == FigureActionState_6_EmigrantLeaving) {
		f->cartGraphicId = image_group(GROUP_FIGURE_MIGRANT_CART) + dir;
		int cartDir = (dir + 4) % 8;
		FigureAction_Common_setCartOffset(f, cartDir);
	}
}

void FigureAction_immigrant(figure *f)
{
	building *b = building_get(f->immigrantBuildingId);
	
	f->terrainUsage = FigureTerrainUsage_Any;
	f->cartGraphicId = 0;
	if (!BuildingIsInUse(b) || b->immigrantFigureId != f->id || !b->houseSize) {
		f->state = FigureState_Dead;
		return;
	}
	
	FigureActionIncreaseGraphicOffset(f, 12);
	
	switch (f->actionState) {
		case FigureActionState_150_Attack:
			FigureAction_Common_handleAttack(f);
			break;
		case FigureActionState_149_Corpse:
			FigureAction_Common_handleCorpse(f);
			break;
		case FigureActionState_1_ImmigrantCreated:
			f->isGhost = 1;
			f->graphicOffset = 0;
			f->waitTicks--;
			if (f->waitTicks <= 0) {
				int xRoad, yRoad;
				if (Terrain_getClosestRoadWithinRadius(b->x, b->y, b->size, 2, &xRoad, &yRoad)) {
					f->actionState = FigureActionState_2_ImmigrantArriving;
					f->destinationX = xRoad;
					f->destinationY = yRoad;
					f->roamLength = 0;
				} else {
					f->state = FigureState_Dead;
				}
			}
			break;
		case FigureActionState_2_ImmigrantArriving:
			f->isGhost = 0;
			FigureMovement_walkTicks(f, 1);
			switch (f->direction) {
				case DIR_FIGURE_AT_DESTINATION:
					f->actionState = FigureActionState_3_ImmigrantEnteringHouse;
					FigureAction_Common_setCrossCountryDestination(f, b->x, b->y);
					f->roamLength = 0;
					break;
				case DIR_FIGURE_REROUTE:
					figure_route_remove(f);
					break;
				case DIR_FIGURE_LOST:
					b->immigrantFigureId = 0;
					b->distanceFromEntry = 0;
					f->state = FigureState_Dead;
					break;
			}
			break;
		case FigureActionState_3_ImmigrantEnteringHouse:
			f->useCrossCountry = 1;
			f->isGhost = 1;
			if (FigureMovement_crossCountryWalkTicks(f, 1) == 1) {
				f->state = FigureState_Dead;
				int maxPeople = model_get_house(b->subtype.houseLevel)->max_people;
				if (b->houseIsMerged) {
					maxPeople *= 4;
				}
				int room = maxPeople - b->housePopulation;
				if (room < 0) {
					room = 0;
				}
				if (room < f->migrantNumPeople) {
					f->migrantNumPeople = room;
				}
				if (!b->housePopulation) {
					BuildingHouse_changeTo(b, BUILDING_HOUSE_SMALL_TENT);
				}
				b->housePopulation += f->migrantNumPeople;
				b->housePopulationRoom = maxPeople - b->housePopulation;
				CityInfo_Population_addPeople(f->migrantNumPeople);
				b->immigrantFigureId = 0;
			}
			f->isGhost = f->inBuildingWaitTicks ? 1 : 0;
			break;
	}
	
	updateDirectionAndGraphic(f);
}

void FigureAction_emigrant(figure *f)
{
	f->terrainUsage = FigureTerrainUsage_Any;
	f->cartGraphicId = 0;
	
	FigureActionIncreaseGraphicOffset(f, 12);
	
	switch (f->actionState) {
		case FigureActionState_150_Attack:
			FigureAction_Common_handleAttack(f);
			break;
		case FigureActionState_149_Corpse:
			FigureAction_Common_handleCorpse(f);
			break;
		case FigureActionState_4_EmigrantCreated:
			f->isGhost = 1;
			f->graphicOffset = 0;
			f->waitTicks++;
			if (f->waitTicks >= 5) {
				int xRoad, yRoad;
				if (!Terrain_getClosestRoadWithinRadius(f->x, f->y, 1, 5, &xRoad, &yRoad)) {
					f->state = FigureState_Dead;
				}
				f->actionState = FigureActionState_5_EmigrantExitingHouse;
				FigureAction_Common_setCrossCountryDestination(f, xRoad, yRoad);
				f->roamLength = 0;
			}
			break;
		case FigureActionState_5_EmigrantExitingHouse:
			f->useCrossCountry = 1;
			f->isGhost = 1;
			if (FigureMovement_crossCountryWalkTicks(f, 1) == 1) {
				f->actionState = FigureActionState_6_EmigrantLeaving;
				f->destinationX = Data_CityInfo.entryPointX;
				f->destinationY = Data_CityInfo.entryPointY;
				f->roamLength = 0;
				f->progressOnTile = 15;
			}
			f->isGhost = f->inBuildingWaitTicks ? 1 : 0;
			break;
		case FigureActionState_6_EmigrantLeaving:
			f->useCrossCountry = 0;
			f->isGhost = 0;
			FigureMovement_walkTicks(f, 1);
			if (f->direction == DIR_FIGURE_AT_DESTINATION ||
				f->direction == DIR_FIGURE_REROUTE ||
				f->direction == DIR_FIGURE_LOST) {
				f->state = FigureState_Dead;
			}
			break;
	}
	updateDirectionAndGraphic(f);
}

void FigureAction_homeless(figure *f)
{
	FigureActionIncreaseGraphicOffset(f, 12);
	f->terrainUsage = FigureTerrainUsage_PreferRoads;
	
	switch (f->actionState) {
		case FigureActionState_150_Attack:
			FigureAction_Common_handleAttack(f);
			break;
		case FigureActionState_149_Corpse:
			FigureAction_Common_handleCorpse(f);
			break;
		case FigureActionState_7_HomelessCreated:
			f->graphicOffset = 0;
			f->waitTicks++;
			if (f->waitTicks > 51) {
				int buildingId = HousePopulation_getClosestHouseWithRoom(f->x, f->y);
				if (buildingId) {
					building *b = building_get(buildingId);
					int xRoad, yRoad;
					if (Terrain_getClosestRoadWithinRadius(b->x, b->y, b->size, 2, &xRoad, &yRoad)) {
						b->immigrantFigureId = f->id;
						f->immigrantBuildingId = buildingId;
						f->actionState = FigureActionState_8_HomelessGoingToHouse;
						f->destinationX = xRoad;
						f->destinationY = yRoad;
						f->roamLength = 0;
					} else {
						f->state = FigureState_Dead;
					}
				} else {
					f->actionState = FigureActionState_10_HomelessLeaving;
					f->destinationX = Data_CityInfo.exitPointX;
					f->destinationY = Data_CityInfo.exitPointY;
					f->roamLength = 0;
					f->waitTicks = 0;
				}
			}
			break;
		case FigureActionState_8_HomelessGoingToHouse:
			f->isGhost = 0;
			FigureMovement_walkTicks(f, 1);
			if (f->direction == DIR_FIGURE_REROUTE || f->direction == DIR_FIGURE_LOST) {
				building_get(f->immigrantBuildingId)->immigrantFigureId = 0;
				f->state = FigureState_Dead;
			} else if (f->direction == DIR_FIGURE_AT_DESTINATION) {
                building *b = building_get(f->immigrantBuildingId);
				f->actionState = FigureActionState_9_HomelessEnteringHouse;
				FigureAction_Common_setCrossCountryDestination(f, b->x, b->y);
				f->roamLength = 0;
			}
			break;
		case FigureActionState_9_HomelessEnteringHouse:
			f->useCrossCountry = 1;
			f->isGhost = 1;
			if (FigureMovement_crossCountryWalkTicks(f, 1) == 1) {
				f->state = FigureState_Dead;
				building *b = building_get(f->immigrantBuildingId);
				if (f->immigrantBuildingId && BuildingIsHouse(b->type)) {
					int maxPeople = model_get_house(b->subtype.houseLevel)->max_people;
					if (b->houseIsMerged) {
						maxPeople *= 4;
					}
					int room = maxPeople - b->housePopulation;
					if (room < 0) {
						room = 0;
					}
					if (room < f->migrantNumPeople) {
						f->migrantNumPeople = room;
					}
					if (!b->housePopulation) {
						BuildingHouse_changeTo(b, BUILDING_HOUSE_SMALL_TENT);
					}
					b->housePopulation += f->migrantNumPeople;
					b->housePopulationRoom = maxPeople - b->housePopulation;
					CityInfo_Population_addPeopleHomeless(f->migrantNumPeople);
					b->immigrantFigureId = 0;
				}
			}
			break;
		case FigureActionState_10_HomelessLeaving:
			FigureMovement_walkTicks(f, 1);
			if (f->direction == DIR_FIGURE_AT_DESTINATION || f->direction == DIR_FIGURE_LOST) {
				f->state = FigureState_Dead;
			} else if (f->direction == DIR_FIGURE_REROUTE) {
				figure_route_remove(f);
			}
			f->waitTicks++;
			if (f->waitTicks > 30) {
				f->waitTicks = 0;
				int buildingId = HousePopulation_getClosestHouseWithRoom(f->x, f->y);
				if (buildingId > 0) {
					building *b = building_get(buildingId);
					int xRoad, yRoad;
					if (Terrain_getClosestRoadWithinRadius(b->x, b->y, b->size, 2, &xRoad, &yRoad)) {
						b->immigrantFigureId = f->id;
						f->immigrantBuildingId = buildingId;
						f->actionState = FigureActionState_8_HomelessGoingToHouse;
						f->destinationX = xRoad;
						f->destinationY = yRoad;
						f->roamLength = 0;
						figure_route_remove(f);
					}
				}
			}
			break;
	}
	if (f->actionState == FigureActionState_149_Corpse) {
		f->graphicId = image_group(GROUP_FIGURE_HOMELESS) +
			FigureActionCorpseGraphicOffset(f) + 96;
	} else {
		f->graphicId = image_group(GROUP_FIGURE_HOMELESS) +
			FigureActionDirection(f) + 8 * f->graphicOffset;
	}
}
