#include "core/textures.h"
#include "sds.h"
#include "assets/assets.h"

/*
 * Returns image id from terrain group. Frees image_name automatically
 */
unsigned int get_terrain_image_id(sds image_name) {
    int i = assets_get_image_id(TEXTURE_TERRAIN_NAME, image_name);
    sdsfree(image_name);
    return i;
}

/*
 * Returns image id from basic group. Frees image_name automatically
 */
unsigned int get_basic_image_id(sds image_name) {
    int i = assets_get_image_id(TEXTURE_BASIC_NAME, image_name);
    sdsfree(image_name);
    return i;
}

sds get_image_name(char *prefix, int index) {
    index += 1; // We use 1 indexed naming
    sds image_name = sdsnew(prefix);
    if (index < 10) {
        image_name = sdscatprintf(image_name, "0%d", index);
    } else {
        image_name = sdscatprintf(image_name, "%d", index);
    }
    return image_name;
}

unsigned int get_terrain_image_id_prefix(char *prefix, int index) {
    return get_terrain_image_id(get_image_name(prefix, index));
}

unsigned int get_basic_image_id_prefix(char *prefix, int index) {
    return get_basic_image_id(get_image_name(prefix, index));
}

unsigned int get_housing_image_id_prefix(char *prefix, int index) {
    sds name = get_image_name(prefix, index);
    int id = assets_get_image_id(TEXTURE_HOUSING_NAME, name);
    sdsfree(name);
    return id;
}

unsigned int get_building_image_id_prefix(char *prefix, int index) {
    sds name = get_image_name(prefix, index);
    int id = assets_get_image_id(TEXTURE_BUILDING_NAME, name);
    sdsfree(name);
    return id;
}

unsigned int get_image_id_prefix(char *assetlist_name, char *prefix, int index) {
    sds name = get_image_name(prefix, index);
    int id = assets_get_image_id(assetlist_name, name);
    sdsfree(name);
    return id;
}