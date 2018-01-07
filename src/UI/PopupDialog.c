#include "PopupDialog.h"
#include "Window.h"
#include "../Data/Screen.h"

#include "graphics/image_button.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"

#define GROUP 5

static void buttonOk(int param1, int param2);
static void buttonCancel(int param1, int param2);

static image_button buttons[] = {
	{192, 100, 34, 34, IB_NORMAL, 96, 0, buttonOk, button_none, 1, 0, 1},
	{256, 100, 34, 34, IB_NORMAL, 96, 4, buttonCancel, button_none, 0, 0, 1},
};

static struct {
	int msgId;
	int okClicked;
	void (*closeFunc)(int accepted);
	int hasButtons;
} data;

void UI_PopupDialog_show(int msgId, void (*closeFunc)(int accepted), int hasOkCancelButtons)
{
	if (UI_Window_getId() == Window_PopupDialog) {
		// don't show popup over popup
		return;
	}
	data.msgId = msgId;
	data.okClicked = 0;
	data.closeFunc = closeFunc;
	data.hasButtons = hasOkCancelButtons;
	UI_Window_goTo(Window_PopupDialog);
}

void UI_PopupDialog_drawBackground()
{
	int xOffset = Data_Screen.offset640x480.x + 80;
	int yOffset = Data_Screen.offset640x480.y + 80;
	outer_panel_draw(xOffset, yOffset, 30, 10);
	Widget_GameText_drawCentered(GROUP, data.msgId, xOffset, yOffset + 20, 480, FONT_LARGE_BLACK);
	if (Widget_GameText_getWidth(GROUP, data.msgId + 1, FONT_NORMAL_BLACK) >= 420) {
		Widget_GameText_drawMultiline(GROUP, data.msgId + 1, xOffset + 30, yOffset + 60, 420, FONT_NORMAL_BLACK);
	} else {
		Widget_GameText_drawCentered(GROUP, data.msgId + 1, xOffset, yOffset + 60, 480, FONT_NORMAL_BLACK);
	}
}

void UI_PopupDialog_drawForeground()
{
	int xOffset = Data_Screen.offset640x480.x + 80;
	int yOffset = Data_Screen.offset640x480.y + 80;
	if (data.hasButtons) {
		image_buttons_draw(xOffset, yOffset, buttons, 2);
	} else {
		Widget_GameText_drawCentered(13, 1, xOffset, yOffset + 128, 480, FONT_NORMAL_BLACK);
	}
}

void UI_PopupDialog_handleMouse(const mouse *m)
{
	if (data.hasButtons) {
		image_buttons_handle_mouse(mouse_in_dialog(m), 80, 80, buttons, 2, 0);
	} else if (m->right.went_up) {
		data.closeFunc(0);
		UI_Window_goBack();
	}
}

void buttonOk(int param1, int param2)
{
	UI_Window_goBack();
	data.closeFunc(1);
}

void buttonCancel(int param1, int param2)
{
	UI_Window_goBack();
	data.closeFunc(0);
}
