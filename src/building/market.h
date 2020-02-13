#ifndef BUILDING_MARKET_H
#define BUILDING_MARKET_H

#include "building/building.h"
#include "game/resource.h"

int building_market_get_max_food_stock(building *market);
int building_market_get_max_goods_stock(building *market);
int building_market_get_storage_destination(building *market);
int is_good_accepted(inventory_type resource, building *market);
void toggle_good_accepted(inventory_type resource, building *market);
void unaccept_all_goods(building *market);


#endif // BUILDING_MARKET_H
