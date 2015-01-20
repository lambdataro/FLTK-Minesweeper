#include <cctype>
#include "Score.h"

// Score のコンストラクタ
Score::Score() : score(999), name("no name")
{
}

// Score の入力オペレータ
std::istream &operator>>(std::istream &ist, Score &s)
{
	ist >> s.score;
	while (isspace(ist.peek())) ist.get(); // 空白の読み飛ばし
	std::getline(ist, s.name);
	
	if (!ist) { // エラー発生時
		s.score = 999;
		s.name = "no name";
	}
	return ist;
}

// Score の出力オペレータ
std::ostream &operator<<(std::ostream &ost, const Score &s)
{
	ost << s.score << " " << s.name << std::endl;
	return ost;
}

// ScoreFile の入力オペレータ
std::istream &operator>>(std::istream &ist, ScoreFile &sf)
{
	ist >> sf.beg_score >> sf.int_score >> sf.adv_score;
	return ist;
}

// ScoreFile の出力オペレータ
std::ostream &operator<<(std::ostream &ost, const ScoreFile &sf)
{
	ost << sf.beg_score << sf.int_score << sf.adv_score;
	return ost;
}

	
