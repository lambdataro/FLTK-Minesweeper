#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include "CellBox.h"
#include "GameWindow.h"
#include "HighscoreWindow.h"
#include "CustomWindow.h"

// スコアファイルの名前
const char *GameWindow::score_filename = ".score";

// コンストラクタ
GameWindow::GameWindow()
{
	init_gui();
	std::ifstream ist(score_filename);
	if (ist)
		ist >> sfile;
	else 
		sfile = ScoreFile();
	ist.close();
}


// デストラクタ
GameWindow::~GameWindow()
{
	delete win;
}


// WindowとGUI部品のインスタンスを生成する．
// サイズと位置は現時点では適当に設定．
void GameWindow::init_gui()
{
	// window
	win = new Fl_Window(100,100,"Minesweeper");
	win->label("Minesweeper");
	
	// menu
	menu = new Fl_Sys_Menu_Bar(0, 0, 300, 25);
	
	// Game
	menu->add("Game", 0, 0, 0, FL_SUBMENU);
	menu->add("Game/New", 0, cb_new_game, this, FL_MENU_DIVIDER);
	menu->add("Game/Beginner", 0,
		cb_beginner, this, FL_MENU_RADIO | FL_MENU_VALUE);
	menu->add("Game/Intermediate", 0, cb_intermediate, this, FL_MENU_RADIO);
    menu->add("Game/Advanced", 0, cb_advanced, this, FL_MENU_RADIO);
    menu->add("Game/Custom Size", 0,
    	cb_custom, this, FL_MENU_DIVIDER | FL_MENU_RADIO);
    menu->add("Game/Highscore", 0,
    	cb_highscore, this, FL_MENU_DIVIDER);
    menu->add("Game/Quit", 0, cb_quit, this, 0);
    // Help
	menu->add("Help", 0, 0, 0, FL_SUBMENU);
    menu->add("Help/About", 0, cb_about, this, 0);
    
    // panel
    panel = new Fl_Box(FL_THIN_DOWN_BOX, 0, 0, 0, 0, "");
    count_box = new Fl_Box(FL_THIN_DOWN_BOX, 0, 0, 0, 0, "");
    time_box = new Fl_Box(FL_THIN_DOWN_BOX, 0, 0, 0, 0, "");
    button = new Fl_Button(0, 0, 0, 0);
    button->callback(cb_new_game, this);
    button->labelsize(10);
    
    win->end();
    win->show();
}


// ボードサイズにあわせて，Widgetを配置する．
void GameWindow::set_board_size(int w, int h, int mines)
{
	if (w != 0) {
		width = w;
		height = h;
		num_mines = mines;
	}
	
	const int pad = 10;		// 余白
	const int ppad = 5; 	// パネル内での余白
#ifdef __APPLE__
  const int menu_h = 0;
#else
	const int menu_h = 25;	// メニューの高さ
#endif
	const int panel_h = 40; // パネルの高さ
	const int cell_s = CellBox::cell_size;
	
	// ウインドウ
	const int win_w = pad + cell_s * width + pad;
	const int win_h =
		menu_h + pad + panel_h + pad + cell_s * height + pad;
	const int win_x = win->x_root();
	const int win_y = win->y_root();
	win->resize(win_x, win_y, win_w, win_h);
	
	// メニュー
	menu->resize(0, 0, win_w, menu_h);
	
	// パネル
	const int panel_x = pad;
	const int panel_y = menu_h + pad;
	const int panel_w = win_w - pad * 2;
	panel->resize(panel_x, panel_y, panel_w, panel_h);
	
	// カウンタ
	count_box->position(panel_x + ppad, panel_y + ppad);
	count_box->size(40, 30);
	
	// ボタン
	const int but_w = 30;
	const int but_x = win_w / 2 - but_w / 2;
	button->position(but_x, panel_y + ppad);
	button->size(but_w, 30);
	set_face(FACE_NORMAL);
	
	// 経過時間
	const int time_w = 40;
	const int time_x = win_w - pad - ppad - time_w;
	time_box->position(time_x, panel_y + ppad);
	time_box->size(time_w, 30);
	
	// 盤面
	board_x = pad;
	board_y = menu_h + pad + panel_h + pad;
	delete_board();
	init_board();
	
	win->redraw();
}


// 盤面を削除する
void GameWindow::delete_board()
{
	for (BoardIter it = board.begin(); it != board.end(); ++it)
		delete it->second;
	board.clear();
}


// 盤面を初期化する
void GameWindow::init_board()
{
	win->begin();
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			Position pt = make_pair(j,i);
			const int cell_s = CellBox::cell_size;
			const int x = board_x + cell_s * j;
			const int y = board_y + cell_s * i;
			CellBox *cell = new CellBox(x, y, pt);
			cell->callback(cb_cell, this);
			board.insert(make_pair(pt, cell));
		}
	}
	win->end();
}


// 戦略の設定
void GameWindow::set_strategy(Strategy *st)
{
	strategy = st;
}


// セルの取得
CellBox *GameWindow::get_cell(const Position &pos)
{
	return board[pos];
}


// タイマーを動かす
void GameWindow::start_timer()
{
	stop_timer();
	Fl::repeat_timeout(1.0, cb_timer, this);
	
}

// タイマーを止める
void GameWindow::stop_timer()
{
	Fl::remove_timeout(cb_timer);
}


// ウインドウ座標(x,y)にあるセルを返す
CellBox *GameWindow::get_xy_cell(int x, int y)
{
	const int cell_s = CellBox::cell_size;
	
	// 座標がボード上にあるかどうかの判定
	if (x < board_x || x >= board_x + cell_s * width) return 0;
	if (y < board_y || y >= board_y + cell_s * height) return 0;
	
	// セル座標の特定
	int cx = (x - board_x) / cell_s;
	int cy = (y - board_y) / cell_s;
	Position pos = make_pair(cx, cy);
	return board[pos];
}


// 指定したセルの周辺セルを求める
void GameWindow::get_fringe(const Position &pos, PosSet *set)
{
	const int x = pos.first;
	const int y = pos.second;
	
	for (int i = y-1; i <= y+1; ++i) {
		for (int j = x-1; j <= x+1; ++j) {
			Position p = make_pair(j, i);
			if (p != pos && is_valid_pos(p))
				set->insert(p);
		}
	}
}


// 正しいセル座標であるかどうかの判定
bool GameWindow::is_valid_pos(const Position &pos)
{
	return pos.first >= 0 && pos.first < width &&
	       pos.second >= 0 && pos.second < height;
}


// カウンタの値を設定する．
void GameWindow::set_count(int value)
{
	static char str[4];
#ifdef WIN32
	sprintf_s(str, sizeof(str), "%03d", value);
#else
	std::snprintf(str, sizeof(str), "%03d", value);
#endif
	count_box->label(str);
}


// 経過時間の値を設定する．
void GameWindow::set_time(int value)
{
	static char str[4];
#ifdef WIN32
	sprintf_s(str, sizeof(str), "%03d", value);
#else
	std::snprintf(str, sizeof(str), "%03d", value);
#endif
	time_box->label(str);
}


// 盤面を取得する
Board *GameWindow::get_board()
{
	return &board;
}


// 顔を設定する
void GameWindow::set_face(FaceType type)
{
	switch (type) {
	case FACE_NORMAL:
		button->label("^_^");
		break;
	case FACE_CLICK:
		button->label("^o^");
		break;
	case FACE_OVER:
		button->label("X_X");
		break;
	case FACE_CLEAR:
		button->label("@@_@@");
		break;
	}
}


// ゲームクリアを知らせる
void GameWindow::game_clear(GameLevel level, int score)
{
	Score *s = 0;
	switch (level) {
	case GAME_BEG:
		s = &sfile.beg_score;
		break;
	case GAME_INT:
		s = &sfile.int_score;
		break;
	case GAME_ADV:
		s = &sfile.adv_score;
		break;
	}
	
	if (!s) return; // カスタムサイズ
	
	if (score < s->score) {
		s->name = input_name();
		s->score = score;
	}
	
	save_score();
}

// ハイスコアをリセットする
void GameWindow::reset_score()
{
	sfile = ScoreFile();
	save_score();
}

// ハイスコアを保存する
void GameWindow::save_score()
{
	std::ofstream ost(score_filename);
	ost << sfile;
	ost.close();
}

// 名前を入力してもらう．
const char* GameWindow::input_name()
{
	const char *name =
		fl_input("Congratulations!\n"
		"You got a highscore. Input your name.",
		"");
	if (name) return name;
	else return "no name";
}

// 盤面サイズの取得
void GameWindow::get_board_size(int *w, int *h, int *m)
{
	*w = width;
	*h = height;
	*m = num_mines;
}


// コールバック: セル操作
void GameWindow::cb_cell(Fl_Widget *w, void *p)
{
	GameWindow *game = static_cast<GameWindow*>(p);

	// 1つ前のイベント状態
	static CellBox *prev_cell = 0;
	static MouseButton prev_btn = MOUSE_NONE;
	
	// セルの取得
	Strategy *st = game->strategy;
	int x = Fl::event_x();
	int y = Fl::event_y();
	CellBox *curr_cell = game->get_xy_cell(x, y);
	
	MouseButton curr_btn;
	switch (Fl::event_button()) {
	case FL_LEFT_MOUSE:
		curr_btn = MOUSE_LEFT;
		break;
	case FL_RIGHT_MOUSE:
		curr_btn = MOUSE_RIGHT;
		break;
	}
	
	if (CellBox::get_is_down()) { // マウスが押された，押された状態で侵入
		if (prev_cell == curr_cell) return; // 同一セルの場合は処理を省略
		if (prev_cell) st->on_mouse(prev_cell, prev_btn, MOUSE_LEAVE);
		if (curr_cell) st->on_mouse(curr_cell, curr_btn, MOUSE_DOWN);
		prev_cell = curr_cell;
		prev_btn = curr_btn;
	}
	else {
		if (prev_cell) st->on_mouse(prev_cell, prev_btn, MOUSE_LEAVE);
		if (curr_cell) st->on_mouse(curr_cell, prev_btn, MOUSE_UP);
		prev_cell = 0;
		prev_btn = MOUSE_NONE;
	}
}


// コールバック: Menu-New, ボタンの動作
void GameWindow::cb_new_game(Fl_Widget *w, void *p)
{
	GameWindow *game = static_cast<GameWindow*>(p);
	game->strategy->on_new_game();
}


// コールバック: Menu-Beginner
void GameWindow::cb_beginner(Fl_Widget *w, void *p)
{
	GameWindow *game = static_cast<GameWindow*>(p);
	game->strategy->on_change_level(GAME_BEG);
}


// コールバック: Menu-Intermediate
void GameWindow::cb_intermediate(Fl_Widget *w, void *p)
{
	GameWindow *game = static_cast<GameWindow*>(p);
	game->strategy->on_change_level(GAME_INT);
}


// コールバック: Menu-Advanced
void GameWindow::cb_advanced(Fl_Widget *w, void *p)
{
	GameWindow *game = static_cast<GameWindow*>(p);
	game->strategy->on_change_level(GAME_ADV);
}


// コールバック: Menu-Custom Size
void GameWindow::cb_custom(Fl_Widget *w, void *p)
{
	GameWindow *game = static_cast<GameWindow*>(p);
	// window の解放は FLTK が行う．
	// CustomWindow が on_change_level を呼ぶ．
	CustomWindow *cw = new CustomWindow(game);
}



// コールバック: Menu-Highscore
void GameWindow::cb_highscore(Fl_Widget *w, void *p)
{
	GameWindow *game = static_cast<GameWindow*>(p);
	// window の解放は FLTK が行う．
	HighscoreWindow *hw = new HighscoreWindow(game);
}


// コールバック: Menu-Quit
void GameWindow::cb_quit(Fl_Widget *w, void *p)
{
	std::exit(EXIT_SUCCESS);
}


// コールバック: Menu-About
void GameWindow::cb_about(Fl_Widget *w, void *p)
{
	fl_message("FLTK Minesweeper Ver.0.6\nby Naoki Takashima.");
}


// コールバック: タイマー
void GameWindow::cb_timer(void *p)
{
	GameWindow *game = static_cast<GameWindow*>(p);
	game->strategy->on_timer();
	Fl::repeat_timeout(1.0, cb_timer, game);
}

