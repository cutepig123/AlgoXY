#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <assert.h>

using namespace std;

// Forward declaration of isBadVersion API.
bool isBadVersion(int version)
{
	return version >= 5;
}

class Solution {
public:
	int firstBadVersion(int n) {
		int first = 1;
		int last = n;
		while (last>first)
		{
			int mid = first + (last - first) / 2;
			if (isBadVersion(mid))
			{
				last = mid;
			}
			else
			{
				first = mid +1;
			}
		}

		return first;
	}


};

void testQ278() {
	Solution s;
	for(int i=5; i<20; i++)
		assert(5 == s.firstBadVersion(i));
}