#include <FL/fl_ask.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Button.H>
#include <cstdlib>
#include "CustomWindow.h"
#include "GameWindow.h"

// 整数を文字列に変換
const char *int_to_string(int n)
{
	static char str[4];
#ifdef WIN32
	sprintf_s(str, sizeof(str), "%d", n);
#else
	std::snprintf(str, sizeof(str), "%d", n);
#endif
	return str;
}

// コンストラクタ
CustomWindow::CustomWindow(GameWindow *g) :
	Fl_Window(180, 160, "Custom Size"), game(g)
{
	set_modal();
	begin();
	width = new Fl_Int_Input(60, 20, 100, 25, "Width ");
	height = new Fl_Int_Input(60, 55, 100, 25, "Height ");
	mines = new Fl_Int_Input(60, 90, 100, 25, "Mines ");
	btn = new Fl_Button(50, 125, 80, 25, "OK");
	btn->callback(cb_btn, this);
	end();
	
	int w, h, m;
	game->get_board_size(&w, &h, &m);
	width->value(int_to_string(w));
	height->value(int_to_string(h));
	mines->value(int_to_string(m));
	
	show();
}

// コールバック関数
void CustomWindow::cb_btn(Fl_Widget *wid, void *p)
{
	CustomWindow *cw = static_cast<CustomWindow*>(p);
	
	int w = std::strtol(cw->width->value(), 0, 0);
	int h = std::strtol(cw->height->value(), 0, 0);
	int m = std::strtol(cw->mines->value(), 0, 0);

	if (w > 0 && w < 100 && h > 0 && h < 100 && w * h > m) {
		cw->game->width = w;
		cw->game->height = h;
		cw->game->num_mines = m;
		cw->hide();
		cw->game->strategy->on_change_level(GAME_CUSTOM);
	}
	else {
		fl_message("Invalid Values.");
	}
}

