//------------------------------------------------------------
// HighscoreWindow
// ハイスコア表示用ウインドウ
//------------------------------------------------------------

#ifndef HIGHSCOREWINDOW_H_INCLUDE
#define HIGHSCOREWINDOW_H_INCLUDE

#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include "GameWindow.h"

class HighscoreWindow : public Fl_Window
{
private:
	Fl_Box *label, *score, *name;
	Fl_Button *clear, *ok;
	GameWindow *game;

public:
	// コンストラクタ
	HighscoreWindow(GameWindow *g);

private:
	void update_score();

	static void cb_close(Fl_Widget *w, void *p);
	static void cb_clear(Fl_Widget *w, void *p);
};

#endif // HIGHSCOREWINDOW_H_INCLUDE

