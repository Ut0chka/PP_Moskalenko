#include <omp.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>


using namespace std;

int max(int first, int second) {
	return first > second ? first : second;
};

//Func for search max element
void findMax(vector<int> v, int test_num, int th_count)
{
	int max_elem = v[0];
	omp_set_num_threads(th_count);
	chrono::high_resolution_clock::time_point t_start = chrono::high_resolution_clock::now();

	#pragma omp parallel for reduction(max : max_elem)
	for (int i = 0; i < v.size(); i++)
	{
		max_elem = v[i] > max_elem ? v[i] : max_elem;
	}
	chrono::high_resolution_clock::time_point t_end = chrono::high_resolution_clock::now();
	chrono::duration<double, milli> t_diff = t_end - t_start;
	cout << "Test: " << test_num <<  "\nMax elem: " << max_elem;
	cout << "\nTime for " << th_count << " thread(s), mc: " << t_diff.count() << "\n\n";
}


int main()
{
	random_device rd;
	mt19937 gen(rd());
	int min = 0;
	int max = 100000;
	int size = 10000000;
	uniform_int_distribution<> range(min, max);
	vector<int> v(size, 0);
	generate(v.begin(), v.end(), [&gen, &range]() {
		return range(gen);
		});

	findMax(v, 1, 1);
	findMax(v, 2, 2);
	findMax(v, 3, 4);
	findMax(v, 4, 8);

	return 0;
}