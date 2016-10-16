#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <mutex>
#include <thread>
#include <assert.h>

using namespace std; 


bool Find(vector<vector<int> > array, int target) {
	int y = 0;
	int x = array[0].size() - 1;
	while (x>=0 && y<array.size())
	{
		if (array[y][x] == target) return true;
		if (target > array[y][x]) {
			y++;
		}
		else {
			x--;
		}
	}
	return false;
}

string replace(string const &str, const char *sOrig, const char *sRep) {
	string::size_type i = 0, j = str.find(sOrig, i);
	string des;
	while(j != string::npos ) {
		des += str.substr(i, j - i);
		des += sRep;
		i = j + 1;
		j = str.find(sOrig, i);
	}
	des += str.substr(i);

	return move(des);
}

void replaceSpace(char *str, int length) {

}

void testReplace() {
	assert("We%20Are%20Happy"== replace("We Are Happy", " ", "%20"));
	assert("We%20Are%20Happy%20" == replace("We Are Happy ", " ", "%20"));
}