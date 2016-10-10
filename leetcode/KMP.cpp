#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <mutex>
#include <thread>
#include <assert.h>

using namespace std;

struct Pattern {
	char s[100];
	int n;
	int next[100];

	void init(char *_s) {
		n = strlen(_s);
		copy(_s, _s + n, s);

		next[0] = 0;
		for (int i = 1; i < n; i++)
		{
			int j = 0;
			for (; j + i < n && s[i + j] == s[j]; j++);
			next[i] = j;
		}
	}

	int findIn(char *str) {
		// i不停的往前走，
		// j在匹配的时候往前走，不配赔的时候就会跳到next[j]
		for (int i = 0, m=strlen(str), j=0; i < m; )
		{
			if (str[i] == s[j]) {
				i++;
				j++;
				if (j==n)
				{
					return i - n;
				}
			}
			else {
				j = next[j];
			}
		}
		return -1;
	}
};

void testKmp() {
	char *s = "1234124";
	Pattern p;
	p.init(s);
	assert(p.findIn("1234121234124123412") == 6);
}