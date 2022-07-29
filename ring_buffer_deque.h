#ifndef RING_BUFFER_DEQUE_H_
# define RING_BUFFER_DEQUE_H_

# include <memory>

template <typename T>
struct DequeNode {
	T		*value;
	DequeNode	*prev;
	DequeNode	*next;
};

template <typename T, typename Alloc = std::allocator<T> >
class RingBufferQueue {
	private:
		typedef DequeNode<T>					node;
		typedef typename Alloc::template rebind<node>::other	node_allocator_type;
		typedef typename node_allocator_type::pointer		node_pointer;
		typedef typename node_allocator_type::reference		node_reference;
	public:
		typedef Alloc						allocator_type;
		typedef typename allocator_type::value_type		value_type;
		typedef typename allocator_type::reference		reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef typename allocator_type::pointer		pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
	private:
		node_allocator_type	node_alloc_;
		allocator_type		alloc_;
		node_pointer		node_;
		size_t			size_;
	public:
		RingBufferQueue() = delete;
		RingBufferQueue(size_t size, const allocator_type &alloc = allocator_type()) :
			node_alloc_(),
			alloc_(alloc),
			node_(nullptr),
			size_(size) {
			if (size < 1) {
				throw std::exception();
			}
			node_pointer begin = __node_init(value_type());
			node_ = begin;
			for (size_t i = 1; i < size_; ++i) {
				__node_init(value_type(), node_);
				node_ = node_->next;
			}
			begin->prev = node_;
			node_->next = begin;
		}

		RingBufferQueue(const RingBufferQueue &other) :
			node_alloc_(other.node_alloc_),
			alloc_(other.alloc_),
			size_(other.size_) {
			node_pointer tmp_other = other.node_;
			node_pointer begin = __node_init(*tmp_other->value);
			node_ = begin;
			for (size_t i = 1; i < size_; ++i) {
				__node_init(*tmp_other->value, node_);
				tmp_other = tmp_other->next;
				node_ = node_->next;
			}
			begin->prev = node_;
			node_->next = begin;
		}

		RingBufferQueue(RingBufferQueue &&other) = default;

		RingBufferQueue	&operator=(RingBufferQueue &&other) = default;

		~RingBufferQueue() {
			for (node_pointer tmp = node_->next; tmp != node_; tmp = tmp->next) {
				alloc_.destroy(tmp->value);
				alloc_.deallocate(tmp->value, 1);
				node_alloc_.destroy(tmp);
				node_alloc_.deallocate(tmp, 1);
			}
			alloc_.destroy(node_->value);
			alloc_.deallocate(node_->value, 1);
			node_alloc_.destroy(node_);
			node_alloc_.deallocate(node_, 1);
		}

		void	push(const_reference val) {
			alloc_.destroy(node_->value);
			alloc_.construct(node_->value, val);
		}

		void	push(value_type &&val) {
			alloc_.destroy(node_->value);
			alloc_.construct(node_->value, std::move(val));
		}

		void	pop() {
			alloc_.destroy(node_->value);
		}

		reference	get_value() noexcept { return *node_->value; }

		const_reference	get_value() const noexcept { return *node_->value; }

		size_t		get_size() const noexcept { return size_; }

		void		clear() {
			for (node_pointer tmp = node_->next; tmp != node_; tmp = tmp->next) {
				alloc_.destroy(tmp->value);
				alloc_.deallocate(tmp->value, 1);
			}
			alloc_.destroy(node_->value);
			alloc_.deallocate(node_->value, 1);
		}

		RingBufferQueue	&increment(size_t num) noexcept {
			for (; num > 0; --num) {
				node_ = node_->next;
			}
			return *this;
		}

		RingBufferQueue	&decriment(size_t num) noexcept {
			for (; num > 0; --num) {
				node_ = node_->prev;
			}
			return *this;
		}

	private:
		node_pointer	__node_init(const_reference val, node_pointer prev) {
			node_pointer	new_node = node_alloc_.allocate(1);
			node_alloc_.construct(new_node, node());
			new_node->value = alloc_.allocate(1);
			alloc_.construct(new_node->value, val);
			new_node->prev = prev;
			prev->next = new_node;
			return new_node;
		}

		node_pointer	__node_init(const_reference val) {
			node_pointer	new_node = node_alloc_.allocate(1);
			node_alloc_.construct(new_node, node());
			new_node->value = alloc_.allocate(1);
			alloc_.construct(new_node->value, val);
			return new_node;
		}
};

#endif // RING_BUFFER_DEQUE_H_