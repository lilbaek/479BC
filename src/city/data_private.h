#ifndef CITY_DATA_PRIVATE_H
#define CITY_DATA_PRIVATE_H

#include <stdint.h>

#include "city/finance.h"

extern struct city_data_t {
    struct {
        int16_t senate_placed;
        uint8_t senate_x;
        uint8_t senate_y;
        int16_t senate_grid_offset;
        int32_t senate_building_id;
        int32_t hippodrome_placed;
        int8_t barracks_x;
        int8_t barracks_y;
        int16_t barracks_grid_offset;
        int32_t barracks_building_id;
        int32_t barracks_placed;
        int8_t distribution_center_x;
        int8_t distribution_center_y;
        int16_t distribution_center_grid_offset;
        int32_t distribution_center_building_id;
        int32_t distribution_center_placed;
        int32_t trade_center_building_id;
    } building;
    struct {
        int32_t treasury;
        int32_t tax_percentage;
        finance_overview last_year;
        finance_overview this_year;
    } finance;
    struct {
        int32_t culture;
        int32_t prosperity;
        int32_t peace;
        int32_t favor;
        struct {
            int32_t theater;
            int32_t religion;
            int32_t school;
            int32_t library;
            int32_t academy;
        } culture_points;
        int32_t prosperity_treasury_last_year;
        int32_t prosperity_max;
        int32_t peace_destroyed_buildings;
        int32_t peace_years_of_peace;
        int32_t peace_num_criminals;
        int32_t peace_num_rioters;
        int32_t peace_riot_cause;
        int32_t favor_salary_penalty;
        int32_t favor_milestone_penalty;
        int32_t favor_ignored_request_penalty;
        int32_t favor_last_year;
        int32_t favor_change; // 0 = dropping, 1 = stalling, 2 = rising
    } ratings;
    struct {
        int8_t other_player[18068];
        int8_t unknown_00a0;
        int8_t unknown_00a1;
        int8_t unknown_00a2;
        int8_t unknown_00a3;
        int8_t unknown_00a4;
        int8_t unknown_00a6;
        int8_t unknown_00a7;
        int32_t unused_27d0;
        int32_t unknown_27e0[4];
        int16_t unknown_27f0;
        int16_t unknown_27f4[18];
        int16_t unknown_2828;
        int16_t unused_28ca;
        int8_t unknown_2924[272];
        int32_t unknown_2b6c;
        int32_t unknown_2c20[1400];
        int32_t housesRequiringUnknownToEvolve[8];
        int32_t unknown_4238[4];
        int32_t unknown_4284;
        int32_t unknown_4294[2];
        int32_t unknown_4334;
        int32_t unknown_4374[2];
        int16_t unknown_439c[3];
        int8_t padding_43b2[2];
        int32_t unknown_43d8[5];
        int32_t unknown_43f0;
        int32_t unused_4454;
        int32_t unknown_446c[4];
        int32_t unused_4488;
        int32_t unused_nativeForceAttack;
        int32_t unused_44e0[2];
        int32_t unused_44ec;
        int32_t unused_44f8;
        int32_t unused_4524[11];
        int8_t unknown_458e;
        int8_t unused_45a5[6];
        int8_t unknown_464c[232];
    } unused;
} city_data;

#endif // CITY_DATA_PRIVATE_H
