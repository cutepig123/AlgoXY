#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <mutex>
#include <thread>
#include <assert.h>

using namespace std;

//最长递增子序列
//不需要是连续字符串

//最长公共子序列
/*
Am and Bn LCS
LCS(i,j)	=	0					if i=0 or j=0
			=	LCS(i-1, j-1) + 1	if i, j>0 and xi=yj
			=	max( LCS(i-1, j), LCS(i, j-1) )	if i, j>0 and xi!=yj
*/
int LCS(string const &a, string const &b,int i, int j, vector<vector<int>> &cache) {
	if (cache.empty()) {
		cache.resize(a.size()+1);
		for (auto& t:cache)
		{
			t.resize(b.size()+1, -1);
		}
	}
	if (cache[i][j] >= 0)return cache[i][j];

	int res = 0;
	if (i == 0 || j == 0)res= 0;
	else if (a[i] == b[j]) res = LCS(a, b, i - 1, j - 1, cache) + 1;
	else res = max(LCS(a, b, i , j - 1, cache), LCS(a, b, i - 1, j, cache));

	cache[i][j] = res;
	return res;
}

struct LCSImpl {
	vector<vector<int>> cache;
	string const &ma;
	string const &mb;
	LCSImpl(string const &a, string const &b):ma(a), mb(b) {

	}

	int operator()() {
		return LCS(ma, mb, ma.length(), mb.length(), cache);
	}
};
void testLCS() {
	
	assert(4 == LCSImpl("BDCABA","ABCBDAB")());
}