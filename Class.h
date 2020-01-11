#pragma once

#include <iostream> 
#include <string>
#include <vector>
#include "Troop.h"
#include "TroopLeader.h"
#include "SpecialTroopGroup.h"
using namespace std;

class Class
{
public:
	int id;
	char unit;
	int day;
	int timeslot;
	Troop *Troop;
	TroopLeader *Leader;
	vector<SpecialTroopGroup*> special;
};