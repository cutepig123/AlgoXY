#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <assert.h>

using namespace std;

struct MyList
{
	int v;
	MyList *next;
};

void ListInit(MyList *p)
{
	p->next = 0;
	p->v = 0;
}

MyList *ListCreate(vector<int> const &vec) {
	MyList *h = 0, *t=0;
	for (auto i:vec)
	{
		MyList *p = new MyList;
		p->v = i;
		p->next = 0;

		if (!h)h = t= p;
		else
		{
			t->next = p;
			t = p;
		}
	}
	return h;
}

string ListPrint(MyList *p) {
	string d;
	while (p)
	{
		char s[10];
		sprintf(s, "%d ", p->v);
		d += s;
		p = p->next;
	}
	return move(d);
}

//Order all odd items + all even items
MyList* ListReOrder(MyList *h) {
	if (!h || !h->next)return h;

	MyList *p0 = 0, *t0 = 0;
	MyList *p1 = 0, *t1 = 0;
	bool is0 = true;
	for (MyList *p=h; p; p = p->next)
	{
		if (is0)
		{
			if (!p0)
				p0 = t0 = p;
			else
			{
				t0->next = p;
				t0 = p;
			}
		}
		else
		{
			if (!p1)
				p1 = t1 = p;
			else
			{
				t1->next = p;
				t1 = p;
			}
		}
		is0 = !is0;
	}
	if (p0)
	{
		t0->next = p1;
		return p0;
	}
	else
	{
		t1->next = 0;
		return p1;
	}
}
void testList() {
	MyList * p = ListCreate(vector<int>{1, 2, 3, 4, 5});
	assert(ListPrint(p)=="1 2 3 4 5 ");

	MyList* pOrder = ListReOrder(p);
	assert(ListPrint(pOrder) == "1 3 5 2 4 ");
}
