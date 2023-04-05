#include <assert.h>
#include "building/model.h"

#include "city/resource.h"

const model_house MODEL_SMALL_TENT = { -99,-10,0,0,0,0,0,0,0,1,0,0,0,0,3,5,1, };
const model_house MODEL_LARGE_TENT = { -12,-5,0,1,0,0,0,0,0,1,0,0,0,0,3,7,1, };
const model_house MODEL_SMALL_SHACK = { -7,0,0,1,0,0,1,0,0,1,0,0,0,0,3,9,1, };
const model_house MODEL_LARGE_SHACK = { -6,0,1,1,0,1,0,0,1,0,0,0,0,3,11,1, };
const model_house MODEL_SMALL_HOVEL = { -6,4,0,1,1,0,1,0,0,2,0,0,0,0,3,13,2, };
const model_house MODEL_LARGE_HOVEL = { 6,12,10,1,1,0,1,0,0,2,0,0,0,0,3,15,2, };
const model_house MODEL_SMALL_CASA = { 10,16,10,2,1,1,1,0,0,2,0,0,0,0,2,17,2, };
const model_house MODEL_LARGE_CASA = { 14,20,10,2,1,1,1,1,0,2,1,0,0,0,2,19,2, };
const model_house MODEL_SMALL_INSULA = { 18,25,25,2,1,1,1,1,0,3,1,0,0,0,2,19,3, };
const model_house MODEL_MEDIUM_INSULA = { 22,32,25,2,1,1,1,1,1,3,1,0,1,0,2,20,3, };
const model_house MODEL_LARGE_INSULA = { 29,40,25,2,1,2,1,1,1,3,1,1,1,0,2,84,3, };
const model_house MODEL_GRAND_INSULA = { 37,48,35,2,1,2,1,1,1,4,1,1,1,0,2,84,4, };
const model_house MODEL_SMALL_VILLA = { 45,53,35,2,2,2,1,1,1,9,1,1,1,1,1,40,9, };
const model_house MODEL_MEDIUM_VILLA = { 50,58,40,2,2,2,1,1,2,10,1,1,1,1,1,42,10, };
const model_house MODEL_LARGE_VILLA = { 55,63,45,2,2,3,1,1,2,11,1,1,1,1,1,90,11, };
const model_house MODEL_GRAND_VILLA = { 60,68,50,2,3,3,1,1,2,11,1,1,1,1,1,100,11, };
const model_house MODEL_SMALL_PALACE = { 65,74,55,2,3,3,1,1,2,12,1,1,1,2,1,106,12, };
const model_house MODEL_MEDIUM_PALACE = { 70,80,60,2,4,3,1,1,2,12,1,1,1,2,1,112,12, };
const model_house MODEL_LARGE_PALACE = { 76,90,70,2,4,3,1,1,2,15,1,1,1,2,1,190,15, };
const model_house MODEL_LUXURY_PALACE = { 85,100,80,2,4,3,1,1,2,16,1,1,1,2,1,200,16, };
const model_house MODEL_HOUSE_NONE = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

const model_building MODEL_NONE = { 0,0,0,0,0, 0 };
const model_building MODEL_MENU_FARMS = { 0,0,0,0,0, 0 };
const model_building MODEL_MENU_RAW_MATERIALS = { 0,0,0,0,0, 0 };
const model_building MODEL_MENU_WORKSHOPS = { 0,0,0,0,0, 0 };
const model_building MODEL_ROAD = { 4,0,0,0,0, 0 };
const model_building MODEL_WALL = { 12,0,0,0,0, 0 };
const model_building MODEL_DRAGGABLE_RESERVOIR = { 30,0,0,0,0, 0 };
const model_building MODEL_UNUSED = { 8,-2,1,1,2, 0 };
const model_building MODEL_CLEAR_LAND = { 2,0,0,0,0, 0 };
const model_building MODEL_HOUSE_SMALL_TENT = { 10,-3,1,1,3, 0 };
const model_building MODEL_HOUSE_LARGE_TENT = { 0,-3,1,1,3, 0 };
const model_building MODEL_HOUSE_SMALL_SHACK = { 0,-2,1,1,2, 0 };
const model_building MODEL_HOUSE_LARGE_SHACK = { 0,-2,1,1,2, 0 };
const model_building MODEL_HOUSE_SMALL_HOVEL = { 0,-2,1,1,2, 0 };
const model_building MODEL_HOUSE_LARGE_HOVEL = { 0,-2,1,1,2, 0 };
const model_building MODEL_HOUSE_SMALL_CASA = { 0,-1,1,1,1, 0 };
const model_building MODEL_HOUSE_LARGE_CASA = { 0,-1,1,1,1, 0 };
const model_building MODEL_HOUSE_SMALL_INSULA = { 0,0,1,1,1, 0 };
const model_building MODEL_HOUSE_MEDIUM_INSULA = { 0,0,1,1,1, 0 };
const model_building MODEL_HOUSE_LARGE_INSULA = { 0,0,0,0,0, 0 };
const model_building MODEL_HOUSE_GRAND_INSULA = { 0,0,0,0,0, 0 };
const model_building MODEL_HOUSE_SMALL_VILLA = { 0,1,2,-1,2, 0 };
const model_building MODEL_HOUSE_MEDIUM_VILLA = { 0,1,2,-1,2, 0 };
const model_building MODEL_HOUSE_LARGE_VILLA = { 0,2,2,-2,2, 0 };
const model_building MODEL_HOUSE_GRAND_VILLA = { 0,2,2,-2,2, 0 };
const model_building MODEL_HOUSE_SMALL_PALACE = { 0,3,2,-1,6, 0 };
const model_building MODEL_HOUSE_MEDIUM_PALACE = { 0,3,2,-1,6, 0 };
const model_building MODEL_HOUSE_LARGE_PALACE = { 0,4,2,-1,6, 0 };
const model_building MODEL_HOUSE_LUXURY_PALACE = { 0,4,2,-1,6, 0 };
const model_building MODEL_AMPHITHEATER = { 100,4,1,-1,4, 12 };
const model_building MODEL_THEATER = { 50,2,1,-1,2, 8 };
const model_building MODEL_HIPPODROME = { 3500,-3,1,1,3,150 };
const model_building MODEL_COLOSSEUM = { 1500,-3,1,1,3,100 };
const model_building MODEL_GLADIATOR_SCHOOL = { 75,-3,1,1,3, 8 };
const model_building MODEL_LION_HOUSE = { 75,-3,1,1,3, 8 };
const model_building MODEL_ACTOR_COLONY = { 50,2,1,-1,2, 5 };
const model_building MODEL_CHARIOT_MAKER = { 75,-3,1,1,3, 10 };
const model_building MODEL_PLAZA = { 15,4,1,-2,2, 0 };
const model_building MODEL_GARDENS = { 12,3,1,-1,3, 0 };
const model_building MODEL_FORT_LEGIONARIES = { 1000,-20,2,2,8, 0 };
const model_building MODEL_SMALL_STATUE = { 12,3,1,-1,3, 0 };
const model_building MODEL_MEDIUM_STATUE = { 60,10,1,-2,4, 0 };
const model_building MODEL_LARGE_STATUE = { 150,14,2,-2,5, 0 };
const model_building MODEL_FORT_JAVELIN = { 1000,-20,2,2,8, 0 };
const model_building MODEL_FORT_MOUNTED = { 1000,-20,2,2,8, 0 };
const model_building MODEL_DOCTOR = { 30,0,0,0,0, 5 };
const model_building MODEL_HOSPITAL = { 300,-1,2,1,2, 30 };
const model_building MODEL_BATHHOUSE = { 50,4,1,-1,4, 10 };
const model_building MODEL_BARBER = { 25,2,1,-1,2, 2 };
const model_building MODEL_DISTRIBUTION_CENTER_UNUSED = { 0,0,0,0,0, 0 };
const model_building MODEL_SCHOOL = { 50,-2,1,1,2, 10 };
const model_building MODEL_ACADEMY = { 100,4,1,1,4, 30 };
const model_building MODEL_LIBRARY = { 75,4,1,-1,4, 20 };
const model_building MODEL_FORT_GROUND = { 0,0,0,0,0, 0 };
const model_building MODEL_PREFECTURE = { 30,-2,1,1,2, 6 };
const model_building MODEL_TRIUMPHAL_ARCH = { 0,18,2,3,5, 0 };
const model_building MODEL_FORT = { 250,-20,2,2,8, 0 };
const model_building MODEL_GATEHOUSE = { 100,-4,1,1,3, 0 };
const model_building MODEL_TOWER = { 150,-8,1,2,3, 6 };
const model_building MODEL_SMALL_TEMPLE_CERES = { 50,4,2,-1,6, 2 };
const model_building MODEL_SMALL_TEMPLE_NEPTUNE = { 50,4,2,-1,6, 2 };
const model_building MODEL_SMALL_TEMPLE_MERCURY = { 50,4,2,-1,6, 2 };
const model_building MODEL_SMALL_TEMPLE_MARS = { 50,4,2,-1,6, 2 };
const model_building MODEL_SMALL_TEMPLE_VENUS = { 50,4,2,-1,6, 2 };
const model_building MODEL_LARGE_TEMPLE_CERES = { 150,14,2,-2,5, 5 };
const model_building MODEL_LARGE_TEMPLE_NEPTUNE = { 150,14,2,-2,5, 5 };
const model_building MODEL_LARGE_TEMPLE_MERCURY = { 150,14,2,-2,5, 5 };
const model_building MODEL_LARGE_TEMPLE_MARS = { 150,14,2,-2,5, 5 };
const model_building MODEL_LARGE_TEMPLE_VENUS = { 150,14,2,-2,5, 5 };
const model_building MODEL_MARKET = { 40,-2,1,1,6, 5 };
const model_building MODEL_GRANARY = { 100,-4,1,2,2, 6 };
const model_building MODEL_WAREHOUSE = { 70,-5,2,2,3, 6 };
const model_building MODEL_WAREHOUSE_SPACE = { 0,0,0,0,0, 0 };
const model_building MODEL_SHIPYARD = { 100,-8,2,2,3, 10 };
const model_building MODEL_DOCK = { 100,-8,2,2,3, 12 };
const model_building MODEL_WHARF = { 60,-8,2,2,3, 6 };
const model_building MODEL_GOVERNORS_HOUSE = { 150,12,2,-2,3, 0 };
const model_building MODEL_GOVERNORS_VILLA = { 400,20,2,-3,4, 0 };
const model_building MODEL_GOVERNORS_PALACE = { 700,28,2,-4,5, 0 };
const model_building MODEL_MISSION_POST = { 100,-3,1,1,2, 20 };
const model_building MODEL_ENGINEERS_POST = { 30,0,1,1,1, 5 };
const model_building MODEL_LOW_BRIDGE = { 40,0,0,0,0, 0 };
const model_building MODEL_SHIP_BRIDGE = { 100,0,0,0,0, 0 };
const model_building MODEL_SENATE = { 250,8,2,-2,2, 20 };
const model_building MODEL_SENATE_UPGRADED = { 400,8,2,-1,8, 30 };
const model_building MODEL_FORUM = { 75,3,2,-1,2, 6 };
const model_building MODEL_FORUM_UPGRADED = { 125,3,2,-1,2, 8 };
const model_building MODEL_NATIVE_HUT = { 50,0,0,0,0, 0 };
const model_building MODEL_NATIVE_MEETING = { 50,0,0,0,0, 0 };
const model_building MODEL_RESERVOIR = { 80,-6,1,2,3, 0 };
const model_building MODEL_FOUNTAIN = { 15,0,0,0,0, 4 };
const model_building MODEL_WELL = { 5,-1,1,2,1, 0 };
const model_building MODEL_NATIVE_CROPS = { 0,0,0,0,0, 0 };
const model_building MODEL_MILITARY_ACADEMY = { 1000,-3,1,1,3, 20 };
const model_building MODEL_BARRACKS = { 150,-6,1,1,3, 10 };
const model_building MODEL_MENU_SMALL_TEMPLES = { 0,0,0,0,0, 0 };
const model_building MODEL_MENU_LARGE_TEMPLES = { 0,0,0,0,0, 0 };
const model_building MODEL_ORACLE = { 200,8,2,-1,6, 0 };
const model_building MODEL_BURNING_RUIN = { 0,-1,1,1,2, 0 };
const model_building MODEL_WHEAT_FARM = { 40,-2,1,1,2, 10 };
const model_building MODEL_VEGETABLE_FARM = { 40,-2,1,1,2, 10 };
const model_building MODEL_FRUIT_FARM = { 40,2,1,1,2, 10 };
const model_building MODEL_OLIVE_FARM = { 40,2,1,1,2, 10 };
const model_building MODEL_VINES_FARM = { 40,2,1,1,2, 10 };
const model_building MODEL_PIG_FARM = { 40,-2,1,1,2, 10 };
const model_building MODEL_MARBLE_QUARRY = { 50,-6,1,1,4, 10 };
const model_building MODEL_IRON_MINE = { 50,-6,1,1,4, 10 };
const model_building MODEL_TIMBER_YARD = { 40,-4,1,1,3, 10 };
const model_building MODEL_CLAY_PIT = { 40,-3,1,1,2, 10 };
const model_building MODEL_WINE_WORKSHOP = { 45,-1,1,1,1, 10 };
const model_building MODEL_OIL_WORKSHOP = { 50,-4,1,1,2, 10 };
const model_building MODEL_WEAPONS_WORKSHOP = { 50,-4,1,1,2, 10 };
const model_building MODEL_FURNITURE_WORKSHOP = { 40,-4,1,1,2, 10 };
const model_building MODEL_POTTERY_WORKSHOP = { 40,-4,1,1,2, 10 };
const model_building MODEL_ROADBLOCK = { 12,0,0,0,0 };
const model_building MODEL_WORK_CAMP = { 150,-10,2,3,4,20 };
const model_building MODEL_ARCHITECT_GUILD = { 200,-8,1,2,4,12 };
const model_building MODEL_GRAND_TEMPLE_CERES = { 2500,20,2,-4,5,50 };
const model_building MODEL_GRAND_TEMPLE_NEPTUNE = { 2500,20,2,-4,5,50 };
const model_building MODEL_GRAND_TEMPLE_MERCURY = { 2500,20,2,-4,5,50 };
const model_building MODEL_GRAND_TEMPLE_MARS = { 2500,20,2,-4,5,50 };
const model_building MODEL_GRAND_TEMPLE_VENUS = { 2500,20,2,-4,5,50 };
const model_building MODEL_PANTHEON = { 3500,20,2,-4,5,50 };
const model_building MODEL_LIGHTHOUSE = { 1250,6,1,-1,4,20 };
const model_building MODEL_MESS_HALL = { 100,-8,1,2,4,10 };
const model_building MODEL_TAVERN = { 40,-2,1,1,6,8 };
const model_building MODEL_GRAND_GARDEN = { 400,0,0,0,0,0 };
const model_building MODEL_ARENA = { 500,-3,1,1,3,25 };
const model_building MODEL_NULL = { 0,0,0,0,0 };
const model_building MODEL_LARARIUM = { 45, 4, 1, -1, 3, 0 };
const model_building MODEL_NYMPHAEUM = { 250,12,2,-1,6,0 };
const model_building MODEL_SMALL_MAUSOLEUM = { 300,-8,1,3,5,0 };
const model_building MODEL_LARGE_MAUSOLEUM = { 750,-10,1,3,6,0 };
const model_building MODEL_WATCHTOWER = { 100,-6,1,2,3,8, };
const model_building MODEL_CARAVANSERAI = { 500,-10,2,3,4,20 };
const model_building MODEL_PALISADE = { 6,0,0,0,0,0 };
const model_building MODEL_HIGHWAY = { 100,-4,1,2,3,0 };
const model_building MODEL_GOLD_MINE = { 100,-6,1,1,4,30 };
const model_building MODEL_CITY_MINT = { 250,-3,1,1,3,40 };

const model_building *model_get_building(building_type type)
{
    switch (type) {
        case BUILDING_NONE:
            return &MODEL_NONE;
        case BUILDING_MENU_FARMS:
            return &MODEL_MENU_FARMS;
        case BUILDING_MENU_RAW_MATERIALS:
            return &MODEL_MENU_RAW_MATERIALS;
        case BUILDING_MENU_WORKSHOPS:
            return &MODEL_MENU_WORKSHOPS;
        case BUILDING_ROAD:
            return &MODEL_ROAD;
        case BUILDING_WALL:
            return &MODEL_WALL;
        case BUILDING_DRAGGABLE_RESERVOIR:
            return &MODEL_DRAGGABLE_RESERVOIR;
        case BUILDING_UNUSED:
            return &MODEL_UNUSED;
        case BUILDING_CLEAR_LAND:
            return &MODEL_CLEAR_LAND;
        case BUILDING_HOUSE_SMALL_TENT:
            return &MODEL_HOUSE_SMALL_TENT;
        case BUILDING_HOUSE_LARGE_TENT:
            return &MODEL_HOUSE_LARGE_TENT;
        case BUILDING_HOUSE_SMALL_SHACK:
            return &MODEL_HOUSE_SMALL_SHACK;
        case BUILDING_HOUSE_LARGE_SHACK:
            return &MODEL_HOUSE_LARGE_SHACK;
        case BUILDING_HOUSE_SMALL_HOVEL:
            return &MODEL_HOUSE_SMALL_HOVEL;
        case BUILDING_HOUSE_LARGE_HOVEL:
            return &MODEL_HOUSE_LARGE_HOVEL;
        case BUILDING_HOUSE_SMALL_CASA:
            return &MODEL_HOUSE_SMALL_CASA;
        case BUILDING_HOUSE_LARGE_CASA:
            return &MODEL_HOUSE_LARGE_CASA;
        case BUILDING_HOUSE_SMALL_INSULA:
            return &MODEL_HOUSE_SMALL_INSULA;
        case BUILDING_HOUSE_MEDIUM_INSULA:
            return &MODEL_HOUSE_MEDIUM_INSULA;
        case BUILDING_HOUSE_LARGE_INSULA:
            return &MODEL_HOUSE_LARGE_INSULA;
        case BUILDING_HOUSE_GRAND_INSULA:
            return &MODEL_HOUSE_GRAND_INSULA;
        case BUILDING_HOUSE_SMALL_VILLA:
            return &MODEL_HOUSE_SMALL_VILLA;
        case BUILDING_HOUSE_MEDIUM_VILLA:
            return &MODEL_HOUSE_MEDIUM_VILLA;
        case BUILDING_HOUSE_LARGE_VILLA:
            return &MODEL_HOUSE_LARGE_VILLA;
        case BUILDING_HOUSE_GRAND_VILLA:
            return &MODEL_HOUSE_GRAND_VILLA;
        case BUILDING_HOUSE_SMALL_PALACE:
            return &MODEL_HOUSE_SMALL_PALACE;
        case BUILDING_HOUSE_MEDIUM_PALACE:
            return &MODEL_HOUSE_MEDIUM_PALACE;
        case BUILDING_HOUSE_LARGE_PALACE:
            return &MODEL_HOUSE_LARGE_PALACE;
        case BUILDING_HOUSE_LUXURY_PALACE:
            return &MODEL_HOUSE_LUXURY_PALACE;
        case BUILDING_AMPHITHEATER:
            return &MODEL_AMPHITHEATER;
        case BUILDING_THEATER:
            return &MODEL_THEATER;
        case BUILDING_HIPPODROME:
            return &MODEL_HIPPODROME;
        case BUILDING_COLOSSEUM:
            return &MODEL_COLOSSEUM;
        case BUILDING_GLADIATOR_SCHOOL:
            return &MODEL_GLADIATOR_SCHOOL;
        case BUILDING_LION_HOUSE:
            return &MODEL_LION_HOUSE;
        case BUILDING_ACTOR_COLONY:
            return &MODEL_ACTOR_COLONY;
        case BUILDING_CHARIOT_MAKER:
            return &MODEL_CHARIOT_MAKER;
        case BUILDING_PLAZA:
            return &MODEL_PLAZA;
        case BUILDING_GARDENS:
            return &MODEL_GARDENS;
        case BUILDING_FORT_LEGIONARIES:
            return &MODEL_FORT_LEGIONARIES;
        case BUILDING_SMALL_STATUE:
            return &MODEL_SMALL_STATUE;
        case BUILDING_MEDIUM_STATUE:
            return &MODEL_MEDIUM_STATUE;
        case BUILDING_LARGE_STATUE:
            return &MODEL_LARGE_STATUE;
        case BUILDING_FORT_JAVELIN:
            return &MODEL_FORT_JAVELIN;
        case BUILDING_FORT_MOUNTED:
            return &MODEL_FORT_MOUNTED;
        case BUILDING_DOCTOR:
            return &MODEL_DOCTOR;
        case BUILDING_HOSPITAL:
            return &MODEL_HOSPITAL;
        case BUILDING_BATHHOUSE:
            return &MODEL_BATHHOUSE;
        case BUILDING_BARBER:
            return &MODEL_BARBER;
        case BUILDING_DISTRIBUTION_CENTER_UNUSED:
            return &MODEL_DISTRIBUTION_CENTER_UNUSED;
        case BUILDING_SCHOOL:
            return &MODEL_SCHOOL;
        case BUILDING_ACADEMY:
            return &MODEL_ACADEMY;
        case BUILDING_LIBRARY:
            return &MODEL_LIBRARY;
        case BUILDING_FORT_GROUND:
            return &MODEL_FORT_GROUND;
        case BUILDING_PREFECTURE:
            return &MODEL_PREFECTURE;
        case BUILDING_TRIUMPHAL_ARCH:
            return &MODEL_TRIUMPHAL_ARCH;
        case BUILDING_FORT:
            return &MODEL_FORT;
        case BUILDING_GATEHOUSE:
            return &MODEL_GATEHOUSE;
        case BUILDING_TOWER:
            return &MODEL_TOWER;
        case BUILDING_SMALL_TEMPLE_CERES:
            return &MODEL_SMALL_TEMPLE_CERES;
        case BUILDING_SMALL_TEMPLE_NEPTUNE:
            return &MODEL_SMALL_TEMPLE_NEPTUNE;
        case BUILDING_SMALL_TEMPLE_MERCURY:
            return &MODEL_SMALL_TEMPLE_MERCURY;
        case BUILDING_SMALL_TEMPLE_MARS:
            return &MODEL_SMALL_TEMPLE_MARS;
        case BUILDING_SMALL_TEMPLE_VENUS:
            return &MODEL_SMALL_TEMPLE_VENUS;
        case BUILDING_LARGE_TEMPLE_CERES:
            return &MODEL_LARGE_TEMPLE_CERES;
        case BUILDING_LARGE_TEMPLE_NEPTUNE:
            return &MODEL_LARGE_TEMPLE_NEPTUNE;
        case BUILDING_LARGE_TEMPLE_MERCURY:
            return &MODEL_LARGE_TEMPLE_MERCURY;
        case BUILDING_LARGE_TEMPLE_MARS:
            return &MODEL_LARGE_TEMPLE_MARS;
        case BUILDING_LARGE_TEMPLE_VENUS:
            return &MODEL_LARGE_TEMPLE_VENUS;
        case BUILDING_MARKET:
            return &MODEL_MARKET;
        case BUILDING_GRANARY:
            return &MODEL_GRANARY;
        case BUILDING_WAREHOUSE:
            return &MODEL_WAREHOUSE;
        case BUILDING_WAREHOUSE_SPACE:
            return &MODEL_WAREHOUSE_SPACE;
        case BUILDING_SHIPYARD:
            return &MODEL_SHIPYARD;
        case BUILDING_DOCK:
            return &MODEL_DOCK;
        case BUILDING_WHARF:
            return &MODEL_WHARF;
        case BUILDING_GOVERNORS_HOUSE:
            return &MODEL_GOVERNORS_HOUSE;
        case BUILDING_GOVERNORS_VILLA:
            return &MODEL_GOVERNORS_VILLA;
        case BUILDING_GOVERNORS_PALACE:
            return &MODEL_GOVERNORS_PALACE;
        case BUILDING_MISSION_POST:
            return &MODEL_MISSION_POST;
        case BUILDING_ENGINEERS_POST:
            return &MODEL_ENGINEERS_POST;
        case BUILDING_LOW_BRIDGE:
            return &MODEL_LOW_BRIDGE;
        case BUILDING_SHIP_BRIDGE:
            return &MODEL_SHIP_BRIDGE;
        case BUILDING_SENATE:
            return &MODEL_SENATE;
        case BUILDING_SENATE_UPGRADED:
            return &MODEL_SENATE_UPGRADED;
        case BUILDING_FORUM:
            return &MODEL_FORUM;
        case BUILDING_FORUM_UPGRADED:
            return &MODEL_FORUM_UPGRADED;
        case BUILDING_NATIVE_HUT:
            return &MODEL_NATIVE_HUT;
        case BUILDING_NATIVE_MEETING:
            return &MODEL_NATIVE_MEETING;
        case BUILDING_RESERVOIR:
            return &MODEL_RESERVOIR;
        case BUILDING_FOUNTAIN:
            return &MODEL_FOUNTAIN;
        case BUILDING_WELL:
            return &MODEL_WELL;
        case BUILDING_NATIVE_CROPS:
            return &MODEL_NATIVE_CROPS;
        case BUILDING_MILITARY_ACADEMY:
            return &MODEL_MILITARY_ACADEMY;
        case BUILDING_BARRACKS:
            return &MODEL_BARRACKS;
        case BUILDING_MENU_SMALL_TEMPLES:
            return &MODEL_MENU_SMALL_TEMPLES;
        case BUILDING_MENU_LARGE_TEMPLES:
            return &MODEL_MENU_LARGE_TEMPLES;
        case BUILDING_ORACLE:
            return &MODEL_ORACLE;
        case BUILDING_BURNING_RUIN:
            return &MODEL_BURNING_RUIN;
        case BUILDING_WHEAT_FARM:
            return &MODEL_WHEAT_FARM;
        case BUILDING_VEGETABLE_FARM:
            return &MODEL_VEGETABLE_FARM;
        case BUILDING_FRUIT_FARM:
            return &MODEL_FRUIT_FARM;
        case BUILDING_OLIVE_FARM:
            return &MODEL_OLIVE_FARM;
        case BUILDING_VINES_FARM:
            return &MODEL_VINES_FARM;
        case BUILDING_PIG_FARM:
            return &MODEL_PIG_FARM;
        case BUILDING_MARBLE_QUARRY:
            return &MODEL_MARBLE_QUARRY;
        case BUILDING_IRON_MINE:
            return &MODEL_IRON_MINE;
        case BUILDING_TIMBER_YARD:
            return &MODEL_TIMBER_YARD;
        case BUILDING_CLAY_PIT:
            return &MODEL_CLAY_PIT;
        case BUILDING_WINE_WORKSHOP:
            return &MODEL_WINE_WORKSHOP;
        case BUILDING_OIL_WORKSHOP:
            return &MODEL_OIL_WORKSHOP;
        case BUILDING_WEAPONS_WORKSHOP:
            return &MODEL_WEAPONS_WORKSHOP;
        case BUILDING_FURNITURE_WORKSHOP:
            return &MODEL_FURNITURE_WORKSHOP;
        case BUILDING_POTTERY_WORKSHOP:
            return &MODEL_POTTERY_WORKSHOP;
        case BUILDING_ROADBLOCK:
        case BUILDING_GARDEN_WALL_GATE:
        case BUILDING_HEDGE_GATE_DARK:
        case BUILDING_HEDGE_GATE_LIGHT:
            return &MODEL_ROADBLOCK;
        case BUILDING_WORKCAMP:
            return &MODEL_WORK_CAMP;
        case BUILDING_ARCHITECT_GUILD:
            return &MODEL_ARCHITECT_GUILD;
        case BUILDING_GRAND_TEMPLE_CERES:
            return &MODEL_GRAND_TEMPLE_CERES;
        case BUILDING_GRAND_TEMPLE_NEPTUNE:
            return &MODEL_GRAND_TEMPLE_NEPTUNE;
        case BUILDING_GRAND_TEMPLE_MERCURY:
            return &MODEL_GRAND_TEMPLE_MERCURY;
        case BUILDING_GRAND_TEMPLE_MARS:
            return &MODEL_GRAND_TEMPLE_MARS;
        case BUILDING_GRAND_TEMPLE_VENUS:
            return &MODEL_GRAND_TEMPLE_VENUS;
        case BUILDING_PANTHEON:
            return &MODEL_PANTHEON;
        case BUILDING_MESS_HALL:
            return &MODEL_MESS_HALL;
        case BUILDING_LIGHTHOUSE:
            return &MODEL_LIGHTHOUSE;
        case BUILDING_TAVERN:
            return &MODEL_TAVERN;
        case BUILDING_GRAND_GARDEN:
            return &MODEL_GRAND_GARDEN;
        case BUILDING_ARENA:
            return &MODEL_ARENA;
        case BUILDING_LARARIUM:
            return &MODEL_LARARIUM;
        case BUILDING_NYMPHAEUM:
            return &MODEL_NYMPHAEUM;
        case BUILDING_WATCHTOWER:
            return &MODEL_WATCHTOWER;
        case BUILDING_SMALL_MAUSOLEUM:
            return &MODEL_SMALL_MAUSOLEUM;
        case BUILDING_LARGE_MAUSOLEUM:
            return &MODEL_LARGE_MAUSOLEUM;
        case BUILDING_CARAVANSERAI:
            return &MODEL_CARAVANSERAI;
        case BUILDING_PALISADE:
        case BUILDING_PALISADE_GATE:
            return &MODEL_PALISADE;
        case BUILDING_HIGHWAY:
            return &MODEL_HIGHWAY;
        case BUILDING_GOLD_MINE:
            return &MODEL_GOLD_MINE;
        case BUILDING_CITY_MINT:
            return &MODEL_CITY_MINT;
        default:
            break;
    }

    if ((type >= BUILDING_PINE_TREE && type <= BUILDING_SMALL_STATUE_ALT_B) ||
        type == BUILDING_HEDGE_DARK || type == BUILDING_HEDGE_LIGHT ||
        type == BUILDING_DECORATIVE_COLUMN || type == BUILDING_GARDEN_WALL ||
        type == BUILDING_COLONNADE || type == BUILDING_GARDEN_WALL || 
        type == BUILDING_ROOFED_GARDEN_WALL || type == BUILDING_GARDEN_PATH ||
        type == BUILDING_GLADIATOR_STATUE) {
        return &MODEL_SMALL_STATUE;
    }

    if (type == BUILDING_SMALL_POND || type == BUILDING_OBELISK ||
        type == BUILDING_LEGION_STATUE || type == BUILDING_DOLPHIN_FOUNTAIN) {
        return &MODEL_MEDIUM_STATUE;
    }

    if (type == BUILDING_LARGE_POND || type == BUILDING_HORSE_STATUE) {
        return &MODEL_LARGE_STATUE;
    }
    assert("Invalid building" && 0);
    return &MODEL_NONE;
}

const model_house *model_get_house(house_level level)
{
    switch (level) {
        case HOUSE_SMALL_TENT:
            return &MODEL_SMALL_TENT;
        case HOUSE_LARGE_TENT:
            return &MODEL_LARGE_TENT;
        case HOUSE_SMALL_SHACK:
            return &MODEL_SMALL_SHACK;
        case HOUSE_LARGE_SHACK:
            return &MODEL_LARGE_SHACK;
        case HOUSE_SMALL_HOVEL:
            return &MODEL_SMALL_HOVEL;
        case HOUSE_LARGE_HOVEL:
            return &MODEL_LARGE_HOVEL;
        case HOUSE_SMALL_CASA:
            return &MODEL_SMALL_CASA;
        case HOUSE_LARGE_CASA:
            return &MODEL_LARGE_CASA;
        case HOUSE_SMALL_INSULA:
            return &MODEL_SMALL_INSULA;
        case HOUSE_MEDIUM_INSULA:
            return &MODEL_MEDIUM_INSULA;
        case HOUSE_LARGE_INSULA:
            return &MODEL_LARGE_INSULA;
        case HOUSE_GRAND_INSULA:
            return &MODEL_GRAND_INSULA;
        case HOUSE_SMALL_VILLA:
            return &MODEL_SMALL_VILLA;
        case HOUSE_MEDIUM_VILLA:
            return &MODEL_MEDIUM_VILLA;
        case HOUSE_LARGE_VILLA:
            return &MODEL_LARGE_VILLA;
        case HOUSE_GRAND_VILLA:
            return &MODEL_GRAND_VILLA;
        case HOUSE_SMALL_PALACE:
            return &MODEL_SMALL_PALACE;
        case HOUSE_MEDIUM_PALACE:
            return &MODEL_MEDIUM_PALACE;
        case HOUSE_LARGE_PALACE:
            return &MODEL_LARGE_PALACE;
        case HOUSE_LUXURY_PALACE:
            return &MODEL_LUXURY_PALACE;
    }
    assert("Invalid house level" && 0);
    return &MODEL_HOUSE_NONE;
}

int model_house_uses_inventory(house_level level, resource_type inventory)
{
    const model_house *house = model_get_house(level);
    switch (inventory) {
        case RESOURCE_WINE:
            return house->wine;
        case RESOURCE_OIL:
            return house->oil;
        case RESOURCE_FURNITURE:
            return house->furniture;
        case RESOURCE_POTTERY:
            return house->pottery;
        default:
            return 0;
    }
}
