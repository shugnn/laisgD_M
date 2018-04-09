#if !defined(ANA_H)
#define ANA_H
#if _MSC_VER > 1000
#pragma once
#endif
#include <vector>
#include"DataStruct.h"
using namespace std;

class   CAna
{
public:
	CAna();
	~CAna();
	void Load();
private:
	void ReadDate();
	void ReadMin5();
	void Train();
	void Channel();
	void Moneys();
	void Hexin();
private:
	vector<DMdata> dmdatas;
	vector<DMtrain> dmtrains[8];
	vector<vector<vector<vector<int>>>>sorters;
};


#endif