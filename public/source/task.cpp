// this file you need to fill
// этот файл вам нужно заполнить
#include "task.h"
#include <iostream>
#include <future>
#include <mutex>
#include <functional>

#define The_Number_of_Elements_Processed_by_the_Thread 5000

struct iterators_list
{
	std::vector<Triangle>::iterator ordinary;
	std::vector<int>::iterator counter;
};

struct transmitted_iterators 
{
	std::vector<Triangle>::iterator begin;
	std::vector<Triangle>::iterator end;
	std::vector<int>::iterator begin_counter;
	std::vector<int>::iterator end_counter;
};

iterators_list partition(transmitted_iterators iterator, std::function<bool(const Triangle& a)>func)
{
	auto _UFirst = _Get_unwrapped(iterator.begin);
	auto _ULast = _Get_unwrapped(iterator.end);

	auto _UFirst_Counter = _Get_unwrapped(iterator.begin_counter);
	auto _ULast_Counter = _Get_unwrapped(iterator.end_counter);

	for (;;) { // find any out-of-order pair
		for (;;) { // skip in-place elements at beginning
			if (_UFirst == _ULast) {
				_Seek_wrapped(iterator.begin, _UFirst);
				_Seek_wrapped(iterator.begin_counter, _UFirst_Counter);
				iterators_list returned_iterators;
				returned_iterators.ordinary = iterator.begin;
				returned_iterators.counter = iterator.begin_counter;
				return returned_iterators;
			}

			if (!func(*_UFirst)) {
				break;
			}

			++_UFirst;
			++_UFirst_Counter;
		}

		do { // skip in-place elements at end
			--_ULast;
			--_ULast_Counter;
			if (_UFirst == _ULast) {
				_Seek_wrapped(iterator.begin, _UFirst);
				_Seek_wrapped(iterator.begin_counter, _UFirst_Counter);
				iterators_list returned_iterators;
				returned_iterators.ordinary = iterator.begin;
				returned_iterators.counter = iterator.begin_counter;
				return returned_iterators;
			}
		} while (!func(*_ULast));

		std::iter_swap(_UFirst, _ULast); // out of place, swap and loop
		std::iter_swap(_UFirst_Counter, _ULast_Counter); // out of place, swap and loop
		++_UFirst;
		++_UFirst_Counter;
	}
}

int sortVectorInAscendingOrder(transmitted_iterators iterator)
{
	auto const size = iterator.end - iterator.begin;
	if (size <= 1) return 0;

	auto half = iterator.begin + size / 2;
	auto half_counter = iterator.begin_counter + size / 2;

	const Triangle last_in_half = *half;

	std::swap(*half, *(iterator.end - 1));
	std::swap(*half_counter, *(iterator.end_counter - 1));

	auto iterators_of_separated_vectors = partition(iterator, [&](const Triangle& A)->bool 
		{
			auto get_max = [&](const Triangle& A) {
				Point max;

				if (sqrt(A.a.x * A.a.x + A.a.y * A.a.y) - sqrt(A.b.x * A.b.x + A.b.y * A.b.y) > 0) {
					if (sqrt(A.a.x * A.a.x + A.a.y * A.a.y) - sqrt(A.c.x * A.c.x + A.c.y * A.c.y) > 0) {
						max = A.a;
					}
					else max = A.c;
				}
				else {
					if (sqrt(A.b.x * A.b.x + A.b.y * A.b.y) - sqrt(A.c.x * A.c.x + A.c.y * A.c.y) > 0) {
						max = A.b;
					}
					else max = A.c;
				}
				return max;
			};

			Point max;
			Point max_second;
			max = get_max(A);
			max_second = get_max(last_in_half);

			return sqrt(max.x * max.x + max.y * max.y) - sqrt(max_second.x * max_second.x + max_second.y * max_second.y) < 0;
		}
	);

	std::swap(*iterators_of_separated_vectors.ordinary, *(iterator.end - 1));
	std::swap(*iterators_of_separated_vectors.counter, *(iterator.end_counter - 1));
	
	transmitted_iterators first_part_of_the_truncated_vector;
	first_part_of_the_truncated_vector.begin = iterator.begin;
	first_part_of_the_truncated_vector.end = iterators_of_separated_vectors.ordinary;
	first_part_of_the_truncated_vector.begin_counter = iterator.begin_counter;
	first_part_of_the_truncated_vector.end_counter = iterators_of_separated_vectors.counter;

	transmitted_iterators second_part_of_the_truncated_vector;
	second_part_of_the_truncated_vector.begin = iterators_of_separated_vectors.ordinary + 1;
	second_part_of_the_truncated_vector.end = iterator.end;
	second_part_of_the_truncated_vector.begin_counter = iterators_of_separated_vectors.counter + 1;
	second_part_of_the_truncated_vector.end_counter = iterator.end_counter;


	if (size > The_Number_of_Elements_Processed_by_the_Thread) {
		auto left = std::async(std::launch::async, [&]() {
			return sortVectorInAscendingOrder(first_part_of_the_truncated_vector);
			});
		sortVectorInAscendingOrder(second_part_of_the_truncated_vector);
	}
	else {
		sortVectorInAscendingOrder(first_part_of_the_truncated_vector);
		sortVectorInAscendingOrder(second_part_of_the_truncated_vector);
	}
	return 0;
}



bool finding_and_recording_triangle_matches(transmitted_iterators match_search_iterators, 
	std::vector<int>::iterator out_count_begin, std::vector<int>::iterator out_count_end)
{
	auto get_max = [&](const Triangle& A) {
		Point max;

		if (sqrt(A.a.x * A.a.x + A.a.y * A.a.y) - sqrt(A.b.x * A.b.x + A.b.y * A.b.y) > 0) {
			if (sqrt(A.a.x * A.a.x + A.a.y * A.a.y) - sqrt(A.c.x * A.c.x + A.c.y * A.c.y) > 0) {
				max = A.a;
			}
			else max = A.c;
		}
		else {
			if (sqrt(A.b.x * A.b.x + A.b.y * A.b.y) - sqrt(A.c.x * A.c.x + A.c.y * A.c.y) > 0) {
				max = A.b;
			}
			else max = A.c;
		}
		return max;
	};
	auto comparison_of_triangles = [&](const Triangle& A, const Triangle& B)->int {
		Point _A = get_max(A);
		Point _B = get_max(B);
		if (sqrt(pow(_A.x - _B.x, 2) + pow(_A.y - _B.y, 2)) <= 0.1)
		{
			return 1; //The points have absolute equality.
		}
		else if (sqrt(pow(_A.x - _B.x, 2) + pow(_A.y - _B.y, 2)) <= 1.5)
		{
			if ((int)A.a.x == (int)B.a.x)
			{
				if ((int)A.a.y == (int)B.a.y)
					return 0; //The points have relative equality.
			}
			else if ((int)A.a.x == (int)B.b.x) {
				if ((int)A.a.y == (int)B.b.y)
					return 0; //The points have relative equality.
			}
			else if ((int)A.a.x == (int)B.c.x) {
				if ((int)A.a.y == (int)B.c.y)
					return 0; //The points have relative equality.
			}

			else if ((int)A.b.x == (int)B.a.x)
			{
				if ((int)A.b.y == (int)B.a.y)
					return 0; //The points have relative equality.
			}
			else if ((int)A.b.x == (int)B.b.x) {
				if ((int)A.b.y == (int)B.b.y)
					return 0; //The points have relative equality.
			}
			else if ((int)A.b.x == (int)B.c.x) {
				if ((int)A.b.y == (int)B.c.y)
					return 0; //The points have relative equality.
			}

			else if ((int)A.c.x == (int)B.a.x)
			{
				if ((int)A.c.y == (int)B.a.y)
					return 0; //The points have relative equality.
			}
			else if ((int)A.c.x == (int)B.b.x) {
				if ((int)A.c.y == (int)B.b.y)
					return 0; //The points have relative equality.
			}
			else if ((int)A.c.x == (int)B.c.x) {
				if ((int)A.c.y == (int)B.c.y)
					return 0; //The points have relative equality.
			}
			else {
				return -1;
			}
		}
		else return -1;
	};


	/*
	if (size > The_Number_of_Elements_Processed_by_the_Thread) {
		auto left = std::async(std::launch::async, [&]() {
			return finding_and_recording_triangle_matches();
			});
		finding_and_recording_triangle_matches();
	}
	else {
		finding_and_recording_triangle_matches();
		finding_and_recording_triangle_matches();
	}*/
	return 0;
}

void Task::checkIntersections(const std::vector<Triangle> &in_triangles, std::vector<int> &out_count)
{
	std::vector<Triangle> sorted_vector;
	sorted_vector = in_triangles; //Copying a vector for further sorting.
	
	std::vector<int> IDS; //A vector storing the numbers of triangles.
	IDS.reserve(in_triangles.size());
	IDS.resize(in_triangles.size());
	std::cout << "Time start" << std::endl;
	auto time_start = std::chrono::steady_clock::now();

	for (int i = 0; i < in_triangles.size(); ++i){
		IDS.at(i) = i;
	}

	auto time_end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start);
	std::cout << "The time: " << elapsed_ms.count() << " ns\n";
	for (int i = 0; i < IDS.size(); ++i) {
		std::cout << "-";
	}
	transmitted_iterators iterator_trnsmt;
	iterator_trnsmt.begin = sorted_vector.begin();
	iterator_trnsmt.end = sorted_vector.end();
	iterator_trnsmt.begin_counter = IDS.begin();
	iterator_trnsmt.end_counter = IDS.end();
	
	sortVectorInAscendingOrder(iterator_trnsmt);
	//Reserving memory in the heap. To avoid repeated calls and requests to get memory from the heap.
	out_count.reserve(in_triangles.size());
	out_count.resize(in_triangles.size());
	
	out_count = IDS;
	

}