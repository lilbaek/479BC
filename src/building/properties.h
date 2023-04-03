#ifndef BUILDING_PROPERTIES_H
#define BUILDING_PROPERTIES_H

#include "building/type.h"

typedef struct {
    int size;
    int fire_proof;
    int image_group;
    int image_offset;
    int rotation_offset;
    int building_id;
    char* asset_name;
    char* asset_image_id;
} building_properties;

typedef struct {
    int type;
    building_properties properties;
    const char* asset_name;
    const char* asset_image_id;
} Tiberius_building_properties_mapping;

const building_properties *building_properties_for_type(building_type type);

void init_Tiberius_building_properties(void);

#endif // BUILDING_PROPERTIES_H
