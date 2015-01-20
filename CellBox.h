﻿//------------------------------------------------------------
// CellBox.h
// Cellウィジェットの定義
//------------------------------------------------------------

#ifndef CELLBOX_H_INCLUDE
#define CELLBOX_H_INCLUDE

#include <FL/Fl_Box.H>
#include <utility>

//============================================================
// CellState
// セルの状態
//============================================================
enum CellState {
	CELL_CLOSE,		// セルが閉じている
	CELL_OPEN,		// セルが開いている
	CELL_FLAG,		// セルが閉じていて，旗が立っている
	CELL_TMPOPEN	// 一時的にセルが開いている
};

//============================================================
// MouseButton
// マウスのボタン
//============================================================
enum MouseButton {
	MOUSE_NONE,		// ボタンが押されていない
	MOUSE_LEFT,		// マウスの左ボタン
	MOUSE_RIGHT,	// マウスの右ボタン
};

//============================================================
// Position
// セルの座標
//============================================================
typedef std::pair<int,int> Position;

//============================================================
// CellBox
// セルのWidget
//============================================================
class CellBox : public Fl_Box
{
private:
	CellState 	state;		// セルの状態
	Position  	pos;		// セルの座標
	int         value;		// セル周囲の地雷数
	bool        is_mine;	// 地雷かどうか
	
	static bool is_down;	// マウスボタンが押されている

public:
	//----------------------------------------
	// セルの大きさ
	//----------------------------------------
	static const int cell_size;

	//----------------------------------------
	// コンストラクタ
	// x, y : Window上でのセルのピクセル座標
	// pos  : セルの盤面座標
	//----------------------------------------
	CellBox(int x, int y, const Position &pos);
	
	//----------------------------------------
	// セルの状態をリセットする
	//----------------------------------------
	void reset();
	
	//----------------------------------------
	// セルの状態を変更する
	// st : 新しい状態
	//----------------------------------------
	void set_state(CellState st);
	
	//----------------------------------------
	// イベントハンドラ
	// ev : イベントの内容
	//----------------------------------------
	int handle(int ev);
	
	//----------------------------------------
	// セルの地雷数を増やす
	//----------------------------------------
	void incr_value();
	
	//----------------------------------------
	// セルを地雷に設定する
	//----------------------------------------
	void set_mine();
	
	//----------------------------------------
	// セルの状態を取得
	//----------------------------------------
	CellState get_state() const;
	
	//----------------------------------------
	// セルの盤面座標を取得
	//----------------------------------------
	Position get_pos() const;
	
	//----------------------------------------
	// セルの地雷数を取得
	//----------------------------------------
	int get_value() const;
	
	//----------------------------------------
	// セルは地雷かどうかを取得
	//----------------------------------------
	bool get_is_mine() const;
	
	//----------------------------------------
	// マウスが押されているか
	//----------------------------------------
	static bool get_is_down();
	
private:
	void set_state_open();
};

#endif // CELLBOX_H_INCLUDE
