// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "string"
#include "iostream"
#include "assert.h"
#include "vector"
#include "map"
#include "deque"
#include "algorithm"

using namespace std;

int first_deno(int kinds)
{
	int x[] = { 1, 5, 10, 25, 50 };
	assert(kinds >= 1 && kinds <= 5);
	return x[kinds - 1];
}

int cc(int amout, int kinds)
{
	if (amout == 0) return 1;
	if (amout < 0 || kinds == 0) return 0;

	return cc(amout, kinds - 1) + cc(amout - first_deno(kinds), kinds);
}

int solve1()
{
	return cc(100, 5);
}

int solve2()
{
	int n = 0;
	for (int a = 0; a <= 100 / 1; a++)
	{
		int left1 = 100 - a * 1;

		for (int b = 0; b <= left1 / 5; b++)
		{
			int lef2 = left1 - b * 5;

			for (int c = 0; c <= lef2 / 10; c++)
			{
				int lef3 = lef2 - 10 * c;

				for (int d = 0; d <= lef3 / 25; d++)
				{
					int lef4 = lef3 - 25 * d;

					if (lef4 % 50 == 0)n++;
				}
			}
		}
	}
	return n;
}

int main(int argc, char* argv[])
{
	std::cout << solve1() << "\n";
	std::cout << solve2() << "\n";
	return 0;
}

