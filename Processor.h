#pragma once

#include <iostream> 
#include <string>
#include <vector>
#include <random>
#include "Evaluator.h"
#include "Loader_s.h"
using namespace std;

class Processor
{
public:
	mt19937 rng;
	vector<Evaluator*>  community;
	int  communityTotal;
	vector<Evaluator*> nextGen;
	float genGap;
	float elitism;
	float mutProb;
	Loader_s d;

	Processor(int p, float g, float e, float m)
	{
		communityTotal = 20;
		genGap = -1;
		elitism = 0.1;
		mutProb = 0.2;
		if (p != 0)
			communityTotal = p;
		if (g != 0)
			genGap = g;
		if (e != 0)
			elitism = e;
		if (m != 0)
			mutProb = m;
	}

	void initialize()
	{
		for (int i = 0; i< communityTotal; i++)
		{
			community.push_back(new Evaluator);
			community[i]->fillRandomly();
		}
	}
	void evaluate()
	{
		for (int i = 0; i< communityTotal; i++)
			community[i]->evaluateFitness();
	}


	void sortPopulation()
	{
		float sum = 0;
		for (int i = 0; i< communityTotal; i++)
			sum = sum + community[i]->fitness;
		for (int i = 0; i< communityTotal; i++)
			community[i]->relFitness = community[i]->fitness / sum;
		for (int i = 1; i< communityTotal; i++)
		{
			float key = community[i]->relFitness;
			int j = i - 1;
			while (j>0 && community[j]->relFitness>key)
			{
				community[j + 1]->relFitness = community[j]->relFitness;
				j--;
			}
			community[j + 1]->relFitness = key;
		}
	}


	void selectParents(Evaluator* & p1, Evaluator* & p2)
	{

		int wheel[100];
		for (int i = 0; i<100; i++)
			wheel[i] = 0;
		int filled = 0;
		for (int i = 0; i< communityTotal; i++)
		{
			int temp = community[i]->relFitness * 100;
			for (int j = filled; j<temp + filled; j++)
				wheel[j] = i;
			filled = filled + temp;
		}
		uniform_int_distribution<int> randomGenerator(0, 100);
		p1 = community[wheel[randomGenerator(rng)]];
		p2 = community[wheel[randomGenerator(rng)]];
		while (p1 == p2)
		{
			p2 = community[wheel[randomGenerator(rng)]];
		}
	}

	void crossoverTwoPoint(Evaluator* parent1, Evaluator* parent2, Evaluator* & child1, Evaluator* & child2)
	{
		uniform_int_distribution<int> randomGenerator(0, parent1->total);
		int point1 = randomGenerator(rng);
		int point2 = randomGenerator(rng);
		for (int i = 0; i<point1; i++)
		{
			child1->chromosomes.push_back(parent1->chromosomes[i]);
			child2->chromosomes.push_back(parent2->chromosomes[i]);
		}
		for (int i = point1; i<point2; i++)
		{
			child1->chromosomes.push_back(parent2->chromosomes[i]);
			child2->chromosomes.push_back(parent1->chromosomes[i]);
		}
		for (int i = point2; i<parent1->total; i++)
		{
			child1->chromosomes.push_back(parent1->chromosomes[i]);
			child2->chromosomes.push_back(parent2->chromosomes[i]);
		}
	}

	void generateNext()
	{
		nextGen.clear();
		for (int i = 0; i< communityTotal; i++)
			community[i]->evaluateFitness();
		sortPopulation();
		if (genGap == -1)
		{
			int childGentotal = communityTotal;
			for (int i = 0; i< communityTotal / 2; i++)
			{
				Evaluator* s1 = new Evaluator();
				Evaluator* s2 = new Evaluator();
				Evaluator* p1, *p2;
				selectParents(p1, p2);
				crossoverTwoPoint(p1, p2, s1, s2);
				nextGen.push_back(s1);
				nextGen.push_back(s2);
			}
			if (communityTotal % 2 != 0)
			{
				Evaluator* extra = community[0];
				nextGen[communityTotal - 1] = extra;
			}
			for (int i = 1; i< communityTotal; i++)
				nextGen[i]->evaluateFitness();


			int passOn = elitism* communityTotal;
			for (int i = 0; i< communityTotal - passOn; i++)
				nextGen.push_back(community[i]);

			float sum = 0;
			for (int i = 0; i< communityTotal; i++)
				sum = sum + nextGen[i]->fitness;
			for (int i = 0; i< communityTotal; i++)
				nextGen[i]->relFitness = community[i]->fitness / sum;
			for (int i = 1; i< communityTotal * 2 - passOn; i++)
			{
				float key = nextGen[i]->relFitness;
				int j = i - 1;
				while (j>0 && nextGen[j]->relFitness>key)
				{
					nextGen[j + 1]->relFitness = nextGen[j]->relFitness;
					j--;
				}
				nextGen[j + 1]->relFitness = key;
			}
			int j = 0;
			for (int i = communityTotal * 2 - passOn - 1; i> communityTotal; i--) {
				community[j] = nextGen[i]; j++;
			}
		}
		else
		{
			int passOn = (1 - genGap)* communityTotal;
			int newGentotal = communityTotal - passOn;
			for (int i = 0; i<passOn; i++)
				nextGen.push_back(community[communityTotal - 1 - i]);
			for (int i = passOn; i< communityTotal / 2; i++)
			{
				Evaluator* s1 = new Evaluator();
				Evaluator* s2 = new Evaluator();
				Evaluator* p1, *p2;
				selectParents(p1, p2);
				crossoverTwoPoint(p1, p2, s1, s2);
				nextGen.push_back(s1);
				nextGen.push_back(s2);
			}
			for (int i = 0; i< communityTotal; i++)
				community[i] = nextGen[i];
		}
	}

	void mutateRandom()
	{
		uniform_int_distribution<int> randomGenerator(0, 1000);
		for (int i = 0; i< communityTotal; i++)
		{
			for (int j = 0; j< community[i]->total; j = j + 6)
			{
				if (randomGenerator(rng) / 1000 * 1.0<mutProb)
				{
					uniform_int_distribution<int> mutationSelector(j, j + 6);
					uniform_int_distribution<int> mutationGenerator(0, d.getSystem()->chromosomes.size());
					int to = mutationSelector(rng);
					int with = mutationGenerator(rng);
					community[i]->chromosomes[to] = d.getSystem()->chromosomes[with];
				}
			}
		}
		evaluate();
	}

	void mutateABit()
	{
		for (int i = 0; i< communityTotal; i++)
		{
			community[i]->mutate();
		}
		sortPopulation();
	}
};
