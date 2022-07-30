#ifndef SORT_H_
# define SORT_H_

# include <vector>
# include <math.h>

# define SIZE_THRES_HOLD_ 16

template <typename P>
void	bubble_sort(P array, size_t start, size_t end) {
	int		end_flag = 1;

	while (end_flag)
	{
		end_flag = 0;
		for (size_t it_first = start, it_second = start + 1; it_first != end; ++it_first, ++it_second) {
			if (array[it_second] < array[it_first]) {
				std::swap(array[it_second], array[it_first]);
				++end_flag;
			}
		}
	}
}

template <typename P>
void	heapify(P array, int size, int i) {
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

template <typename P>
void	heap_sort(P array, int size) {
	for (int i = size / 2 - 1; i >= 0; --i) {
		heapify(array, size, i);
	}
	for (int i = size - 1; i >= 0; --i) {
		std::swap(array[0], array[i]);
		heapify(array, i, 0);
	}
}

template <typename T>
T	find_pivot(const T &first, const T &second, const T &third) {
	if (first >= second && first <= third) {
		return first;
	} else if (second >= first && second <= third) {
		return second;
	} else {
		return third;
	}
}

template <typename P>
size_t	partition(P array, size_t start, size_t end) {
	typename std::remove_pointer<P>::type	pivot = find_pivot(array[start], array[start + ((end - start) >> 1)], array[end]);
	size_t	count = 0;
	for (size_t i = start + 1; i <= end; ++i) {
		if (array[i] <= pivot) {
			++count;
		}
	}
	size_t	pivot_index = start + count;
	std::swap(array[pivot_index], array[start]);
	for (size_t i = start, j = end; i < pivot_index && j > pivot_index; ++i, --j) {
		while (i < pivot_index && array[i] <= pivot) {
			++i;
		}
		while (j > pivot_index && array[j] > pivot) {
			--j;
		}
		if (i < pivot_index && j > pivot_index) {
			std::swap(array[i], array[j]);
		}
	}
	return pivot_index;
}

template <typename P>
void	quick_sort(P array, size_t start, size_t end, size_t recursion_limit) {
	if (--recursion_limit == 0 || start >= end) {
		return ;
	}
	int	p = partition(array, start, end);
	quick_sort(array, start, p, recursion_limit);
	quick_sort(array, p + 1, end, recursion_limit);
}

template <typename T>
void	sort(std::vector<T> &vec) {
	if (vec.size() < SIZE_THRES_HOLD_) {
		bubble_sort(vec.data(), 0, vec.size() - 1);
	} else {
		size_t	recursion_limit	= floor(log(vec.size()) / log(2));
		quick_sort(vec.data(), 0, vec.size() - 1, recursion_limit);
		heap_sort(vec.data(), vec.size());
	}
}

#endif // SORT_H_