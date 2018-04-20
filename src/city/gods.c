#include "gods.h"

#include "building/count.h"
#include "building/granary.h"
#include "building/industry.h"
#include "building/warehouse.h"
#include "city/constants.h"
#include "city/culture.h"
#include "city/data_private.h"
#include "city/finance.h"
#include "city/health.h"
#include "city/message.h"
#include "city/sentiment.h"
#include "city/trade.h"
#include "core/calc.h"
#include "core/random.h"
#include "figure/formation_legion.h"
#include "figuretype/water.h"
#include "game/settings.h"
#include "game/time.h"
#include "scenario/property.h"
#include "scenario/invasion.h"

#include "Data/CityInfo.h"

#define TIE 10

void city_gods_reset()
{
    for (int i = 0; i < MAX_GODS; i++) {
        god_status *god = &city_data.religion.gods[i];
        god->target_happiness = 50;
        god->happiness = 50;
        Data_CityInfo.godWrathBolts[i] = 0;
        god->blessing_done = 0;
        god->small_curse_done = 0;
        god->unused1 = 0;
        god->unused2 = 0;
        god->unused3 = 0;
        Data_CityInfo.godMonthsSinceFestival[i] = 0;
    }
    Data_CityInfo.godAngryMessageDelay = 0;
}

static void perform_blessing(god_type god)
{
    switch (god) {
        case GOD_CERES:
            city_message_post(1, MESSAGE_BLESSING_FROM_CERES, 0, 0);
            building_bless_farms();
            break;
        case GOD_NEPTUNE:
            city_message_post(1, MESSAGE_BLESSING_FROM_NEPTUNE, 0, 0);
            Data_CityInfo.godBlessingNeptuneDoubleTrade = 1;
            break;
        case GOD_MERCURY:
            city_message_post(1, MESSAGE_BLESSING_FROM_MERCURY, 0, 0);
            building_granary_bless();
            break;
        case GOD_MARS:
            city_message_post(1, MESSAGE_BLESSING_FROM_MARS, 0, 0);
            Data_CityInfo.godBlessingMarsEnemiesToKill = 10;
            break;
        case GOD_VENUS:
            city_message_post(1, MESSAGE_BLESSING_FROM_VENUS, 0, 0);
            city_sentiment_change_happiness(25);
            break;
    }
}

static void perform_small_curse(god_type god)
{
    switch (god) {
        case GOD_CERES:
            city_message_post(1, MESSAGE_CERES_IS_UPSET, 0, 0);
            building_curse_farms(0);
            break;
        case GOD_NEPTUNE:
            city_message_post(1, MESSAGE_NEPTUNE_IS_UPSET, 0, 0);
            figure_sink_all_ships();
            Data_CityInfo.godCurseNeptuneSankShips = 1;
            break;
        case GOD_MERCURY:
            city_message_post(1, MESSAGE_MERCURY_IS_UPSET, 0, 0);
            building_granary_warehouse_curse(0);
            break;
        case GOD_MARS:
            if (scenario_invasion_start_from_mars()) {
                city_message_post(1, MESSAGE_MARS_IS_UPSET, 0, 0);
            } else {
                city_message_post(1, MESSAGE_WRATH_OF_MARS_NO_MILITARY, 0, 0);
            }
            break;
        case GOD_VENUS:
            city_message_post(1, MESSAGE_VENUS_IS_UPSET, 0, 0);
            city_sentiment_set_max_happiness(50);
            city_sentiment_change_happiness(-5);
            city_health_change(-10);
            city_sentiment_update();
            break;
    }
}

static int perform_large_curse(god_type god)
{
    switch (god) {
        case GOD_CERES:
            city_message_post(1, MESSAGE_WRATH_OF_CERES, 0, 0);
            building_curse_farms(1);
            break;
        case GOD_NEPTUNE:
            if (city_data.trade.num_sea_routes <= 0) {
                city_message_post(1, MESSAGE_WRATH_OF_NEPTUNE_NO_SEA_TRADE, 0, 0);
                return 0;
            } else {
                city_message_post(1, MESSAGE_WRATH_OF_NEPTUNE, 0, 0);
                figure_sink_all_ships();
                Data_CityInfo.godCurseNeptuneSankShips = 1;
                city_trade_start_sea_trade_problems(80);
            }
            break;
        case GOD_MERCURY:
            city_message_post(1, MESSAGE_WRATH_OF_MERCURY, 0, 0);
            building_granary_warehouse_curse(1);
            break;
        case GOD_MARS:
            if (formation_legion_curse()) {
                city_message_post(1, MESSAGE_WRATH_OF_MARS, 0, 0);
                scenario_invasion_start_from_mars();
            } else {
                city_message_post(1, MESSAGE_WRATH_OF_MARS_NO_MILITARY, 0, 0);
            }
            break;
        case GOD_VENUS:
            city_message_post(1, MESSAGE_WRATH_OF_VENUS, 0, 0);
            city_sentiment_set_max_happiness(40);
            city_sentiment_change_happiness(-10);
            if (city_data.health.value >= 80) {
                city_health_change(-50);
            } else if (city_data.health.value >= 60) {
                city_health_change(-40);
            } else {
                city_health_change(-20);
            }
            Data_CityInfo.godCurseVenusActive = 1;
            city_sentiment_update();
            break;
    }
    return 1;
}

static void update_god_moods()
{
    for (int i = 0; i < MAX_GODS; i++) {
        god_status *god = &city_data.religion.gods[i];
        if (god->happiness < god->target_happiness) {
            god->happiness++;
        } else if (god->happiness > god->target_happiness) {
            god->happiness--;
        }
        if (scenario_is_tutorial_1()) {
            if (god->happiness < 50) {
                god->happiness = 50;
            }
        }
        if (god->happiness > 50) {
            god->small_curse_done = 0;
        }
        if (god->happiness < 50) {
            god->blessing_done = 0;
        }
    }

    int god_id = random_byte() & 7;
    if (god_id < MAX_GODS) {
        god_status *god = &city_data.religion.gods[god_id];
        if (god->happiness >= 50) {
            Data_CityInfo.godWrathBolts[god_id] = 0;
        } else if (god->happiness < 40) {
            if (god->happiness >= 20) {
                Data_CityInfo.godWrathBolts[god_id] += 1;
            } else if (god->happiness >= 10) {
                Data_CityInfo.godWrathBolts[god_id] += 2;
            } else {
                Data_CityInfo.godWrathBolts[god_id] += 5;
            }
        }
        if (Data_CityInfo.godWrathBolts[god_id] > 50) {
            Data_CityInfo.godWrathBolts[god_id] = 50;
        }
    }
    if (game_time_day() != 0) {
        return;
    }

    // handle blessings, curses, etc every month
    for (int i = 0; i < MAX_GODS; i++) {
        Data_CityInfo.godMonthsSinceFestival[i]++;
    }
    if (god_id >= MAX_GODS) {
        if (city_gods_calculate_least_happy()) {
            god_id = Data_CityInfo.godLeastHappy - 1;
        }
    }
    if (!setting_gods_enabled()) {
        return;
    }
    if (god_id < MAX_GODS) {
        god_status *god = &city_data.religion.gods[god_id];
        if (god->happiness >= 100 && !god->blessing_done) {
            god->blessing_done = 1;
            perform_blessing(god_id);
        } else if (Data_CityInfo.godWrathBolts[god_id] >= 20 && !god->small_curse_done &&
                Data_CityInfo.godMonthsSinceFestival[god_id] > 3) {
            god->small_curse_done = 1;
            Data_CityInfo.godWrathBolts[god_id] = 0;
            god->happiness += 12;
            perform_small_curse(god_id);
        } else if (Data_CityInfo.godWrathBolts[god_id] >= 50 &&
                Data_CityInfo.godMonthsSinceFestival[god_id] > 3) {
            if (scenario_campaign_rank() < 4 && !scenario_is_custom()) {
                // no large curses in early scenarios
                god->small_curse_done = 0;
                return;
            }
            Data_CityInfo.godWrathBolts[god_id] = 0;
            god->happiness += 30;
            if (!perform_large_curse(god_id)) {
                return;
            }
        }
    }

    int min_happiness = 100;
    for (int i = 0; i < MAX_GODS; i++) {
        if (city_data.religion.gods[i].happiness < min_happiness) {
            min_happiness = city_data.religion.gods[i].happiness;
        }
    }
    if (Data_CityInfo.godAngryMessageDelay) {
        Data_CityInfo.godAngryMessageDelay--;
    } else if (min_happiness < 30) {
        Data_CityInfo.godAngryMessageDelay = 20;
        if (min_happiness < 10) {
            city_message_post(0, MESSAGE_GODS_WRATHFUL, 0, 0);
        } else {
            city_message_post(0, MESSAGE_GODS_UNHAPPY, 0, 0);
        }
    }
}

void city_gods_calculate_moods(int update_moods)
{
    // base happiness: percentage of houses covered
    for (int i = 0; i < MAX_GODS; i++) {
        city_data.religion.gods[i].target_happiness = city_culture_coverage_religion(i);
    }

    int max_temples = 0;
    int max_god = TIE;
    int min_temples = 100000;
    int min_god = TIE;
    for (int i = 0; i < MAX_GODS; i++) {
        int num_temples = 0;
        switch (i) {
            case GOD_CERES:
                num_temples = building_count_total(BUILDING_SMALL_TEMPLE_CERES) + building_count_total(BUILDING_LARGE_TEMPLE_CERES);
                break;
            case GOD_NEPTUNE:
                num_temples = building_count_total(BUILDING_SMALL_TEMPLE_NEPTUNE) + building_count_total(BUILDING_LARGE_TEMPLE_NEPTUNE);
                break;
            case GOD_MERCURY:
                num_temples = building_count_total(BUILDING_SMALL_TEMPLE_MERCURY) + building_count_total(BUILDING_LARGE_TEMPLE_MERCURY);
                break;
            case GOD_MARS:
                num_temples = building_count_total(BUILDING_SMALL_TEMPLE_MARS) + building_count_total(BUILDING_LARGE_TEMPLE_MARS);
                break;
            case GOD_VENUS:
                num_temples = building_count_total(BUILDING_SMALL_TEMPLE_VENUS) + building_count_total(BUILDING_LARGE_TEMPLE_VENUS);
                break;
        }
        if (num_temples == max_temples) {
            max_god = TIE;
        } else if (num_temples > max_temples) {
            max_temples = num_temples;
            max_god = i;
        }
        if (num_temples == min_temples) {
            min_god = TIE;
        } else if (num_temples < min_temples) {
            min_temples = num_temples;
            min_god = i;
        }
    }
    // happiness factor based on months since festival (max 40)
    for (int i = 0; i < MAX_GODS; i++) {
        int festival_penalty = Data_CityInfo.godMonthsSinceFestival[i];
        if (festival_penalty > 40) {
            festival_penalty = 40;
        }
        city_data.religion.gods[i].target_happiness += 12 - festival_penalty;
    }

    // BUG poor Venus never gets points here!
    if (max_god < 4) {
        if (city_data.religion.gods[max_god].target_happiness >= 50) {
            city_data.religion.gods[max_god].target_happiness = 100;
        } else {
            city_data.religion.gods[max_god].target_happiness += 50;
        }
    }
    if (min_god < 4) {
        city_data.religion.gods[min_god].target_happiness -= 25;
    }
    int min_happiness;
    if (city_data.population.population < 100) {
        min_happiness = 50;
    } else if (city_data.population.population < 200) {
        min_happiness = 40;
    } else if (city_data.population.population < 300) {
        min_happiness = 30;
    } else if (city_data.population.population < 400) {
        min_happiness = 20;
    } else if (city_data.population.population < 500) {
        min_happiness = 10;
    } else {
        min_happiness = 0;
    }
    for (int i = 0; i < MAX_GODS; i++) {
        city_data.religion.gods[i].target_happiness = calc_bound(city_data.religion.gods[i].target_happiness, min_happiness, 100);
    }
    if (update_moods) {
        update_god_moods();
    }
}

int city_gods_calculate_least_happy()
{
    int maxGod = 0;
    int maxWrath = 0;
    for (int i = 0; i < MAX_GODS; i++) {
        if (Data_CityInfo.godWrathBolts[i] > maxWrath) {
            maxGod = i + 1;
            maxWrath = Data_CityInfo.godWrathBolts[i];
        }
    }
    if (maxGod > 0) {
        Data_CityInfo.godLeastHappy = maxGod;
        return 1;
    }
    int minHappiness = 40;
    for (int i = 0; i < MAX_GODS; i++) {
        if (city_data.religion.gods[i].happiness < minHappiness) {
            maxGod = i + 1;
            minHappiness = city_data.religion.gods[i].happiness;
        }
    }
    Data_CityInfo.godLeastHappy = maxGod;
    return maxGod > 0;
}

void city_gods_schedule_festival()
{
    Data_CityInfo.plannedFestivalGod = Data_CityInfo.festivalGod;
    Data_CityInfo.plannedFestivalSize = Data_CityInfo.festivalSize;
    int cost;
    if (Data_CityInfo.festivalSize == FESTIVAL_SMALL) {
        Data_CityInfo.plannedFestivalMonthsToGo = 2;
        cost = Data_CityInfo.festivalCostSmall;
    } else if (Data_CityInfo.festivalSize == FESTIVAL_LARGE) {
        Data_CityInfo.plannedFestivalMonthsToGo = 3;
        cost = Data_CityInfo.festivalCostLarge;
    } else {
        Data_CityInfo.plannedFestivalMonthsToGo = 4;
        cost = Data_CityInfo.festivalCostGrand;
    }

    city_finance_process_sundry(cost);

    if (Data_CityInfo.festivalSize == FESTIVAL_GRAND) {
        building_warehouses_remove_resource(RESOURCE_WINE, Data_CityInfo.festivalWineGrand);
    }
}

void city_gods_check_festival()
{
    if (Data_CityInfo.festivalEffectMonthsDelayFirst) {
        --Data_CityInfo.festivalEffectMonthsDelayFirst;
    }
    if (Data_CityInfo.festivalEffectMonthsDelaySecond) {
        --Data_CityInfo.festivalEffectMonthsDelaySecond;
    }
    if (Data_CityInfo.plannedFestivalSize <= FESTIVAL_NONE) {
        return;
    }
    Data_CityInfo.plannedFestivalMonthsToGo--;
    if (Data_CityInfo.plannedFestivalMonthsToGo > 0) {
        return;
    }
    // throw a party!
    if (Data_CityInfo.festivalEffectMonthsDelayFirst <= 0) {
        Data_CityInfo.festivalEffectMonthsDelayFirst = 12;
        switch (Data_CityInfo.plannedFestivalSize) {
            case FESTIVAL_SMALL: city_sentiment_change_happiness(7); break;
            case FESTIVAL_LARGE: city_sentiment_change_happiness(9); break;
            case FESTIVAL_GRAND: city_sentiment_change_happiness(12); break;
        }
    } else if (Data_CityInfo.festivalEffectMonthsDelaySecond <= 0) {
        Data_CityInfo.festivalEffectMonthsDelaySecond = 12;
        switch (Data_CityInfo.plannedFestivalSize) {
            case FESTIVAL_SMALL: city_sentiment_change_happiness(2); break;
            case FESTIVAL_LARGE: city_sentiment_change_happiness(3); break;
            case FESTIVAL_GRAND: city_sentiment_change_happiness(5); break;
        }
    }
    Data_CityInfo.monthsSinceFestival = 1;
    Data_CityInfo.godMonthsSinceFestival[Data_CityInfo.plannedFestivalGod] = 0;
    switch (Data_CityInfo.plannedFestivalSize) {
        case FESTIVAL_SMALL: city_message_post(1, MESSAGE_SMALL_FESTIVAL, 0, 0); break;
        case FESTIVAL_LARGE: city_message_post(1, MESSAGE_LARGE_FESTIVAL, 0, 0); break;
        case FESTIVAL_GRAND: city_message_post(1, MESSAGE_GRAND_FESTIVAL, 0, 0); break;
    }
    Data_CityInfo.plannedFestivalSize = FESTIVAL_NONE;
    Data_CityInfo.plannedFestivalMonthsToGo = 0;
}

int city_god_happiness(int god_id)
{
    return city_data.religion.gods[god_id].happiness;
}

