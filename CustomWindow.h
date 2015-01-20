//------------------------------------------------------------
// CustomWindow
// カスタムサイズ入力用ウインドウ
//------------------------------------------------------------

#ifndef CUSTOMWINDOW_H_INCLUDE
#define CUSTOMWINDOW_H_INCLUDE

#include <FL/Fl_Window.H>
#include <FL/Fl_Int_Input.H>
#include "GameWindow.h"

class CustomWindow : public Fl_Window
{
private:
	GameWindow *game;
	Fl_Int_Input *width, *height, *mines;
	Fl_Button *btn;

public:
	// コンストラクタ
	CustomWindow(GameWindow *g);

private:
	static void cb_btn(Fl_Widget *w, void *p);
};

#endif // CUSTOMWINDOW_H_INCLUDE

