#include "BuildingInfo.h"

#include "building/building.h"
#include "game/resource.h"
#include "graphics/image.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/text.h"

void UI_BuildingInfo_drawForum(BuildingInfoContext *c)
{
	c->helpId = 76;
	window_building_play_sound(c, "wavs/forum.wav");
	outer_panel_draw(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	lang_text_draw_centered(106, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, FONT_LARGE_BLACK);
	image_draw(image_group(GROUP_RESOURCE_ICONS) + RESOURCE_DENARII,
		c->xOffset + 16, c->yOffset + 36);

	building *b = building_get(c->buildingId);
	int width = lang_text_draw(106, 2,
		c->xOffset + 44, c->yOffset + 43, FONT_NORMAL_BLACK);
	lang_text_draw_amount(8, 0, b->taxIncomeOrStorage,
		c->xOffset + 44 + width, c->yOffset + 43, FONT_NORMAL_BLACK);

	if (!c->hasRoadAccess) {
		window_building_draw_description(c, 69, 25);
	} else if (b->numWorkers <= 0) {
		window_building_draw_description_at(c, 72, 106, 10);
	} else if (c->workerPercentage >= 100) {
		window_building_draw_description_at(c, 72, 106, 5);
	} else if (c->workerPercentage >= 75) {
		window_building_draw_description_at(c, 72, 106, 6);
	} else if (c->workerPercentage >= 50) {
		window_building_draw_description_at(c, 72, 106, 7);
	} else if (c->workerPercentage >= 25) {
		window_building_draw_description_at(c, 72, 106, 8);
	} else {
		window_building_draw_description_at(c, 72, 106, 9);
	}

	inner_panel_draw(c->xOffset + 16, c->yOffset + 136, c->widthBlocks - 2, 4);
	window_building_draw_employment(c, c->yOffset + 142);
}

void UI_BuildingInfo_drawSenate(BuildingInfoContext *c)
{
	c->advisor = 1;
	c->helpId = 77;
	window_building_play_sound(c, "wavs/senate.wav");
	outer_panel_draw(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	lang_text_draw_centered(105, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, FONT_LARGE_BLACK);
	image_draw(image_group(GROUP_RESOURCE_ICONS) + RESOURCE_DENARII,
		c->xOffset + 16, c->yOffset + 36);

	building *b = building_get(c->buildingId);
	int width = lang_text_draw(105, 2,
		c->xOffset + 44, c->yOffset + 43, FONT_NORMAL_BLACK);
	lang_text_draw_amount(8, 0, b->taxIncomeOrStorage,
		c->xOffset + 44 + width, c->yOffset + 43, FONT_NORMAL_BLACK);

	if (!c->hasRoadAccess) {
		window_building_draw_description(c, 69, 25);
	} else if (b->numWorkers <= 0) {
		window_building_draw_description_at(c, 72, 106, 10);
	} else if (c->workerPercentage >= 100) {
		window_building_draw_description_at(c, 72, 106, 5);
	} else if (c->workerPercentage >= 75) {
		window_building_draw_description_at(c, 72, 106, 6);
	} else if (c->workerPercentage >= 50) {
		window_building_draw_description_at(c, 72, 106, 7);
	} else if (c->workerPercentage >= 25) {
		window_building_draw_description_at(c, 72, 106, 8);
	} else {
		window_building_draw_description_at(c, 72, 106, 9);
	}

	inner_panel_draw(c->xOffset + 16, c->yOffset + 136, c->widthBlocks - 2, 4);
	window_building_draw_employment(c, c->yOffset + 142);

	lang_text_draw(105, 3, c->xOffset + 60, c->yOffset + 220, FONT_NORMAL_BLACK);
}

void UI_BuildingInfo_drawGovernorsHome(BuildingInfoContext *c)
{
	c->helpId = 78;
	window_building_play_sound(c, "wavs/gov_palace.wav");
	outer_panel_draw(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	lang_text_draw_centered(103, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, FONT_LARGE_BLACK);
	lang_text_draw_multiline(103, 1,
		c->xOffset + 32, c->yOffset + 16 * c->heightBlocks - 143,
		16 * (c->widthBlocks - 4), FONT_NORMAL_BLACK);
}
