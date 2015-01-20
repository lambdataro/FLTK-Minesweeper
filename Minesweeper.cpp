#include <FL/fl_ask.H>
#include <cstdlib>
#include <ctime>
#include <set>
#include "Minesweeper.h"

//----------------------------------------
// Minesweeper
// コンストラクタ
// w : 操作するウインドウ
//----------------------------------------
Minesweeper::Minesweeper(GameWindow *w) :
	Strategy(), win(w)
{
	on_change_level(GAME_BEG);
}


//----------------------------------------
// on_mouse
// マウス処理
// cell : 処理対象のセル
// btn  : 関連するマウスボタン
// st   : ボタンの状態
//----------------------------------------
void Minesweeper::on_mouse(CellBox *cell, MouseButton btn, MouseState st)
{
	switch (btn) {
	case MOUSE_LEFT:
		on_mouse_left(cell, st);
		break;
	case MOUSE_RIGHT:
		on_mouse_right(cell, st);
		break;
	}
}


//----------------------------------------
// on_mouse_left
// マウスの左ボタンの処理
// cell : 処理対象のセル
// st   : ボタンの状態
//----------------------------------------
void Minesweeper::on_mouse_left(CellBox *cell, MouseState st)
{
	switch (st) {
	case MOUSE_DOWN:
		if (state == GAME_PLAYING || state == GAME_INIT) {
			win->set_face(FACE_CLICK);
			if (cell->get_state() == CELL_CLOSE)
				cell->set_state(CELL_TMPOPEN); 
			else
				highlight(cell);
		}
		break;
	case MOUSE_LEAVE:
		if (cell->get_state() == CELL_TMPOPEN)
			cell->set_state(CELL_CLOSE);
		else {
			cancel_highlight(cell);
			auto_open(cell);
		}
		break;
	case MOUSE_UP:
		if (state == GAME_INIT) start_game(cell->get_pos());
		if (state == GAME_PLAYING) {
			win->set_face(FACE_NORMAL);
			if (cell->get_state() == CELL_CLOSE) open_cell(cell);
		}
		break;
	}
}


//----------------------------------------
// on_mouse_right
// マウスの右ボタンの処理
// cell : 処理対象のセル
// st   : ボタンの状態
//----------------------------------------
void Minesweeper::on_mouse_right(CellBox *cell, MouseState st)
{
	switch (st) {
	case MOUSE_UP:
		if (state == GAME_INIT)
			start_game(cell->get_pos());
		if (state == GAME_PLAYING) {	
			if (cell->get_state() == CELL_CLOSE) {
				win->set_count(--count);
				cell->set_state(CELL_FLAG);
			}
			else if (cell->get_state() == CELL_FLAG) {
				win->set_count(++count);
				cell->set_state(CELL_CLOSE);
			}
			if (check_game_clear()) game_clear();
		}
		break;
	}
}


//----------------------------------------
// on_new_game
// ゲームをリセットする
//----------------------------------------
void Minesweeper::on_new_game()
{
	win->stop_timer();
	win->set_time(time = 0);
	win->set_count(count = num_mines);
	state = GAME_INIT;
	win->set_face(FACE_NORMAL);
	
	Board *board = win->get_board();
	for (BoardIter it = board->begin(); it != board->end(); ++it)
		it->second->reset();
}


//----------------------------------------
// on_change_level
// レベル変更時の処理
// level : レベル
//----------------------------------------
void Minesweeper::on_change_level(GameLevel lev)
{	
	level = lev;
	switch (lev) {
	case GAME_BEG:
		width = 9;
		height = 9;
		num_mines = 10;
		break;
	case GAME_INT:
		width = 16;
		height = 16;
		num_mines = 40;
		break;
	case GAME_ADV:
		width = 30;
		height = 16;
		num_mines = 99;
		break;
	case GAME_CUSTOM:
		win->get_board_size(&width, &height, &num_mines);
		break;
	}
	win->set_board_size(width, height, num_mines);
	
	win->stop_timer();
	state = GAME_INIT;
	time = 0;
	win->set_time(time);
	count = num_mines;
	win->set_count(count);
}


//----------------------------------------
// on_timer
// タイマーの処理
//----------------------------------------
void Minesweeper::on_timer()
{
	if (++time > 999) time = 999;
	win->set_time(time);
}


//----------------------------------------
// start_game
// ゲーム開始の処理
// pos : 最初にクリックされたセル
//----------------------------------------
void Minesweeper::start_game(const Position &pos)
{
	state = GAME_PLAYING;
	init_mine(pos);
	win->start_timer();
}


//----------------------------------------
// init_mine
// 地雷を配置する
// pos : 最初にクリックされたセル
//----------------------------------------
void Minesweeper::init_mine(const Position &pos)
{
	std::srand(std::time(0));
	PosSet set;
	set.insert(pos);
	int n = 0;
	while (n < num_mines) {
		int x = rand() % width;
		int y = rand() % height;
		Position p = make_pair(x, y);
		if (set.find(p) == set.end()) { // setに含まれないとき
			set.insert(p);
			win->get_cell(p)->set_mine();
			PosSet fringe;
			win->get_fringe(p, &fringe);
			for (PosSetIter it = fringe.begin(); it != fringe.end(); ++it)
				win->get_cell(*it)->incr_value();
			++n;
		}
	}	
}


//----------------------------------------
// open_cell
// セルを開く
// cell : 開くセル
//----------------------------------------
void Minesweeper::open_cell(CellBox *cell)
{
	if (cell->get_is_mine()) {
		game_over();
		return;
	}

	cell->set_state(CELL_OPEN);
	if (cell->get_value() > 0) { // 数字セル
		if (check_game_clear()) game_clear();
		return;
	}

	PosSet visit;	// これから訪問数するセル
	PosSet checked;	// 訪問済みのセル
	
	Position pos = cell->get_pos();
	checked.insert(pos);
	win->get_fringe(pos, &visit);
	
	while (!visit.empty()) {
		PosSetIter it = visit.begin();
		if (checked.find(*it) == checked.end()) { // 未訪問
			CellBox *cell = win->get_cell(*it);
			cell->set_state(CELL_OPEN);
			if (cell->get_value() == 0) win->get_fringe(*it, &visit);
			checked.insert(*it);
		}
		visit.erase(it);
	}
	
	if (check_game_clear()) game_clear();
}


//----------------------------------------
// highlight
// セルをハイライト
//----------------------------------------
void Minesweeper::highlight(CellBox *cell)
{
	if (cell->get_state() == CELL_CLOSE)
		cell->set_state(CELL_TMPOPEN);
	PosSet set;
	win->get_fringe(cell->get_pos(), &set);
	for (PosSetIter it = set.begin(); it != set.end(); ++it)
		if (win->get_cell(*it)->get_state() == CELL_CLOSE)
			win->get_cell(*it)->set_state(CELL_TMPOPEN);
}


//----------------------------------------
// cancel_highlight
// ハイライト終了
//----------------------------------------
void Minesweeper::cancel_highlight(CellBox *cell)
{
	if (cell->get_state() == CELL_TMPOPEN)
		cell->set_state(CELL_CLOSE);
	PosSet set;
	win->get_fringe(cell->get_pos(), &set);
	for (PosSetIter it = set.begin(); it != set.end(); ++it)
		if (win->get_cell(*it)->get_state() == CELL_TMPOPEN)
			win->get_cell(*it)->set_state(CELL_CLOSE);
}


//----------------------------------------
// auto_open
// セルの自動オープン
//----------------------------------------
void Minesweeper::auto_open(CellBox *cell)
{
	PosSet set;
	win->get_fringe(cell->get_pos(), &set);
	
	if (cell->get_value() == 0) return;
	
	int n = 0;
	for (PosSetIter it = set.begin(); it != set.end(); ++it)
		if (win->get_cell(*it)->get_state() == CELL_FLAG)
			++n;
	
	if (n == cell->get_value())
		for (PosSetIter it = set.begin(); it != set.end(); ++it)
			if (win->get_cell(*it)->get_state() == CELL_CLOSE)
				open_cell(win->get_cell(*it));
}


//----------------------------------------
// game_over
// ゲームオーバー時の処理
//----------------------------------------
void Minesweeper::game_over()
{
	win->stop_timer();
	win->set_face(FACE_OVER);
	state = GAME_OVER;
	Board *board = win->get_board();
	for (BoardIter it = board->begin(); it != board->end(); ++it) {
		if (it->second->get_is_mine() &&
				it->second->get_state() != CELL_FLAG)
			it->second->set_state(CELL_OPEN);
	}
}


//----------------------------------------
// check_game_clear
// クリア判定
//----------------------------------------
bool Minesweeper::check_game_clear()
{
	if (count != 0) return false;
	Board *board = win->get_board();
	for (BoardIter it = board->begin(); it != board->end(); ++it)
		if (it->second->get_state() == CELL_CLOSE) return false;
	return true;
}


//----------------------------------------
// game_clear
// ゲームクリア時の処理
//----------------------------------------
void Minesweeper::game_clear()
{
	win->stop_timer();
	win->set_face(FACE_CLEAR);
	state = GAME_OVER;
	win->game_clear(level, time);
}

