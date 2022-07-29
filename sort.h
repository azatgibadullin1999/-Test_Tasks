#ifndef SORT_H_
# define SORT_H_

# include <vector>

template <typename P>
size_t	partition(P array, size_t start, size_t end) {
	typename std::remove_pointer<P>::type	pivot = array[start];
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
void	sort(P array, size_t start, size_t end) {
	if (start >= end)
		return ;
	int	p = partition(array, start, end);
	sort(array, start, p);
	sort(array, p + 1, end);
}

template <typename T>
void	quickSort(std::vector<T> &vec) {
	sort(vec.data(), 0, vec.size() - 1);
}

#endif // SORT_H_