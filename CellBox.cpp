#include <iostream>
#include <FL/Fl.H>
#include "CellBox.h"

// 数字に対応する文字列
static const char *digit_str[] = {
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

// 数字に対応する色
static const Fl_Color digit_color[] = {
	FL_BLACK,			// 未使用
	FL_BLUE,			// 1
	FL_DARK_GREEN,		// 2
	FL_RED,				// 3
	FL_DARK_BLUE,		// 4
	FL_DARK_YELLOW,		// 5
	FL_DARK_CYAN,		// 6
	FL_BLACK,			// 7
	FL_DARK_MAGENTA,	// 8
	FL_BLACK,			// 未使用
};

// セルの大きさ
const int CellBox::cell_size = 20;

// セルが押されているかどうか
bool CellBox::is_down = false;

// コンストラクタ
CellBox::CellBox(int x, int y, const Position &p) :
	Fl_Box(x, y, 20, 20), pos(p)
{
	labelfont(FL_HELVETICA_BOLD);
	reset();
}

// セルの状態をリセットする
void CellBox::reset()
{
	set_state(CELL_CLOSE);
	color(FL_BACKGROUND_COLOR);
	value = 0;
	is_mine = false;
	is_down = false;
}


// セルの状態を変更する
void CellBox::set_state(CellState st)
{
	state = st;
	switch (st) {
	case CELL_OPEN:
		set_state_open();
		break;
	case CELL_CLOSE:
		box(FL_UP_BOX);
		label("");
		break;
	case CELL_FLAG:
		box(FL_UP_BOX);
		labelcolor(FL_RED);
		label("F");
		break;
	case CELL_TMPOPEN:
		box(FL_THIN_DOWN_BOX);
		label("");
		break;
	}
}

// セルをCELL_OPEN状態にする
void CellBox::set_state_open()
{
	box(FL_THIN_DOWN_BOX);
	if (is_mine) {
		color(FL_RED);
		labelcolor(FL_BLACK);
		label("@");
	}
	else if (value >= 1 && value <= 8) {
		labelcolor(digit_color[value]);
		label(digit_str[value]);
	}
	else
		label("");
}

// イベントハンドラ
int CellBox::handle(int ev)
{
	switch (ev) {
	case FL_PUSH:
		is_down = true;
		switch (Fl::event_button()) {
		case FL_LEFT_MOUSE:
			do_callback();
			return 1;
		case FL_RIGHT_MOUSE:
			do_callback();
			return 1;
		}
	case FL_RELEASE:
		is_down = false;
		switch (Fl::event_button()) {
		case FL_LEFT_MOUSE:
			do_callback();
			return 1;
		case FL_RIGHT_MOUSE:
			do_callback();
			return 1;
		}
	case FL_DRAG:
	case FL_MOVE:
		do_callback();
		return 1;		
	}
	return Fl_Box::handle(ev);
}

// セルの地雷数を増やす
void CellBox::incr_value()
{
	if (!is_mine) ++value;
}

// セルを地雷に設定する
void CellBox::set_mine()
{
	is_mine = true;
}

// セルの状態を取得
CellState CellBox::get_state() const
{
	return state;
}

// セルの盤面座標を取得
Position CellBox::get_pos() const
{
	return pos;
}

// セルの地雷数を取得
int CellBox::get_value() const
{
	return value;
}

// セルは地雷かどうかを取得
bool CellBox::get_is_mine() const
{
	return is_mine;
}

// マウスのが押されているか
bool CellBox::get_is_down()
{
	return is_down;
}

