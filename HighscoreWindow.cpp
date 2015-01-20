#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <sstream>
#include "GameWindow.h"
#include "HighscoreWindow.h"


// コンストラクタ
HighscoreWindow::HighscoreWindow(GameWindow *g) :
	Fl_Window(400, 140, "Highscores"), game(g)
{
	set_modal();
	begin();
	
	label = new Fl_Box(20, 20, 120, 60);
	label->label("Beginner:\nIntermediate:\nAdvanced:");
	label->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
	
	score = new Fl_Box(160, 20, 60, 60, "");
	score->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
	
	name = new Fl_Box(240, 20, 140, 60, "");
	name->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
	
	clear = new Fl_Button(60, 100, 140, 25, "Clear Highscore");
	clear->callback(cb_clear, this);
	ok = new Fl_Button(260, 100, 80, 25, "OK");
	ok->callback(cb_close, this);
	ok->take_focus();
	
	update_score();
	
	end();
	show();
}


// 表示を更新
void HighscoreWindow::update_score()
{
	std::ostringstream str1;
	str1 << game->sfile.beg_score.score << "\n"
	     << game->sfile.int_score.score << "\n"
	     << game->sfile.adv_score.score;
	score->copy_label(str1.str().c_str());
	
	std::ostringstream str2;
	str2 << game->sfile.beg_score.name << "\n"
	     << game->sfile.int_score.name << "\n"
	     << game->sfile.adv_score.name;
	name->copy_label(str2.str().c_str());
}


// コールバック(close)
void HighscoreWindow::cb_close(Fl_Widget *w, void *p)
{
	HighscoreWindow *hw = static_cast<HighscoreWindow*>(p);
	hw->hide();
}


// コールバック(clear)
void HighscoreWindow::cb_clear(Fl_Widget *w, void *p)
{
	HighscoreWindow *hw = static_cast<HighscoreWindow*>(p);
	hw->game->reset_score();
	hw->update_score();
}

