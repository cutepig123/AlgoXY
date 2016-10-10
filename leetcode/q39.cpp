#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <assert.h>

using namespace std;

class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        auto p1 = lower_bound(nums.begin(), nums.end(), target);	// the 1st value >= val
		auto p2 = upper_bound(nums.begin(), nums.end(), target);	// the 1st value > val
		if(p1==nums.end()) return {-1,-1};
		if(*p1!=target) return {-1,-1};
		return {p1-nums.begin(),p2-1-nums.begin()};
    }
};

void testQ39()
{
	Solution s;
	vector<int> v=s.searchRange(vector<int>{5, 7, 7, 8, 8, 10}, 8);
	assert(v[0]==3 && v[1]==4);
}
