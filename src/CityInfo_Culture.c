#include "CityInfo.h"
#include "core/calc.h"
#include "Data/CityInfo.h"
#include "Data/Constants.h"

#include "building/building.h"
#include "building/count.h"

void CityInfo_Culture_calculateDemandsForAdvisors()
{
	// emigration
	switch (Data_CityInfo.populationEmigrationCause) {
		case EmigrationCause_NoFood:
			Data_CityInfo.populationEmigrationCauseTextId = 2;
			break;
		case EmigrationCause_NoJobs:
			Data_CityInfo.populationEmigrationCauseTextId = 1;
			break;
		case EmigrationCause_HighTaxes:
			Data_CityInfo.populationEmigrationCauseTextId = 3;
			break;
		case EmigrationCause_LowWages:
			Data_CityInfo.populationEmigrationCauseTextId = 0;
			break;
		case EmigrationCause_ManyTents:
			Data_CityInfo.populationEmigrationCauseTextId = 4;
			break;
		default:
			Data_CityInfo.populationEmigrationCauseTextId = 5;
			break;
	}
	// health
	Data_CityInfo.healthDemand = 0;
	int max = 0;
	if (Data_CityInfo.housesRequiringBathhouseToEvolve > max) {
		Data_CityInfo.healthDemand = 1;
		max = Data_CityInfo.housesRequiringBathhouseToEvolve;
	}
	if (Data_CityInfo.housesRequiringBarberToEvolve > max) {
		Data_CityInfo.healthDemand = 2;
		max = Data_CityInfo.housesRequiringBarberToEvolve;
	}
	if (Data_CityInfo.housesRequiringClinicToEvolve > max) {
		Data_CityInfo.healthDemand = 3;
		max = Data_CityInfo.housesRequiringClinicToEvolve;
	}
	if (Data_CityInfo.housesRequiringHospitalToEvolve > max) {
		Data_CityInfo.healthDemand = 4;
	}
	// education
	Data_CityInfo.educationDemand = 0;
	if (Data_CityInfo.housesRequiringMoreEducationToEvolve > Data_CityInfo.housesRequiringEducationToEvolve) {
		Data_CityInfo.educationDemand = 1; // schools(academies?)
	} else if (Data_CityInfo.housesRequiringMoreEducationToEvolve == Data_CityInfo.housesRequiringEducationToEvolve) {
		Data_CityInfo.educationDemand = 2; // libraries
	} else if (Data_CityInfo.housesRequiringMoreEducationToEvolve || Data_CityInfo.housesRequiringEducationToEvolve) {
		Data_CityInfo.educationDemand = 3; // more education
	}
	// entertainment
	Data_CityInfo.entertainmentDemand = 0;
	if (Data_CityInfo.housesRequiringEntertainmentToEvolve > Data_CityInfo.housesRequiringMoreEntertainmentToEvolve) {
		Data_CityInfo.entertainmentDemand = 1;
	} else if (Data_CityInfo.housesRequiringMoreEntertainmentToEvolve) {
		Data_CityInfo.entertainmentDemand = 2;
	}
	// religion
	Data_CityInfo.religionDemand = 0;
	max = 0;
	if (Data_CityInfo.housesRequiringReligionToEvolve > max) {
		Data_CityInfo.religionDemand = 1;
		max = Data_CityInfo.housesRequiringReligionToEvolve;
	}
	if (Data_CityInfo.housesRequiringMoreReligionToEvolve > max) {
		Data_CityInfo.religionDemand = 2;
		max = Data_CityInfo.housesRequiringMoreReligionToEvolve;
	}
	if (Data_CityInfo.housesRequiringEvenMoreReligionToEvolve > max) {
		Data_CityInfo.religionDemand = 3;
	}
}

void CityInfo_Culture_calculateEntertainment()
{
	Data_CityInfo.citywideAverageEntertainment = 0;
	Data_CityInfo.entertainmentTheaterShows = 0;
	Data_CityInfo.entertainmentTheaterNoShowsWeighted = 0;
	Data_CityInfo.entertainmentAmphitheaterShows = 0;
	Data_CityInfo.entertainmentAmphitheaterNoShowsWeighted = 0;
	Data_CityInfo.entertainmentColosseumShows = 0;
	Data_CityInfo.entertainmentColosseumNoShowsWeighted = 0;
	Data_CityInfo.entertainmentHippodromeShows = 0;
	Data_CityInfo.entertainmentHippodromeNoShowsWeighted = 0;
	Data_CityInfo.entertainmentNeedingShowsMost = 0;

	Data_CityInfo.citywideAverageReligion = 0;
	Data_CityInfo.citywideAverageEducation = 0;
	Data_CityInfo.citywideAverageHealth = 0;

	int numHouses = 0;
	for (int i = 1; i < MAX_BUILDINGS; i++) {
		struct Data_Building *b = building_get(i);
		if (BuildingIsInUse(i) && b->houseSize) {
			numHouses++;
			Data_CityInfo.citywideAverageEntertainment += b->data.house.entertainment;
			Data_CityInfo.citywideAverageReligion += b->data.house.numGods;
			Data_CityInfo.citywideAverageEducation += b->data.house.education;
			Data_CityInfo.citywideAverageHealth += b->data.house.health;
		}
	}
	if (numHouses) {
		Data_CityInfo.citywideAverageEntertainment /= numHouses;
		Data_CityInfo.citywideAverageReligion /= numHouses;
		Data_CityInfo.citywideAverageEducation /= numHouses;
		Data_CityInfo.citywideAverageHealth /= numHouses;
	}
	for (int i = 1; i < MAX_BUILDINGS; i++) {
		struct Data_Building *b = building_get(i);
		if (!BuildingIsInUse(i)) {
			continue;
		}
		switch (b->type) {
			case BUILDING_THEATER:
				if (b->data.entertainment.days1) {
					Data_CityInfo.entertainmentTheaterShows++;
				} else {
					Data_CityInfo.entertainmentTheaterNoShowsWeighted++;
				}
				break;
			case BUILDING_AMPHITHEATER:
				if (b->data.entertainment.days1) {
					Data_CityInfo.entertainmentAmphitheaterShows++;
				} else {
					Data_CityInfo.entertainmentAmphitheaterNoShowsWeighted += 2;
				}
				if (b->data.entertainment.days2) {
					Data_CityInfo.entertainmentAmphitheaterShows++;
				} else {
					Data_CityInfo.entertainmentAmphitheaterNoShowsWeighted += 2;
				}
				break;
			case BUILDING_COLOSSEUM:
				if (b->data.entertainment.days1) {
					Data_CityInfo.entertainmentColosseumShows++;
				} else {
					Data_CityInfo.entertainmentColosseumNoShowsWeighted += 3;
				}
				if (b->data.entertainment.days2) {
					Data_CityInfo.entertainmentColosseumShows++;
				} else {
					Data_CityInfo.entertainmentColosseumNoShowsWeighted += 3;
				}
				break;
			case BUILDING_HIPPODROME:
				if (b->data.entertainment.days1) {
					Data_CityInfo.entertainmentHippodromeShows++;
				} else {
					Data_CityInfo.entertainmentHippodromeNoShowsWeighted += 100;
				}
				break;
		}
	}
	int worstShows = 0;
	if (Data_CityInfo.entertainmentTheaterNoShowsWeighted > worstShows) {
		worstShows = Data_CityInfo.entertainmentTheaterNoShowsWeighted;
		Data_CityInfo.entertainmentNeedingShowsMost = 1;
	}
	if (Data_CityInfo.entertainmentAmphitheaterNoShowsWeighted > worstShows) {
		worstShows = Data_CityInfo.entertainmentAmphitheaterNoShowsWeighted;
		Data_CityInfo.entertainmentNeedingShowsMost = 2;
	}
	if (Data_CityInfo.entertainmentColosseumNoShowsWeighted > worstShows) {
		worstShows = Data_CityInfo.entertainmentColosseumNoShowsWeighted;
		Data_CityInfo.entertainmentNeedingShowsMost = 3;
	}
	if (Data_CityInfo.entertainmentHippodromeNoShowsWeighted > worstShows) {
		Data_CityInfo.entertainmentNeedingShowsMost = 4;
	}

	Data_CityInfo.festivalCostSmall = Data_CityInfo.population / 20 + 10;
	Data_CityInfo.festivalCostLarge = Data_CityInfo.population / 10 + 20;
	Data_CityInfo.festivalCostGrand = Data_CityInfo.population / 5 + 40;
	Data_CityInfo.festivalWineGrand = Data_CityInfo.population / 500 + 1;
	Data_CityInfo.festivalNotEnoughWine = 0;
	if (Data_CityInfo.resourceStored[RESOURCE_WINE] < Data_CityInfo.festivalWineGrand) {
		Data_CityInfo.festivalNotEnoughWine = 1;
		if (Data_CityInfo.festivalSize == Festival_Grand) {
			Data_CityInfo.festivalSize = Festival_Large;
		}
	}
}
