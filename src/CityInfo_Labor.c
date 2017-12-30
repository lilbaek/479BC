#include "CityInfo.h"
#include "Data/CityInfo.h"

#include "building/building.h"
#include "building/model.h"
#include "city/constants.h"
#include "city/message.h"
#include "core/calc.h"
#include "game/time.h"
#include "scenario/property.h"

#define MAX_CATS 10

typedef enum {
    LABOR_CATEGORY_INDUSTRY_COMMERCE = 0,
    LABOR_CATEGORY_FOOD_PRODUCTION = 1,
    LABOR_CATEGORY_ENGINEERING = 2,
    LABOR_CATEGORY_WATER = 3,
    LABOR_CATEGORY_PREFECTURES = 4,
    LABOR_CATEGORY_MILITARY = 5,
    LABOR_CATEGORY_ENTERTAINMENT = 6,
    LABOR_CATEGORY_HEALTH_EDUCATION = 7,
    LABOR_CATEGORY_GOVERNANCE_RELIGION = 8
} labor_category;

static int buildingTypeToLaborCategory[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,//0
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,//10
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,//20
	6, 6, 6, 6, 6, 6, 6, 6, -1, -1,//30
	-1, -1, -1, -1, -1, -1, 7, 7, 7, 7,//40
	0, 7, 7, 7, -1, 4, -1, 5, 5, 5,//50
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8,//60
	0, 1, 0, -1, 0, 0, 0, -1, -1, -1,//70
	7, 2, -1, -1, 8, 8, 8, 8, -1, -1,//80
	-1, 3, -1, -1, 5, 5, -1, -1, 8, -1,//90
	1, 1, 1, 0, 0, 1, 0, 0, 0, 0,//100
	0, 0, 0, 0, 0, -1, -1, -1, -1, -1,//110
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1//120
};

static struct {
	labor_category category;
	int workers;
} builtInPriority[MAX_CATS] = {
    {LABOR_CATEGORY_ENGINEERING, 3},
    {LABOR_CATEGORY_WATER, 1},
    {LABOR_CATEGORY_PREFECTURES, 3},
    {LABOR_CATEGORY_MILITARY, 2},
    {LABOR_CATEGORY_FOOD_PRODUCTION, 4},
    {LABOR_CATEGORY_INDUSTRY_COMMERCE, 2},
    {LABOR_CATEGORY_ENTERTAINMENT, 1},
    {LABOR_CATEGORY_HEALTH_EDUCATION, 1},
    {LABOR_CATEGORY_GOVERNANCE_RELIGION, 1},
};

static void setBuildingWorkerWeight();
static void allocateWorkersToWater();
static void allocateWorkersToBuildings();

static int isIndustryDisabled(building *b) {
	if (b->type < BUILDING_WHEAT_FARM || b->type > BUILDING_POTTERY_WORKSHOP) {
		return 0;
	}
	int resourceId = b->outputResourceId;
	if (Data_CityInfo.resourceIndustryMothballed[resourceId]) {
		return 1;
	}
	return 0;
}

static int shouldHaveWorkers(building *b, int category, int checkAccess)
{
	if (category < 0) {
		return 0;
	}

	// exceptions: hippodrome 'other' tiles and disabled industries
	if (category == LABOR_CATEGORY_ENTERTAINMENT) {
		if (b->type == BUILDING_HIPPODROME && b->prevPartBuildingId) {
			return 0;
		}
	} else if (category == LABOR_CATEGORY_FOOD_PRODUCTION || category == LABOR_CATEGORY_INDUSTRY_COMMERCE) {
		if (isIndustryDisabled(b)) {
			return 0;
		}
	}
	// engineering and water are always covered
	if (category == LABOR_CATEGORY_ENGINEERING || category == LABOR_CATEGORY_WATER) {
		return 1;
	}
	if (checkAccess) {
		return b->housesCovered > 0 ? 1 : 0;
	}
	return 1;
}

void CityInfo_Labor_calculateWorkersNeededPerCategory()
{
	for (int cat = 0; cat < MAX_CATS; cat++) {
		Data_CityInfo.laborCategory[cat].buildings = 0;
		Data_CityInfo.laborCategory[cat].totalHousesCovered = 0;
		Data_CityInfo.laborCategory[cat].workersAllocated = 0;
		Data_CityInfo.laborCategory[cat].workersNeeded = 0;
	}
	for (int i = 1; i < MAX_BUILDINGS; i++) {
        building *b = building_get(i);
		if (!BuildingIsInUse(b)) {
			continue;
		}
		int category = buildingTypeToLaborCategory[b->type];
		b->laborCategory = category;
		if (!shouldHaveWorkers(b, category, 1)) {
			continue;
		}
		Data_CityInfo.laborCategory[category].workersNeeded += model_get_building(b->type)->laborers;
		Data_CityInfo.laborCategory[category].totalHousesCovered += b->housesCovered;
		Data_CityInfo.laborCategory[category].buildings++;
	}
}

void CityInfo_Labor_checkEmployment()
{
	int origNeeded = Data_CityInfo.workersNeeded;
	CityInfo_Labor_allocateWorkersToCategories();
	// senate unemployment display is delayed when unemployment is rising
	if (Data_CityInfo.unemploymentPercentage < Data_CityInfo.unemploymentPercentageForSenate) {
		Data_CityInfo.unemploymentPercentageForSenate = Data_CityInfo.unemploymentPercentage;
	} else if (Data_CityInfo.unemploymentPercentage < Data_CityInfo.unemploymentPercentageForSenate + 5) {
		Data_CityInfo.unemploymentPercentageForSenate = Data_CityInfo.unemploymentPercentage;
	} else {
		Data_CityInfo.unemploymentPercentageForSenate += 5;
	}
	if (Data_CityInfo.unemploymentPercentageForSenate > 100) {
		Data_CityInfo.unemploymentPercentageForSenate = 100;
	}

	// workers needed message
	if (!origNeeded && Data_CityInfo.workersNeeded > 0) {
		if (game_time_year() >= scenario_property_start_year()) {
			city_message_post_with_message_delay(MESSAGE_CAT_WORKERS_NEEDED, 0, MESSAGE_WORKERS_NEEDED, 6);
		}
	}
}

void CityInfo_Labor_allocateWorkersToCategories()
{
	int workersNeeded = 0;
	for (int i = 0; i < MAX_CATS; i++) {
		Data_CityInfo.laborCategory[i].workersAllocated = 0;
		workersNeeded += Data_CityInfo.laborCategory[i].workersNeeded;
	}
	Data_CityInfo.workersNeeded = 0;
	if (workersNeeded <= Data_CityInfo.workersAvailable) {
		for (int i = 0; i < MAX_CATS; i++) {
			Data_CityInfo.laborCategory[i].workersAllocated = Data_CityInfo.laborCategory[i].workersNeeded;
		}
		Data_CityInfo.workersEmployed = workersNeeded;
	} else {
		// not enough workers
		int available = Data_CityInfo.workersAvailable;
		// distribute by user-defined priority
		for (int p = 1; p <= 9 && available > 0; p++) {
			for (int c = 0; c < 9; c++) {
				if (p == Data_CityInfo.laborCategory[c].priority) {
					int toAllocate = Data_CityInfo.laborCategory[c].workersNeeded;
					if (toAllocate > available) {
						toAllocate = available;
					}
					Data_CityInfo.laborCategory[c].workersAllocated = toAllocate;
					available -= toAllocate;
					break;
				}
			}
		}
		// (sort of) round-robin distribution over unprioritized categories:
		int guard = 0;
		do {
			guard++;
			if (guard >= Data_CityInfo.workersAvailable) {
				break;
			}
			for (int p = 0; p < 9; p++) {
				int cat = builtInPriority[p].category;
				if (!Data_CityInfo.laborCategory[cat].priority) {
					int needed = Data_CityInfo.laborCategory[cat].workersNeeded - Data_CityInfo.laborCategory[cat].workersAllocated;
					if (needed > 0) {
						int toAllocate = builtInPriority[p].workers;
						if (toAllocate > available) {
							toAllocate = available;
						}
						if (toAllocate > needed) {
							toAllocate = needed;
						}
						Data_CityInfo.laborCategory[cat].workersAllocated += toAllocate;
						available -= toAllocate;
						if (available <= 0) {
							break;
						}
					}
				}
			}
		} while (available > 0);

		Data_CityInfo.workersEmployed = Data_CityInfo.workersAvailable;
		for (int i = 0; i < 9; i++) {
			Data_CityInfo.workersNeeded +=
				Data_CityInfo.laborCategory[i].workersNeeded - Data_CityInfo.laborCategory[i].workersAllocated;
		}
	}
	Data_CityInfo.workersUnemployed = Data_CityInfo.workersAvailable - Data_CityInfo.workersEmployed;
	Data_CityInfo.unemploymentPercentage =
		calc_percentage(Data_CityInfo.workersUnemployed, Data_CityInfo.workersAvailable);
}

void CityInfo_Labor_allocateWorkersToBuildings()
{
	setBuildingWorkerWeight();
	allocateWorkersToWater();
	allocateWorkersToBuildings();
}

static void setBuildingWorkerWeight()
{
	int waterPer10kPerBuilding = calc_percentage(100, Data_CityInfo.laborCategory[LABOR_CATEGORY_WATER].buildings);
	for (int i = 1; i < MAX_BUILDINGS; i++) {
        building *b = building_get(i);
		if (!BuildingIsInUse(b)) {
			continue;
		}
		int cat = buildingTypeToLaborCategory[b->type];
		if (cat == LABOR_CATEGORY_WATER) {
			b->percentageHousesCovered = waterPer10kPerBuilding;
		} else if (cat >= 0) {
			b->percentageHousesCovered = 0;
			if (b->housesCovered) {
				b->percentageHousesCovered =
					calc_percentage(100 * b->housesCovered,
						Data_CityInfo.laborCategory[cat].totalHousesCovered);
			}
		}
	}
}

static void allocateWorkersToBuildings()
{
	int categoryWorkersNeeded[MAX_CATS];
	int categoryWorkersAllocated[MAX_CATS];
	for (int i = 0; i < MAX_CATS; i++) {
		categoryWorkersAllocated[i] = 0;
		categoryWorkersNeeded[i] =
			Data_CityInfo.laborCategory[i].workersAllocated < Data_CityInfo.laborCategory[i].workersNeeded
			? 1 : 0;
	}
	for (int i = 1; i < MAX_BUILDINGS; i++) {
        building *b = building_get(i);
		if (!BuildingIsInUse(b)) {
			continue;
		}
		int cat = buildingTypeToLaborCategory[b->type];
		if (cat == LABOR_CATEGORY_WATER || cat < 0) {
			// water is handled by allocateWorkersToWater()
			continue;
		}
		b->numWorkers = 0;
		if (!shouldHaveWorkers(b, cat, 0)) {
			continue;
		}
		if (b->percentageHousesCovered > 0) {
            int requiredWorkers = model_get_building(b->type)->laborers;
			if (categoryWorkersNeeded[cat]) {
				int numWorkers = calc_adjust_with_percentage(
					Data_CityInfo.laborCategory[cat].workersAllocated,
					b->percentageHousesCovered) / 100;
				if (numWorkers > requiredWorkers) {
					numWorkers = requiredWorkers;
				}
				b->numWorkers = numWorkers;
				categoryWorkersAllocated[cat] += numWorkers;
			} else {
				b->numWorkers = requiredWorkers;
			}
		}
	}
	for (int i = 0; i < MAX_CATS; i++) {
		if (categoryWorkersNeeded[i]) {
			// watch out: categoryWorkersNeeded is now reset to 'unallocated workers available'
			if (categoryWorkersAllocated[i] >= Data_CityInfo.laborCategory[i].workersAllocated) {
				categoryWorkersNeeded[i] = 0;
				categoryWorkersAllocated[i] = 0;
			} else {
				categoryWorkersNeeded[i] = Data_CityInfo.laborCategory[i].workersAllocated - categoryWorkersAllocated[i];
			}
		}
	}
	for (int i = 1; i < MAX_BUILDINGS; i++) {
        building *b = building_get(i);
		if (!BuildingIsInUse(b)) {
			continue;
		}
		int cat = buildingTypeToLaborCategory[b->type];
		if (cat < 0 || cat == LABOR_CATEGORY_WATER || cat == LABOR_CATEGORY_MILITARY) {
			continue;
		}
		if (!shouldHaveWorkers(b, cat, 0)) {
			continue;
		}
		if (b->percentageHousesCovered > 0 && categoryWorkersNeeded[cat]) {
            int requiredWorkers = model_get_building(b->type)->laborers;
			if (b->numWorkers < requiredWorkers) {
				int needed = requiredWorkers - b->numWorkers;
				if (needed > categoryWorkersNeeded[cat]) {
					b->numWorkers += categoryWorkersNeeded[cat];
					categoryWorkersNeeded[cat] = 0;
				} else {
					b->numWorkers += needed;
					categoryWorkersNeeded[cat] -= needed;
				}
			}
		}
	}
}

static void allocateWorkersToWater()
{
	static int startBuildingId = 1;

	int percentageNotFilled = 100 - calc_percentage(
		Data_CityInfo.laborCategory[LABOR_CATEGORY_WATER].workersAllocated,
		Data_CityInfo.laborCategory[LABOR_CATEGORY_WATER].workersNeeded);

	int buildingsToSkip = calc_adjust_with_percentage(
		Data_CityInfo.laborCategory[LABOR_CATEGORY_WATER].buildings, percentageNotFilled);

	int workersPerBuilding;
	if (buildingsToSkip == Data_CityInfo.laborCategory[LABOR_CATEGORY_WATER].buildings) {
		workersPerBuilding = 1;
	} else {
		workersPerBuilding = Data_CityInfo.laborCategory[LABOR_CATEGORY_WATER].workersAllocated /
			(Data_CityInfo.laborCategory[LABOR_CATEGORY_WATER].buildings - buildingsToSkip);
	}
	int buildingId = startBuildingId;
	startBuildingId = 0;
	for (int guard = 1; guard < MAX_BUILDINGS; guard++, buildingId++) {
		if (buildingId >= 2000) {
			buildingId = 1;
		}
		building *b = building_get(buildingId);
		if (!BuildingIsInUse(b) || buildingTypeToLaborCategory[b->type] != LABOR_CATEGORY_WATER) {
			continue;
		}
		b->numWorkers = 0;
		if (b->percentageHousesCovered > 0) {
			if (percentageNotFilled > 0) {
				if (buildingsToSkip) {
					--buildingsToSkip;
				} else if (startBuildingId) {
					b->numWorkers = workersPerBuilding;
				} else {
					startBuildingId = buildingId;
					b->numWorkers = workersPerBuilding;
				}
			} else {
				b->numWorkers = model_get_building(b->type)->laborers;
			}
		}
	}
	if (!startBuildingId) {
		// no buildings assigned or full employment
		startBuildingId = 1;
	}
}

void CityInfo_Labor_update()
{
	CityInfo_Labor_calculateWorkersNeededPerCategory();
	CityInfo_Labor_checkEmployment();
	CityInfo_Labor_allocateWorkersToBuildings();
}
