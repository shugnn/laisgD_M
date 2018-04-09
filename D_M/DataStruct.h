#if !defined(SERVER_DATATYPE_STRUCT_H)
#define SERVER_DATATYPE_STRUCT_H
#if _MSC_VER > 1000
#pragma once
#endif 
#pragma warning(disable : 4996)
struct DateLine
{
	int date = 0;
	int open = 0;
	int high = 0;
	int lowe = 0;
	int clos = 0;
	float amou = 0;
	int volu = 0;
	int rese = 0;
};
struct Min5Line
{
	short sdate = 0;
	short sminu = 0;
	float open = 0;
	float high = 0;
	float lowe = 0;
	float clos = 0;
	float amou = 0;
	int volu = 0;
	int date = 0;
};
struct DMdata
{
	char code[16];
	int date;
	float am1;
	float am2;
	float hi1;
	float lo1;
	float cl0;
	float ncell[8];
	float fit[8];
	short ini = 0;
	/*
					float fit_oo;
					float fit_oc;
					float fit_co;
					float fit_cc;
					float fit_oup;
					float fit_odn;
					float fit_cup;
					float fit_cdn;

*/
};

struct DMtrain
{
	float wt[8];
	double dist;
};

inline bool TrainSort(const DMtrain ob0, const DMtrain ob1)
{
	return (ob0.dist<ob1.dist);
}
struct Conner
{
	int date = 0;
	float averp=0;
};
struct Chan
{
	int date = 0;
	float averp = 0;
	Conner leftup;
	Conner leftdn;
	Conner rightup;
	Conner rightdn;
	float width = 0;
	float centre = 0;
};
struct Moneis
{
	char code[16];
	int date = 0;
	float amou = 0;
	float open = 0;
	float clos = 0;
	float amts = 0;
	float c0c1 = 0;
	float o0c1 = 0;
	float c1c2 = 0;
	float c1c4 = 0;
	float c4c5 = 0;
	float oAo0 = 0;
	float oAc0 = 0;
	float amt1 = 0;
	float amt2 = 0;
	float amt3 = 0;
	float amt4 = 0;
};
inline bool AmounSort(const Moneis ob0, const Moneis ob1)
{
	return (ob0.amt4 < ob1.amt4);
}
#endif