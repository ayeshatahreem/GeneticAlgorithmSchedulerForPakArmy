#pragma once

#include <iostream> 
#include <string>
#include <vector>
#include "Loader.h"
using namespace std;

class Loader_s
{
private:
	static Loader* lptr;

public:

Loader_s::Loader_s(){
}

Loader* Loader_s::getSystem()
{
	if (lptr == nullptr)
	{
		lptr = new Loader();
	}
	return lptr;
}

};
Loader* Loader_s::lptr = nullptr;
