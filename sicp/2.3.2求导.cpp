// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "string"
#include "iostream"
#include "sstream"
#include "assert.h"
#include "vector"
#include "map"
#include "deque"
#include "algorithm"
#include "memory"

using namespace std;

bool isVariable(const char *e)
{
	return e[0] >= 'a' && e[0] <= 'z';
}

bool isSameVariable(const char *e1, const char *e2)
{
	return strcmp(e1, e2) == 0;
}

class ExpressionVar;

class ExpressionBase
{
public:
	virtual ~ExpressionBase() = 0{};
	virtual std::shared_ptr<ExpressionBase> deriv(ExpressionVar const &x) = 0;
	virtual std::string print() = 0;
};

class ExpressionConst :public ExpressionBase
{
	double m_e1;

public:
	explicit ExpressionConst(std::string e2)
	{
		m_e1 = atof(e2.c_str());
	}

	virtual std::shared_ptr<ExpressionBase> deriv(ExpressionVar const &x)
	{
		return std::shared_ptr<ExpressionBase>(new ExpressionConst("0"));
	}

	virtual std::string print()
	{
		std::stringstream s;
		s << m_e1;
		return s.str();
	}
};

class ExpressionVar :public ExpressionBase
{
	std::string m_e1;

public:
	explicit ExpressionVar(std::string e2)
		:m_e1(e2)
	{}

	virtual std::shared_ptr<ExpressionBase> deriv(ExpressionVar const &x)
	{
		if (isSameVariable(m_e1.c_str(), x.m_e1.c_str()))
			return  std::shared_ptr<ExpressionBase>(new ExpressionConst("1"));

		return std::shared_ptr<ExpressionBase>(new ExpressionConst("0"));
	}

	virtual std::string print()
	{
		std::stringstream s;
		s << m_e1;
		return s.str();
	}
};

class ExpressionSum:public ExpressionBase
{
	std::vector<std::shared_ptr<ExpressionBase> > m_ve;
	
	void simplify()
	{
		for (auto it = m_ve.begin(); it != m_ve.end();)
		{
			if ((*it)->print() == "0")
				it = m_ve.erase(it);
			else
				++it;
		}
	}
public:
	ExpressionSum(std::shared_ptr<ExpressionBase> e1, std::shared_ptr<ExpressionBase> e2)
	{
		m_ve.push_back(e1);
		m_ve.push_back(e2);
		simplify();
	}

	ExpressionSum(std::vector<std::shared_ptr<ExpressionBase> > ve)
	{
		m_ve = ve;
		simplify();
	}

	virtual std::shared_ptr<ExpressionBase> deriv(ExpressionVar const &x)
	{
		std::vector<std::shared_ptr<ExpressionBase> > veder;
		std::for_each(m_ve.begin(), m_ve.end(), [&](std::shared_ptr<ExpressionBase> e){veder.push_back(e->deriv(x); });
		return std::shared_ptr<ExpressionBase>(new ExpressionSum(veder));
	}

	virtual std::string print()
	{
		std::stringstream s;
		std::for_each(m_ve.begin(), m_ve.end(), [&s](std::shared_ptr<ExpressionBase> e){ s << e->print() << "+"; });
		
		std::string r = s.str();
		if (r.empty())
		{
			r = "0";
		}
		else
			r = r.substr(0, r.length() - 1);

		return r;
	}
};

class ExpressionMul :public ExpressionBase
{
	std::shared_ptr<ExpressionBase> m_e1, m_e2;

public:
	ExpressionMul(std::shared_ptr<ExpressionBase> e1, std::shared_ptr<ExpressionBase> e2)
		:m_e1(e1), m_e2(e2)
	{}

	virtual std::shared_ptr<ExpressionBase> deriv(ExpressionVar const &x)
	{
		std::shared_ptr<ExpressionBase> p1(new ExpressionMul(m_e1, m_e2->deriv(x)));
		std::shared_ptr<ExpressionBase> p2(new ExpressionMul(m_e2, m_e1->deriv(x)));

		return std::shared_ptr<ExpressionBase>(new ExpressionSum(p1, p2));
	}

	virtual std::string print()
	{
		std::stringstream s;
		std::string o1, o2;
		o1 = m_e1->print();
		o2 = m_e2->print();
		s << o1 << '*' << o2;
		return s.str();
	}
};

int main(int argc, char* argv[])
{
	std::shared_ptr<ExpressionVar> x(new ExpressionVar("x"));
	std::shared_ptr<ExpressionBase> x2(new ExpressionMul(x,x));
	std::shared_ptr<ExpressionBase> res = x2->deriv(*x);

	std::cout << res->print() << "\n";
	return 0;
}

