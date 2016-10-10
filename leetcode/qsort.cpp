#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <mutex>
#include <thread>
#include <assert.h>

using namespace std;

int(*g_partition1)(int *data, int left, int right) = 0;

void qsort(int *data, int left, int right) {
	if (left<right)
	{
		int mid = g_partition1(data, left, right);
		qsort(data, left, mid - 1);
		qsort(data, mid + 1, right);
	}
}

int partition1(int *data, int left, int right) {
	if (left < right) {
		int pivot = data[right];
		int i = left;
		int j = right - 1;

		while (i<j)
		{
			// �������ҵ�һ������prvot��
			for (; i<right && data[i] <= pivot; i++)
			{
			}
			// ��you��zuo�ҵ�һ��xiao��prvot��
			for (; j>=i && data[j] >= pivot; j--)
			{
			}

			if (i<j)
			{
				swap(data[i], data[j]);
			}
		}
		swap(data[i], data[right]);
		return i;
	}
	assert(0);
	return -1;
}

//�㷨��������ķ�����һ��ѭ���㶨
int partition2(int *data, int left, int right) {
	if (left < right) {
		int pivot = data[right];
		int i = left;
		for (int j = i; j < right; j++)
		{
			if (data[j]<pivot)
			{
				swap(data[i], data[j]);
				i++;
			}
		}
		swap(data[i], data[right]);
		return i;
	}
	assert(0);
	return -1;
}

void testQsort() {
	int data[] = { 3,1,5,4,9 };
	int n = sizeof(data) / sizeof(data[0]);

	g_partition1 = partition2;
	qsort(data, 0, n - 1);
}
