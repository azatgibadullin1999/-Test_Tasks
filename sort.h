#ifndef SORT_H_
# define SORT_H_

# include <type_traits>
# include <math.h>

namespace ft {

# define SIZE_THRES_HOLD_ 16

template <typename Iter>
void	bubble_sort(Iter first, Iter last)
{
	int		end_flag = 1;

	while (end_flag) {
		end_flag = 0;
		for (Iter itFirst = first, itSecond = ++Iter(first); itFirst != last; ++itFirst, ++itSecond) {
			if (*itSecond < *itFirst) {
				std::swap(*itSecond, *itFirst);
				++end_flag;
			}
		}
	}
}

template <typename Iter>
void	heapify(Iter array, int size, int i) {
	int	largest = i;
	int	l = 2 * i + 1;
	int	r = 2 * i + 2;

	if (l < size && array[l] > array[largest]) {
		largest = l;
	}
	if (r < size && array[r] > array[largest]) {
		largest = r;
	}
	if (largest != i) {
		std::swap(array[i], array[largest]);
		heapify(array, size, largest);
	}

}

template <typename Iter>
void	heap_sort(Iter array, int size) {
	for (int i = size / 2 - 1; i >= 0; --i) {
		heapify(array, size, i);
	}
	for (int i = size - 1; i >= 0; --i) {
		std::swap(array[0], array[i]);
		heapify(array, i, 0);
	}
}

template <typename Iter>
typename std::iterator_traits<Iter>::value_type	find_pivot(Iter start, Iter end) {
	Iter	pivot = start + ((end - start) >> 1);
	std::swap(*pivot, *end);
	return *end;
}

template <typename Iter>
Iter	partition(Iter start, Iter end) {
	auto	pivot = find_pivot(start, end);
	Iter	pivot_it = start;

	for (Iter it = start; it != end; ++it) {
		if (*it <= pivot) {
			std::swap(*it, *pivot_it++);
		}
	}
	std::swap(*pivot_it, *end);
	return pivot_it;
}

template <typename Iter>
void	quick_sort(Iter begin, Iter end, size_t recursion_limit) {
	if (begin >= end) {
		return ;
	} else if (--recursion_limit == 0) {
		heap_sort(begin, std::distance(begin, end + 1));
	} else {
		Iter	pivot_pos = partition(begin, end);
		quick_sort(begin, pivot_pos - 1, recursion_limit);
		quick_sort(pivot_pos + 1, end, recursion_limit);
	}
}

template <typename Iter>
void	sort(Iter begin, Iter end) {
	typename std::iterator_traits<Iter>::difference_type	size = std::distance(begin, end);
	if (size <= 1) {
		return ;
	} else if (size < SIZE_THRES_HOLD_) {
		bubble_sort(begin, end);
	} else {
		size_t	recursion_limit	= floor(log(size) / log(2));
		quick_sort(begin, end - 1, recursion_limit);
	}
}

}

#endif // SORT_H_