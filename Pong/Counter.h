#pragma once

#include <iostream>

class Counter
{
public:
	Counter() :
		count1(0),
		count2(0)
	{
		std::cout << count1 << ":" << count2 << std::endl;
	}

	void Add1();
	void Add2();

	int count1;
	int count2;
};

