#pragma once
#include<Windows.h>
#include<iostream>
#include <fstream>
#include<time.h>
#include<iomanip>
#include<io.h>
#include <algorithm>
#include<sstream>
using namespace std;
#include"StkAna.h"
string dir_day_sz = "C:\\shugnn0411\\PROGRAM\\new_tdx\\vipdoc\\sz\\lday\\";// "D:\\new_hxzq_hc\\vipdoc\\sz\\lday\\"; 
string dir_day_sh = "C:\\shugnn0411\\PROGRAM\\new_tdx\\vipdoc\\sh\\lday\\";// "D:\\new_hxzq_hc\\vipdoc\\sh\\lday\\";
string dir_fz5_sz = "C:\\shugnn0411\\PROGRAM\\new_tdx\\vipdoc\\sz\\fzline\\";//"D:\\new_hxzq_hc\\vipdoc\\sz\\fzline\\";
string dir_fz5_sh = "C:\\shugnn0411\\PROGRAM\\new_tdx\\vipdoc\\sh\\fzline\\";//"D:\\new_hxzq_hc\\vipdoc\\sh\\fzline\\";
string dir_rslt = "C:\\shugnn0411\\APPS\\SK\\D_M\\Rslt";//"D:\\VS2010\\SK\\D_M\\Rslt\\";
int timeA = 20140101, timeB = 20180126;
bool DTL = true;
CAna::CAna()
{
	time_t tm; time(&tm); cout << __FUNCTION__ << ctime(&tm);
}
CAna::~CAna()
{
	time_t tm; time(&tm); cout << __FUNCTION__ << ctime(&tm);
}
void CAna::Load()
{
	Moneys();
	return;
	Hexin();

	ReadDate();
	ReadMin5();
	Train();
	return;
}
void CAna::ReadDate()
{
	dmdatas.reserve(10000);
	_finddata_t info;
	int handle = _findfirst((dir_day_sz + "sz*.day").c_str(), &info);
	if (handle >0)
	{
		do
		{
			ifstream infile(dir_day_sz + string(info.name), ios::binary | ios::in);
			if (infile.is_open())
			{
				DateLine dl0, dl1, dl2, dl3, dl4, dl5, dl6;
				infile.seekg(0, ios::beg);
				while (!infile.eof())
				{
					dl6 = dl5; dl5 = dl4; dl4 = dl3; dl3 = dl2; dl2 = dl1; dl1 = dl0;
					infile.read((char*)&dl0, sizeof(dl0));
					if (dl1.date<timeA)continue;
					if (dl0.open <= 0 || dl1.open <= 0 || dl2.open <= 0 || dl3.open <= 0 || dl4.open <= 0 || dl5.open <= 0 || dl6.open <= 0)continue;
					if (dl0.clos <= 0 || dl1.clos <= 0 || dl2.clos <= 0 || dl3.clos <= 0 || dl4.clos <= 0 || dl5.clos <= 0 || dl6.clos <= 0)continue;
					if (dl0.amou <= 100000 || dl1.amou <= 100000 || dl2.amou <= 100000 || dl3.amou <= 100000 || dl4.amou <= 100000 || dl5.amou <= 100000 || dl6.amou <= 100000)continue;
					float o0c1 = (float)(1.0*dl0.open / dl1.clos - 1) * 100;
					float o1c2 = (float)(1.0*dl1.open / dl2.clos - 1) * 100;
					float o2c3 = (float)(1.0*dl2.open / dl3.clos - 1) * 100;
					float o3c4 = (float)(1.0*dl3.open / dl4.clos - 1) * 100;
					if (abs(o0c1) > 11 || abs(o1c2) > 11 || abs(o2c3) > 11 || abs(o3c4) > 11)continue;
					float c3c4 = (float)(1.0*dl3.clos / dl4.clos - 1) * 100;
					if (c3c4 < 9.8)continue;
					DMdata dm;
					memset(&dm, 0, sizeof(dm));
					strcpy(dm.code, string(info.name).substr(0, 8).c_str());
					dm.date = dl1.date;
					dm.am1 = dl1.amou;
					dm.am2 = dl2.amou;
					dm.hi1 = (float)dl1.high/100.0;
					dm.lo1 = (float)dl1.lowe/100.0;
					dm.cl0 = (float)dl0.clos/100.0;
					float amrt1 = log(dl1.amou * 3 / (dl4.amou + dl5.amou + dl6.amou)) / log(1.2);
					float amrt2 = log(dl2.amou * 3 / (dl4.amou + dl5.amou + dl6.amou)) / log(1.2);
					float avprt1 = ((dl1.amou / dl1.volu) / (dl2.clos/100.0) - 1) * 100;
					float avprt2 = ((dl2.amou / dl2.volu) / (dl3.clos/100.0) - 1) * 100;
					dm.ncell[0] = amrt2 > 10 ? 10 : (amrt2 < -10 ? -10 : amrt2);//d2:am to 3am (-10,10)
					dm.ncell[1] = avprt2 > 10 ? 10 : (avprt2 < -10 ? -10 : avprt2);//d2:avpr2 to cl3 (-10,10)
					dm.ncell[4] = amrt1 > 10 ? 10 : (amrt1 < -10 ? -10 : amrt1);//d1:am to 3am (-10,10)
					dm.ncell[5] = avprt1 > 10 ? 10 : (avprt1 < -10 ? -10 : avprt1);//d1:avpr1 to cl2 (-10,10)
					dm.fit[0] = (float)(1.0*dl0.open / dl1.open - 1) * 100;//fit_oo;
					dm.fit[1] = (float)(1.0*dl0.clos / dl1.open - 1) * 100;//fit_co
					dm.fit[2] = (float)(1.0*dl0.open / dl1.clos - 1) * 100;//fit_oc
					dm.fit[3] = (float)(1.0*dl0.clos / dl1.clos - 1) * 100;//fit_cc
					dm.ini = 1;
					dmdatas.push_back(dm);
				}
				infile.close();
			}
		} while (_findnext(handle, &info) >= 0);
		_findclose(handle);
	}
	handle = _findfirst((dir_day_sh + "sh*.day").c_str(), &info);
	if (handle >0)
	{
		do
		{
			ifstream infile(dir_day_sh + string(info.name), ios::binary | ios::in);
			if (infile.is_open())
			{
				DateLine dl0, dl1, dl2, dl3, dl4, dl5, dl6;
				infile.seekg(0, ios::beg);
				while (!infile.eof())
				{
					dl6 = dl5; dl5 = dl4; dl4 = dl3; dl3 = dl2; dl2 = dl1; dl1 = dl0;
					infile.read((char*)&dl0, sizeof(dl0));
					if (dl1.date<timeA)continue;
					if (dl0.open <= 0 || dl1.open <= 0 || dl2.open <= 0 || dl3.open <= 0 || dl4.open <= 0 || dl5.open <= 0 || dl6.open <= 0)continue;
					if (dl0.clos <= 0 || dl1.clos <= 0 || dl2.clos <= 0 || dl3.clos <= 0 || dl4.clos <= 0 || dl5.clos <= 0 || dl6.clos <= 0)continue;
					if (dl0.amou <= 100000 || dl1.amou <= 100000 || dl2.amou <= 100000 || dl3.amou <= 100000 || dl4.amou <= 100000 || dl5.amou <= 100000 || dl6.amou <= 100000)continue;
					float o0c1 = (float)(1.0*dl0.open / dl1.clos - 1) * 100;
					float o1c2 = (float)(1.0*dl1.open / dl2.clos - 1) * 100;
					float o2c3 = (float)(1.0*dl2.open / dl3.clos - 1) * 100;
					float o3c4 = (float)(1.0*dl3.open / dl4.clos - 1) * 100;
					if (abs(o0c1) > 11 || abs(o1c2) > 11 || abs(o2c3) > 11 || abs(o3c4) > 11)continue;
					float c3c4 = (float)(1.0*dl3.clos / dl4.clos - 1) * 100;
					if (c3c4 < 9.8)continue;
					DMdata dm;
					memset(&dm, 0, sizeof(dm));
					strcpy(dm.code, string(info.name).substr(0, 8).c_str());
					dm.date = dl1.date;
					dm.am1 = dl1.amou;
					dm.am2 = dl2.amou;
					dm.hi1 = (float)dl1.high / 100.0;
					dm.lo1 = (float)dl1.lowe / 100.0;
					dm.cl0 = (float)dl0.clos / 100.0;
					float amrt1 = log(dl1.amou * 3 / (dl4.amou + dl5.amou + dl6.amou)) / log(1.2);
					float amrt2 = log(dl2.amou * 3 / (dl4.amou + dl5.amou + dl6.amou)) / log(1.2);
					float avprt1 = ((dl1.amou / dl1.volu) / (dl2.clos / 100.0) - 1) * 100;
					float avprt2 = ((dl2.amou / dl2.volu) / (dl3.clos / 100.0) - 1) * 100;
					dm.ncell[0] = amrt2 > 10 ? 10 : (amrt2 < -10 ? -10 : amrt2);//d2:am to 3am (-10,10)
					dm.ncell[1] = avprt2 > 10 ? 10 : (avprt2 < -10 ? -10 : avprt2);//d2:avpr2 to cl3 (-10,10)
					dm.ncell[4] = amrt1 > 10 ? 10 : (amrt1 < -10 ? -10 : amrt1);//d1:am to 3am (-10,10)
					dm.ncell[5] = avprt1 > 10 ? 10 : (avprt1 < -10 ? -10 : avprt1);//d1:avpr1 to cl2 (-10,10)
					dm.fit[0] = (float)(1.0*dl0.open / dl1.open - 1) * 100;//fit_oo;
					dm.fit[1] = (float)(1.0*dl0.clos / dl1.open - 1) * 100;//fit_co
					dm.fit[2] = (float)(1.0*dl0.open / dl1.clos - 1) * 100;//fit_oc
					dm.fit[3] = (float)(1.0*dl0.clos / dl1.clos - 1) * 100;//fit_cc
					dm.ini = 1;
					dmdatas.push_back(dm);
				}
				infile.close();
			}
		} while (_findnext(handle, &info) >= 0);
		_findclose(handle);
	}
	dmdatas.shrink_to_fit();
	cout << "Find d3stop datas " << dmdatas.size() << endl;
	return;
}
void CAna::ReadMin5()
{
	ifstream infile;
	char code[16];
	short inis = 0;
	for (auto dm = dmdatas.begin(); dm != dmdatas.end();)
	{
		strcpy(code, dm->code);
		if (string(code).find("sz", 0) == 0)
		{
			if (infile.is_open())infile.close();
			infile.open(dir_fz5_sz + string(code)+".lc5", ios::binary | ios::in);
			if (!infile.is_open()){ dm++; continue; }
		}
		else if (string(code).find("sh", 0) == 0)
		{
			if (infile.is_open())infile.close();
			infile.open(dir_fz5_sh + string(code) + ".lc5", ios::binary | ios::in);
			if (!infile.is_open()){ dm++; continue; }
		}
		else
		{
			cout << "code file error " << code << endl;
			dm++;
			continue;
		}
		Min5Line ml1[48],ml2[48];
		memset(&ml1, 0, sizeof(ml1));
		memset(&ml2, 0, sizeof(ml2));
		infile.seekg(0, ios::beg);
		while (!infile.eof())
		{
			Min5Line ml0;
			infile.read((char*)&ml0, sizeof(ml0));
			ml0.date = (int)(ml0.sdate / 2048 + 2004) * 10000 + floor(fmod(ml0.sdate, 2048) / 100) * 100 + fmod(fmod(ml0.sdate, 2048), 100);
			short mm = ml0.sminu < 785 ? (ml0.sminu - 575) / 5 :24+ (ml0.sminu - 785) / 5;
			if (ml0.date < dm->date)
			{
				if (mm >= 0 && mm < 48)ml2[mm] = ml0;
				continue;
			}
			if (ml0.date == dm->date)
			{
				if (mm >= 0 && mm < 48)ml1[mm] = ml0;
				continue;
			}
			if (ml0.date > dm->date)
			{
				if (ml1[0].date == dm->date && ml2[0].date<dm->date)
				if (ml1[0].open > 0 && ml1[0].high > 0 && ml1[0].lowe > 0 && ml1[0].clos > 0 && ml1[0].volu > 0 && ml1[0].amou > 0)
				if (ml2[0].open > 0 && ml2[0].high > 0 && ml2[0].lowe > 0 && ml2[0].clos > 0 && ml2[0].volu > 0 && ml2[0].amou > 0)
				{
					float upam1 = 0, dnam1 = 0, upam2 = 0, dnam2 = 0, avhi1 = 0, avlo1 = 0, avhi2 = 0, avlo2 = 0;
					int k = 0;
					while (k<48)
					{
						if (ml1[k].open <= 0 || ml1[k].high <= 0 || ml1[k].lowe <= 0 || ml1[k].clos <= 0 || ml1[k].volu <= 0 || ml1[k].amou <= 0)if (k>0)ml1[k] = ml1[k - 1];
						if (ml2[k].open <= 0 || ml2[k].high <= 0 || ml2[k].lowe <= 0 || ml2[k].clos <= 0 || ml2[k].volu <= 0 || ml2[k].amou <= 0)if (k>0)ml2[k] = ml2[k - 1];
						float averrate1 = 0, averrate2 = 0;
						if (k == 0)averrate1 = ((ml1[0].amou / ml1[0].volu) / ml1[0].open - 1) * 100;
						else averrate1 = ((ml1[k].amou / ml1[k].volu) / (ml1[k - 1].amou / ml1[k - 1].volu) - 1) * 100;
						if (averrate1 > 0.5)upam1 += ml1[k].amou;
						if (averrate1 < -0.5)dnam1 += ml1[k].amou;
						if (ml1[k].amou / ml1[k].volu>avhi1 || avhi1 == 0)avhi1 = ml1[k].amou / ml1[k].volu;
						if (ml1[k].amou / ml1[k].volu<avlo1 || avlo1 == 0)avlo1 = ml1[k].amou / ml1[k].volu;

						if (k == 0)averrate2 = ((ml2[0].amou / ml2[0].volu) / ml2[0].open - 1) * 100;
						else averrate2 = ((ml2[k].amou / ml2[k].volu) / (ml2[k - 1].amou / ml2[k - 1].volu) - 1) * 100;
						if (averrate2 > 0.5)upam2 += ml2[k].amou;
						if (averrate2 < -0.5)dnam2 += ml2[k].amou;
						if (ml2[k].amou / ml2[k].volu>avhi2 || avhi2 == 0)avhi2 = ml2[k].amou / ml2[k].volu;
						if (ml2[k].amou / ml2[k].volu<avlo2 || avlo2 == 0)avlo2 = ml2[k].amou / ml2[k].volu;
						k++;
					}
					dm->ncell[2] = (upam2 - dnam2) / dm->am2 * 10;//d2:pos,nev ln(-10,10)
					if (avhi2 - avlo2>0)dm->ncell[3] = (ml2[47].clos - avlo2) / (avhi2 - avlo2) * 20 - 10;//d2:hi,lo,cl locate(-10,10)
					else dm->ncell[3] = 0;
					dm->ncell[6] = (upam1 - dnam1) / dm->am1 * 10;//d1:pos,nev ln(-10,10)
					if (avhi1 - avlo1>0)dm->ncell[7] = (ml1[47].clos - avlo1) / (avhi1 - avlo1) * 20 - 10;//d1:hi,lo,cl locate(-10,10)
					else dm->ncell[7] = 0;
					dm->fit[4] = (ml1[47].clos / dm->lo1 - 1) * 100>5.0 ? (ml0.open / ml1[47].clos - 1) * 100 : 99;//fit_oup
					dm->fit[5] = (ml1[47].clos / dm->hi1 - 1) * 100 < -5.0 ? (ml0.open / ml1[47].clos - 1) * 100 : 99;//fit_odn
					dm->fit[6] = (ml1[47].clos / dm->lo1 - 1) * 100 > 5.0 ? (dm->cl0 / ml1[47].clos - 1) * 100 : 99;//fit_cup
					dm->fit[7] = (ml1[47].clos / dm->hi1 - 1) * 100 < -5.0 ? (dm->cl0 / ml1[47].clos - 1) * 100 : 99;//fit_cdn
					dm->ini = 2;
					++inis;
				}
				memcpy(&ml2, &ml1, sizeof(ml1));
				memset(&ml1,0,sizeof(ml1));
				ml1[0] = ml0;
				dm++;
				if (dm == dmdatas.end() || strcmp(dm->code, code) != 0)break;
			}
		}
		if (infile.eof())dm++;
	}
	if (infile.is_open())infile.close();
	
	string fn = dir_rslt + "test.txt";
	ofstream outfile(fn, ios::out | ios::trunc);
	if (outfile.is_open())
	{
		for (auto dm = dmdatas.begin(); dm != dmdatas.end();dm++)
		{
			outfile << dm->code << " " << dm->date << " ini " << dm->ini 
				<< " necll "<< dm->ncell[0] << " " << dm->ncell[1] << " " << dm->ncell[2] << " " << dm->ncell[3] << " " << dm->ncell[4] << " " << dm->ncell[5] << " " << dm->ncell[6] << " " << dm->ncell[7] << " " 
				<< " fit " << dm->fit[0] << " " << dm->fit[1] << " " << dm->fit[2] << " " << dm->fit[3] << " " << dm->fit[4] << " " << dm->fit[5] << " " << dm->fit[6] << " " << dm->fit[7] << " " << endl;
		}
		outfile.close();
	}
	time_t tm; time(&tm); cout << "IniMin5  " << inis << " "<<ctime(&tm);
	return;
}
void CAna::Train()
{
	for (short e = 0; e < 8;e++)dmtrains[e].reserve(1000);
	for (float w0 = 0; abs(w0) <= 1.0; w0 = (w0 <0 ? int(-pow(10, 1.0 / 10) * w0 * 100) / 100.0 : (w0>0 ? -w0 : 0.1)))
	for (float w1 = 0; abs(w1) <= 1.0; w1 = (w1 <0 ? int(-pow(10, 1.0 / 10) * w1 * 100) / 100.0 : (w1>0 ? -w1 : 0.1)))
	for (float w2 = 0; abs(w2) <= 1.0; w2 = (w2 <0 ? int(-pow(10, 1.0 / 10) * w2 * 100) / 100.0 : (w2>0 ? -w2 : 0.1)))
	for (float w3 = 0; abs(w3) <= 1.0; w3 = (w3 <0 ? int(-pow(10, 1.0 / 10) * w3 * 100) / 100.0 : (w3>0 ? -w3 : 0.1)))
	{
		DMtrain dmtrain;
		memset(&dmtrain, 0, sizeof(dmtrain));
		dmtrain.wt[0] = w0;
		dmtrain.wt[1] = w1;
		dmtrain.wt[2] = w2;
		dmtrain.wt[3] = w3;
		for (short ft = 0; ft < 2; ft++)
		{
			int cn=0;
			double dist = 0;
			for (auto dm = dmdatas.begin(); dm != dmdatas.end(); dm++)
			{
				if (dm->ini<2)continue;
				dist += pow(
					(
					pow(w0*dm->ncell[0] - dm->fit[ft], 2.0) +
					pow(w1*dm->ncell[1] - dm->fit[ft], 2.0) +
					pow(w2*dm->ncell[2] - dm->fit[ft], 2.0) +
					pow(w3*dm->ncell[3] - dm->fit[ft], 2.0)
					) / 4,
					0.5);
				cn++;
			}
			dmtrain.dist = dist / cn;
			dmtrains[ft].push_back(dmtrain);
			if ((int)dmtrains[ft].size() >= 1000)
			{
				sort(dmtrains[ft].begin(), dmtrains[ft].end(), TrainSort);
				dmtrains[ft].erase(dmtrains[ft].end() - 200, dmtrains[ft].end());
			}
		}		
	}
	string fn = dir_rslt + "dmtrains4.txt";
	ofstream outfile(fn, ios::out | ios::trunc);
	if (outfile.is_open())
	{
		for (int ft = 0; ft < 2;ft++)
		for (auto it = dmtrains[ft].begin(); it != dmtrains[ft].end(); it++)
		{
			outfile <<"ft "<<ft<<" wt "<< it->wt[0] << " " << it->wt[1] << " " << it->wt[2] << " " << it->wt[3] << " dist " << it->dist << endl;
		}
		outfile.close();
	}
	time_t tm; time(&tm); cout << "ft0-1 ok " << ctime(&tm);

	for (float w0 = 0; abs(w0) <= 1.0; w0 = (w0 <0 ? int(-pow(10, 1.0 / 4)* w0 * 100) / 100.0 : (w0>0 ? -w0 : 0.1)))
	for (float w1 = 0; abs(w1) <= 1.0; w1 = (w1 <0 ? int(-pow(10, 1.0 / 4)* w1 * 100) / 100.0 : (w1>0 ? -w1 : 0.1)))
	for (float w2 = 0; abs(w2) <= 1.0; w2 = (w2 <0 ? int(-pow(10, 1.0 / 4)* w2 * 100) / 100.0 : (w2>0 ? -w2 : 0.1)))
	for (float w3 = 0; abs(w3) <= 1.0; w3 = (w3 <0 ? int(-pow(10, 1.0 / 4)* w3 * 100) / 100.0 : (w3>0 ? -w3 : 0.1)))
	for (float w4 = 0; abs(w4) <= 1.0; w4 = (w4 <0 ? int(-pow(10, 1.0 / 4)* w4 * 100) / 100.0 : (w4>0 ? -w4 : 0.1)))
	for (float w5 = 0; abs(w5) <= 1.0; w5 = (w5 <0 ? int(-pow(10, 1.0 / 4)* w5 * 100) / 100.0 : (w5>0 ? -w5 : 0.1)))
	for (float w6 = 0; abs(w6) <= 1.0; w6 = (w6 <0 ? int(-pow(10, 1.0 / 4)* w6 * 100) / 100.0 : (w6>0 ? -w6 : 0.1)))
	for (float w7 = 0; abs(w7) <= 1.0; w7 = (w7 <0 ? int(-pow(10, 1.0 / 4)* w7 * 100) / 100.0 : (w7>0 ? -w7 : 0.1)))
	{
		DMtrain dmtrain;
		memset(&dmtrain, 0, sizeof(dmtrain));
		dmtrain.wt[0] = w0;
		dmtrain.wt[1] = w1;
		dmtrain.wt[2] = w2;
		dmtrain.wt[3] = w3;
		dmtrain.wt[4] = w4;
		dmtrain.wt[5] = w5;
		dmtrain.wt[6] = w6;
		dmtrain.wt[7] = w7;
		for (short ft = 2; ft < 8; ft++)
		{
			int cn = 0;
			double dist = 0;
			for (auto dm = dmdatas.begin(); dm != dmdatas.end(); dm++)
			{
				if (dm->ini<2)continue;
				dist += pow(
					(
					pow(w0*dm->ncell[0] - dm->fit[ft], 2.0) +
					pow(w1*dm->ncell[1] - dm->fit[ft], 2.0) +
					pow(w2*dm->ncell[2] - dm->fit[ft], 2.0) +
					pow(w3*dm->ncell[3] - dm->fit[ft], 2.0) +
					pow(w4*dm->ncell[4] - dm->fit[ft], 2.0) +
					pow(w5*dm->ncell[5] - dm->fit[ft], 2.0) +
					pow(w6*dm->ncell[6] - dm->fit[ft], 2.0) +
					pow(w7*dm->ncell[7] - dm->fit[ft], 2.0)
					) / 8,
					0.5);
				cn++;
			}
			dmtrain.dist = dist / cn;
			dmtrains[ft].push_back(dmtrain);
			if ((int)dmtrains[ft].size() >= 1000)
			{
				sort(dmtrains[ft].begin(), dmtrains[ft].end(), TrainSort);
				dmtrains[ft].erase(dmtrains[ft].end() - 200, dmtrains[ft].end());
			}
		}
	}
	string fn2 = dir_rslt + "dmtrains8.txt";
	ofstream outfile2(fn2, ios::out | ios::trunc);
	if (outfile2.is_open())
	{
		for (int ft = 2; ft < 8; ft++)
		for (auto it = dmtrains[ft].begin(); it != dmtrains[ft].end(); it++)
		{
			outfile2 << "ft " << ft << " wt " << it->wt[0] << " " << it->wt[1] << " " << it->wt[2] << " " << it->wt[3] 
				<< it->wt[4] << " " << it->wt[5] << " " << it->wt[6] << " " << it->wt[7] << " dist " << it->dist << endl;
		}
		outfile2.close();
	}
	cout << "ft2-7 ok " << ctime(&tm);
	return;
}
void CAna::Channel()
{
	dmdatas.reserve(10000);
	_finddata_t info;
	int handle = _findfirst((dir_day_sz + "sz*.day").c_str(), &info);
	if (handle >0)
	{
		do
		{
			ifstream infile(dir_day_sz + string(info.name), ios::binary | ios::in);
			if (infile.is_open())
			{
				vector<Chan> chans;
				chans.reserve(3000);
				DateLine dl;
				Conner up, dn;
				int drc = 0;
				infile.seekg(0, ios::beg);
				while (!infile.eof())
				{
					infile.read((char*)&dl, sizeof(dl));
					if (dl.date<timeA)continue;
					if (dl.open <= 0 || dl.high <= 0 || dl.lowe <= 0 || dl.clos <= 0 || dl.amou<= 0 || dl.volu <= 0 || dl.date <= 0)continue;					
					Chan chan;
					memset(&chan, 0, sizeof(chan));
					chan.date = dl.date;
					chan.averp = dl.amou / dl.volu;
					if (chan.averp>up.averp || up.averp <= 0)
					{
						up.date = chan.date; up.averp = chan.averp;
					}
					if (chan.averp<dn.averp || dn.averp <= 0)
					{
						dn.date = chan.date; dn.averp = chan.averp;
					}
					if ((chan.averp / up.averp - 1) * 100 < -10 && drc!=-1)
					{
						chan.leftup = up;
						up.averp = 0;
						up.date = 0;
						drc = -1;
					}
					if ((chan.averp / dn.averp - 1) * 100 > 10 && drc!=1)
					{
						chan.leftdn = dn;
						dn.averp = 0;
						dn.date = 0;
						drc = 1;
					}
					chans.push_back(chan);
				}
				infile.close();
				if (drc >= 1)chans.back().leftup = up;
				if (drc <= -1)chans.back().leftdn = dn;
				chans.shrink_to_fit();
				for (auto chan = chans.end() - 1; chan >= chans.begin(); chan--)
				{

				}
			}
		} while (_findnext(handle, &info) >= 0);
		_findclose(handle);
	}

	return;
}
void CAna::Moneys()
{
	vector<Moneis> moneis;
	moneis.reserve(5000 * 1000);
	_finddata_t info;
	int handle = _findfirst((dir_day_sz + "sz*.day").c_str(), &info);
	if (handle >0)
	{
		do
		{
			if (string(info.name).find("000", 2) != 2 && string(info.name).find("002", 2) != 2 && string(info.name).find("300", 2) != 2)continue;
			ifstream infile(dir_day_sz + string(info.name), ios::binary | ios::in);
			if (infile.is_open())
			{
				DateLine dl;
				float cc = 0, oo = 0, oc = 0, amt = 0, ams15 = 0;
				int cnt = 0;
				infile.seekg(0, ios::beg);
				while (!infile.eof())
				{
					infile.read((char*)&dl, sizeof(dl));
					if (dl.date<timeA)continue;
					if (dl.date>timeB)timeB = dl.date;
					if (dl.open <= 0 || dl.high <= 0 || dl.lowe <= 0 || dl.clos <= 0 || dl.amou <= 0 || dl.volu <= 0 || dl.date <= 0)continue;
					Moneis money;
					memset(&money, 0, sizeof(money));
					strcpy(money.code, string(info.name).substr(0, 8).c_str());
					money.date = dl.date;
					money.amou = dl.amou;
					money.open = dl.open;
					money.clos = dl.clos;
					if (cnt >= 1)
					{
						oo = (money.open / (moneis.end() - 1)->open-1)*100;
						oc = (money.open / (moneis.end() - 1)->clos-1)*100;
						cc = (money.clos / (moneis.end() - 1)->clos-1)*100;
						money.c0c1 = cc;
						money.o0c1 = oc;
						if(cnt>=2)money.c1c2 = (moneis.end() - 1)->c0c1;
						if (cnt >= 4)money.c1c4 = ((moneis.end() - 1)->clos / (moneis.end() - 4)->clos - 1) * 100;
						if (cnt >= 5)money.c4c5 = ((moneis.end() - 4)->clos / (moneis.end() - 5)->clos - 1) * 100;
						(moneis.end() - 1)->oAo0 = oo;
						(moneis.end() - 1)->oAc0 = oc;
						ams15 += (moneis.end() - 1)->amou / 15;
						if (cnt >= 15)
						{
							if (cnt >= 16)ams15 -= (moneis.end() - 16)->amou / 15;
							if (ams15 > 0)money.amts = money.amou / ams15;
							money.amt1 = (moneis.end() - 1)->amts;
							money.amt2 = (moneis.end() - 2)->amts;
							money.amt3 = (moneis.end() - 3)->amts;
							money.amt4 = (moneis.end() - 4)->amts;
						}
					}
					moneis.push_back(money);
					cnt++;
				}
				infile.close();
			}
		} while (_findnext(handle, &info) >= 0);
		_findclose(handle);
	}
	handle = _findfirst((dir_day_sh + "sh*.day").c_str(), &info);
	if (handle >0)
	{
		do
		{
			if (string(info.name).find("600", 2) != 2 && string(info.name).find("601", 2) != 2 && string(info.name).find("603", 2) != 2)continue;
			ifstream infile(dir_day_sh + string(info.name), ios::binary | ios::in);
			if (infile.is_open())
			{
				DateLine dl;
				float cc = 0, oo = 0, oc = 0, amt = 0, ams15 = 0;
				int cnt = 0;
				infile.seekg(0, ios::beg);
				while (!infile.eof())
				{
					infile.read((char*)&dl, sizeof(dl));
					if (dl.date<timeA)continue;
					if (dl.date>timeB)timeB = dl.date;
					if (dl.open <= 0 || dl.high <= 0 || dl.lowe <= 0 || dl.clos <= 0 || dl.amou <= 0 || dl.volu <= 0 || dl.date <= 0)continue;
					Moneis money;
					memset(&money, 0, sizeof(money));
					strcpy(money.code, string(info.name).substr(0, 8).c_str());
					money.date = dl.date;
					money.amou = dl.amou;
					money.open = dl.open;
					money.clos = dl.clos;
					if (cnt >= 1)
					{
						oo = (money.open / (moneis.end() - 1)->open - 1) * 100;
						oc = (money.open / (moneis.end() - 1)->clos - 1) * 100;
						cc = (money.clos / (moneis.end() - 1)->clos - 1) * 100;
						money.c0c1 = cc;
						money.o0c1 = oc;
						if (cnt >= 2)money.c1c2 = (moneis.end() - 1)->c0c1;
						if (cnt >= 4)money.c1c4 = ((moneis.end() - 1)->clos / (moneis.end() - 4)->clos - 1) * 100;
						if (cnt >= 5)money.c4c5 = ((moneis.end() - 4)->clos / (moneis.end() - 5)->clos - 1) * 100;
						(moneis.end() - 1)->oAo0 = oo;
						(moneis.end() - 1)->oAc0 = oc;
						ams15 += (moneis.end() - 1)->amou / 15;
						if (cnt >= 15)
						{
							if (cnt >= 16)ams15 -= (moneis.end() - 16)->amou / 15;
							if (ams15 > 0)money.amts = money.amou / ams15;
							money.amt1 = (moneis.end() - 1)->amts;
							money.amt2 = (moneis.end() - 2)->amts;
							money.amt3 = (moneis.end() - 3)->amts;
							money.amt4 = (moneis.end() - 4)->amts;
						}
					}
					moneis.push_back(money);
					cnt++;
				}
				infile.close();
			}
		} while (_findnext(handle, &info) >= 0);
		_findclose(handle);
	}
	moneis.shrink_to_fit();
	cout << "Moneis " << moneis.size()<<endl;

	int years = timeB / 10000 - timeA / 10000 + 1;
	sorters.reserve(years);
	int size = 0;
	for (auto it = moneis.begin(); it != moneis.end(); it++)
	{
		if (it->amou <= 0 || it->open <= 0 || it->clos <= 0 || it->amts <= 0)continue;
		if (it->c4c5 < 9.8)continue;
		int yy = it->date / 10000 - timeA / 10000;
		while (yy+1 > (int)sorters.size())
		{
			vector<vector<vector<int>>>ysort;
			sorters.push_back(ysort);
		}
		int mm = (it->date % 10000) / 100 - 1;
		while (mm+1 > (int)sorters[yy].size())
		{
			vector<vector<int>>msort;
			sorters[yy].push_back(msort);
		}
		int dd = it->date % 10000 % 100 - 1;
		while (dd+1 > (int)sorters[yy][mm].size())
		{
			vector<int>dsort;
			dsort.push_back(0);
			sorters[yy][mm].push_back(dsort);
		}
		int dis = it - moneis.begin();
		sorters[yy][mm][dd].push_back(dis);
		sorters[yy][mm][dd][0]++;
		size++;
	}
	cout << "Sorter over. " <<size<< endl;
	/*
	string fn0 = dir_rslt + "ccMoneys.txt";
	ofstream outfile0(fn0, ios::out | ios::trunc);
	double balmax0 = 0;
	int vid0,id0 = 0;
	DTL = false;
	for (double cc = -30; cc <= 30; cc += 1)//double loc = 0;//
	for (double am3 = 0.1; am3 <= 10; am3 *= 1.1)
	for (double am2 = 0.1; am2 <= 10; am2 *= 1.1)
	for (double am1 = 0.1; am1 <= 10; am1 *= 1.1)
	{
		double bal0 = 100000,  max0 = 0, rev0 = 0,cnts=0;
		for (auto yy = sorters.begin(); yy != sorters.end(); yy++)
		for (auto mm = yy->begin(); mm != yy->end(); mm++)
		for (auto dd = mm->begin(); dd != mm->end(); dd++)
		{
			if (dd->front() <= 0)continue;
			vector<Moneis> ddmoneis;
			ddmoneis.reserve(dd->front());
			for (auto dsk = dd->begin() + 1; dsk != dd->end(); dsk++)
			{
				int dis = (*dsk);
				auto amt = moneis.begin() + dis;
				ddmoneis.push_back(*amt);
			}
			ddmoneis.shrink_to_fit();
			sort(ddmoneis.begin(), ddmoneis.end(), AmounSort);
			double fitcc = 0;
			int cnt = 0;
			auto sk = ddmoneis.begin();
			while (cnt < 10 && sk != ddmoneis.end())
			{
				if (abs(sk->o0c1)<11)//weight kick
				if (sk->c1c2 < 9.8)//cant operate
				if (sk->amt3>am3 && sk->amt2>am2 && sk->amt1>am1 && sk->c1c4<cc)
				{
					fitcc += sk->c0c1;
					++cnt;
					if (DTL && outfile0.is_open())
					{
						outfile0 << "cnt	" << sk->date << "	" << sk->code << "	fitcc	" << sk->c0c1 << endl;
					}
				}
				++sk;
			}
			if (cnt > 0)
			{
				fitcc /= cnt;
				bal0 *= (1 + fitcc / 100);
				if (bal0 > max0)max0 = bal0;
				double fall0 = (1 - bal0 / max0) * 100;
				if (fall0 > rev0)rev0 = fall0;
				cnts += cnt;
				if (DTL && outfile0.is_open())
				{
					outfile0 << fixed << setprecision(2) << "fit	" << fitcc << "	bal	" << bal0 << "	rev	" << rev0 << endl;
				}
			}
		}
		if (bal0 > balmax0)
		{
			balmax0 = bal0; 
			vid0 = id0;
			cout << fixed << setprecision(2) << id0 << " cc " << cc << " am3 " << am3 << " am2 " << am2 << " am1 " << am1 
				<<setprecision(0) << " bal0 " << bal0 <<" cnts "<<cnts<< " max " << max0 << " rev " << rev0 << endl;
			if (!DTL && outfile0.is_open())
			{
				outfile0 << fixed << setprecision(2)
					<< id0 << " cc " << cc << " am3 " << am3 << " am2 " << am2 << " am1 " << am1
					<< " bal0 " << bal0 << " cnts " << cnts << " max " << max0 << " rev " << rev0 << endl;
			}
		}
		id0++;
	}
	if (outfile0.is_open())outfile0.close();
	cout << "bal max0 " << balmax0 << " vid " << vid0 << endl;
	*/
	DTL = false;
	string fn1;
	if (DTL)fn1 = dir_rslt + "ooMoneysDTL.txt";
	else fn1 = dir_rslt + "ooMoneys.txt";
	ofstream outfile1(fn1, ios::out | ios::trunc);
	double balmax1 = 0;
	int vid1, id1 = 0;
	for (double am3 = 0.2; am3 <= 5; am3 *= 1.5)for (double am3b = am3*1.5; am3b <= 6; am3b *= 1.5)
	for (double am2 = 0.2; am2 <= 5; am2 *= 1.5)for (double am2b = am2*1.5; am2b <= 6; am2b *= 1.5)
	for (double am1 = 0.2; am1 <= 5; am1 *= 1.5)for (double am1b = am1*1.5; am1b <= 6; am1b *= 1.5)
	for (double cc = -20; cc <= 15; cc += 5)for (double ccb = cc + 5; ccb <= 20; ccb += 5)
	for (double oc = -10; oc <= 9; oc += 1)for (double ocb = oc + 1; ocb <= 10; ocb += 1)
	{
<<<<<<< HEAD
		if (id1 <28*1000*1000){ id1++; continue; }//18474317//1401147//42123788
=======
		if (id1 <9*1000*1000){ id1++; continue; }//18474317//1401147//42123788
>>>>>>> a2c3e11642e5b2df22612a35645e9ab548620bd1
		//if (id1 < 8666266){ id1++; continue; }//18474317//1401147
		//if (id1 > 3666266)break;
		double bal1 = 100000, max1 = 0, rev1 = 0, cnts = 0;
		for (auto yy = sorters.begin(); yy != sorters.end(); yy++)
		for (auto mm = yy->begin(); mm != yy->end(); mm++)
		for (auto dd = mm->begin(); dd != mm->end(); dd++)
		{
			if (dd->front() <= 0)continue;
			vector<Moneis> ddmoneis;
			ddmoneis.reserve(dd->front());
			for (auto dsk = dd->begin() + 1; dsk != dd->end(); dsk++)
			{
				int dis = (*dsk);
				auto amt = moneis.begin() + dis;
				ddmoneis.push_back(*amt);
			}
			ddmoneis.shrink_to_fit();
			sort(ddmoneis.begin(), ddmoneis.end(), AmounSort);
			double fitoo = 0;
			int cnt = 0;
			auto sk = ddmoneis.begin();
			while (cnt < 10 && sk != ddmoneis.end())
			{
				if (abs(sk->oAc0)<11)//weight kick
				if (sk->o0c1 < 9.8)//cant operate
				if (am3 <= sk->amt3 && sk->amt3<am3 * 2)
				if (am2 <= sk->amt2 && sk->amt2<am2 * 2)
				if (am1 <= sk->amt1 && sk->amt1<am1 * 2)
				if (cc <= sk->c1c4 && sk->c1c4<cc + 2)
				if (oc <= sk->o0c1 && sk->o0c1<oc+1)
				{
					fitoo += sk->oAo0;
					++cnt;
					if (DTL && outfile1.is_open())
					{
						//outfile1 << "cnt	" << sk->date << "	" << sk->code << "	fitoo	" << sk->oAo0 << endl;
					}
				}
				++sk;
			}
			if (cnt > 0)
			{
				fitoo /= cnt;
				bal1 *= (1 + fitoo / 100);
				if (bal1 > max1)max1 = bal1;
				double fall1 = (1 - bal1 / max1) * 100;
				if (fall1 > rev1)rev1 = fall1;
				cnts += cnt;
				if (DTL && outfile1.is_open())
				{
					outfile1 << fixed << setprecision(2) << "fit	" << fitoo << "	bal	" << bal1 << "	rev	" << rev1 << endl;
				}
			}
		}
		if (bal1 > balmax1 || bal1>10000000)
		{
			if (bal1 > balmax1)
			{
				balmax1 = bal1;
				vid1 = id1;
			}
			time_t tm; time(&tm);
			cout << fixed 
				<< id1 << " cc " << setprecision(0) << cc << " " << ccb << " am3 " << setprecision(2) << am3 << " " << am3b << " am2 " << am2 << " " << am2b << " am1 " << am1 << " " << am1b << " oc " << setprecision(0) << oc << " " << ocb << endl
				<< "  balmax1 " << balmax1 << "    bal1 " << setprecision(0) << bal1 << " cnts " << cnts << " max " << max1 << " rev " << setprecision(2) << rev1 << " " << ctime(&tm);
			if (!DTL)
			{
				ofstream outfile1(fn1, ios::out | ios::trunc);
				if (outfile1.is_open())
				{
					outfile1 << fixed
						<< id1 << " cc " << setprecision(2) << cc << " " << ccb << " am3 " << am3 << " " << am3b << " am2 " << am2 << " " << am2b << " am1 " << am1 << " " << am1b << " oc " << oc << " " << ocb
						<< "  balmax1 " << balmax1 << " bal1 " << bal1 << " cnts " << cnts << " max " << max1 << " rev " << rev1 << endl;
					outfile1.close();
				}
			}
		}
<<<<<<< HEAD
		if (fmod(id1, 100000) == 0)
		{
			time_t tm; time(&tm);
			cout << fixed << id1 / 100000 << "M cc " << setprecision(0) << cc << " " << ccb << " am3 " << setprecision(2) << am3 << " " << am3b << " am2 " << am2 << " " << am2b << " am1 " << am1 << " " << am1b << " oc " << setprecision(0) << oc << " " << ocb << ctime(&tm);
=======
		if (fmod(id1, 10*10000) == 0)
		{
			time_t tm; time(&tm);
			cout << fixed << id1  << " cc " << setprecision(0) << cc << " " << ccb << " am3 " << setprecision(2) << am3 << " " << am3b << " am2 " << am2 << " " << am2b << " am1 " << am1 << " " << am1b << " oc " << setprecision(0) << oc << " " << ocb << ctime(&tm);
>>>>>>> a2c3e11642e5b2df22612a35645e9ab548620bd1
		}
		id1++;
	}
	if (outfile1.is_open())outfile1.close();
	//cout << "bal max0 " << balmax0 << " vid " << vid0 << endl;
	cout << "bal max1 " << balmax1 << " vid " << vid1 << endl;
	return;
}
void CAna::Hexin()
{
	byte type[6];//byte[6]，6 字节长度，固定为{ 0x68，0x64，0x31，0x2E， 0x30，0x00 }，用于识别数据文件类型；
	DWORD recnum;//dword，4 字节长度，记录“内容”区域的记录条数；
	WORD start;//word，2 字节长度，记录“内容”区域的开始位置(权息和股本结构需要 + 0x10000)；
	WORD recsiz;//word，2 字节长度，记录“内容”区域每条记录的字节长度；
	WORD rownum;//word，2字节长度，记录“列定义”的列个数(高字节可能不是0x00, 需要去除)；
	byte rowdef1,rowdef2,rowdef3,rowdef4;
	WORD indlen;// •word，2 个字节，记录本索引数据区域的字节长度；
	WORD indnum;//	•word，2 个字节，记录本索引数据区域中所包含的索引的条数（高字节需要 & 0x1FFF）；
	/////	•byte[]，不定长的索引记录，每条索引记录为18个字节长度，因此总长度 = 索引条数 * 0x12；
	byte stype; //•byte，1 字节长度，标识证券品种类型，目前已知的包括3 类4 种标示，分别是：0x10，国内证券；0x48和0x50，港股；0x4A，基金。在“权息资料”中，使用0x50标示港股，在“现金流量”中，使用0x48标示港股，不知道是设计的 bug，还是存在其他用意；
	byte sname[9];//	•byte[9]，9 字节长度，标识交易品种的符号，目前已知是ASCII格式，从整个文件的多字节字符处理来看，文件的整体字符编码应该是GB2312编码，因此这一部分原始的处理应该是使用GB2312编码通读；
	WORD nullsrecnum;//	•word，2 字节长度，标识隶属此交易品种的记录区域中实际上未使用的记录条数，有关的组织方式下面再详细解释；
	DWORD srecstart;//	•dword，4 字节长度，标识隶属于此交易品种的记录的开始下标，注意是记录在数据文件中从0 开始排列的顺序号，而不是文件的偏移地址，其实际的偏移地址 = “内容”区域的开始地址 + 此开始下标 * 记录的长度；
	WORD srecnum;//	•word，2 个字节长度，标识隶属于此交易品种的记录的条数；
	typedef char TIM[8];
	typedef int DAT;
	ifstream infile("D:\\同花顺软件\\同花顺\\finance\\权息资料.财经", ios::binary | ios::in);
	if (infile.is_open())
	{
		infile.read((char*)&type, sizeof(type));
		infile.read((char*)&recnum, sizeof(recnum));
		infile.read((char*)&start, sizeof(start));
		infile.read((char*)&recsiz, sizeof(recsiz));
		infile.read((char*)&rownum, sizeof(rownum));
		for (int i = 0; i < rownum; i++)
		{
			infile.read((char*)&rowdef1, sizeof(rowdef1));
			infile.read((char*)&rowdef2, sizeof(rowdef2));
			infile.read((char*)&rowdef3, sizeof(rowdef3));
			infile.read((char*)&rowdef4, sizeof(rowdef4));
			cout << dec << rowdef1 << " " <<rowdef2 << " " << rowdef3 << " " << dec << rowdef4 << endl;
			//printf("%x\n", rowdef);
			//printf("%c\n", rowdef);
			//printf("%s\n", rowdef);
			
		}
		//索引区的开始位置 = 0x10 + 0x04 * 列定义数量 + 0x02 * 列定义数量
		infile.seekg(0x10 + 0x04 * rownum + 0x02 * rownum,ios::beg);
		infile.read((char*)&indlen, sizeof(indlen));
		infile.read((char*)&indnum, sizeof(indnum));
		cout <<dec<< indlen << " " << indnum << endl;
		for (int i = 0; i < indnum; i++)
		{
			infile.seekg(0x10 + 0x04 * rownum + 0x02 * rownum + 0x04 + i * 0x12, ios::beg);
			infile.read((char*)&stype, sizeof(stype));
			infile.read((char*)&sname, sizeof(sname));
			infile.read((char*)&nullsrecnum, sizeof(nullsrecnum));
			infile.read((char*)&srecstart, sizeof(srecstart));
			infile.read((char*)&srecnum, sizeof(srecnum));
			cout << hex << stype << " " << sname << " " << nullsrecnum << " " << srecstart << " " << srecnum << endl;
			//infile.seekg(start + 0x10000 + srecstart*recsiz, ios::beg);
			infile.seekg(start + 0x10000+ srecstart*recsiz, ios::beg);
			for (int j = 0; j < srecnum; j++)
			{
				char line[1024] = {0};
				infile.read(line, sizeof(recsiz));
				TIM a0, a1, a2, a3,a4;
				DAT b0, b1, b2, b3, b4;
				char cc[512];
				std::stringstream strm(line);
				strm >> a0 >> a1 >> a2 >> b0 >> b1 >> b2 >> b3 >> b4 >> a3 >> a4 >> cc;
			}
		}
		infile.close();
	}
}