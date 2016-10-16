//TODO: impl weak_ptr

#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <mutex>
#include <thread>
#include <assert.h>

using namespace std;

template <class T>
class MySharedPtr
{
public:
	MySharedPtr() :m_p(0), m_n(0) {

	}
	~MySharedPtr() {
		if (m_n) {
			--(*m_n);
			if (0 == *m_n) {
				delete m_p;
				m_p = 0;
			}
		}

		if (m_nWeackCnt) {
			delete m_nWeackCnt;
			m_nWeackCnt = 0;
		}
	}

	explicit MySharedPtr(T *p) {
		m_p = p;
		if (m_p) {
			m_n = new int(1);
			m_nWeackCnt = new int(0);
		}
		else {
			m_n = 0;
			m_nWeackCnt = 0;
		}
	}

	MySharedPtr(MySharedPtr const &rhs) {
		m_n = rhs.m_n;
		m_p = rhs.m_p;
		m_nWeackCnt = rhs.m_nWeackCnt;

		if (m_n) {
			++(*m_n);
		}
	}

	MySharedPtr& operator=(MySharedPtr rhs) {
		if (this != &rhs) {
			rhs.swap(*this);
		}
		return *this;
	}

	void swap(MySharedPtr &rhs) {
		std::swap(m_p, rhs.m_p);
		std::swap(m_n, rhs.m_n);
		std::swap(m_nWeackCnt, rhs.m_nWeackCnt);
	}

	operator bool() const {
		return m_p!=0;
	}

	T* get() {
		return m_p;
	}

	T& operator*() const {
		return *m_p;
	}
	bool unique() const {
		return !m_n || 1 == *m_n;
	}

	int weak_cnt() const {
		return m_nWeackCnt ? *m_nWeackCnt : 0;
	}
private:
	T *m_p;
	int *m_n;
	int *m_nWeackCnt;

	friend class MyWeakPtr;
};

//template <class T>
//class MyWeakPtr
//{
//public:
//	MyWeakPtr() {
//	}
//	~MyWeakPtr() {
//	}
//
//private:
//	T *m_p;
//	int *m_n;
//};

void testSharedPtr() {
	MySharedPtr<int> p(new int(1));

	p = p;

	assert(*p == 1);
	assert(p.unique());

	{
		MySharedPtr<int> p2(p);

		p = p2;
		assert(*p2 == 1);
		assert(!p2.unique());
		assert(!p.unique());
		assert(p.get()== p2.get());
	}

	assert(*p == 1);
	assert(p.unique());
}
