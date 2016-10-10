#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <mutex>
#include <thread>
#include <assert.h>

using namespace std;

int parent(int i)
{
	return i / 2;
}

int left(int i)
{
	return i * 2;
}

int right(int i)
{
	return i * 2 + 1;
}

struct Heap {
	int a[100];
	int n;

	int size() {
		return n;
	}

	int &operator[](int i) {
		return a[i];
	}
};

// pre-condition: left(i), right(i) 已经是最大堆了， 而i可能还不是
void MaxHeapify(Heap &A, int i) {
	int l = left(i);
	int r = right(i);
	int largest;
	if (l <= A.size() && A[l] > A[i]) {//如果左节点大于当前结点，则选取左节点
		largest = l;
	}
	else {
		largest = i;
	}

	if (r<=A.size() && A[r]>A[largest])
	{
		largest = r;
	}
	if (largest!=i)
	{
		swap(A[i], A[largest]);
		MaxHeapify(A, largest);
	}
}

void BuildMaxheap(Heap &A) {
	for (int i = A.size()/2; i >=1; i--)
	{
		MaxHeapify(A, i);
	}
}

void HeapSort(Heap &A) {
	BuildMaxheap(A);
	for (int i = A.size(); i >=2; i--)
	{
		swap(A[1], A[i]);
		A.n--;
		MaxHeapify(A, 1);
	}
}

// Priority Queue
struct MaxPrioQueue
{
	Heap A;
	MaxPrioQueue() {
		A.n = 0;
	}

	int Max() {
		return A[1];
	}

	int ExtractMax() {
		if (A.size()<1)
		{
			assert(0);
			return -1;
		}
		int nMax = A[1];	//拿出最大相，把最后一个放到第一箱里面，然后再排序
		A[1] = A[A.size()];
		A.n--;
		MaxHeapify(A, 1);
		return nMax;
	}
	void IncreaseKey(int i, int key) {
		if (key<A[i])
		{
			assert(0);
			return;
		}
		A[i] = key;
		for (; i>1 && A[parent(i)]<A[i]; i=parent(i))
		{
			swap(A[i], A[parent(i)]);
		}
	}
	void InsertKey(int key) {
		//首先加入-无穷大的数字来扩展堆，然后调用IncreaseKey来调整关键字
		A.n++;
		A[A.n] = -10000;
		IncreaseKey(A.n, key);
	}
};

void testHeapsort() {
	int data[] = { 3,1,5,4,9 };
	int n = sizeof(data) / sizeof(data[0]);
	
	Heap A;
	copy(data, data + n, A.a + 1);
	A.n = n;

	HeapSort(A);
}

void testPrioQueue() {
	int data[] = { 3,1,5,4,9 };
	int n = sizeof(data) / sizeof(data[0]);
	
	MaxPrioQueue Q;
	Q.InsertKey(3);
	Q.InsertKey(1);
	Q.InsertKey(5);
	assert(5 == Q.ExtractMax());
	Q.InsertKey(4);
	Q.InsertKey(9);
	assert(9 == Q.ExtractMax());
	assert(4 == Q.ExtractMax());
	assert(3 == Q.ExtractMax());
	assert(1 == Q.ExtractMax());

	assert(Q.A.n == 0);
}
