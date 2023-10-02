#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <clocale>

using namespace std;

//Функция поиска максимального элемента
int findMax(int count, vector<int> v)
{
	int max_elem = 0;
	vector<int> thread_max(count);
	vector<thread> threads;
	int step = v.size() / count;
	for (int i = 0; i < count; i++)
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
	cout << "Максимальные для каждого потока: ";
	for (auto& t : thread_max)
	{
		cout << t << " ";
	}
	max_elem = *(max_element(thread_max.begin(), thread_max.end()));
	cout << "\nCамый большой: " << max_elem << endl;
	return max_elem;
}

void test(vector<int>& v, int test_num, int threads_num) {
	cout << "Тест " << test_num << endl;
	chrono::high_resolution_clock::time_point t_start = chrono::high_resolution_clock::now();
	findMax(threads_num, v);
	chrono::high_resolution_clock::time_point t_end = chrono::high_resolution_clock::now();
	chrono::duration<double, milli> t_diff = t_end - t_start;
	std::cout << "Время для " << test_num << "-х потоков, мс: " << t_diff.count() << endl;
}

int main()
{
	setlocale(LC_ALL, "");
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

	test(v, 1, 1);
	test(v, 2, 2);
	test(v, 3, 4);
	test(v, 4, 8);

	return 0;
}