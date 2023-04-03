#include "properties.h"

#include "assets/assets.h"
#include "core/image_group.h"
#include "type.h"
#include "core/textures.h"


Tiberius_building_properties_mapping Tiberius_building_properties[] = {
        {BUILDING_ROADBLOCK,            {1, 1, 0, 0, 0},   TEXTURE_BUILDING_NAME,        TEXTURE_BUILDING_ROADBLOCK},
        {BUILDING_WORKCAMP,             {3, 0, 0, 0, 0},   "Logistics",        "Workcamp Central"},
        {BUILDING_GRAND_TEMPLE_CERES,   {7, 1, 0, 0, 0},   "Religion",         "Ceres Complex Off"},
        {BUILDING_GRAND_TEMPLE_NEPTUNE, {7, 1, 0, 0, 0},   "Religion",         "Neptune Complex Off"},
        {BUILDING_GRAND_TEMPLE_MERCURY, {7, 1, 0, 0, 0},   "Religion",         "Mercury Complex Off"},
        {BUILDING_GRAND_TEMPLE_MARS,    {7, 1, 0, 0, 0},   "Religion",         "Mars Complex Off"},
        {BUILDING_GRAND_TEMPLE_VENUS,   {7, 1, 0, 0, 0},   "Religion",         "Venus Complex Off"},
        {BUILDING_SMALL_POND,           {2, 1, 0, 0, 0},   "Aesthetics",       "s pond south off"},
        {BUILDING_LARGE_POND,           {3, 1, 0, 0, 0},   "Aesthetics",       "l pond south off"},
        {BUILDING_PINE_TREE,            {1, 1, 0, 0, 0},   "Aesthetics",       "ornamental pine"},
        {BUILDING_FIR_TREE,             {1, 1, 0, 0, 0},   "Aesthetics",       "ornamental fir"},
        {BUILDING_OAK_TREE,             {1, 1, 0, 0, 0},   "Aesthetics",       "ornamental oak"},
        {BUILDING_ELM_TREE,             {1, 1, 0, 0, 0},   "Aesthetics",       "ornamental elm"},
        {BUILDING_FIG_TREE,             {1, 1, 0, 0, 0},   "Aesthetics",       "ornamental fig"},
        {BUILDING_PLUM_TREE,            {1, 1, 0, 0, 0},   "Aesthetics",       "ornamental plum"},
        {BUILDING_PALM_TREE,            {1, 1, 0, 0, 0},   "Aesthetics",       "ornamental palm"},
        {BUILDING_DATE_TREE,            {1, 1, 0, 0, 0},   "Aesthetics",       "ornamental date"},
        {BUILDING_PINE_PATH,            {1, 1, 0, 0, 0},   "Aesthetics",       "path orn pine",},
        {BUILDING_FIR_PATH,             {1, 1, 0, 0, 0},   "Aesthetics",       "path orn fir",},
        {BUILDING_OAK_PATH,             {1, 1, 0, 0, 0},   "Aesthetics",       "path orn oak",},
        {BUILDING_ELM_PATH,             {1, 1, 0, 0, 0},   "Aesthetics",       "path orn elm"},
        {BUILDING_FIG_PATH,             {1, 1, 0, 0, 0},   "Aesthetics",       "path orn fig"},
        {BUILDING_PLUM_PATH,            {1, 1, 0, 0, 0},   "Aesthetics",       "path orn plum"},
        {BUILDING_PALM_PATH,            {1, 1, 0, 0, 0},   "Aesthetics",       "path orn palm"},
        {BUILDING_DATE_PATH,            {1, 1, 0, 0, 0},   "Aesthetics",       "path orn date"},
        {BUILDING_PAVILION_BLUE,        {1, 1, 0, 0, 0},   "Aesthetics",       "pavilion blue"},
        {BUILDING_PAVILION_RED,         {1, 1, 0, 0, 0},   "Aesthetics",       "pavilion red"},
        {BUILDING_PAVILION_ORANGE,      {1, 1, 0, 0, 0},   "Aesthetics",       "pavilion orange"},
        {BUILDING_PAVILION_YELLOW,      {1, 1, 0, 0, 0},   "Aesthetics",       "pavilion yellow"},
        {BUILDING_PAVILION_GREEN,       {1, 1, 0, 0, 0},   "Aesthetics",       "pavilion green"},
        {BUILDING_SMALL_STATUE_ALT,     {1, 1, 0, 0, 13},  "Aesthetics",       "sml statue 2"},
        {BUILDING_SMALL_STATUE_ALT_B,   {1, 1, 0, 0, 13},  "Aesthetics",       "sml statue 3"},
        {BUILDING_OBELISK,              {2, 1, 0, 0, 0},   "Aesthetics",       "obelisk"},
        {BUILDING_PANTHEON,             {7, 1, 0, 0, 0},   "Religion",         "Pantheon Off"},
        {BUILDING_ARCHITECT_GUILD,      {2, 1, 0, 0, 0},   "Logistics",        "Arch Guild OFF"},
        {BUILDING_MESS_HALL,            {3, 0, 0, 0, 0},   "Military",         "Mess OFF Central"},
        {BUILDING_LIGHTHOUSE,           {3, 1, 0, 0, 0},   "Logistics",        "Lighthouse OFF"},
        {BUILDING_TAVERN,               {2, 0, 0, 0, 0},   "Entertainment",    "Tavern OFF"},
        {BUILDING_GRAND_GARDEN,         {2, 1, 0, 0, 0},   "",                 ""},
        {BUILDING_ARENA,                {3, 0, 0, 0, 0},   "Entertainment",    "Arena OFF"},
        {BUILDING_HORSE_STATUE,         {3, 1, 0, 0, 1},   "Aesthetics",       "Eque Statue"},
        {BUILDING_DOLPHIN_FOUNTAIN,     {2, 1, 0, 0, 0},   "",                 ""},
        {BUILDING_HEDGE_DARK,           {1, 1, 0, 0, 0},   "Aesthetics",       "D Hedge 01"},
        {BUILDING_HEDGE_LIGHT,          {1, 1, 0, 0, 0},   "Aesthetics",       "L Hedge 01"},
        {BUILDING_GARDEN_WALL,          {1, 1, 0, 0, 0},   "Aesthetics",       "C Garden Wall 01"},
        {BUILDING_LEGION_STATUE,        {2, 1, 0, 0, 1},   "Aesthetics",       "legio statue"},
        {BUILDING_DECORATIVE_COLUMN,    {1, 1, 0, 0, 0},   "Aesthetics",       "sml col B"},
        {BUILDING_COLONNADE,            {1, 1, 0, 0, 0},   "Aesthetics",       "G Colonnade 01"},
        {BUILDING_GARDEN_PATH,          {1, 1, 0, 0, 0},   "Aesthetics",       "Garden Path 01"},
        {BUILDING_LARARIUM,             {1, 0, 0, 0, 0},   "Religion",         "Lararium 01"},
        {BUILDING_NYMPHAEUM,            {3, 0, 0, 0, 0},   "Religion",         "Nymphaeum OFF"},
        {BUILDING_SMALL_MAUSOLEUM,      {2, 1, 0, 0, 1},   "Religion",         "Mausoleum S"},
        {BUILDING_LARGE_MAUSOLEUM,      {3, 1, 0, 0, 0},   "Religion",         "Mausoleum L"},
        {BUILDING_WATCHTOWER,           {2, 1, 0, 0, 0},   "Military",         "Watchtower C OFF"},
        {BUILDING_LIBRARY,              {2, 0, 0, 0, 0},   "Health_Education", "Downgraded_Library"},
        {BUILDING_CARAVANSERAI,         {4, 1, 0, 0, 0},   "Logistics",        "Caravanserai_C_OFF"},
        {BUILDING_SMALL_STATUE,         {1, 1, 0, 0, -12}, "Aesthetics",       "V Small Statue"},
        {BUILDING_ROOFED_GARDEN_WALL,   {1, 1, 0, 0, 0},   "Aesthetics",       "R Garden Wall 01"},
        {BUILDING_GARDEN_WALL_GATE,     {1, 1, 0, 0, 0},   "Aesthetics",       "Garden Gate"},
        {BUILDING_PALISADE,             {1, 1, 0, 0, 0},   "Military",         "Pal Wall C 01"},
        {BUILDING_HEDGE_GATE_DARK,      {1, 1, 0, 0, 0},   "Aesthetics",       "D Hedge Gate"},
        {BUILDING_HEDGE_GATE_LIGHT,     {1, 1, 0, 0, 0},   "Aesthetics",       "L Hedge Gate"},
        {BUILDING_PALISADE_GATE,        {1, 1, 0, 0, 0},   "Military",         "Palisade_Gate"},
        {BUILDING_MEDIUM_STATUE,        {2, 1, 0, 0, 1},   "Aesthetics",       "Med_Statue_R"},
        {BUILDING_GLADIATOR_STATUE,     {1, 1, 0, 0, 1},   "Aesthetics",       ""},
        {BUILDING_HIGHWAY,              {2, 1, 0, 0, 0},   "Logistics",        "Highway_Placement"},
        {BUILDING_GOLD_MINE,            {2, 0, 0, 0, 0},   "Industry",         "Gold_Mine_C_OFF"},
        {BUILDING_CITY_MINT,            {3, 0, 0, 0, 0},   "Administration",   "City_Mint_ON"}
};

#define Tiberius_BUILDINGS (sizeof(Tiberius_building_properties) / sizeof(Tiberius_building_properties_mapping))

void init_Tiberius_building_properties(void) {
    for (int i = 0; i < Tiberius_BUILDINGS; ++i) {
        if (Tiberius_building_properties[i].asset_image_id) {
            Tiberius_building_properties[i].properties.image_group =
                    assets_get_image_id(Tiberius_building_properties[i].asset_name,
                                        Tiberius_building_properties[i].asset_image_id);
        } else {
            Tiberius_building_properties[i].properties.image_group =
                    assets_get_group_id(Tiberius_building_properties[i].asset_name);
        }
    }
}

// TODO: Fix me!
static building_properties properties[170] = {
        // SZ FIRE GRP OFF
        {0, 0, 0,                             0,  0, BUILDING_NONE},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0, BUILDING_MENU_FARMS},
        {0, 0, 0,                             0,  0, BUILDING_MENU_RAW_MATERIALS},
        {0, 0, 0,                             0,  0, BUILDING_MENU_WORKSHOPS},
        {1, 0, 112,                           0,  0, BUILDING_ROAD},
        {1, 0, 24,                            26, 0, BUILDING_WALL,      TEXTURE_BUILDING_NAME, TEXTURE_BUILDING_WALL},
        {1, 0, 0,                             0,  0, BUILDING_DRAGGABLE_RESERVOIR},
        {1, 0, 19,                            2,  0, BUILDING_UNUSED},
        {0, 0, 0,                             0,  0, BUILDING_CLEAR_LAND},
        {1, 0, 0,                             0,  0},
        {1, 0, 0,                             0,  0},
        {1, 0, 0,                             0,  0},
        {1, 0, 0,                             0,  0},
        {1, 0, 0,                             0,  0},
        {1, 0, 0,                             0,  0},
        {1, 0, 0,                             0,  0},
        {1, 0, 0,                             0,  0},
        {1, 0, 0,                             0,  0},
        {1, 0, 0,                             0,  0},
        {2, 0, 0,                             0,  0},
        {2, 0, 0,                             0,  0},
        {2, 0, 0,                             0,  0},
        {2, 0, 0,                             0,  0},
        {3, 0, 0,                             0,  0},
        {3, 0, 0,                             0,  0},
        {3, 0, 0,                             0,  0},
        {3, 0, 0,                             0,  0},
        {4, 0, 0,                             0,  0},
        {4, 0, 0,                             0,  0},
        {3, 0, 45,                            0,  0, BUILDING_AMPHITHEATER},
        {2, 0, 46,                            0,  0, BUILDING_THEATER, TEXTURE_BUILDING_NAME, TEXTURE_BUILDING_THEATER},
        {5, 1, 213,                           0,  0, BUILDING_HIPPODROME},
        {5, 1, 48,                            0,  0, BUILDING_COLOSSEUM},
        {3, 0, 49,                            0,  0, BUILDING_GLADIATOR_SCHOOL},
        {3, 0, 50,                            0,  0, BUILDING_LION_HOUSE},
        {3, 0, 51,                            0,  0, BUILDING_ACTOR_COLONY},
        {3, 0, 52,                            0,  0, BUILDING_CHARIOT_MAKER},
        {1, 1, 58,                            0,  0, BUILDING_PLAZA, TEXTURE_BASIC_NAME, TEXTURE_BASIC_PLAZA},
        {1, 1, 59,                            1,  0, BUILDING_GARDENS},
        {3, 1, 66,                            0,  0, BUILDING_FORT_LEGIONARIES},
        {1, 1, 61,                            0,  0, BUILDING_SMALL_STATUE},
        {2, 1, 61,                            1,  0, BUILDING_MEDIUM_STATUE},
        {3, 1, 61,                            2,  0, BUILDING_LARGE_STATUE},
        {3, 1, 66,                            0,  0, BUILDING_FORT_JAVELIN},
        {3, 1, 66,                            0,  0, BUILDING_FORT_MOUNTED},
        {1, 0, 68,                            0,  0, BUILDING_DOCTOR},
        {3, 0, 70,                            0,  0, BUILDING_HOSPITAL},
        {2, 0, 185,                           0,  0, BUILDING_BATHHOUSE},
        {1, 0, 67,                            0,  0, BUILDING_BARBER},
        {3, 0, 66,                            0,  0, BUILDING_DISTRIBUTION_CENTER_UNUSED},
        {2, 0, 41,                            0,  0, BUILDING_SCHOOL},
        {3, 0, 43,                            0,  0, BUILDING_ACADEMY},
        {2, 0, 42,                            0,  0, BUILDING_LIBRARY},
        {4, 1, 66,                            1,  0, BUILDING_FORT_GROUND},
        {1, 0, 64,                            0,  0, BUILDING_PREFECTURE, TEXTURE_BUILDING_NAME, TEXTURE_BUILDING_PREFECTURE},
        {3, 1, 205,                           0,  0, BUILDING_TRIUMPHAL_ARCH},
        {3, 1, 66,                            0,  0, BUILDING_FORT},
        {2, 1, 17,                            1,  0, BUILDING_GATEHOUSE, TEXTURE_BUILDING_NAME, TEXTURE_BUILDING_TOWER},
        {2, 1, 17,                            0,  0, BUILDING_TOWER, TEXTURE_BUILDING_NAME, TEXTURE_BUILDING_TOWER},
        {2, 0, 71,                            0,  0, BUILDING_SMALL_TEMPLE_CERES, TEXTURE_RELEGION_NAME, TEXTURE_RELEGION_TEMPLE_CERES},
        {2, 0, 72,                            0,  0, BUILDING_SMALL_TEMPLE_NEPTUNE, TEXTURE_RELEGION_NAME, TEXTURE_RELEGION_TEMPLE_NEPTUNE},
        {2, 0, 73,                            0,  0, BUILDING_SMALL_TEMPLE_MERCURY, TEXTURE_RELEGION_NAME, TEXTURE_RELEGION_TEMPLE_MERCURY},
        {2, 0, 74,                            0,  0, BUILDING_SMALL_TEMPLE_MARS, TEXTURE_RELEGION_NAME, TEXTURE_RELEGION_TEMPLE_MARS},
        {2, 0, 75,                            0,  0, BUILDING_SMALL_TEMPLE_VENUS, TEXTURE_RELEGION_NAME, TEXTURE_RELEGION_TEMPLE_VENUS},
        {3, 0, 71,                            1,  0, BUILDING_LARGE_TEMPLE_CERES},
        {3, 0, 72,                            1,  0, BUILDING_LARGE_TEMPLE_NEPTUNE},
        {3, 0, 73,                            1,  0, BUILDING_LARGE_TEMPLE_MERCURY},
        {3, 0, 74,                            1,  0, BUILDING_LARGE_TEMPLE_MARS},
        {3, 0, 75,                            1,  0, BUILDING_LARGE_TEMPLE_VENUS},
        {2, 0, 22,                            0,  0, BUILDING_MARKET,    TEXTURE_BUILDING_NAME,  TEXTURE_BUILDING_MARKET},
        {3, 0, 99,                            0,  0, BUILDING_GRANARY,    TEXTURE_BUILDING_NAME, TEXTURE_BUILDING_GRANARY},
        {1, 1, 82,                            0,  0, BUILDING_WAREHOUSE},
        {1, 1, 82,                            0,  0, BUILDING_WAREHOUSE_SPACE},
        {2, 0, 77,                            0,  0, BUILDING_SHIPYARD},
        {3, 0, 78,                            0,  0, BUILDING_DOCK},
        {2, 0, 79,                            0,  0, BUILDING_WHARF},
        {3, 0, 85,                            0,  0, BUILDING_GOVERNORS_HOUSE},
        {4, 0, 86,                            0,  0, BUILDING_GOVERNORS_VILLA},
        {5, 0, 87,                            0,  0, BUILDING_GOVERNORS_PALACE},
        {2, 1, 184,                           0,  0, BUILDING_MISSION_POST},
        {1, 1, 81,                            0,  0, BUILDING_ENGINEERS_POST, TEXTURE_BUILDING_NAME,  TEXTURE_BUILDING_ENGINEERS_POST},
        {1, 1, 0,                             0,  0, BUILDING_LOW_BRIDGE},
        {1, 1, 0,                             0,  0, BUILDING_SHIP_BRIDGE},
        {0, 0, 0,                             0,  0, BUILDING_SENATE},
        {5, 0, 62,                            0,  0, BUILDING_SENATE_UPGRADED},
        {2, 0, 63,                            0,  0, BUILDING_FORUM},
        {0, 0, 0,                             0,  0, BUILDING_FORUM_UPGRADED},
        {1, 1, 183,                           0,  0, BUILDING_NATIVE_HUT},
        {2, 1, 183,                           2,  0, BUILDING_NATIVE_MEETING},
        {3, 1, 25,                            0,  0, BUILDING_RESERVOIR},
        {1, 1, 54,                            0,  0, BUILDING_FOUNTAIN, TEXTURE_BUILDING_NAME, TEXTURE_BUILDING_FOUNTAIN},
        {1, 1, 23,                            0,  0, BUILDING_WELL,      TEXTURE_BUILDING_NAME, TEXTURE_BUILDING_WELL},
        {1, 1, 100,                           0,  0, BUILDING_NATIVE_CROPS},
        {3, 0, 201,                           0,  0, BUILDING_MILITARY_ACADEMY},
        {3, 0, 166,                           0,  0, BUILDING_BARRACKS},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {2, 0, 76,                            0,  0, BUILDING_ORACLE},
        {1, 1, 0,                             0,  0},
        {3, 0, 37,                            0,  0, BUILDING_WHEAT_FARM},
        {3, 0, 37,                            0,  0, BUILDING_VEGETABLE_FARM},
        {3, 0, 37,                            0,  0, BUILDING_FRUIT_FARM},
        {3, 0, 37,                            0,  0, BUILDING_OLIVE_FARM},
        {3, 0, 37,                            0,  0, BUILDING_VINES_FARM},
        {3, 0, 37,                            0,  0, BUILDING_PIG_FARM},
        {2, 0, 38,                            0,  0, BUILDING_MARBLE_QUARRY},
        {2, 0, 39,                            0,  0, BUILDING_IRON_MINE},
        {2, 0, 65,                            0,  0, BUILDING_TIMBER_YARD},
        {2, 0, 40,                            0,  0, BUILDING_CLAY_PIT},
        {2, 0, 44,                            0,  0, BUILDING_WINE_WORKSHOP},
        {2, 0, 122,                           0,  0, BUILDING_OIL_WORKSHOP},
        {2, 0, 123,                           0,  0, BUILDING_WEAPONS_WORKSHOP},
        {2, 0, 124,                           0,  0, BUILDING_FURNITURE_WORKSHOP},
        {2, 0, 125,                           0,  0, BUILDING_POTTERY_WORKSHOP},
        {0, 0, 0,                             0,  0, BUILDING_ROADBLOCK, TEXTURE_BUILDING_NAME, TEXTURE_BUILDING_ROADBLOCK},
        {3, 0, GROUP_BUILDING_WAREHOUSE,      0,  0, BUILDING_WORKCAMP}, // ?
        {7, 0, GROUP_BUILDING_TEMPLE_CERES,   1,  0, BUILDING_GRAND_TEMPLE_CERES},
        {7, 0, GROUP_BUILDING_TEMPLE_NEPTUNE, 1,  0, BUILDING_GRAND_TEMPLE_NEPTUNE},
        {7, 0, GROUP_BUILDING_TEMPLE_MERCURY, 1,  0, BUILDING_GRAND_TEMPLE_MERCURY},
        {7, 0, GROUP_BUILDING_TEMPLE_MARS,    1,  0, BUILDING_GRAND_TEMPLE_MARS},
        {7, 0, GROUP_BUILDING_TEMPLE_VENUS,   1,  0, BUILDING_GRAND_TEMPLE_VENUS},
        {1, 1, 0,                             0,  0},
        {1, 1, 0,                             0,  0},
        {1, 1, 0,                             0,  0},
        {1, 1, 0,                             0,  0},
        {1, 1, 0,                             0,  0},
        {1, 1, 0,                             0,  0},
        {1, 1, 0,                             0,  0},
        {1, 1, 0,                             0,  0},
        {1, 1, 0,                             0,  0},
        {2, 1, 216,                           0,  0, BUILDING_ELM_TREE},
        {1, 1, 0,                             0,  0},
        {1, 1, 0,                             0,  0},
        {1, 1, 0,                             0,  0},
        {1, 1, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
        {0, 0, 0,                             0,  0},
};

static int is_vanilla_building_with_changed_properties(building_type type) {
    switch (type) {
        case BUILDING_LIBRARY:
        case BUILDING_SMALL_STATUE:
        case BUILDING_MEDIUM_STATUE:
            return 1;
        default:
            return 0;
    }
}

const building_properties *building_properties_for_type(building_type type) {
    if (type < 0 || type > BUILDING_TYPE_MAX) {
        return &properties[0];
    }
    if (type >= BUILDING_ROADBLOCK || is_vanilla_building_with_changed_properties(type)) {
        for (int i = 0; i < Tiberius_BUILDINGS; ++i) {
            if (Tiberius_building_properties[i].type == type) {
                return &Tiberius_building_properties[i].properties;
            }
        }
    }

    return &properties[type];
}
