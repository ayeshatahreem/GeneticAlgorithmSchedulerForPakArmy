#pragma once

#include <iostream> 
#include <string>
#include<fstream>
#include <vector>
#include "Troop.h"
#include "TroopLeader.h"
#include "TargetArea.h"
#include "SpecialTroopGroup.h"
#include "Class.h"
#include "Loader_s.h"
using namespace std;


class Loader
{
public:
	vector<TroopLeader*> TroopLeaders;
	vector<Troop*> Troops;
	vector<Class*> chromosomes;
	vector<TargetArea*> TargetAreas;
	vector<SpecialTroopGroup*> groups;

	void inputLeaders()
	{
		ifstream in;
		in.open("TroopLeaders.txt");
		int IDCount = 0;
		while (!in.eof())
		{
			string str;
			getline(in, str);
			string n = str.substr(0, str.find_first_of(","));
			string a = str.substr(str.find_first_of(",") + 2, 6);
			TroopLeader* p = new TroopLeader;
			p->id = IDCount++;
			p->name = n;
			for (int i = 0; i<6; i++)
			{
				if (a[i] == 1)
					p->presence.push_back(1);
				else
					p->presence.push_back(0);
			}
			TroopLeaders.push_back(p);
		}
		in.close();
	}

	void inputTargetAreas()
	{
		ifstream in;
		in.open("TargetAreas.txt");
		int IDCount = 0;
		while (!in.eof())
		{
			string str;
			getline(in, str);
			TargetArea *p = new TargetArea;
			p->id = IDCount++;
			p->name = str;
			TargetAreas.push_back(p);
		}
		in.close();
	}

	void inputTroops()
	{
		ifstream in;
		in.open("Troops.txt");
		int IDCount = 0;
		while (!in.eof())
		{
			string str;
			getline(in, str);
			Troop *p = new Troop;
			p->id = IDCount++;
			p->code = str.substr(0, str.find_first_of(" "));
			p->name = str.substr(str.find_first_of(" ") + 1, str.length());
			Troops.push_back(p);
		}
		in.close();
	}
	void inputSpecialTroopGroups()
	{
		ifstream in;
		in.open("TroopGroups.txt");
		int IDCount = 0;
		while (!in.eof())
		{
			string str;
			in >> str;
			SpecialTroopGroup* p = new SpecialTroopGroup;
			p->id = IDCount++;
			p->name = str;
			groups.push_back(p);
		}
		in.close();
	}

	void inputchromosomes()
	{
		ifstream in;
		in.open("SubTroops.txt");
		int IDCount = 0;
		if (in.is_open())
		{
			while (!in.eof())
			{
				string str;
				Class *p = new Class;
				p->id = IDCount++;
				in >> str;
				int i = 0;
				while (str != Troops[i]->code)
					i++;
				p->Troop = Troops[i];
				in >> str;
				p->unit = str[0];
				int t;
				in >> t;
				i = 0;
				while (t != TroopLeaders[i]->id)
					i++;
				p->Leader = TroopLeaders[i];
				int s;
				in >> s;
				i = 0;
				while (s != groups[i]->id)
					i++;
				p->special.push_back(groups[i]);
				chromosomes.push_back(p);
			}
			in.close();
		}
	}
};

