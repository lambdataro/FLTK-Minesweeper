//------------------------------------------------------------
// GameWindow.h
// ゲームの盤面
//------------------------------------------------------------

#ifndef GAMEWINDOW_H_INCLUDE
#define GAMEWINDOW_H_INCLUDE

#include <FL/Fl_Window.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <set>
#include <map>
#include "CellBox.h"
#include "Score.h"

using std::make_pair;

// 前方宣言
class HighscoreWindow;
class CustomWindow;

//============================================================
// MouseState
// マウスの状態
//============================================================
enum MouseState {
	MOUSE_DOWN,		// マウスが押された，押された状態で領域に入った
	MOUSE_LEAVE,	// マウスが押された状態で領域を出た
	MOUSE_UP,		// マウスが離された
};

//============================================================
// GameLevel
// ゲームのレベル
//============================================================
enum GameLevel {
	GAME_BEG,		// 初級
	GAME_INT,		// 中級
	GAME_ADV,		// 上級
	GAME_CUSTOM,	// カスタムサイズ
};

//============================================================
// FaceType
// 顔文字の種類
//============================================================
enum FaceType {
	FACE_NORMAL,	// 通常時
	FACE_CLICK,		// クリック時
	FACE_OVER,		// ゲームオーバー時
	FACE_CLEAR,		// ゲームクリア時
};

//============================================================
// Strategy
// ゲームの戦略
//============================================================
class Strategy
{
public:
	// マウスの処理
	virtual void on_mouse(CellBox *cell, MouseButton btn, MouseState st) = 0;

	// メニューの処理
	virtual void on_new_game() = 0;
	virtual void on_change_level(GameLevel lev) = 0;
	
	// タイマー
	virtual void on_timer() = 0;
};


//============================================================
// Board, BoardIter
// 盤面のデータ構造
//============================================================
typedef std::map<Position, CellBox*> Board;
typedef Board::iterator BoardIter;


//============================================================
// PosSet, PosSetIter
// 盤面位置の集合
//============================================================
typedef std::set<Position> PosSet;
typedef PosSet::iterator PosSetIter;


//============================================================
// GameWindow
// ゲームの盤面
//============================================================
class GameWindow
{
private:
	Strategy *strategy;	// 戦略
	int width;			// 横方向のセル数
	int height;			// 縦方向のセル数
	int num_mines;		// 地雷の数
	Board board;		// 盤面(セルのマップ)
	ScoreFile sfile;	// ハイスコア
	static const char *score_filename;		// スコアファイルの名前
	
	friend class HighscoreWindow;
	friend class CustomWindow;
	
public:
	//----------------------------------------
	// コンストラクタ
	//----------------------------------------
	GameWindow();

	//----------------------------------------
	// デストラクタ
	//----------------------------------------
	~GameWindow();

	//----------------------------------------
	// 現在のGUI部品を破棄して盤面サイズの変更
	// x    　: 横方向のセル数
	// y    　: 縦方向のセル数
	// mines　: 地雷数
	//----------------------------------------
	void set_board_size(int w, int h, int mines);
	
	//----------------------------------------
	// 戦略の設定
	// st : 戦略
	//----------------------------------------
	void set_strategy(Strategy *st);
	
	//----------------------------------------
	// セルの取得
	// pos : 盤面座標
	//----------------------------------------
	CellBox *get_cell(const Position &pos);
	
	//----------------------------------------
	// タイマーを動かす
	//----------------------------------------
	void start_timer();
	
	//----------------------------------------
	// タイマーを止める
	//----------------------------------------
	void stop_timer();
	
	//----------------------------------------
	// ウインドウ座標(x,y)にあるセルを返す
	// x, y : 座標
	//----------------------------------------
	CellBox *get_xy_cell(int x, int y);
	
	//----------------------------------------
	// 指定したセルの周辺セルを求める
	// pos : セルの座標
	// set : 結果を出力する集合
	//----------------------------------------
	void get_fringe(const Position &pos, PosSet *set);
	
	//----------------------------------------
	// カウンタの値を設定する．
	// value : 値
	//----------------------------------------
	void set_count(int value);
	
	//----------------------------------------
	// 経過時間の値を設定する．
	// value : 値
	//----------------------------------------
	void set_time(int value);
	
	//----------------------------------------
	// 盤面を取得する
	//----------------------------------------
	Board *get_board();
	
	//----------------------------------------
	// 顔を設定する
	// type : 顔の種類
	//----------------------------------------
	void set_face(FaceType type);
	
	//----------------------------------------
	// ゲームクリアを知らせる
	// level : レベル
	// score : スコア
	//----------------------------------------
	void game_clear(GameLevel level, int score);
	
	//----------------------------------------
	// スコアをリセットする
	//----------------------------------------
	void reset_score();
	
	//----------------------------------------
	// 盤面サイズの取得
	//----------------------------------------
	void get_board_size(int *w, int *h, int *m);
	
private:
	// GUI部品
	Fl_Window *win;		// ウインドウ
	Fl_Sys_Menu_Bar *menu;	// メニューバー
	Fl_Box *panel;		// カウンタ等が乗っている箱
	Fl_Box *count_box;	// カウンタ表示箱
	Fl_Box *time_box;	// 経過時間表示箱
	Fl_Button *button;	// スマイルボタン

	// 盤面のピクセル座標
	int board_x, board_y;
	
private:
	void init_gui();
	void delete_board();
	void init_board();
	bool is_valid_pos(const Position &pos);
	const char* input_name();
	void save_score();
	
private:
	// staticコールバック関数
	static void cb_cell(Fl_Widget *w, void *p);
	static void cb_new_game(Fl_Widget *w, void *p);
	static void cb_beginner(Fl_Widget *w, void *p);
	static void cb_intermediate(Fl_Widget *w, void *p);
	static void cb_advanced(Fl_Widget *w, void *p);
	static void cb_custom(Fl_Widget *w, void *p);
	static void cb_highscore(Fl_Widget *w, void *p);
	static void cb_quit(Fl_Widget *w, void *p);
	static void cb_about(Fl_Widget *w, void *p);
	static void cb_timer(void *p);
};

#endif // GAMEWINDOW_H_INCLUDE

