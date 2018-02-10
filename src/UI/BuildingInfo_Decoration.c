#include "BuildingInfo.h"

#include "graphics/lang_text.h"
#include "graphics/panel.h"

void UI_BuildingInfo_drawGarden(BuildingInfoContext *c)
{
	c->helpId = 80;
	window_building_play_sound(c, "wavs/park.wav");
	outer_panel_draw(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	lang_text_draw_centered(79, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, FONT_LARGE_BLACK);
	window_building_draw_description_at(c, 16 * c->heightBlocks - 158, 79, 1);
}

void UI_BuildingInfo_drawPlaza(BuildingInfoContext *c)
{
	c->helpId = 80;
	window_building_play_sound(c, "wavs/plaza.wav");
	UI_BuildingInfo_drawFigureImagesLocal(c);
	outer_panel_draw(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	lang_text_draw_centered(137, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, FONT_LARGE_BLACK);
	UI_BuildingInfo_drawFigureList(c);
	window_building_draw_description_at(c, 16 * c->heightBlocks - 113, 137, 1);
}

void UI_BuildingInfo_drawStatue(BuildingInfoContext *c)
{
	c->helpId = 79;
	window_building_play_sound(c, "wavs/statue.wav");
	UI_BuildingInfo_drawFigureImagesLocal(c);
	outer_panel_draw(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	lang_text_draw_centered(80, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, FONT_LARGE_BLACK);
	window_building_draw_description_at(c, 16 * c->heightBlocks - 158, 80, 1);
}

void UI_BuildingInfo_drawTriumphalArch(BuildingInfoContext *c)
{
	c->helpId = 79;
	window_building_play_sound(c, "wavs/statue.wav");
	UI_BuildingInfo_drawFigureImagesLocal(c);
	outer_panel_draw(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	lang_text_draw_centered(80, 2, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, FONT_LARGE_BLACK);
	window_building_draw_description_at(c, 16 * c->heightBlocks - 158, 80, 3);
}

