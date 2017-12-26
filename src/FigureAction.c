#include "FigureAction_private.h"

#include "Data/CityInfo.h"

#include "figure/figure.h"
#include "figuretype/animal.h"
#include "figuretype/cartpusher.h"
#include "figuretype/crime.h"
#include "figuretype/docker.h"
#include "figuretype/entertainer.h"
#include "figuretype/maintenance.h"
#include "figuretype/market.h"
#include "figuretype/migrant.h"
#include "figuretype/missile.h"
#include "figuretype/native.h"
#include "figuretype/service.h"
#include "figuretype/soldier.h"
#include "figuretype/trader.h"
#include "figuretype/wall.h"
#include "figuretype/water.h"

static void figure_nobody_action(figure *f)
{
}

static void (*figureActionCallbacks[])(figure *f) = {
	figure_nobody_action, //0
	figure_immigrant_action,
	figure_emigrant_action,
	figure_homeless_action,
	figure_cartpusher_action,
	figure_labor_seeker_action,
	figure_explosion_cloud_action,
	figure_tax_collector_action,
	figure_engineer_action,
	figure_warehouseman_action,
	figure_prefect_action, //10
	figure_soldier_action,
	figure_soldier_action,
	figure_soldier_action,
	figure_military_standard_action,
	figure_entertainer_action,
	figure_entertainer_action,
	figure_entertainer_action,
	figure_entertainer_action,
	figure_trade_caravan_action,
	figure_trade_ship_action, //20
	figure_trade_caravan_donkey_action,
	figure_protestor_action,
	figure_criminal_action,
	figure_rioter_action,
	figure_fishing_boat_action,
	figure_market_trader_action,
	figure_priest_action,
	figure_school_child_action,
	figure_teacher_action,
	figure_librarian_action, //30
	figure_barber_action,
	figure_bathhouse_worker_action,
	figure_doctor_action,
	figure_doctor_action,
	figure_worker_action,
	figure_nobody_action, // mapFlag: editor only
	figure_flotsam_action,
	figure_docker_action,
	figure_market_buyer_action,
	figure_patrician_action, //40
	figure_indigenous_native_action,
	figure_tower_sentry_action,
	FigureAction_enemy43_Spear,
	FigureAction_enemy44_Sword,
	FigureAction_enemy45_Sword,
	FigureAction_enemy46_Camel,
	FigureAction_enemy47_Elephant,
	FigureAction_enemy48_Chariot,
	FigureAction_enemy49_FastSword,
	FigureAction_enemy50_Sword, //50
	FigureAction_enemy51_Spear,
	FigureAction_enemy52_MountedArcher,
	FigureAction_enemy53_Axe,
	FigureAction_enemy54_Gladiator,
	figure_nobody_action,
	figure_nobody_action,
	FigureAction_enemyCaesarLegionary,
	figure_native_trader_action,
	figure_arrow_action,
	figure_javelin_action, //60
	figure_bolt_action,
	figure_ballista_action,
	figure_nobody_action,
	figure_missionary_action,
	figure_seagulls_action,
	figure_delivery_boy_action,
	figure_shipwreck_action,
	figure_sheep_action,
	figure_wolf_action,
	figure_zebra_action, //70
	figure_spear_action,
	figure_hippodrome_horse_action,
	figure_nobody_action,
	figure_nobody_action,
	figure_nobody_action,
	figure_nobody_action,
	figure_nobody_action,
	figure_nobody_action,
	figure_nobody_action
}; //80

void FigureAction_handle()
{
	Data_CityInfo.numEnemiesInCity = 0;
	Data_CityInfo.numRiotersInCity = 0;
	Data_CityInfo.numAttackingNativesInCity = 0;
	Data_CityInfo.numAnimalsInCity = 0;
	Data_CityInfo.numImperialSoldiersInCity = 0;
	Data_CityInfo.numSoldiersInCity = 0;
	Data_CityInfo.entertainmentHippodromeHasShow = 0;
	if (Data_CityInfo.riotersOrAttackingNativesInCity > 0) {
		Data_CityInfo.riotersOrAttackingNativesInCity--;
	}
	for (int i = 1; i < MAX_FIGURES; i++) {
		figure *f = figure_get(i);
		if (f->state) {
			if (f->targetedByFigureId) {
                figure *attacker = figure_get(f->targetedByFigureId);
				if (attacker->state != FigureState_Alive) {
					f->targetedByFigureId = 0;
				}
				if (attacker->targetFigureId != i) {
					f->targetedByFigureId = 0;
				}
			}
			figureActionCallbacks[f->type](f);
			if (f->state == FigureState_Dead) {
				figure_delete(f);
			}
		}
	}
}
