#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;

//Func for search max element
int findMax(vector<int> v, int test_num, int th_count)
{
	int max_elem = 0;
	vector<int> thread_max(th_count);
	vector<thread> threads;
	int step = v.size() / th_count;
	chrono::high_resolution_clock::time_point t_start = chrono::high_resolution_clock::now();
	for (int i = 0; i < th_count; i++)
	{
		thread th([i, step, &v, &thread_max]
		{
				thread_max[i] = *max_element(v.begin() + i * step, v.begin() + (i + 1) * step);
		});

		threads.push_back(move(th));
	}
	for (auto& tt : threads)
	{
		if (tt.joinable())
			tt.join();
	}
	max_elem = *(max_element(thread_max.begin(), thread_max.end()));
	chrono::high_resolution_clock::time_point t_end = chrono::high_resolution_clock::now();
	chrono::duration<double, milli> t_diff = t_end - t_start;
	cout << "Test " << test_num << endl;
	cout << "Max elems in each threads: ";
	for (auto& t : thread_max)
	{
		cout << t << " ";
	}
	cout << "\nMax Maximus Maximilian: " << max_elem << endl;
	cout << "Time for " << th_count << " thread(s), mc: " << t_diff.count() << "\n\n";
	return max_elem;
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