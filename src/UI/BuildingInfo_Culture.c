#include "BuildingInfo.h"

#include "building/building.h"
#include "graphics/image.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"

static void drawInfo(BuildingInfoContext *c, int helpId, const char *soundFile, int groupId)
{
	c->helpId = helpId;
	window_building_play_sound(c, soundFile);
	outer_panel_draw(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	lang_text_draw_centered(groupId, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, FONT_LARGE_BLACK);

	if (!c->hasRoadAccess) {
		window_building_draw_description(c, 69, 25);
	} else if (building_get(c->buildingId)->numWorkers <= 0) {
		window_building_draw_description(c, groupId, 2);
	} else {
		window_building_draw_description(c, groupId, 3);
	}
	inner_panel_draw(c->xOffset + 16, c->yOffset + 136, c->widthBlocks - 2, 4);
	window_building_draw_employment(c, 142);
}

void UI_BuildingInfo_drawClinic(BuildingInfoContext *c)
{
	drawInfo(c, 65, "wavs/clinic.wav", 81);
}

void UI_BuildingInfo_drawHospital(BuildingInfoContext *c)
{
	drawInfo(c, 66, "wavs/hospital.wav", 82);
}

void UI_BuildingInfo_drawBathhouse(BuildingInfoContext *c)
{
	c->helpId = 64;
	window_building_play_sound(c, "wavs/baths.wav");
	outer_panel_draw(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	lang_text_draw_centered(83, 0, c->xOffset, c->yOffset + 10, 16 * c->widthBlocks, FONT_LARGE_BLACK);

    building *b = building_get(c->buildingId);
	if (!b->hasWaterAccess) {
		window_building_draw_description(c, 83, 4);
	} else if (!c->hasRoadAccess) {
		window_building_draw_description(c, 69, 25);
	} else if (b->numWorkers <= 0) {
		window_building_draw_description(c, 83, 2);
	} else {
		window_building_draw_description(c, 83, 3);
	}
	inner_panel_draw(c->xOffset + 16, c->yOffset + 136, c->widthBlocks - 2, 4);
	window_building_draw_employment(c, 142);
}

void UI_BuildingInfo_drawBarber(BuildingInfoContext *c)
{
	drawInfo(c, 63, "wavs/barber.wav", 84);
}

void UI_BuildingInfo_drawSchool(BuildingInfoContext *c)
{
	drawInfo(c, 68, "wavs/school.wav", 85);
}

void UI_BuildingInfo_drawAcademy(BuildingInfoContext *c)
{
	drawInfo(c, 69, "wavs/academy.wav", 86);
}

void UI_BuildingInfo_drawLibrary(BuildingInfoContext *c)
{
	drawInfo(c, 70, "wavs/library.wav", 87);
}

static void drawTemple(BuildingInfoContext *c, const char *soundFile, int groupId, int graphicOffset)
{
	c->helpId = 67;
	window_building_play_sound(c, soundFile);
	outer_panel_draw(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	lang_text_draw_centered(groupId, 0, c->xOffset, c->yOffset + 12, 16 * c->widthBlocks, FONT_LARGE_BLACK);
	inner_panel_draw(c->xOffset + 16, c->yOffset + 56, c->widthBlocks - 2, 4);
	window_building_draw_employment(c, 62);
	if (c->hasRoadAccess) {
		image_draw(graphicOffset + image_group(GROUP_PANEL_WINDOWS),
			c->xOffset + 190, c->yOffset + 16 * c->heightBlocks - 118);
	} else {
		window_building_draw_description_at(c, 16 * c->heightBlocks - 128, 69, 25);
	}
}

void UI_BuildingInfo_drawTempleCeres(BuildingInfoContext *c)
{
	drawTemple(c, "wavs/temple_farm.wav", 92, 21);
}

void UI_BuildingInfo_drawTempleNeptune(BuildingInfoContext *c)
{
	drawTemple(c, "wavs/temple_ship.wav", 93, 22);
}

void UI_BuildingInfo_drawTempleMercury(BuildingInfoContext *c)
{
	drawTemple(c, "wavs/temple_comm.wav", 94, 23);
}

void UI_BuildingInfo_drawTempleMars(BuildingInfoContext *c)
{
	drawTemple(c, "wavs/temple_war.wav", 95, 24);
}

void UI_BuildingInfo_drawTempleVenus(BuildingInfoContext *c)
{
	drawTemple(c, "wavs/temple_love.wav", 96, 25);
}

void UI_BuildingInfo_drawOracle(BuildingInfoContext *c)
{
	c->helpId = 67;
	window_building_play_sound(c, "wavs/oracle.wav");
	outer_panel_draw(c->xOffset, c->yOffset, c->widthBlocks, c->heightBlocks);
	lang_text_draw_centered(110, 0, c->xOffset, c->yOffset + 12, 16 * c->widthBlocks, FONT_LARGE_BLACK);
	lang_text_draw_centered(13, 1,
		c->xOffset, c->yOffset + 16 * c->heightBlocks - 24,
		16 * c->widthBlocks, FONT_NORMAL_BLACK);
	window_building_draw_description_at(c, 16 * c->heightBlocks - 143, 110, 1);
}

