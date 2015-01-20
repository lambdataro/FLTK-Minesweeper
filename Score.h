#ifndef SCORE_H_INCLUDE
#define SCORE_H_INCLUDE

#include <fstream>
#include <string>

struct Score
{
	int score;			// 点数
	std::string name;	// 名前
	
	// コンストラクタ
	Score();
	// 入力オペレータ
	friend std::istream &operator>>(std::istream &ist, Score &s);
	// 出力オペレータ
	friend std::ostream &operator<<(std::ostream &ost, const Score &s);
	
};

struct ScoreFile
{
	Score beg_score;	// 初級
	Score int_score;	// 中級
	Score adv_score;	// 上級
	
	// 入力オペレータ
	friend std::istream &operator>>(std::istream &ist, ScoreFile &sf);
	// 出力オペレータ
	friend std::ostream &operator<<(std::ostream &ost, const ScoreFile &sf);
};

#endif // SCORE_H_INCLUDE

