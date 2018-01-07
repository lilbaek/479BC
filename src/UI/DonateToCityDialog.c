#include "AllWindows.h"
#include "../Widget.h"
#include "../Graphics.h"
#include "Advisors_private.h"

#include "city/finance.h"
#include "core/calc.h"
#include "game/resource.h"
#include "graphics/arrow_button.h"
#include "graphics/generic_button.h"
#include "graphics/graphics.h"

static void buttonSetAmount(int param1, int param2);
static void buttonDonate(int param1, int param2);
static void buttonCancel(int param1, int param2);
static void arrowButtonAmount(int isDown, int param2);

static generic_button buttons[] = {
	{336, 283, 496, 303, GB_IMMEDIATE, buttonCancel, button_none, 0, 0},
	{144, 283, 304, 303, GB_IMMEDIATE, buttonDonate, button_none, 0, 0},
	{128, 216, 192, 236, GB_IMMEDIATE, buttonSetAmount, button_none, 0, 0},
	{208, 216, 272, 236, GB_IMMEDIATE, buttonSetAmount, button_none, 1, 0},
	{288, 216, 352, 236, GB_IMMEDIATE, buttonSetAmount, button_none, 2, 0},
	{368, 216, 432, 236, GB_IMMEDIATE, buttonSetAmount, button_none, 3, 0},
	{448, 216, 512, 236, GB_IMMEDIATE, buttonSetAmount, button_none, 4, 0},
};

static arrow_button arrowButtons[] = {
	{240, 242, 17, 24, arrowButtonAmount, 1, 0},
	{264, 242, 15, 24, arrowButtonAmount, 0, 0},
};

static int focusButtonId;
static int arrowButtonFocus;

void UI_DonateToCityDialog_init()
{
	if (Data_CityInfo.donateAmount > Data_CityInfo.personalSavings) {
		Data_CityInfo.donateAmount = Data_CityInfo.personalSavings;
	}
}

void UI_DonateToCityDialog_drawBackground()
{
	UI_Advisor_drawGeneralBackground();

	int baseOffsetX = Data_Screen.offset640x480.x;
	int baseOffsetY = Data_Screen.offset640x480.y;

	Widget_Panel_drawOuterPanel(baseOffsetX + 64, baseOffsetY + 160, 32, 10);
	Graphics_drawImage(image_group(GROUP_RESOURCE_ICONS) + RESOURCE_DENARII,
		baseOffsetX + 96, baseOffsetY + 176);
	Widget_GameText_drawCentered(52, 16,
		baseOffsetX + 80, baseOffsetY + 176, 496, FONT_LARGE_BLACK);

	Widget_Panel_drawInnerPanel(baseOffsetX + 112, baseOffsetY + 208, 26, 4);

	Widget_Text_drawNumberCentered(0, baseOffsetX + 124, baseOffsetY + 221, 64, FONT_NORMAL_WHITE);
	Widget_Text_drawNumberCentered(500, baseOffsetX + 204, baseOffsetY + 221, 64, FONT_NORMAL_WHITE);
	Widget_Text_drawNumberCentered(2000, baseOffsetX + 284, baseOffsetY + 221, 64, FONT_NORMAL_WHITE);
	Widget_Text_drawNumberCentered(5000, baseOffsetX + 364, baseOffsetY + 221, 64, FONT_NORMAL_WHITE);
	Widget_GameText_drawCentered(52, 19, baseOffsetX + 444, baseOffsetY + 221, 64, FONT_NORMAL_WHITE);

	Widget_GameText_draw(52, 17, baseOffsetX + 128, baseOffsetY + 248, FONT_NORMAL_WHITE);
	Widget_Text_drawNumber(Data_CityInfo.donateAmount, '@', " ",
		baseOffsetX + 316, baseOffsetY + 248, FONT_NORMAL_WHITE);

	Widget_GameText_drawCentered(13, 4,
		baseOffsetX + 336, baseOffsetY + 288, 160, FONT_NORMAL_BLACK);
	Widget_GameText_drawCentered(52, 18,
		baseOffsetX + 144, baseOffsetY + 288, 160, FONT_NORMAL_BLACK);
}

void UI_DonateToCityDialog_drawForeground()
{
    graphics_in_dialog();

	button_border_draw(128, 216, 64, 20, focusButtonId == 3);
	button_border_draw(208, 216, 64, 20, focusButtonId == 4);
	button_border_draw(288, 216, 64, 20, focusButtonId == 5);
	button_border_draw(368, 216, 64, 20, focusButtonId == 6);
	button_border_draw(448, 216, 64, 20, focusButtonId == 7);

	button_border_draw(336, 283, 160, 20, focusButtonId == 1);
	button_border_draw(144, 283, 160, 20, focusButtonId == 2);

	arrow_buttons_draw(0, 0, arrowButtons, 2);

    graphics_reset_dialog();
}

void UI_DonateToCityDialog_handleMouse(const mouse *m)
{
	arrowButtonFocus = 0;
	if (m->right.went_up) {
		UI_Window_goTo(Window_Advisors);
	} else {
		const mouse *m_dialog = mouse_in_dialog(m);
		if (!generic_buttons_handle_mouse(m_dialog, 0, 0, buttons, 7, &focusButtonId)) {
			arrowButtonFocus = arrow_buttons_handle_mouse(m_dialog, 0, 0, arrowButtons, 2);
		}
	}
}

static void buttonSetAmount(int param1, int param2)
{
	int amount;
	switch (param1) {
		case 0: amount = 0; break;
		case 1: amount = 500; break;
		case 2: amount = 2000; break;
		case 3: amount = 5000; break;
		case 4: amount = 1000000; break;
		default: return;
	}
	Data_CityInfo.donateAmount = calc_bound(amount, 0, Data_CityInfo.personalSavings);
	UI_Window_requestRefresh();
}

static void buttonDonate(int param1, int param2)
{
    city_finance_process_donation(Data_CityInfo.donateAmount);
    Data_CityInfo.personalSavings -= Data_CityInfo.donateAmount;
	city_finance_calculate_totals();
	UI_Window_goTo(Window_Advisors);
}

static void buttonCancel(int param1, int param2)
{
	UI_Window_goTo(Window_Advisors);
}

static void arrowButtonAmount(int isDown, int param2)
{
	if (isDown == 0) {
		Data_CityInfo.donateAmount += 10;
	} else if (isDown == 1) {
		Data_CityInfo.donateAmount -= 10;
	}
	Data_CityInfo.donateAmount = calc_bound(Data_CityInfo.donateAmount, 0, Data_CityInfo.personalSavings);
	UI_Window_requestRefresh();
}

void UI_DonateToCityDialog_getTooltip(struct TooltipContext *c)
{
	if (!focusButtonId && !arrowButtonFocus) {
		return;
	}
	c->type = TooltipType_Button;
	if (focusButtonId == 1) {
		c->textId = 98;
	} else if (focusButtonId == 2) {
		c->textId = 99;
	} else if (focusButtonId) {
		c->textId = 100;
	} else if (arrowButtonFocus) {
		c->textId = 101;
	}
}
