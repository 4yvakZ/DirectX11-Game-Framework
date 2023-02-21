#include "Counter.h"


void Counter::Add1()
{
	count1++;
	std::cout << count1 << ":" << count2 << std::endl;
}

void Counter::Add2()
{
	count2++;
	std::cout << count1 << ":" << count2 << std::endl;
}
