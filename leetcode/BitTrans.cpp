#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <mutex>
#include <thread>
#include <assert.h>

using namespace std;
//
//-把一个7进制的数转换成3进制的数，7进制的数用string存储
//能处理大数的转换

class BigInt {
	vector<char> m_n;
	int m_base;
public:
	BigInt() :m_base(0), m_n(0) {
	}

	BigInt(vector<char> const &s, int base) {
		m_base = base;
		m_n = s;
	}

	//return <商，余数>
	std::pair<BigInt, int> div(int b) {
		std::pair<BigInt, int> p;
		p.first.m_base = m_base;
		int t = 0;
		for (auto i : m_n) {
			t += i;
			p.first.m_n.push_back(t / b);
			t = m_base * (t%b);
		}
		
		p.second = t/ m_base;
		return move(p);
	}
	bool isZero() const {
		for (auto i : m_n)
			if (i != 0)return false;
		return true;
	}
	bool operator==(BigInt const &r) const {
		if (m_base != r.m_base)return false;
		if (m_n.size() != r.m_n.size())return false;
		for (size_t i = 0, length=m_n.size(); i < length; i++)
		{
			if (m_n[i] != r.m_n[i])return false;
		}
		return true;
	}
};

BigInt trans(BigInt const &s, int tobase) {
	vector<char> v;
	BigInt s2(s);
	while (!s2.isZero())
	{
		auto t = s2.div(tobase);
		s2 = t.first;
		v.push_back(t.second);
	}

	reverse(v.begin(), v.end());
	BigInt des(move(v), tobase);
	return move(des);
}
void testTrans() {
	{
		BigInt n(vector<char>{4, 8, 7, 7}, 10);
		std::pair<BigInt, int> t = n.div(16);

		auto tHex = trans(n, 16);
		assert(tHex == BigInt(vector<char>{1, 3, 0, 13}, 16));
	}

	{
		BigInt n(vector<char>{1, 3, 0, 13}, 16);
		auto t10 = trans(n, 10);
		assert(t10 == BigInt(vector<char>{4, 8, 7, 7}, 10));
	}

	{
		BigInt n(vector<char>{15,15}, 16);
		auto t10 = trans(n, 10);
		assert(t10 == BigInt(vector<char>{2,5,5}, 10));
	}
}

class Cfg {
public:
	bool load(const char *file) { return true; }
	string get(const char *section, const char *field) {
		if (!strcmp(section, "Calibration") && !strcmp(field, "Resolution")) {
			return "3, 4";
		}
		if (!strcmp(section, "Calibration") && !strcmp(field, "FOV")) {
			return "30, 40";
		}
		return "";
	}

	void set(const char *section, const char *field, const char *value) {
	}
};

string ConvToStr(const char *type, void *p, int n) {
	string des;
	char t[10];
	if (!strcmp(type, "int")) {
		int *pn = (int*)p;
		for (int i = 0; i < n; i++)
		{
			des += itoa(pn[i], t, 10);
			if (i + 1 < n) des += ", ";
		}
	}
	return move(des);
}

vector<string> split(string s) {
	vector<string> des;
	int i = 0, j=0;
	while ((j = s.find(',', i)) != string::npos) {
		des.push_back(s.substr(i, j - i));
		i = j + 1;
	}
	if(i<s.length())
		des.push_back(s.substr(i));
	return move(des);
}

void ConvToData(const char *str, const char *type, void *p, int n) {
	vector<string> vec = split(str);
	for (int i = 0, length=vec.size(); i < length; i++)
	{
		if (!strcmp(type, "int")) {
			int *pn = (int*)p;
			pn[i] = atoi(vec[i].c_str());
		}
	}
}

#define FILE_CFG_START_SECTION(Cfgx, section, isWrite)	\
	Cfg &cfg=Cfgx;			\
	string sec(section);	\
	bool isWr = isWrite;

#define FILE_CFG_FIELD_ITEM_ARRAY(name, type, p, n)		\
	if(isWr) cfg.set(sec.c_str(), name, ConvToStr(type, p, n).c_str());	\
	else ConvToData(cfg.get(sec.c_str(), name).c_str(), type, p, n);

void testCfg() {

	assert((vector<string>{"a", "b"}) == split("a,b"));

	int reso[2] = { 3,4 };
	assert(string("3, 4") == ConvToStr("int", reso, 2));

	Cfg cfgx;
	
	{
		FILE_CFG_START_SECTION(cfgx, "Calibration", true);
		FILE_CFG_FIELD_ITEM_ARRAY("Resolution", "int", reso, 2);
	}

	{
		FILE_CFG_START_SECTION(cfgx, "Calibration", false);
		FILE_CFG_FIELD_ITEM_ARRAY("Resolution", "int", reso, 2);
	}
}