#ifndef SCENARIO_EDITOR_MAP_H
#define SCENARIO_EDITOR_MAP_H

#include "map/point.h"

void scenario_editor_set_entry_point(int x, int y);
void scenario_editor_set_exit_point(int x, int y);

void scenario_editor_set_river_entry_point(int x, int y);
void scenario_editor_set_river_exit_point(int x, int y);

void scenario_editor_clear_herd_points(void);
map_point scenario_editor_herd_point(int id);
void scenario_editor_set_herd_point(int id, int x, int y);

void scenario_editor_clear_fishing_points(void);
map_point scenario_editor_fishing_point(int id);
void scenario_editor_set_fishing_point(int id, int x, int y);

int scenario_editor_count_invasion_points(void);
void scenario_editor_clear_invasion_points(void);
map_point scenario_editor_invasion_point(int id);
void scenario_editor_set_invasion_point(int id, int x, int y);

void scenario_editor_updated_terrain(void);

#endif // SCENARIO_EDITOR_MAP_H
