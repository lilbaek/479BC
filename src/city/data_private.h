#ifndef CITY_DATA_PRIVATE_H
#define CITY_DATA_PRIVATE_H

#include <stdint.h>

#include "city/emperor.h"
#include "city/finance.h"
#include "city/resource.h"

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
        emperor_gift gifts[3];
        int32_t selected_gift_size;
        int32_t months_since_gift;
        int32_t gift_overdose_penalty;
    } emperor;
    struct {
        int32_t treasury;
        int32_t tax_percentage;
        finance_overview last_year;
        finance_overview this_year;
        int32_t interest_so_far;
        int32_t salary_so_far;
        int32_t wages_so_far;
        int16_t stolen_this_year;
        int16_t stolen_last_year;
    } finance;
    struct {
        int32_t taxed_plebs;
        int32_t taxed_patricians;
        int32_t untaxed_plebs;
        int32_t untaxed_patricians;
        int32_t percentage_taxed_plebs;
        int32_t percentage_taxed_patricians;
        int32_t percentage_taxed_people;
        struct {
            int32_t collected_plebs;
            int32_t collected_patricians;
            int32_t uncollected_plebs;
            int32_t uncollected_patricians;
        } yearly;
        struct {
            int32_t collected_plebs;
            int32_t collected_patricians;
            int32_t uncollected_plebs;
            int32_t uncollected_patricians;
        } monthly;
    } taxes;
    struct {
        int32_t population;
        int32_t population_last_year;
        int32_t school_age;
        int32_t academy_age;
        struct {
            int32_t values[2400];
            int32_t next_index;
            int32_t count;
        } monthly;

        int32_t yearly_update_requested;
        int32_t yearly_births;
        int32_t yearly_deaths;
        int32_t immigration_duration;
        int32_t emigration_duration;
        int32_t lost_removal;
        int32_t lost_homeless;
        int32_t last_change;
        int32_t total_all_years;
        int32_t total_years;
        int32_t average_per_year;
    } population;
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
        int16_t space_in_warehouses[16];
        int16_t stored_in_warehouses[16];
        int32_t space_in_workshops[6];
        int32_t stored_in_workshops[6];//2900
    } resource;
    struct {
        int8_t march_enemy;
        int8_t march_horse;
        int8_t march_wolf;
        int8_t shoot_arrow;
        int8_t hit_soldier;
        int8_t hit_spear;
        int8_t hit_club;
        int8_t hit_elephant;
        int8_t hit_axe;
        int8_t hit_wolf;
        int8_t die_citizen;
        int8_t die_soldier;
    } sound;
    struct {
        int8_t other_player[18068];
        int8_t unknown_00a0;
        int8_t unknown_00a1;
        int8_t unknown_00a2;
        int8_t unknown_00a3;
        int8_t unknown_00a4;
        int8_t unknown_00a6;
        int8_t unknown_00a7;
        int32_t unknown_00c0;
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
        int32_t unknown_order;
        uint8_t faction_bytes[2];
    } unused;
} city_data;

#endif // CITY_DATA_PRIVATE_H
