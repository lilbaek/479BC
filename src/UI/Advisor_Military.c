#include "Advisors_private.h"
#include "Window.h"

#include "city/view.h"
#include "figure/formation_legion.h"
#include "graphics/generic_button.h"
#include "graphics/graphics.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/text.h"
#include "map/grid.h"
#include "scenario/invasion.h"

static void buttonGoToLegion(int legionId, int param2);
static void buttonReturnToFort(int legionId, int param2);
static void buttonEmpireService(int legionId, int param2);

static generic_button fortCustomButtons[] = {
	{400, 83, 430, 113, GB_IMMEDIATE, buttonGoToLegion, button_none, 1, 0},
	{480, 83, 510, 113, GB_IMMEDIATE, buttonReturnToFort, button_none, 1, 0},
	{560, 83, 590, 113, GB_IMMEDIATE, buttonEmpireService, button_none, 1, 0},
	{400, 127, 430, 157, GB_IMMEDIATE, buttonGoToLegion, button_none, 2, 0},
	{480, 127, 510, 157, GB_IMMEDIATE, buttonReturnToFort, button_none, 2, 0},
	{560, 127, 590, 157, GB_IMMEDIATE, buttonEmpireService, button_none, 2, 0},
	{400, 171, 430, 201, GB_IMMEDIATE, buttonGoToLegion, button_none, 3, 0},
	{480, 171, 510, 201, GB_IMMEDIATE, buttonReturnToFort, button_none, 3, 0},
	{560, 171, 590, 201, GB_IMMEDIATE, buttonEmpireService, button_none, 3, 0},
	{400, 215, 430, 245, GB_IMMEDIATE, buttonGoToLegion, button_none, 4, 0},
	{480, 215, 510, 245, GB_IMMEDIATE, buttonReturnToFort, button_none, 4, 0},
	{560, 215, 590, 245, GB_IMMEDIATE, buttonEmpireService, button_none, 4, 0},
	{400, 259, 430, 289, GB_IMMEDIATE, buttonGoToLegion, button_none, 5, 0},
	{480, 259, 510, 289, GB_IMMEDIATE, buttonReturnToFort, button_none, 5, 0},
	{560, 259, 590, 289, GB_IMMEDIATE, buttonEmpireService, button_none, 5, 0},
	{400, 303, 430, 333, GB_IMMEDIATE, buttonGoToLegion, button_none, 6, 0},
	{480, 303, 510, 333, GB_IMMEDIATE, buttonReturnToFort, button_none, 6, 0},
	{560, 303, 590, 333, GB_IMMEDIATE, buttonEmpireService, button_none, 6, 0},
};

static int focusButtonId;
static int numLegions;

void UI_Advisor_Military_drawBackground(int *advisorHeight)
{
	int baseOffsetX = Data_Screen.offset640x480.x;
	int baseOffsetY = Data_Screen.offset640x480.y;

	*advisorHeight = 26;
	outer_panel_draw(baseOffsetX, baseOffsetY, 40, *advisorHeight);
	Graphics_drawImage(image_group(GROUP_ADVISOR_ICONS) + 1, baseOffsetX + 10, baseOffsetY + 10);
	lang_text_draw(51, 0, baseOffsetX + 60, baseOffsetY + 12, FONT_LARGE_BLACK);

	lang_text_draw(51, 1, baseOffsetX + 390, baseOffsetY + 43, FONT_SMALL_PLAIN);
	lang_text_draw(51, 2, baseOffsetX + 390, baseOffsetY + 58, FONT_SMALL_PLAIN);
	lang_text_draw(51, 3, baseOffsetX + 470, baseOffsetY + 43, FONT_SMALL_PLAIN);
	lang_text_draw(51, 4, baseOffsetX + 470, baseOffsetY + 58, FONT_SMALL_PLAIN);
	lang_text_draw(51, 5, baseOffsetX + 550, baseOffsetY + 43, FONT_SMALL_PLAIN);
	lang_text_draw(51, 6, baseOffsetX + 550, baseOffsetY + 58, FONT_SMALL_PLAIN);
	lang_text_draw(138, 36, baseOffsetX + 290, baseOffsetY + 58, FONT_SMALL_PLAIN);

	numLegions = formation_get_num_legions();

    int enemy_text_id;
    if (Data_CityInfo.numEnemiesInCity) {
        enemy_text_id = 10;
    } else if (Data_CityInfo.numImperialSoldiersInCity) {
        enemy_text_id = 11;
    } else if (scenario_invasion_exists_upcoming()) {
        enemy_text_id = 9;
    } else {
        enemy_text_id = 8;
    }
    int distant_battle_text_id;
    if (Data_CityInfo.distantBattleRomanMonthsToReturn > 0) {
        distant_battle_text_id = 15;
    } else if (Data_CityInfo.distantBattleRomanMonthsToTravel > 0) {
        distant_battle_text_id = 14;
    } else if (Data_CityInfo.distantBattleMonthsToBattle > 0) {
        distant_battle_text_id = 13;
    } else {
        distant_battle_text_id = 12;
    }
	if (numLegions <= 0) {
		Graphics_drawImage(image_group(GROUP_BULLET), baseOffsetX + 100, baseOffsetY + 359);
		lang_text_draw(51, enemy_text_id, baseOffsetX + 120, baseOffsetY + 358, FONT_NORMAL_BLACK);

		Graphics_drawImage(image_group(GROUP_BULLET), baseOffsetX + 100, baseOffsetY + 379);
		lang_text_draw(51, distant_battle_text_id, baseOffsetX + 120, baseOffsetY + 378, FONT_NORMAL_BLACK);
	} else {
		// has forts
		Graphics_drawImage(image_group(GROUP_BULLET), baseOffsetX + 100, baseOffsetY + 349);
		int width = lang_text_draw_amount(8, 46, Data_CityInfo.militaryTotalSoldiers,
			baseOffsetX + 120, baseOffsetY + 348, FONT_NORMAL_BLACK);
		width += lang_text_draw(51, 7, baseOffsetX + 120 + width, baseOffsetY + 348, FONT_NORMAL_BLACK);
		lang_text_draw_amount(8, 48, Data_CityInfo.militaryTotalLegions,
			baseOffsetX + 120 + width, baseOffsetY + 348, FONT_NORMAL_BLACK);

		Graphics_drawImage(image_group(GROUP_BULLET), baseOffsetX + 100, baseOffsetY + 369);
		lang_text_draw(51, enemy_text_id, baseOffsetX + 120, baseOffsetY + 368, FONT_NORMAL_BLACK);

		Graphics_drawImage(image_group(GROUP_BULLET), baseOffsetX + 100, baseOffsetY + 389);
		lang_text_draw(51, distant_battle_text_id, baseOffsetX + 120, baseOffsetY + 388, FONT_NORMAL_BLACK);
	}

	inner_panel_draw(baseOffsetX + 32, baseOffsetY + 70, 36, 17);
	if (numLegions <= 0) {
		lang_text_draw_multiline(51, 16, baseOffsetX + 64, baseOffsetY + 200, 496, FONT_NORMAL_GREEN);
		return;
	}

	for (int i = 0; i < numLegions; i++) {
		int formationId = formation_for_legion(i + 1);
		const formation *m = formation_get(formationId);
		button_border_draw(baseOffsetX + 38, baseOffsetY + 77 + 44 * i, 560, 40, 0);
		Graphics_drawImage(image_group(GROUP_FIGURE_FORT_STANDARD_ICONS) + m->legion_id,
			baseOffsetX + 48, baseOffsetY + 82 + 44 * i);
		lang_text_draw(138, m->legion_id,
			baseOffsetX + 100, baseOffsetY + 83 + 44 * i, FONT_NORMAL_WHITE);
		int width = Widget_Text_drawNumber(m->num_figures, '@', " ",
			baseOffsetX + 100, baseOffsetY + 100 + 44 * i, FONT_NORMAL_GREEN);
		switch (m->figure_type) {
			case FIGURE_FORT_LEGIONARY:
				lang_text_draw(138, 33, baseOffsetX + 100 + width, baseOffsetY + 100 + 44 * i, FONT_NORMAL_GREEN);
				break;
			case FIGURE_FORT_MOUNTED:
				lang_text_draw(138, 34, baseOffsetX + 100 + width, baseOffsetY + 100 + 44 * i, FONT_NORMAL_GREEN);
				break;
			case FIGURE_FORT_JAVELIN:
				lang_text_draw(138, 35, baseOffsetX + 100 + width, baseOffsetY + 100 + 44 * i, FONT_NORMAL_GREEN);
				break;
		}
		lang_text_draw_centered(138, 37 + m->morale / 5,
			baseOffsetX + 240, baseOffsetY + 91 + 44 * i, 150, FONT_NORMAL_GREEN);

		int graphicId = image_group(GROUP_FORT_ICONS);
		button_border_draw(baseOffsetX + 400, baseOffsetY + 83 + 44 * i, 30, 30, 0);
		Graphics_drawImage(graphicId, baseOffsetX + 403, baseOffsetY + 86 + 44 * i);

		button_border_draw(baseOffsetX + 480, baseOffsetY + 83 + 44 * i, 30, 30, 0);
		if (m->is_at_fort) {
			Graphics_drawImage(graphicId + 2, baseOffsetX + 483, baseOffsetY + 86 + 44 * i);
		} else {
			Graphics_drawImage(graphicId + 1, baseOffsetX + 483, baseOffsetY + 86 + 44 * i);
		}

		button_border_draw(baseOffsetX + 560, baseOffsetY + 83 + 44 * i, 30, 30, 0);
		if (m->empire_service) {
			Graphics_drawImage(graphicId + 3, baseOffsetX + 563, baseOffsetY + 86 + 44 * i);
		} else {
			Graphics_drawImage(graphicId + 4, baseOffsetX + 563, baseOffsetY + 86 + 44 * i);
		}
	}
}

void UI_Advisor_Military_drawForeground()
{
    graphics_in_dialog();

	numLegions = formation_get_num_legions();
	for (int i = 0; i < numLegions; i++) {
		button_border_draw(400, 83 + 44 * i, 30, 30, focusButtonId == 3 * i + 1);
		button_border_draw(480, 83 + 44 * i, 30, 30, focusButtonId == 3 * i + 2);
		button_border_draw(560, 83 + 44 * i, 30, 30, focusButtonId == 3 * i + 3);
	}
	graphics_reset_dialog();
}

void UI_Advisor_Military_handleMouse(const mouse *m)
{
	generic_buttons_handle_mouse(mouse_in_dialog(m), 0, 0, fortCustomButtons, 3 * numLegions, &focusButtonId);
}

static void buttonGoToLegion(int legionId, int param2)
{
	const formation *m = formation_get(formation_for_legion(legionId));
	city_view_go_to_grid_offset(map_grid_offset(m->x_home, m->y_home));
	UI_Window_goTo(Window_City);
}

static void buttonReturnToFort(int legionId, int param2)
{
	formation *m = formation_get(formation_for_legion(legionId));
	if (!m->in_distant_battle) {
		formation_legion_return_home(m);
		UI_Window_requestRefresh();
	}
}

static void buttonEmpireService(int legionId, int param2)
{
	int formationId = formation_for_legion(legionId);
    formation_toggle_empire_service(formationId);
	formation_calculate_figures();
	UI_Window_requestRefresh();
}

