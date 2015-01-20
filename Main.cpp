#include <FL/Fl.H>
#include "Minesweeper.h"

//============================================================
// Main
// エントリーポイント
//============================================================
int main()
{
	GameWindow win;
	Minesweeper mine(&win);
	win.set_strategy(&mine);
	return Fl::run();
}

