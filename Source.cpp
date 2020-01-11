#include <iostream> 
#include <string>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <vector>
#include <random>

#include "Troop.h"
#include "TroopLeader.h"
#include "TargetArea.h"
#include "SpecialTroopGroup.h"
#include "Class.h"
#include "Loader_s.h"
#include "Evaluator.h"
#include "Processor.h"
using namespace std;

class TargetArea;
class TroopLeader;
class Troop;
class SpecialTroopGroup;
class Class;
class Evaluator;
class Loader;
class Processor;

int main()
{
	system("Color 8F");
	cout << '\t' << '\t' << '\t' << '\t' << "INTELLIGENT PAK-ARMY SCHEDULER" << endl << endl;
	cout << '\t' << '\t' << '\t' << '\t' << "AGENT STAR REPORTING ON DUTY" << endl;
	cout << "Taking input from files!! Duzz Duzz!!" << endl;
	Loader_s d;
	d.getSystem()->inputLeaders();
	d.getSystem()->inputTargetAreas();
	d.getSystem()->inputTroops();
	d.getSystem()->inputSpecialTroopGroups();
	d.getSystem()->inputchromosomes();
	cout << "Pls wait, I'm searching very hard for the solution :') " << endl;
	string output;
	int p = 0, g = 0, e = 0, m = 0, n = 0;
	Processor generator(p, g, e, m);
	Evaluator *optimal;
	float maxFitness = 0;
	generator.initialize();
	cout << "10% ..." << endl;
	generator.evaluate();
	generator.sortPopulation();
	cout << "20% ..." << endl;
	optimal = generator.community[generator.communityTotal - 1];
	int c = 0;
	n = 10;
	while (c<n)
	{
		generator.generateNext();
		generator.evaluate();
		generator.mutateABit();
		generator.sortPopulation();
		if (c == 1){
			cout << "30% ... " << endl;
		}
		if (c == 2){
			cout << "40% ... " << endl;
		}
		if (c == 3){
			cout << "50% ..." << endl;
		}
		if (c == 4){
			cout << "60% ..." << endl;
		}
		if (c == 5){
			cout << "70% ... " << endl;
		}
		if (generator.community[generator.communityTotal - 1]->fitness > optimal->fitness)
			optimal = generator.community[generator.communityTotal - 1];

		c++;
	}
	cout << "80% ..." << endl;
	optimal->removemoreThanOnces();
	cout << "Generating excel sheet to present Chief of Army Staff !" << endl;
	ofstream out;
	out.open("PakArmyScheduler.csv");
	int r = 1;
	out << endl << "OPERATION-1, Week-1, Week-2, Week-3, Week-4, Week-5, Week-6" << endl;

	for (int i = 0; i<optimal->total / 2; i++)
	{
		if (i % 6 == 0)
			out << endl << "City " << r++;
		if (optimal->chromosomes[i] != nullptr)
			out << ", " << optimal->chromosomes[i]->Troop->name + ":" + optimal->chromosomes[i]->unit + "- " + optimal->chromosomes[i]->Leader->name;
	}
	out << endl;
	cout << "90% ..." << endl;
	r = 1;
	out << endl << "OPERATION-2, Week-1, Week-2, Week-3, Week-4, Week-5, Week-6" << endl;
	for (int i = optimal->total / 2; i<optimal->total; i++)
	{
		if (i % 6 == 0)
			out << endl << "City " << r++;
		if (optimal->chromosomes[i] != nullptr)
			out << "," << optimal->chromosomes[i]->Troop->name + ":" + optimal->chromosomes[i]->unit + "- " + optimal->chromosomes[i]->Leader->name;

	}
	cout << "100% ..." << endl;
	cout << "Finalizing now,!" << endl;
	out.close();

	cout << "Schedule on excel sheet for Chief of Army Staff is ready !" << endl;
	out.open("ViolationConstraintsReport.txt");
	string tracking[8];
	tracking[0] = "Hard contraints alongwith violation count:\n1.Troop leader can lead one troop group at a time = ";
	tracking[1] = "2. Troop group should be assigned one duty at one particular time = ";
	tracking[2] = "3. A target area should have one troop at a time = ";
	tracking[3] = "4. All troops must be assigned to their duties = ";
	tracking[4] = "5. A troop must have one duty at a particular area within every two duty periods = ";
	tracking[5] = "Soft tracking alongwith violation count:\n1. Troop leader must be available at the time of operation = ";
	tracking[6] = "2. Troop leader should have no more than two duties in a single period = ";
	tracking[7] = "3. Officer having clashes should not have more than three duties during a single operation = ";
	for (int i = 0; i < 8; i++)
	{
		out << tracking[i] << optimal->CounterAttack[i] << endl;
	}
	out.close();
	system("PAUSE");
	return 0;
}