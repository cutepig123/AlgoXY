
#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <mutex>
#include <thread>
#include <complex>
#include <iterator>
#include <assert.h>

using namespace std;

// DFT: y(k) = sum (j=0 to n-1) (a(j)*w(n)^kj)
// where w(n) = exp^(2*pi*i/n)
// 推倒参考算法导论30章

#define PI	3.14159



std::complex<double>	w(int n) {
	return exp(std::complex<double>(0, 2 * PI / n));
}

std::complex<double>	w(int n, int k) {
	return pow(w(n), k);
}

std::complex<double>	dft(std::vector<std::complex<double>> const &a, int k) {
	std::complex<double> r;
	int n = a.size();
	for (int j = 0; j < n; j++)
	{
		r += a[j] * std::pow(w(n), k*j);
	}
	return move(r);
}

std::vector<std::complex<double>> dft(std::vector<std::complex<double>> const &a) {
	int n = a.size();
	std::vector<std::complex<double>> Y(n);
	for (size_t i = 0; i < n; i++)
	{
		Y[i] = dft(a, i);
	}
	return move(Y);
}

std::vector<std::complex<double>> fft_ex(std::vector<std::complex<double>> const &a, double sign) {
	int n = a.size();
	if (n == 1)return a;
	std::complex<double> wn = w(n,sign);
	std::complex<double> w (1);
	std::vector<std::complex<double>> a0, a1;
	copy_if(a.begin(), a.end(), std::back_inserter(a0), [&a](auto &i) {return (&i - &a[0]) % 2 == 0; });
	copy_if(a.begin(), a.end(), std::back_inserter(a1), [&a](auto &i) {return (&i - &a[0]) % 2 == 1; });
	auto Y0 = fft_ex(a0, sign);
	auto Y1 = fft_ex(a1, sign);
	std::vector<std::complex<double>> Y(n);
	for (int k = 0; k <= n/2-1; k++)
	{
		Y[k] = Y0[k] + w*Y1[k];
		Y[k+n/2] = Y0[k] - w*Y1[k];
		w = w*wn;
	}
	return move(Y);
}

std::vector<std::complex<double>> fft(std::vector<std::complex<double>> const &a) {
	return fft_ex(a, 1);
}

std::vector<std::complex<double>> ifft(std::vector<std::complex<double>> const &a) {
	auto y= fft_ex(a, -1);
	double n = a.size();
	for (auto &i:y)
	{
		i /= n;
	}
	return move(y);
}

bool operator==(std::vector<std::complex<double>> const &a, std::vector<std::complex<double>> const &b) {
	if (a.size() != b.size())return false;
	for (size_t i = 0; i < a.size(); i++)
	{
		if (abs(a[i] - b[i]) > 1e-3)return false;
	}
	return true;
}

void testFft() {
	std::vector<std::complex<double>> a = { 1,2,3,4,5,6,7,8 };
	auto y1 = dft(a);
	auto y2 = fft(a);

	assert(y1 == y2);

	auto a2 = ifft(y2);
	assert(a == a2);
}