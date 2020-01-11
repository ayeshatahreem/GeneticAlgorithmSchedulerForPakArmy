#pragma once

#include <iostream> 
#include <string>
#include <random>
#include <ctime>
#include <vector>
using namespace std;
#include "Class.h"
#include "Loader_s.h"
class Evaluator
{
public:
	vector<Class*> chromosomes;
	int total;
	float fitness;
	float relFitness;
	mt19937 rng;
	vector<int> tracking;
	vector<int> CounterAttack;
	vector<Class*> absent;
	vector<Class*> moreThanOnce;
	Loader_s d;
	Evaluator()
	{
		fitness = 0;
		for (int i = 0; i<8; i++)
		{
			tracking.push_back(0);
			CounterAttack.push_back(0);
		}
		absent.clear();
		moreThanOnce.clear();
		total = 6 * 18 * 2;
	}

	void fillRandomly()
	{
		uniform_int_distribution<int> randomGenerator(0, d.getSystem()->chromosomes.size() - 1);
		rng.seed(time(NULL));
		chromosomes.clear();
		for (int i = 0; i<total; i++)
		{
			int index = randomGenerator(rng);
			chromosomes.push_back(d.getSystem()->chromosomes[index]);
		}
	}

	float evaluateFitness()
	{
		fitness = 0;
		int count = 0;
		tracking[count] = 0;
		CounterAttack[count] = 0;
		for (int a = 0; a<6; a++)
		{
			for (int i = a; i<total / 2; i = i + 6)
			{
				bool check = true;
				for (int j = i + 6; j<total && check == false; j = j + 6)
				{
					if (chromosomes[j]->Leader == chromosomes[i]->Leader)
					{
						check = false;
						CounterAttack[count]++;
					}
				}
				if (check)
					fitness++;
			}
		}
		for (int a = total / 2; a<6; a++)
		{
			for (int i = a; i<total; i = i + 6)
			{
				bool check = true;
				for (int j = i + 6; j<total && check == false; j = j + 6)
				{
					if (chromosomes[j]->Leader == chromosomes[i]->Leader)
					{
						check = false;
						CounterAttack[count]++;
					}
				}
				if (check)
					fitness++;
			}
		}
		if (CounterAttack[count] == 0)
			tracking[count] = 1;

		count++;
		tracking[count] = 0;
		CounterAttack[count] = 0;
		for (int a = 0; a<6; a++)
		{
			for (int i = a; i<total / 2; i = i + 6)
			{
				for (int k = 0; k<chromosomes[i]->special.size(); k++)
				{
					bool check = true;
					for (int j = i + 6; j<total; j = j + 6)
					{
						for (int l = 0; l<chromosomes[j]->special.size(); l++)
						{
							if (chromosomes[i]->special[k] == chromosomes[j]->special[l])
							{
								check = false;
								CounterAttack[count]++;
							}
						}
					}
					if (check)
						fitness++;
				}
			}
		}
		for (int a = total / 2; a<6; a++)
		{
			for (int i = a; i<total; i = i + 6)
			{
				for (int k = 0; k<chromosomes[i]->special.size(); k++)
				{
					bool check = true;
					for (int j = i + 6; j<total; j = j + 6)
					{
						for (int l = 0; l<chromosomes[j]->special.size(); l++)
						{
							if (chromosomes[i]->special[k] == chromosomes[j]->special[l])
							{
								check = false;
								CounterAttack[count]++;
							}
						}
					}
					if (check)
						fitness++;
				}
			}
		}
		if (CounterAttack[count] == 0)
			tracking[count] = 1;

		count++;
		tracking[count] = 1;
		CounterAttack[count] = 0;

		count++;
		tracking[count] = 0;
		CounterAttack[count] = 0;
		absent.clear();
		moreThanOnce.clear();
		for (int i = 0; i<d.getSystem()->chromosomes.size(); i++)
		{
			int c = 0;
			for (int j = 0; j<total; j++)
			{
				if (chromosomes[j] == d.getSystem()->chromosomes[i])
					c++;
			}
			if (c == 1)
				fitness++;
			else if (c == 0)
				absent.push_back(d.getSystem()->chromosomes[i]);
			else
				moreThanOnce.push_back(d.getSystem()->chromosomes[i]);
		}
		CounterAttack[count] = absent.size();
		if (CounterAttack[count] == 0)
			tracking[count] = 1;

		count++;
		tracking[count] = 0;
		CounterAttack[count] = moreThanOnce.size();
		if (CounterAttack[count] == 0)
			tracking[count] = 1;

		count++;
		tracking[count] = 0;
		CounterAttack[count] = 0;
		for (int a = 0; a<6; a++)
		{
			for (int i = a; i<total; i = i + 6)
			{
				if (chromosomes[i]->Leader->presence[a])
					fitness = fitness + 0.5;
				else
					CounterAttack[count]++;
			}
		}
		if (CounterAttack[count] == 0)
			tracking[count] = 1;

		count++;
		tracking[count] = 0;
		CounterAttack[count] = 0;
		for (int i = 0; i<total / 2; i++)
		{
			int c = 0;
			for (int j = i; j<total / 2; j++)
			{
				if (chromosomes[i]->Leader == chromosomes[j]->Leader)
					c++;
			}
			if (c>2)
				CounterAttack[count]++;
			else
				fitness = fitness + 0.5;
		}
		for (int i = total / 2; i<total; i++)
		{
			int c = 0;
			for (int j = i; j<total; j++)
			{
				if (chromosomes[i]->Leader == chromosomes[j]->Leader)
					c++;
				else
					fitness = fitness + 0.5;
			}
			if (c>2)
				CounterAttack[count]++;
			else
				fitness = fitness + 0.5;
		}
		if (CounterAttack[count] == 0)
			tracking[count] = 1;

		count++;
		tracking[count] = 0;
		CounterAttack[count] = 0;
		for (int i = 0; i<total / 2; i++)
		{
			for (int k = 0; k<chromosomes[i]->special.size(); k++)
			{
				int c = 0;
				for (int j = i; j<total / 2; j++)
				{
					for (int l = 0; l<chromosomes[j]->special.size(); l++)
					{
						if (chromosomes[i]->special[k] == chromosomes[j]->special[l])
							c++;
					}
				}
				if (c>4)
					CounterAttack[count]++;
				else
					fitness = fitness + 0.5;
			}
		}
		for (int i = total / 2; i<total; i++)
		{
			for (int k = 0; k<chromosomes[i]->special.size(); k++)
			{
				int c = 0;
				for (int j = i; j<total; j++)
				{
					for (int l = 0; l<chromosomes[j]->special.size(); l++)
					{
						if (chromosomes[i]->special[k] == chromosomes[j]->special[l])
							c++;
					}
				}
				if (c>4)
					CounterAttack[count]++;
				else
					fitness = fitness + 0.5;
			}
		}
		if (CounterAttack[count] == 0)
			tracking[count] = 1;
		return fitness;
	}

	void mutate()
	{
		for (int i = 0; i<total && absent.size() != 0; i++)
		{
			for (int j = 0; j<moreThanOnce.size() && absent.size() != 0; j++)
			{
				if (moreThanOnce[j] == chromosomes[i])
				{
					chromosomes[i] = absent[0];
					evaluateFitness();
				}
			}
		}
	}

	void removemoreThanOnces()
	{
		for (int i = 0; i<moreThanOnce.size(); i++)
		{
			int e = 0;
			for (int j = 0; j<total; j++)
			{
				if (moreThanOnce[i] == chromosomes[j])
				{
					e++;
					if (e>1)
					{
						chromosomes[j] = nullptr;
					}
				}
			}
		}
		CounterAttack[4] = 0;
		tracking[4] = 1;
	}
};

