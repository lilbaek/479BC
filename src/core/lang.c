#include "building/type.h"
#include "core/lang.h"

#include "core/buffer.h"
#include "core/file.h"
#include "core/io.h"
#include "core/log.h"
#include "core/string.h"
#include "scenario/building.h"
#include "translation/translation.h"

#include <stdlib.h>
#include <string.h>

#define MAX_TEXT_ENTRIES 1000
#define MAX_TEXT_DATA 200000
#define MIN_TEXT_SIZE (28 + MAX_TEXT_ENTRIES * 8)
#define MAX_TEXT_SIZE (MIN_TEXT_SIZE + MAX_TEXT_DATA)

#define MAX_MESSAGE_ENTRIES 400
#define MAX_MESSAGE_DATA 460000
#define MIN_MESSAGE_SIZE 32024
#define MAX_MESSAGE_SIZE (MIN_MESSAGE_SIZE + MAX_MESSAGE_DATA)

#define BUFFER_SIZE 400000

#define FILE_TEXT_ENG "c3.eng"
#define FILE_MM_ENG "c3_mm.eng"
#define FILE_TEXT_RUS "c3.rus"
#define FILE_MM_RUS "c3_mm.rus"
#define FILE_EDITOR_TEXT_ENG "c3_map.eng"
#define FILE_EDITOR_MM_ENG "c3_map_mm.eng"

static struct {
    struct {
        int32_t offset;
        int32_t in_use;
    } text_entries[MAX_TEXT_ENTRIES];
    uint8_t text_data[MAX_TEXT_DATA];

    lang_message message_entries[MAX_MESSAGE_ENTRIES];
    uint8_t message_data[MAX_MESSAGE_DATA];
} data;

static int file_exists_in_dir(const char *dir, const char *file) {
    char path[2 * FILE_NAME_MAX];
    path[2 * FILE_NAME_MAX - 1] = 0;
    strncpy(path, dir, 2 * FILE_NAME_MAX - 1);
    strncat(path, "/", 2 * FILE_NAME_MAX - 1);
    strncat(path, file, 2 * FILE_NAME_MAX - 1);
    return file_exists(path, NOT_LOCALIZED);
}

int lang_dir_is_valid(const char *dir) {
    if (file_exists_in_dir(dir, FILE_TEXT_ENG) && file_exists_in_dir(dir, FILE_MM_ENG)) {
        return 1;
    }
    if (file_exists_in_dir(dir, FILE_TEXT_RUS) && file_exists_in_dir(dir, FILE_MM_RUS)) {
        return 1;
    }
    return 0;
}

static void parse_text(buffer *buf) {
    buffer_skip(buf, 28); // header
    for (int i = 0; i < MAX_TEXT_ENTRIES; i++) {
        data.text_entries[i].offset = buffer_read_i32(buf);
        data.text_entries[i].in_use = buffer_read_i32(buf);
    }
    buffer_read_raw(buf, data.text_data, MAX_TEXT_DATA);
}

static int load_text(const char *filename, int localizable, uint8_t *buf_data) {
    buffer buf;
    int filesize = io_read_file_into_buffer(filename, localizable, buf_data, BUFFER_SIZE);
    if (filesize < MIN_TEXT_SIZE || filesize > MAX_TEXT_SIZE) {
        return 0;
    }
    buffer_init(&buf, buf_data, filesize);
    parse_text(&buf);
    return 1;
}

static uint8_t *get_message_text(int32_t offset) {
    if (!offset) {
        return 0;
    }
    return &data.message_data[offset];
}

static void parse_message(buffer *buf) {
    buffer_skip(buf, 24); // header
    for (int i = 0; i < MAX_MESSAGE_ENTRIES; i++) {
        lang_message *m = &data.message_entries[i];
        m->type = buffer_read_i16(buf);
        m->message_type = buffer_read_i16(buf);
        buffer_skip(buf, 2);
        m->x = buffer_read_i16(buf);
        m->y = buffer_read_i16(buf);
        m->width_blocks = buffer_read_i16(buf);
        m->height_blocks = buffer_read_i16(buf);
        m->image.id = buffer_read_i16(buf);
        m->image.x = buffer_read_i16(buf);
        m->image.y = buffer_read_i16(buf);
        buffer_skip(buf, 6); // unused image2 id, x, y
        m->title.x = buffer_read_i16(buf);
        m->title.y = buffer_read_i16(buf);
        m->subtitle.x = buffer_read_i16(buf);
        m->subtitle.y = buffer_read_i16(buf);
        buffer_skip(buf, 4);
        buffer_read_i16(buf);
        buffer_read_i16(buf);
        buffer_skip(buf, 14);
        m->urgent = buffer_read_i32(buf);

        get_message_text(buffer_read_i32(buf));
        buffer_skip(buf, 4);
        m->title.text = get_message_text(buffer_read_i32(buf));
        m->subtitle.text = get_message_text(buffer_read_i32(buf));
        m->content.text = get_message_text(buffer_read_i32(buf));
    }
    buffer_read_raw(buf, &data.message_data, MAX_MESSAGE_DATA);
}


static void set_message_parameters(lang_message *m, int title, int text, int urgent, int message_type) {
    m->type = TYPE_MESSAGE;
    m->message_type = message_type;
    m->x = 0;
    m->y = 0;
    m->width_blocks = 30;
    m->height_blocks = 20;
    m->title.x = 0;
    m->title.y = 0;
    m->urgent = urgent;

    m->title.text = translation_for(title);
    m->content.text = translation_for(text);
}


void load_custom_messages(void) {
    int i = 321;
    while (i < MAX_MESSAGE_ENTRIES) {
        if (!data.message_entries[i].content.text) {
            break;
        }
        i++;
    }

    if (i >= MAX_MESSAGE_ENTRIES) {
        log_error("Message entry max exceeded", "", 0);
        return;
    }

    // soldiers starving
    lang_message *m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_MESS_HALL_NEEDS_FOOD, TR_CITY_MESSAGE_TEXT_MESS_HALL_NEEDS_FOOD, 1,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    // soldiers starving, no mess hall
    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_MESS_HALL_NEEDS_FOOD, TR_CITY_MESSAGE_TEXT_MESS_HALL_MISSING, 1,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    // monument completed
    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_GRAND_TEMPLE_COMPLETE, TR_CITY_MESSAGE_TEXT_GRAND_TEMPLE_COMPLETE,
                           0,
                           MESSAGE_TYPE_BUILDING_COMPLETION);
    i += 1;

    // replacement Mercury blessing
    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_MERCURY_BLESSING, TR_CITY_MESSAGE_TEXT_MERCURY_BLESSING, 0,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    // auto festivals
    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_PANTHEON_FESTIVAL, TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_CERES, 0,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_PANTHEON_FESTIVAL, TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_NEPTUNE,
                           0,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_PANTHEON_FESTIVAL, TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_MERCURY,
                           0,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_PANTHEON_FESTIVAL, TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_MARS, 0,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_PANTHEON_FESTIVAL, TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_VENUS, 0,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_MONUMENT_COMPLETE, TR_CITY_MESSAGE_TEXT_PANTHEON_COMPLETE, 0,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_MONUMENT_COMPLETE, TR_CITY_MESSAGE_TEXT_LIGHTHOUSE_COMPLETE, 0,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_NEPTUNE_BLESSING, TR_CITY_MESSAGE_TEXT_NEPTUNE_BLESSING, 0,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_VENUS_BLESSING, TR_CITY_MESSAGE_TEXT_VENUS_BLESSING, 0,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_MONUMENT_COMPLETE, TR_CITY_MESSAGE_TEXT_COLOSSEUM_COMPLETE, 0,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_MONUMENT_COMPLETE, TR_CITY_MESSAGE_TEXT_HIPPODROME_COMPLETE, 0,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_COLOSSEUM_WORKING, TR_CITY_MESSAGE_TEXT_COLOSSEUM_WORKING, 1,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_HIPPODROME_WORKING, TR_CITY_MESSAGE_TEXT_HIPPODROME_WORKING, 1,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    for (int j = 0; j < 12; ++j) {
        m = &data.message_entries[i];
        set_message_parameters(m, TR_CITY_MESSAGE_TITLE_GREAT_GAMES, TR_CITY_MESSAGE_TEXT_NAVAL_GAMES_PLANNING + j, 1,
                               MESSAGE_TYPE_GENERAL);
        i += 1;
    }

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_LOOTING, TR_CITY_MESSAGE_TEXT_LOOTING, 1, MESSAGE_TYPE_DISASTER);
    i += 1;

    for (int j = 0; j < 3; ++j) {
        m = &data.message_entries[i];
        set_message_parameters(m, TR_CITY_MESSAGE_TITLE_GREAT_GAMES, TR_CITY_MESSAGE_TEXT_IMPERIAL_GAMES_PLANNING + j,
                               1,
                               MESSAGE_TYPE_GENERAL);
        i += 1;
    }

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_SICKNESS, TR_CITY_MESSAGE_TEXT_SICKNESS, 1, MESSAGE_TYPE_DISASTER);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_EMPERORS_WRATH, TR_CITY_MESSAGE_TEXT_EMPERORS_WRATH, 1,
                           MESSAGE_TYPE_GENERAL);
    m->urgent = 1;
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_MARS_MINOR_CURSE_PREVENTED,
                           TR_CITY_MESSAGE_TEXT_MARS_MINOR_CURSE_PREVENTED, 1, MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_ENEMIES_LEAVING, TR_CITY_MESSAGE_TEXT_ENEMIES_LEAVING, 1,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

    m = &data.message_entries[i];
    m->urgent = 1;
    set_message_parameters(m, TR_CITY_MESSAGE_TITLE_ROAD_TO_ROME_WARNING, TR_CITY_MESSAGE_TEXT_ROAD_TO_ROME_WARNING, 1,
                           MESSAGE_TYPE_GENERAL);
    i += 1;

}


static int load_message(const char *filename, int localizable, uint8_t *data_buffer) {
    buffer buf;
    int filesize = io_read_file_into_buffer(filename, localizable, data_buffer, BUFFER_SIZE);
    if (filesize < MIN_MESSAGE_SIZE || filesize > MAX_MESSAGE_SIZE) {
        return 0;
    }
    buffer_init(&buf, data_buffer, filesize);
    parse_message(&buf);
    return 1;
}

static int load_files(const char *text_filename, const char *message_filename, int localizable) {
    uint8_t *buffer = (uint8_t *) malloc(BUFFER_SIZE);
    if (!buffer) {
        return 0;
    }
    int success = load_text(text_filename, localizable, buffer) && load_message(message_filename, localizable, buffer);
    free(buffer);
    return success;
}

int lang_load(int is_editor) {
    if (is_editor) {
        return load_files(FILE_EDITOR_TEXT_ENG, FILE_EDITOR_MM_ENG, MAY_BE_LOCALIZED);
    }
    // Prefer language files from localized dir, fall back to main dir
    return
            load_files(FILE_TEXT_ENG, FILE_MM_ENG, MUST_BE_LOCALIZED) ||
            load_files(FILE_TEXT_RUS, FILE_MM_RUS, MUST_BE_LOCALIZED) ||
            load_files(FILE_TEXT_ENG, FILE_MM_ENG, NOT_LOCALIZED) ||
            load_files(FILE_TEXT_RUS, FILE_MM_RUS, NOT_LOCALIZED);
}

const uint8_t *lang_get_string(int group, int index) {
    if (group == CUSTOM_TRANSLATION) {
        return translation_for(index);
    }
    if (group == 92 && !index) {
        return translation_for(TR_BUILDING_SMALL_TEMPLE_CERES_NAME);
    }
    if (group == 93 && !index) {
        return translation_for(TR_BUILDING_SMALL_TEMPLE_NEPTUNE_NAME);
    }
    if (group == 94 && !index) {
        return translation_for(TR_BUILDING_SMALL_TEMPLE_MERCURY_NAME);
    }
    if (group == 95 && !index) {
        return translation_for(TR_BUILDING_SMALL_TEMPLE_MARS_NAME);
    }
    if (group == 96 && !index) {
        return translation_for(TR_BUILDING_SMALL_TEMPLE_VENUS_NAME);
    }

    if (group == 130) {
        switch (index) {
            case 641:
                return translation_for(TR_PHRASE_FIGURE_MISSIONARY_EXACT_4);
            default:
                break;
        }
    }

    if (group == 67 && index == 48) {
        return translation_for(TR_EDITOR_ALLOWED_BUILDINGS_MONUMENTS);
    }

    // Building strings
    if (group == 28 || group == 41) {
        switch (index) {
            case BUILDING_HOUSE_VACANT_LOT:
                return translation_for(TR_BUILDING_VACANT_LOT);
            case BUILDING_ROADBLOCK:
                return translation_for(TR_BUILDING_ROADBLOCK);
            case BUILDING_WORKCAMP:
                return translation_for(TR_BUILDING_WORK_CAMP);
            case BUILDING_GRAND_TEMPLE_CERES:
                return translation_for(TR_BUILDING_GRAND_TEMPLE_CERES);
            case BUILDING_GRAND_TEMPLE_NEPTUNE:
                return translation_for(TR_BUILDING_GRAND_TEMPLE_NEPTUNE);
            case BUILDING_GRAND_TEMPLE_MERCURY:
                return translation_for(TR_BUILDING_GRAND_TEMPLE_MERCURY);
            case BUILDING_GRAND_TEMPLE_MARS:
                return translation_for(TR_BUILDING_GRAND_TEMPLE_MARS);
            case BUILDING_GRAND_TEMPLE_VENUS:
                return translation_for(TR_BUILDING_GRAND_TEMPLE_VENUS);
            case BUILDING_PANTHEON:
                return translation_for(TR_BUILDING_PANTHEON);
            case BUILDING_MENU_GRAND_TEMPLES:
                return translation_for(TR_BUILDING_GRAND_TEMPLE_MENU);
            case BUILDING_ARCHITECT_GUILD:
                return translation_for(TR_BUILDING_ARCHITECT_GUILD);
            case BUILDING_MESS_HALL:
                return translation_for(TR_BUILDING_MESS_HALL);
            case BUILDING_MENU_TREES:
                return translation_for(TR_BUILDING_MENU_TREES);
            case BUILDING_MENU_PATHS:
                return translation_for(TR_BUILDING_MENU_PATHS);
            case BUILDING_MENU_PARKS:
                return translation_for(TR_BUILDING_MENU_PARKS);
            case BUILDING_SMALL_POND:
                return translation_for(TR_BUILDING_SMALL_POND);
            case BUILDING_LARGE_POND:
                return translation_for(TR_BUILDING_LARGE_POND);
            case BUILDING_PINE_TREE:
                return translation_for(TR_BUILDING_PINE_TREE);
            case BUILDING_FIR_TREE:
                return translation_for(TR_BUILDING_FIR_TREE);
            case BUILDING_OAK_TREE:
                return translation_for(TR_BUILDING_OAK_TREE);
            case BUILDING_ELM_TREE:
                return translation_for(TR_BUILDING_ELM_TREE);
            case BUILDING_FIG_TREE:
                return translation_for(TR_BUILDING_FIG_TREE);
            case BUILDING_PLUM_TREE:
                return translation_for(TR_BUILDING_PLUM_TREE);
            case BUILDING_PALM_TREE:
                return translation_for(TR_BUILDING_PALM_TREE);
            case BUILDING_DATE_TREE:
                return translation_for(TR_BUILDING_DATE_TREE);
            case BUILDING_PINE_PATH:
                return translation_for(TR_BUILDING_PINE_PATH);
            case BUILDING_FIR_PATH:
                return translation_for(TR_BUILDING_FIR_PATH);
            case BUILDING_OAK_PATH:
                return translation_for(TR_BUILDING_OAK_PATH);
            case BUILDING_ELM_PATH:
                return translation_for(TR_BUILDING_ELM_PATH);
            case BUILDING_FIG_PATH:
                return translation_for(TR_BUILDING_FIG_PATH);
            case BUILDING_PLUM_PATH:
                return translation_for(TR_BUILDING_PLUM_PATH);
            case BUILDING_PALM_PATH:
                return translation_for(TR_BUILDING_PALM_PATH);
            case BUILDING_DATE_PATH:
                return translation_for(TR_BUILDING_DATE_PATH);
            case BUILDING_PAVILION_BLUE:
                return translation_for(TR_BUILDING_BLUE_PAVILION);
            case BUILDING_PAVILION_RED:
                return translation_for(TR_BUILDING_RED_PAVILION);
            case BUILDING_PAVILION_ORANGE:
                return translation_for(TR_BUILDING_ORANGE_PAVILION);
            case BUILDING_PAVILION_YELLOW:
                return translation_for(TR_BUILDING_YELLOW_PAVILION);
            case BUILDING_PAVILION_GREEN:
                return translation_for(TR_BUILDING_GREEN_PAVILION);
            case BUILDING_SMALL_STATUE_ALT:
                return translation_for(TR_BUILDING_SMALL_STATUE_ALT);
            case BUILDING_SMALL_STATUE_ALT_B:
                return translation_for(TR_BUILDING_SMALL_STATUE_ALT_B);
            case BUILDING_OBELISK:
                return translation_for(TR_BUILDING_OBELISK);
            case BUILDING_LIGHTHOUSE:
                return translation_for(TR_BUILDING_LIGHTHOUSE);
            case BUILDING_MENU_GOV_RES:
                return translation_for(TR_BUILDING_MENU_GOV_RES);
            case BUILDING_MENU_STATUES:
                return translation_for(TR_BUILDING_MENU_STATUES);
            case BUILDING_TAVERN:
                return translation_for(TR_BUILDING_TAVERN);
            case BUILDING_GRAND_GARDEN:
                return translation_for(TR_BUILDING_GRAND_GARDEN);
            case BUILDING_ARENA:
                return translation_for(TR_BUILDING_ARENA);
            case BUILDING_HORSE_STATUE:
                return translation_for(TR_BUILDING_HORSE_STATUE);
            case BUILDING_DOLPHIN_FOUNTAIN:
                return translation_for(TR_BUILDING_DOLPHIN_FOUNTAIN);
            case BUILDING_HEDGE_DARK:
                return translation_for(TR_BUILDING_HEDGE_DARK);
            case BUILDING_HEDGE_LIGHT:
                return translation_for(TR_BUILDING_HEDGE_LIGHT);
            case BUILDING_GARDEN_WALL:
                return translation_for(TR_BUILDING_GARDEN_WALL);
            case BUILDING_LEGION_STATUE:
                return translation_for(TR_BUILDING_LEGION_STATUE);
            case BUILDING_DECORATIVE_COLUMN:
                return translation_for(TR_BUILDING_DECORATIVE_COLUMN);
            case BUILDING_COLONNADE:
                return translation_for(TR_BUILDING_COLONNADE);
            case BUILDING_GARDEN_PATH:
                return translation_for(TR_BUILDING_GARDEN_PATH);
            case BUILDING_LARARIUM:
                return translation_for(TR_BUILDING_LARARIUM);
            case BUILDING_NYMPHAEUM:
                return translation_for(TR_BUILDING_NYMPHAEUM);
            case BUILDING_WATCHTOWER:
                return translation_for(TR_BUILDING_WATCHTOWER);
            case BUILDING_SMALL_MAUSOLEUM:
                return translation_for(TR_BUILDING_SMALL_MAUSOLEUM);
            case BUILDING_LARGE_MAUSOLEUM:
                return translation_for(TR_BUILDING_LARGE_MAUSOLEUM);
            case BUILDING_CARAVANSERAI:
                return translation_for(TR_BUILDING_CARAVANSERAI);
            case BUILDING_ROOFED_GARDEN_WALL:
                return translation_for(TR_BUILDING_ROOFED_GARDEN_WALL);
            case BUILDING_GARDEN_WALL_GATE:
                return translation_for(TR_BUILDING_GARDEN_WALL_GATE);
            case BUILDING_PALISADE:
                return translation_for(TR_BUILDING_PALISADE);
            case BUILDING_GLADIATOR_STATUE:
                return translation_for(TR_BUILDING_GLADIATOR_STATUE);
            case BUILDING_HIGHWAY:
                return translation_for(TR_BUILDING_HIGHWAY);
            case BUILDING_GOLD_MINE:
                return translation_for(TR_BUILDING_GOLD_MINE);
            case BUILDING_CITY_MINT:
                return translation_for(TR_BUILDING_CITY_MINT);
            default:
                break;
        }
    }

    const uint8_t *str = &data.text_data[data.text_entries[group].offset];
    uint8_t prev = 0;
    while (index > 0) {
        if (!*str && (prev >= ' ' || prev == 0)) {
            --index;
        }
        prev = *str;
        ++str;
    }
    while (*str < ' ') { // skip non-printables
        ++str;
    }
    return str;
}

const lang_message *lang_get_message(int id) {
    return &data.message_entries[id];
}


char *get_building_translation(int building_type) {
    switch (building_type) {
        case BUILDING_CLEAR_LAND:
            return gettext("Clear land");
        case BUILDING_MENU_FARMS:
            return gettext("Farms");
        case BUILDING_MENU_RAW_MATERIALS:
            return gettext("Raw Materials");
        case BUILDING_MENU_WORKSHOPS:
            return gettext("Workshops");
        case BUILDING_ROAD:
            return gettext("Road");
        case BUILDING_WALL:
            return gettext("Wall");
        case BUILDING_DRAGGABLE_RESERVOIR:
            return gettext("Reservoir");
        case BUILDING_HOUSE_SMALL_TENT:
            return gettext("House");
        case BUILDING_HOUSE_LARGE_TENT:
            return gettext("Large Tent");
        case BUILDING_HOUSE_SMALL_SHACK:
            return gettext("Small Shack");
        case BUILDING_HOUSE_LARGE_SHACK:
            return gettext("Large Shack");
        case BUILDING_HOUSE_SMALL_HOVEL:
            return gettext("Small Hovel");
        case BUILDING_HOUSE_LARGE_HOVEL:
            return gettext("Large Hovel");
        case BUILDING_HOUSE_SMALL_CASA:
            return gettext("Small Casa");
        case BUILDING_HOUSE_LARGE_CASA:
            return gettext("Large Casa");
        case BUILDING_HOUSE_SMALL_INSULA:
            return gettext("Small Insula");
        case BUILDING_HOUSE_LARGE_INSULA:
            return gettext("Large Insula");
        case BUILDING_HOUSE_SMALL_VILLA:
            return gettext("Small Villa");
        case BUILDING_HOUSE_LARGE_VILLA:
            return gettext("Large Villa");
        case BUILDING_HOUSE_SMALL_PALACE:
            return gettext("Small Palace");
        case BUILDING_HOUSE_LARGE_PALACE:
            return gettext("Large Palace");
        case BUILDING_HOUSE_GRAND_VILLA:
            return gettext("Grand Villa");
        case BUILDING_HOUSE_LUXURY_PALACE:
            return gettext("Luxury Palace");
        case BUILDING_AMPHITHEATER:
            return gettext("Amphitheater");
        case BUILDING_THEATER:
            return gettext("Theater");
        case BUILDING_COLOSSEUM:
            return gettext("Colosseum");
        case BUILDING_HIPPODROME:
            return gettext("Hippodrome");
        case BUILDING_GLADIATOR_SCHOOL:
            return gettext("Gladiator School");
        case BUILDING_LION_HOUSE:
            return gettext("Lion House");
        case BUILDING_ACTOR_COLONY:
            return gettext("Actor Colony");
        case BUILDING_CHARIOT_MAKER:
            return gettext("Chariot Maker");
        case BUILDING_PLAZA:
            return gettext("Plaza");
        case BUILDING_GARDENS:
            return gettext("Gardens");
        case BUILDING_SMALL_STATUE:
            return gettext("Small Statue");
        case BUILDING_MEDIUM_STATUE:
            return gettext("Medium Statue");
        case BUILDING_LARGE_STATUE:
            return gettext("Large Statue");
        case BUILDING_FORT_JAVELIN:
            return gettext("Auxiliaries - Javelin");
        case BUILDING_FORT_MOUNTED:
            return gettext("Auxiliaries - Mounted");
        case BUILDING_DOCTOR:
            return gettext("Doctor");
        case BUILDING_HOSPITAL:
            return gettext("Hospital");
        case BUILDING_BATHHOUSE:
            return gettext("Bathhouse");
        case BUILDING_BARBER:
            return gettext("Barber");
        case BUILDING_SCHOOL:
            return gettext("School");
        case BUILDING_ACADEMY:
            return gettext("Academy");
        case BUILDING_LIBRARY:
            return gettext("Library");
        case BUILDING_FORT_GROUND:
            return gettext("Legionaries");
        case BUILDING_PREFECTURE:
            return gettext("Prefecture");
        case BUILDING_TRIUMPHAL_ARCH:
            return gettext("Triumphal Arch");
        case BUILDING_FORT:
            return gettext("Fort");
        case BUILDING_GATEHOUSE:
            return gettext("Gatehouse");
        case BUILDING_TOWER:
            return gettext("Tower");
        case BUILDING_SMALL_TEMPLE_CERES:
            return gettext("Small Temple of Ceres");
        case BUILDING_SMALL_TEMPLE_NEPTUNE:
            return gettext("Small Temple of Neptune");
        case BUILDING_SMALL_TEMPLE_MERCURY:
            return gettext("Small Temple of Mercury");
        case BUILDING_SMALL_TEMPLE_MARS:
            return gettext("Small Temple of Mars");
        case BUILDING_SMALL_TEMPLE_VENUS:
            return gettext("Small Temple of Venus");
        case BUILDING_LARGE_TEMPLE_CERES:
            return gettext("Large Temple of Ceres");
        case BUILDING_LARGE_TEMPLE_NEPTUNE:
            return gettext("Large Temple of Neptune");
        case BUILDING_LARGE_TEMPLE_MERCURY:
            return gettext("Large Temple of Mercury");
        case BUILDING_LARGE_TEMPLE_MARS:
            return gettext("Large Temple of Mars");
        case BUILDING_LARGE_TEMPLE_VENUS:
            return gettext("Large Temple of Venus");
        case BUILDING_MARKET:
            return gettext("Market");
        case BUILDING_GRANARY:
            return gettext("Granary");
        case BUILDING_WAREHOUSE:
            return gettext("Warehouse");
        case BUILDING_SHIPYARD:
            return gettext("Shipyard");
        case BUILDING_DOCK:
            return gettext("Dock");
        case BUILDING_WHARF:
            return gettext("Wharf");
        case BUILDING_GOVERNORS_HOUSE:
            return gettext("Governor's House");
        case BUILDING_GOVERNORS_VILLA:
            return gettext("Governor's Villa");
        case BUILDING_GOVERNORS_PALACE:
            return gettext("Governor's Palace");
        case BUILDING_MISSION_POST:
            return gettext("Mission Post");
        case BUILDING_ENGINEERS_POST:
            return gettext("Engineer's Post");
        case BUILDING_LOW_BRIDGE:
            return gettext("Low Bridge");
        case BUILDING_SHIP_BRIDGE:
            return gettext("Ship Bridge");
        case BUILDING_SENATE:
        case BUILDING_SENATE_UPGRADED:
            return gettext("Senate");
        case BUILDING_FORUM:
            return gettext("Forum");
        case BUILDING_NATIVE_HUT:
            return gettext("Native Hut");
        case BUILDING_RESERVOIR:
            return gettext("Reservoir");
        case BUILDING_FOUNTAIN:
            return gettext("Fountain");
        case BUILDING_WELL:
            return gettext("Well");
        case BUILDING_NATIVE_CROPS:
            return gettext("Native Crops");
        case BUILDING_MILITARY_ACADEMY:
            return gettext("Military Academy");
        case BUILDING_BARRACKS:
            return gettext("Barracks");
        case BUILDING_MENU_SMALL_TEMPLES:
            return gettext("Small Temples");
        case BUILDING_MENU_LARGE_TEMPLES:
            return gettext("Large Temples");
        case BUILDING_ORACLE:
            return gettext("Oracle");
        case BUILDING_BURNING_RUIN:
            return gettext("Burning Ruin");
        case BUILDING_WHEAT_FARM:
            return gettext("Wheat Farm");
        case BUILDING_VEGETABLE_FARM:
            return gettext("Vegetable Farm");
        case BUILDING_FRUIT_FARM:
            return gettext("Fruit Farm");
        case BUILDING_OLIVE_FARM:
            return gettext("Olive Farm");
        case BUILDING_VINES_FARM:
            return gettext("Vines Farm");
        case BUILDING_PIG_FARM:
            return gettext("Pig Farm");
        case BUILDING_MARBLE_QUARRY:
            return gettext("Marble Quarry");
        case BUILDING_IRON_MINE:
            return gettext("Iron Mine");
        case BUILDING_TIMBER_YARD:
            return gettext("Timber Yard");
        case BUILDING_CLAY_PIT:
            return gettext("Clay Pit");
        case BUILDING_WINE_WORKSHOP:
            return gettext("Wine Workshop");
        case BUILDING_OIL_WORKSHOP:
            return gettext("Oil Workshop");
        case BUILDING_WEAPONS_WORKSHOP:
            return gettext("Weapons Workshop");
        case BUILDING_FURNITURE_WORKSHOP:
            return gettext("Furniture Workshop");
        case BUILDING_POTTERY_WORKSHOP:
            return gettext("Pottery Workshop");
        case BUILDING_ROADBLOCK:
            return gettext("Roadblock");
        case BUILDING_WORKCAMP:
            return gettext("Workcamp");
        case BUILDING_GRAND_TEMPLE_CERES:
            return gettext("Grand Temple of Ceres");
        case BUILDING_GRAND_TEMPLE_NEPTUNE:
            return gettext("Grand Temple of Neptune");
        case BUILDING_GRAND_TEMPLE_MERCURY:
            return gettext("Grand Temple of Mercury");
        case BUILDING_GRAND_TEMPLE_MARS:
            return gettext("Grand Temple of Mars");
        case BUILDING_GRAND_TEMPLE_VENUS:
            return gettext("Grand Temple of Venus");
        case BUILDING_MENU_GRAND_TEMPLES:
            return gettext("Grand Temples");
        case BUILDING_MENU_TREES:
            return gettext("Trees");
        case BUILDING_MENU_PATHS:
            return gettext("Paths");
        case BUILDING_MENU_PARKS:
            return gettext("Parks");
        case BUILDING_SMALL_POND:
            return gettext("Small Pond");
        case BUILDING_LARGE_POND:
            return gettext("Large Pond");
        case BUILDING_PINE_TREE:
            return gettext("Pine Tree");
        case BUILDING_FIR_TREE:
            return gettext("Fir Tree");
        case BUILDING_OAK_TREE:
            return gettext("Oak Tree");
        case BUILDING_ELM_TREE:
            return gettext("Elm Tree");
        case BUILDING_FIG_TREE:
            return gettext("Fig Tree");
        case BUILDING_PLUM_TREE:
            return gettext("Plum Tree");
        case BUILDING_PALM_TREE:
            return gettext("Palm Tree");
        case BUILDING_DATE_TREE:
            return gettext("Date Tree");
        case BUILDING_PINE_PATH:
            return gettext("Pine Path");
        case BUILDING_FIR_PATH:
            return gettext("Fir Path");
        case BUILDING_OAK_PATH:
            return gettext("Oak Path");
        case BUILDING_ELM_PATH:
            return gettext("Elm Path");
        case BUILDING_FIG_PATH:
            return gettext("Fig Path");
        case BUILDING_PLUM_PATH:
            return gettext("Plum Path");
        case BUILDING_PALM_PATH:
            return gettext("Palm Path");
        case BUILDING_DATE_PATH:
            return gettext("Date Path");
        case BUILDING_PAVILION_BLUE:
            return gettext("Blue Pavilion");
        case BUILDING_PAVILION_ORANGE:
            return gettext("Orange Pavilion");
        case BUILDING_PAVILION_GREEN:
            return gettext("Green Pavilion");
        case BUILDING_PAVILION_RED:
            return gettext("Red Pavilion");
        case BUILDING_SMALL_STATUE_ALT:
            return gettext("Small Statue");
        case BUILDING_SMALL_STATUE_ALT_B:
            return gettext("Small Statue");
        case BUILDING_OBELISK:
            return gettext("Obelisk");
        case BUILDING_PANTHEON:
            return gettext("Pantheon");
        case BUILDING_ARCHITECT_GUILD:
            return gettext("Architect's Guild");
        case BUILDING_MESS_HALL:
            return gettext("Mess Hall");
        case BUILDING_LIGHTHOUSE:
            return gettext("Lighthouse");
        case BUILDING_MENU_STATUES:
            return gettext("Statues");
        case BUILDING_MENU_GOV_RES:
            return gettext("Governor's Residence");
        case BUILDING_TAVERN:
            return gettext("Tavern");
        case BUILDING_GRAND_GARDEN:
            return gettext("Grand Garden");
        case BUILDING_ARENA:
            return gettext("Arena");
        case BUILDING_HORSE_STATUE:
            return gettext("Horse Statue");
        case BUILDING_DOLPHIN_FOUNTAIN:
            return gettext("Dolphin Fountain");
        case BUILDING_HEDGE_DARK:
            return gettext("Dark Hedge");
        case BUILDING_HEDGE_LIGHT:
            return gettext("Light Hedge");
        case BUILDING_GARDEN_WALL:
            return gettext("Garden Wall");
        case BUILDING_LEGION_STATUE:
            return gettext("Legion Statue");
        case BUILDING_DECORATIVE_COLUMN:
            return gettext("Decorative Column");
        case BUILDING_COLONNADE:
            return gettext("Colonnade");
        case BUILDING_LARARIUM:
            return gettext("Lararium");
        case BUILDING_NYMPHAEUM:
            return gettext("Nymphaeum");
        case BUILDING_SMALL_MAUSOLEUM:
            return gettext("Small Mausoleum");
        case BUILDING_LARGE_MAUSOLEUM:
            return gettext("Large Mausoleum");
        case BUILDING_WATCHTOWER:
            return gettext("Watchtower");
        case BUILDING_PALISADE:
            return gettext("Palisade");
        case BUILDING_GARDEN_PATH:
            return gettext("Garden Path");
        case BUILDING_CARAVANSERAI:
            return gettext("Caravanserai");
        case BUILDING_ROOFED_GARDEN_WALL:
            return gettext("Roofed Garden Wall");
        case BUILDING_GARDEN_WALL_GATE:
            return gettext("Garden Wall Gate");
        case BUILDING_HEDGE_GATE_DARK:
            return gettext("Dark Hedge Gate");
        case BUILDING_HEDGE_GATE_LIGHT:
            return gettext("Light Hedge Gate");
        case BUILDING_PALISADE_GATE:
            return gettext("Palisade Gate");
        case BUILDING_GLADIATOR_STATUE:
            return gettext("Gladiator Statue");
        case BUILDING_HIGHWAY:
            return gettext("Highway");
        case BUILDING_GOLD_MINE:
            return gettext("Gold Mine");
        case BUILDING_CITY_MINT:
            return gettext("City Mint");
    }
    return "Unknown";
}