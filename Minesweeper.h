//------------------------------------------------------------
// Minesweeper.h
// マインスイーパの定義
//------------------------------------------------------------

#ifndef MINESWEEPER_H_INCLUDE
#define MINESWEEPER_H_INCLUDE

#include "CellBox.h"
#include "GameWindow.h"


//============================================================
// GameState
// ゲームの状態
//============================================================
enum GameState {
	GAME_INIT,		// ゲーム開始待機状態
	GAME_PLAYING,	// ゲーム中
	GAME_OVER,		// ゲーム終了状態(クリア時も含む)
};


//============================================================
// Minesweeper
// マインスイーパの戦略
//============================================================
class Minesweeper : public Strategy
{
private:
	GameWindow *win;	// ゲーム画面
	int width;			// 横方向のセル数
	int height;			// 縦方向のセル数
	int num_mines;		// 地雷の数
	int time;			// 経過時間
	int count;			// 残り地雷数
	GameState state;	// ゲームの状態
	GameLevel level;	// ゲームのレベル

public:
	// コンストラクタ
	Minesweeper(GameWindow *g);
	// イベント関数
	void on_mouse(CellBox *cell, MouseButton btn, MouseState st);
	void on_new_game();
	void on_change_level(GameLevel level);
	void on_timer();
	
private:
	// マウスの処理
	void on_mouse_left(CellBox *cell, MouseState st);
	void on_mouse_right(CellBox *cell, MouseState st);
	// ゲーム開始の処理
	void start_game(const Position &pos);
	// 地雷を設置する
	void init_mine(const Position &pos);
	// セルを開く処理
	void open_cell(CellBox *cell);
	// セルをハイライト
	void highlight(CellBox *cell);
	// ハイライト終了
	void cancel_highlight(CellBox *cell);
	// セルの自動オープン
	void auto_open(CellBox *cell);
	// ゲームオーバー処理
	void game_over();	
	// クリア判定
	bool check_game_clear();
	// ゲームクリア処理
	void game_clear();
};

#endif // MINESWEEPER_H_INCLUDE

