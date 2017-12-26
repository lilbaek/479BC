#include "FigureAction_private.h"

#include "Data/CityInfo.h"

#include "core/calc.h"
#include "figure/formation.h"
#include "figure/properties.h"
#include "figure/route.h"
#include "figuretype/missile.h"
#include "map/figure.h"
#include "map/grid.h"

static const struct {
	int x;
	int y;
} soldierAlternativePoints[] = {{-1, -6},
	{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1},
	{0, -2}, {1, -2}, {2, -2}, {2, -1}, {2, 0}, {2, 1}, {2, 2}, {1, 2},
	{0, 2}, {-1, 2}, {-2, 2}, {-2, 1}, {-2, 0}, {-2, -1}, {-2, -2}, {-1, -2},
	{0, -3}, {1, -3}, {2, -3}, {3, -3}, {3, -2}, {3, -1}, {3, 0}, {3, 1},
	{3, 2}, {3, 3}, {2, 3}, {1, 3}, {0, 3}, {-1, 3}, {-2, 3}, {-3, 3},
	{-3, 2}, {-3, 1}, {-3, 0}, {-3, -1}, {-3, -2}, {-3, -3}, {-2, -3}, {-1, -3},
	{0, -4}, {1, -4}, {2, -4}, {3, -4}, {4, -4}, {4, -3}, {4, -2}, {4, -1},
	{4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {3, 4}, {2, 4}, {1, 4},
	{0, 4}, {-1, 4}, {-2, 4}, {-3, 4}, {-4, 4}, {-4, 3}, {-4, 2}, {-4, 1},
	{-4, 0}, {-4, -1}, {-4, -2}, {-4, -3}, {-4, -4}, {-3, -4}, {-2, -4}, {-1, -4},
	{0, -5}, {1, -5}, {2, -5}, {3, -5}, {4, -5}, {5, -5}, {5, -4}, {5, -3},
	{5, -2}, {5, -1}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5},
	{4, 5}, {3, 5}, {2, 5}, {1, 5}, {0, 5}, {-1, 5}, {-2, 5}, {-3, 5},
	{-4, 5}, {-5, 5}, {-5, 4}, {-5, 3}, {-5, 2}, {-5, 1}, {-5, 0}, {-5, -1},
	{-5, -2}, {-5, -3}, {-5, -4}, {-5, -5}, {-4, -5}, {-3, -5}, {-2, -5}, {-1, -5},
	{0, -6}, {1, -6}, {2, -6}, {3, -6}, {4, -6}, {5, -6}, {6, -6}, {6, -5},
	{6, -4}, {6, -3}, {6, -2}, {6, -1}, {6, 0}, {6, 1}, {6, 2}, {6, 3},
	{6, 4}, {6, 5}, {6, 6}, {5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6},
	{0, 6}, {-1, 6}, {-2, 6}, {-3, 6}, {-4, 6}, {-5, 6}, {-6, 6}, {-6, 5},
	{-6, 4}, {-6, 3}, {-6, 2}, {-6, 1}, {-6, 0}, {-6, -1}, {-6, -2}, {-6, -3},
	{-6, -4}, {-6, -5}, {-6, -6}, {-5, -6}, {-4, -6}, {-3, -6}, {-2, -6}, {-1, -6},
};

void FigureAction_militaryStandard(figure *f)
{
	const formation *m = formation_get(f->formationId);

	f->terrainUsage = FigureTerrainUsage_Any;
	FigureActionIncreaseGraphicOffset(f, 16);
	map_figure_delete(f);
	if (m->is_at_fort) {
		f->x = m->x;
		f->y = m->y;
	} else {
		f->x = m->standard_x;
		f->y = m->standard_y;
	}
	f->gridOffset = map_grid_offset(f->x, f->y);
	f->crossCountryX = 15 * f->x + 7;
	f->crossCountryY = 15 * f->y + 7;
	map_figure_add(f);

	f->graphicId = image_group(GROUP_FIGURE_FORT_STANDARD_POLE) + 20 - m->morale / 5;
	if (m->figure_type == FIGURE_FORT_LEGIONARY) {
		if (m->is_halted) {
			f->cartGraphicId = image_group(GROUP_FIGURE_FORT_FLAGS) + 8;
		} else {
			f->cartGraphicId = image_group(GROUP_FIGURE_FORT_FLAGS) + f->graphicOffset / 2;
		}
	} else if (m->figure_type == FIGURE_FORT_MOUNTED) {
		if (m->is_halted) {
			f->cartGraphicId = image_group(GROUP_FIGURE_FORT_FLAGS) + 26;
		} else {
			f->cartGraphicId = image_group(GROUP_FIGURE_FORT_FLAGS) + 18 + f->graphicOffset / 2;
		}
	} else {
		if (m->is_halted) {
			f->cartGraphicId = image_group(GROUP_FIGURE_FORT_FLAGS) + 17;
		} else {
			f->cartGraphicId = image_group(GROUP_FIGURE_FORT_FLAGS) + 9 + f->graphicOffset / 2;
		}
	}
}

static void javelinLaunchMissile(figure *f)
{
	int xTile, yTile;
	f->waitTicksMissile++;
	if (f->waitTicksMissile > figure_properties_for_type(f->type)->missile_delay) {
		f->waitTicksMissile = 0;
		if (FigureAction_CombatSoldier_getMissileTarget(f, 10, &xTile, &yTile)) {
			f->attackGraphicOffset = 1;
			f->direction = calc_missile_shooter_direction(f->x, f->y, xTile, yTile);
		} else {
			f->attackGraphicOffset = 0;
		}
	}
	if (f->attackGraphicOffset) {
		if (f->attackGraphicOffset == 1) {
			figure_create_missile(f->id, f->x, f->y, xTile, yTile, FIGURE_JAVELIN);
            formation_record_missile_fired(f->formationId);
		}
		f->attackGraphicOffset++;
		if (f->attackGraphicOffset > 100) {
			f->attackGraphicOffset = 0;
		}
	}
}

static void legionaryAttackAdjacentEnemy(figure *f)
{
	int gridOffset = f->gridOffset;
	for (int i = 0; i < 8 && f->actionState != FigureActionState_150_Attack; i++) {
		FigureAction_Combat_attackFigureAt(f,
			gridOffset + map_grid_direction_delta(i));
	}
}

static int soldierFindMopUpTarget(figure *f)
{
	int targetId = f->targetFigureId;
	if (figure_is_dead(figure_get(targetId))) {
		f->targetFigureId = 0;
		targetId = 0;
	}
	if (targetId <= 0) {
		targetId = FigureAction_CombatSoldier_getTarget(f->x, f->y, 20);
		if (targetId) {
			figure *target = figure_get(targetId);
			f->destinationX = target->x;
			f->destinationY = target->y;
			f->targetFigureId = targetId;
			target->targetedByFigureId = f->id;
			f->targetFigureCreatedSequence = target->createdSequence;
		} else {
			f->actionState = FigureActionState_84_SoldierAtStandard;
			f->graphicOffset = 0;
		}
		figure_route_remove(f);
	}
	return targetId;
}

static void updateSoldierGraphicJavelin(figure *f, int dir)
{
	int graphicId = image_group(GROUP_BUILDING_FORT_JAVELIN);
	if (f->actionState == FigureActionState_150_Attack) {
		if (f->attackGraphicOffset < 12) {
			f->graphicId = graphicId + 96 + dir;
		} else {
			f->graphicId = graphicId + 96 + dir + 8 * ((f->attackGraphicOffset - 12) / 2);
		}
	} else if (f->actionState == FigureActionState_149_Corpse) {
		f->graphicId = graphicId + 144 + FigureActionCorpseGraphicOffset(f);
	} else if (f->actionState == FigureActionState_84_SoldierAtStandard) {
		f->graphicId = graphicId + 96 + dir +
			8 * FigureActionMissileLauncherGraphicOffset(f);
	} else {
		f->graphicId = graphicId + dir + 8 * f->graphicOffset;
	}
}

static void updateSoldierGraphicMounted(figure *f, int dir)
{
	int graphicId = image_group(GROUP_FIGURE_FORT_MOUNTED);
	if (f->actionState == FigureActionState_150_Attack) {
		if (f->attackGraphicOffset < 12) {
			f->graphicId = graphicId + 96 + dir;
		} else {
			f->graphicId = graphicId + 96 + dir + 8 * ((f->attackGraphicOffset - 12) / 2);
		}
	} else if (f->actionState == FigureActionState_149_Corpse) {
		f->graphicId = graphicId + 144 + FigureActionCorpseGraphicOffset(f);
	} else {
		f->graphicId = graphicId + dir + 8 * f->graphicOffset;
	}
}

static void updateSoldierGraphicLegionary(figure *f, const formation *m, int dir)
{
	int graphicId = image_group(GROUP_BUILDING_FORT_LEGIONARY);
	if (f->actionState == FigureActionState_150_Attack) {
		if (f->attackGraphicOffset < 12) {
			f->graphicId = graphicId + 96 + dir;
		} else {
			f->graphicId = graphicId + 96 + dir + 8 * ((f->attackGraphicOffset - 12) / 2);
		}
	} else if (f->actionState == FigureActionState_149_Corpse) {
		f->graphicId = graphicId + 152 + FigureActionCorpseGraphicOffset(f);
	} else if (f->actionState == FigureActionState_84_SoldierAtStandard) {
		if (m->is_halted && m->layout == FORMATION_COLUMN && m->missile_attack_timeout) {
			f->graphicId = graphicId + dir + 144;
		} else {
			f->graphicId = graphicId + dir;
		}
	} else {
		f->graphicId = graphicId + dir + 8 * f->graphicOffset;
	}
}

static void updateSoldierGraphic(figure *f, const formation *m)
{
	int dir;
	if (f->actionState == FigureActionState_150_Attack) {
		dir = f->attackDirection;
	} else if (m->missile_fired) {
		dir = f->direction;
	} else if (f->actionState == FigureActionState_84_SoldierAtStandard) {
		dir = m->direction;
	} else if (f->direction < 8) {
		dir = f->direction;
	} else {
		dir = f->previousTileDirection;
	}
	FigureActionNormalizeDirection(dir);
	if (f->type == FIGURE_FORT_JAVELIN) {
		updateSoldierGraphicJavelin(f, dir);
	} else if (f->type == FIGURE_FORT_MOUNTED) {
		updateSoldierGraphicMounted(f, dir);
	} else if (f->type == FIGURE_FORT_LEGIONARY) {
		updateSoldierGraphicLegionary(f, m, dir);
	}
}

void FigureAction_soldier(figure *f)
{
	const formation *m = formation_get(f->formationId);
	Data_CityInfo.numSoldiersInCity++;
	f->terrainUsage = FigureTerrainUsage_Any;
	FigureActionIncreaseGraphicOffset(f, 12);
	f->cartGraphicId = 0;
	if (m->in_use != 1) {
		f->actionState = FigureActionState_149_Corpse;
	}
	int speedFactor;
	if (f->type == FIGURE_FORT_MOUNTED) {
		speedFactor = 3;
	} else if (f->type == FIGURE_FORT_JAVELIN) {
		speedFactor = 2;
	} else {
		speedFactor = 1;
	}
	int layout = m->layout;
	if (f->formationAtRest || f->actionState == FigureActionState_81_SoldierGoingToFort) {
		layout = FORMATION_AT_REST;
	}
	f->formationPositionX = m->x + FigureActionFormationLayoutPositionX(layout, f->indexInFormation);
	f->formationPositionY = m->y + FigureActionFormationLayoutPositionY(layout, f->indexInFormation);
	
	switch (f->actionState) {
		case FigureActionState_150_Attack:
			FigureAction_Common_handleAttack(f);
			break;
		case FigureActionState_149_Corpse:
			FigureAction_Common_handleCorpse(f);
			break;
		case FigureActionState_80_SoldierAtRest:
			map_figure_update(f);
			f->waitTicks = 0;
			f->formationAtRest = 1;
			f->graphicOffset = 0;
			if (f->x != f->formationPositionX || f->y != f->formationPositionY) {
				f->actionState = FigureActionState_81_SoldierGoingToFort;
			}
			break;
		case FigureActionState_81_SoldierGoingToFort:
		case FigureActionState_148_Fleeing:
			f->waitTicks = 0;
			f->formationAtRest = 1;
			f->destinationX = f->formationPositionX;
			f->destinationY = f->formationPositionY;
			f->destinationGridOffsetSoldier = map_grid_offset(f->destinationX, f->destinationY);
			FigureMovement_walkTicks(f, speedFactor);
			if (f->direction == DIR_FIGURE_AT_DESTINATION) {
				f->actionState = FigureActionState_80_SoldierAtRest;
			} else if (f->direction == DIR_FIGURE_REROUTE) {
				figure_route_remove(f);
			} else if (f->direction == DIR_FIGURE_LOST) {
				f->state = FigureState_Dead;
			}
			break;
		case FigureActionState_82_SoldierReturningToBarracks:
			f->formationAtRest = 1;
			f->destinationX = f->sourceX;
			f->destinationY = f->sourceY;
			FigureMovement_walkTicks(f, speedFactor);
			if (f->direction == DIR_FIGURE_AT_DESTINATION || f->direction == DIR_FIGURE_LOST) {
				f->state = FigureState_Dead;
			} else if (f->direction == DIR_FIGURE_REROUTE) {
				figure_route_remove(f);
			}
			break;
		case FigureActionState_83_SoldierGoingToStandard:
			f->formationAtRest = 0;
			f->destinationX = m->standard_x + FigureActionFormationLayoutPositionX(m->layout, f->indexInFormation);
			f->destinationY = m->standard_y + FigureActionFormationLayoutPositionY(m->layout, f->indexInFormation);
			if (f->alternativeLocationIndex) {
				f->destinationX += soldierAlternativePoints[f->alternativeLocationIndex].x;
				f->destinationY += soldierAlternativePoints[f->alternativeLocationIndex].y;
			}
			f->destinationGridOffsetSoldier = map_grid_offset(f->destinationX, f->destinationY);
			FigureMovement_walkTicks(f, speedFactor);
			if (f->direction == DIR_FIGURE_AT_DESTINATION) {
				f->actionState = FigureActionState_84_SoldierAtStandard;
				f->graphicOffset = 0;
			} else if (f->direction == DIR_FIGURE_REROUTE) {
				figure_route_remove(f);
			} else if (f->direction == DIR_FIGURE_LOST) {
				f->alternativeLocationIndex++;
				if (f->alternativeLocationIndex > 168) {
					f->state = FigureState_Dead;
				}
				f->graphicOffset = 0;
			}
			break;
		case FigureActionState_84_SoldierAtStandard:
			f->formationAtRest = 0;
			f->graphicOffset = 0;
			map_figure_update(f);
			f->destinationX = m->standard_x + FigureActionFormationLayoutPositionX(m->layout, f->indexInFormation);
			f->destinationY = m->standard_y + FigureActionFormationLayoutPositionY(m->layout, f->indexInFormation);
			if (f->alternativeLocationIndex) {
				f->destinationX += soldierAlternativePoints[f->alternativeLocationIndex].x;
				f->destinationY += soldierAlternativePoints[f->alternativeLocationIndex].y;
			}
			if (f->x != f->destinationX || f->y != f->destinationY) {
				if (m->missile_fired <= 0 && m->recent_fight <= 0 && m->missile_attack_timeout <= 0) {
					f->actionState = FigureActionState_83_SoldierGoingToStandard;
					f->alternativeLocationIndex = 0;
				}
			}
			if (f->actionState != FigureActionState_83_SoldierGoingToStandard) {
				if (f->type == FIGURE_FORT_JAVELIN) {
					javelinLaunchMissile(f);
				} else if (f->type == FIGURE_FORT_LEGIONARY) {
					legionaryAttackAdjacentEnemy(f);
				}
			}
			break;
		case FigureActionState_85_SoldierGoingToMilitaryAcademy:
            formation_legion_set_trained(m->id);
			f->formationAtRest = 1;
			FigureMovement_walkTicks(f, speedFactor);
			if (f->direction == DIR_FIGURE_AT_DESTINATION) {
				f->actionState = FigureActionState_81_SoldierGoingToFort;
			} else if (f->direction == DIR_FIGURE_REROUTE) {
				figure_route_remove(f);
			} else if (f->direction == DIR_FIGURE_LOST) {
				f->state = FigureState_Dead;
			}
			break;
		case FigureActionState_86_SoldierMoppingUp:
			f->formationAtRest = 0;
			if (soldierFindMopUpTarget(f)) {
				FigureMovement_walkTicks(f, speedFactor);
				if (f->direction == DIR_FIGURE_AT_DESTINATION) {
                    figure *target = figure_get(f->targetFigureId);
					f->destinationX = target->x;
					f->destinationY = target->y;
					figure_route_remove(f);
				} else if (f->direction == DIR_FIGURE_REROUTE || f->direction == DIR_FIGURE_LOST) {
					f->actionState = FigureActionState_84_SoldierAtStandard;
					f->targetFigureId = 0;
					f->graphicOffset = 0;
				}
			}
			break;
		case FigureActionState_87_SoldierGoingToDistantBattle:
			f->formationAtRest = 0;
			f->destinationX = Data_CityInfo.exitPointX;
			f->destinationY = Data_CityInfo.exitPointY;
			FigureMovement_walkTicks(f, speedFactor);
			if (f->direction == DIR_FIGURE_AT_DESTINATION) {
				f->actionState = FigureActionState_89_SoldierAtDistantBattle;
				figure_route_remove(f);
			} else if (f->direction == DIR_FIGURE_REROUTE) {
				figure_route_remove(f);
			} else if (f->direction == DIR_FIGURE_LOST) {
				f->state = FigureState_Dead;
			}
			break;
		case FigureActionState_88_SoldierReturningFromDistantBattle:
			f->isGhost = 0;
			f->waitTicks = 0;
			f->formationAtRest = 1;
			f->destinationX = f->formationPositionX;
			f->destinationY = f->formationPositionY;
			f->destinationGridOffsetSoldier = map_grid_offset(f->destinationX, f->destinationY);
			FigureMovement_walkTicks(f, speedFactor);
			if (f->direction == DIR_FIGURE_AT_DESTINATION) {
				f->actionState = FigureActionState_80_SoldierAtRest;
			} else if (f->direction == DIR_FIGURE_REROUTE) {
				figure_route_remove(f);
			} else if (f->direction == DIR_FIGURE_LOST) {
				f->state = FigureState_Dead;
			}
			break;
		case FigureActionState_89_SoldierAtDistantBattle:
			f->isGhost = 1;
			f->formationAtRest = 1;
			break;
	}
	
	updateSoldierGraphic(f, m);
}
