#ifndef DATA_CITYINFO_H
#define DATA_CITYINFO_H

#include <stdint.h>

extern struct _Data_CityInfo {
	int8_t __unknown_00a5;
	int32_t populationMaxSupported;
	int32_t populationRoomInHouses;
	int32_t populationLastTargetHouseAdd;
	int32_t populationLastTargetHouseRemove;
	int32_t populationMigrationPercentage;
	int32_t populationNewcomersThisMonth;
	int16_t resourceLastTargetWarehouse;
	uint8_t entryPointX;
	uint8_t entryPointY;
	int16_t entryPointGridOffset;
	uint8_t exitPointX;
	uint8_t exitPointY;
	int16_t exitPointGridOffset;
	int32_t populationPercentagePlebs;
	int32_t populationWorkingAge;
	int32_t workersAvailable;
	int32_t salaryAmount;
	int32_t salaryRank;
	int32_t tradeNextImportResourceCaravan;
	int32_t tradeNextImportResourceCaravanBackup;
	int32_t housesRequiringFountainToEvolve;//4268
	int32_t housesRequiringWellToEvolve;//426c
	int32_t housesRequiringMoreEntertainmentToEvolve;//4270
	int32_t housesRequiringMoreEducationToEvolve;//4274
	int32_t housesRequiringEducationToEvolve;//4278
	int32_t housesRequiringSchool;//427c
	int32_t housesRequiringLibrary;//4280
	int32_t housesRequiringBarberToEvolve;
	int32_t housesRequiringBathhouseToEvolve;
	int32_t housesRequiringFoodToEvolve;
	int32_t housesRequiringClinicToEvolve;
	int32_t housesRequiringHospitalToEvolve;
	int32_t housesRequiringBarber;
	int32_t housesRequiringBathhouse;
	int32_t housesRequiringClinic;
	int32_t housesRequiringReligionToEvolve;
	int32_t housesRequiringMoreReligionToEvolve;
	int32_t housesRequiringEvenMoreReligionToEvolve;
	int32_t housesRequiringReligion;
	int32_t entertainmentTheaterShows;
	int32_t entertainmentTheaterNoShowsWeighted;
	int32_t entertainmentAmphitheaterShows;
	int32_t entertainmentAmphitheaterNoShowsWeighted;
	int32_t entertainmentColosseumShows;
	int32_t entertainmentColosseumNoShowsWeighted;
	int32_t entertainmentHippodromeShows;
	int32_t entertainmentHippodromeNoShowsWeighted;
	int32_t entertainmentNeedingShowsMost;
	int32_t citywideAverageEntertainment;
	int32_t housesRequiringEntertainmentToEvolve;
	int32_t monthsSinceFestival;
	int32_t populationEmigrationCauseTextId;
	int32_t numProtestersThisMonth;
	int32_t numCriminalsThisMonth; // muggers+rioters
	int32_t healthDemand;
	int32_t religionDemand;
	int32_t educationDemand;
	int32_t entertainmentDemand;
	int32_t numRiotersInCity;
	int32_t ratingAdvisorSelection;
	int32_t ratingAdvisorExplanationCulture;
	int32_t ratingAdvisorExplanationProsperity;
	int32_t ratingAdvisorExplanationPeace;
	int32_t ratingAdvisorExplanationFavor;
	int32_t playerRank;
	int32_t personalSavings;
	int32_t donateAmount;
	int16_t numAnimalsInCity;
	int32_t tradeNextImportResourceDocker;
	int32_t tradeNextExportResourceDocker;
	int32_t debtState;
	int32_t monthsInDebt;
	int32_t populationLostTroopRequest;
	int32_t victoryHasWonScenario;
	int32_t victoryContinueMonths;
	int32_t victoryContinueMonthsChosen;
	int32_t festivalGod;
	int32_t festivalSize;
	int32_t festivalCostSmall;
	int32_t festivalCostLarge;
	int32_t festivalCostGrand;
	int32_t festivalWineGrand;
	int32_t festivalNotEnoughWine;
	int32_t citywideAverageReligion;
	int32_t citywideAverageEducation;
	int32_t citywideAverageHealth;
	int32_t cultureCoverageReligion;
	int32_t festivalEffectMonthsDelayFirst;
	int32_t festivalEffectMonthsDelaySecond;
	int32_t populationEmigrationCause;
	int32_t riotersOrAttackingNativesInCity;
	int32_t nativeAttackDuration;
	int32_t nativeMissionPostOperational;
	int32_t nativeMainMeetingCenterX;
	int32_t nativeMainMeetingCenterY;
	int32_t godBlessingMarsEnemiesToKill;
	int32_t tutorial1SenateBuilt;
	int32_t numEnemiesInCity;
	int32_t numImperialSoldiersInCity;
	uint8_t distantBattleCityId;
	uint8_t distantBattleEnemyStrength;
	uint8_t distantBattleRomanStrength;
	int8_t distantBattleMonthsToBattle;
	int8_t distantBattleRomanMonthsToReturn;
	int8_t distantBattleRomanMonthsToTravel;
	int8_t distantBattleCityMonthsUntilRoman;
	int8_t distantBattleTotalCount;
	int8_t distantBattleWonCount;
	int8_t distantBattleEnemyMonthsTraveled;
	int8_t distantBattleRomanMonthsTraveled;
	int8_t militaryTotalLegions;
	int8_t militaryTotalLegionsEmpireService;
	int8_t militaryTotalSoldiers;
	int32_t numSoldiersInCity;
	int32_t militaryLegionaryLegions;
	struct {
		int32_t id;
		int32_t size;
	} largestRoadNetworks[10];
	int32_t housesRequiringSecondWineToEvolve;
	int32_t godCurseNeptuneSankShips;
	int32_t entertainmentHippodromeHasShow;
	int32_t messageShownHippodrome;
	int32_t messageShownColosseum;
	int32_t messageShownEmigration;
	int32_t messageShownFired;
	int32_t messageShownVictory;
	int32_t missionSavedGameWritten;
	int32_t tutorial1FireMessageShown;
	int32_t tutorial3DiseaseMessageShown;
	int32_t numAttackingNativesInCity;
} Data_CityInfo;

extern struct _Data_CityInfo_Extra {
	int32_t populationGraphOrder;
	struct {
		int32_t x;
		int32_t y;
		int32_t gridOffset;
	} entryPointFlag;
	struct {
		int32_t x;
		int32_t y;
		int32_t gridOffset;
	} exitPointFlag;
	int32_t ciid;
} Data_CityInfo_Extra;

#endif
