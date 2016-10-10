#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <assert.h>

using namespace std;

union U
{
	char s[3];
	unsigned long l;
};

string base64encode3(const char *s) {
	static char *mapping = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	U u = { 0 };
	u.s[0] = s[2];
	u.s[1] = s[1];
	u.s[2] = s[0];
	
	string d;
	for (int i = 0; i < 4; i++)
	{
		int t1 = (u.l >> ((3-i)*6)) & ((1 << 6) - 1);
		d += mapping[t1];
	}
	return move(d);
}

string base64encode(const char *s, size_t n) {
	string d;
	size_t i = 0;
	for (; i +3 <= n; i+=3)
	{
		d += base64encode3(s + i);
	}
	
	if(n%3!=0)
	{
		char c[3] = { 0 };
		int n1 = n - i;
		for(int j=i; j<n; j++)
		{
			c[j - i] = s[j];
		}
		string dsub= base64encode3(s + i);

		if (n1 == 1)
			dsub[2] = dsub[3] = '=';
		else
			dsub[3] = '=';

		d += dsub;
	}
	return move(d);
}

string base64encode(string s) {
	return base64encode(s.c_str(), s.length());
}

void testBase64() {
	assert(base64encode("Mann") == "TWFubg==");
	assert(base64encode("Mannn") == "TWFubm4=");
	assert(base64encode("Mannnn") == "TWFubm5u");
	assert(base64encode("Encode to Base64 format") == "RW5jb2RlIHRvIEJhc2U2NCBmb3JtYXQ=");
}
