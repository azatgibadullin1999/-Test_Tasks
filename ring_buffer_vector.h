#ifndef RING_BUFFER_VECTOR_H_
# define RING_BUFFER_VECTOR_H_

# include <memory> 

template <typename T, typename Alloc = std::allocator<T> >
class RingBufferVector {
	public:
		typedef Alloc							allocator_type;
		typedef typename allocator_type::value_type			value_type;
		typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference		const_reference;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer			const_pointer;
	private:
		allocator_type	alloc_;
		pointer		array_;
		size_t		pos_;
		size_t		size_;
	public:
		RingBufferVector() = delete;
		RingBufferVector(size_t size, const allocator_type &alloc = allocator_type()) :
			alloc_(alloc),
			array_(nullptr),
			pos_(0),
			size_(size) {
			if (size < 1) {
				throw std::exception();
			}
			array_ = alloc_.allocate(size_);
			for (size_t i = 0; i < size_; ++i) {
				alloc_.construct(&array_[i], value_type());
			}
		}

		RingBufferVector(const RingBufferVector &other) :
			alloc_(other.alloc_),
			pos_(0),
			size_(other.size_) {
			array_ = alloc_.allocate(size_);
			for (size_t i = 0; i < size_; ++i) {
				alloc_.construct(&array_[i], other.array_[i]);
			}
		}

		RingBufferVector(RingBufferVector &&other) = default;

		RingBufferVector	&operator=(RingBufferVector &&other) = default;

		~RingBufferVector() {
			for (size_t i = 0; i < size_; ++i) {
				alloc_.destroy(&array_[i]);	
			}
			alloc_.deallocate(array_, size_);
		}

		void	push(const_reference val) {
			alloc_.destroy(&array_[pos_]);
			alloc_.construct(&array_[pos_], val);
		}

		void	push(value_type &&val) {
			alloc_.destroy(&array_[pos_]);
			alloc_.construct(&array_[pos_], std::move(val));
		}

		void	pop() {
			alloc_.destroy(&array_[pos_]);
			alloc_.construct(&array_[pos_], value_type());
		}

		reference	get_value() noexcept { return array_[pos_]; }

		const_reference	get_value() const noexcept { return array_[pos_]; }

		size_t		get_size() const noexcept { return size_; }

		void		clear() {
			for (size_t i = 0; i < size_; ++i) {
				alloc_.destroy(&array_[i]);	
			}
		}

		RingBufferVector	&increment(size_t num) noexcept {
			pos_ += num;
			if (pos_ >= size_) {
				pos_ %= size_;
			}
			return *this;
		}

		RingBufferVector	&decriment(size_t num) noexcept {
			if (pos_ < num) {
				pos_ += size_ - num;
			} else {
				pos_ -= num;
			}
			return *this;
		}
};

#endif // RING_BUFFER_VECTOR_H_