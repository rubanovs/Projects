#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>

template <typename T, typename Allocator = std::allocator<T>>
class List {
 private:
  class Node {
   public:
    T value;
    Node* next = nullptr;
    Node* prev = nullptr;

    Node() {}

    Node(const T& cur_val, Node* cur_next, Node* cur_prev)
        : value(cur_val), next(cur_next), prev(cur_prev) {}

    Node(Node* cur_next, Node* cur_prev) : next(cur_next), prev(cur_prev) {}

    Node(T&& cur_val, Node* cur_next, Node* cur_prev)
        : value(std::move(cur_val)), next(cur_next), prev(cur_prev) {}

    Node(const Node* another_node) {
      value = another_node->value;
      next = another_node->next;
      prev = another_node->prev;
    }

    Node(Node&& another_node)
        : value(std::move(another_node->value)),
          next(std::move(another_node->next)),
          prev(std::move(another_node->prev)) {
      another_node->next = nullptr;
      another_node->prev = nullptr;
    }

    bool operator==(const Node* equal) const {
      bool loc = true;
      if (value == equal->value && next == equal->next && prev == equal->prev) {
        return loc;
      }
      return !loc;
    }

    bool operator!=(const Node* equal) const { return *this == equal; }
  };

 public:
  using allocator_type = Allocator;
  using value_type = typename allocator_type::value_type;
  using alloc_traits = std::allocator_traits<Allocator>;
  using node_alloc = typename alloc_traits::template rebind_alloc<Node>;
  using node_alloc_traits = typename alloc_traits::template rebind_traits<Node>;

 private:
  Node* ptr_on_head_ = nullptr;
  size_t size_of_list_ = 0;
  node_alloc common_allocator_;

  void destroy_list() {
    if (size_of_list_ != 0) {
      Node* deleted_node = ptr_on_head_->next;
      Node* temp_node = deleted_node->next;
      while (true) {
        node_alloc_traits::destroy(common_allocator_, deleted_node);
        node_alloc_traits::deallocate(common_allocator_, deleted_node, 1);
        if (temp_node != ptr_on_head_) {
          deleted_node = temp_node;
          temp_node = deleted_node->next;
        } else {
          break;
        }
      }
    }
    node_alloc_traits::deallocate(common_allocator_, ptr_on_head_, 1);
  }

 public:
  List() : size_of_list_(0) {}

  List(size_t count, const T& value, const Allocator& alloc = Allocator()) {
    common_allocator_ = alloc;
    Node* ptr = node_alloc_traits::allocate(common_allocator_, 1);
    ptr_on_head_ = ptr;
    Node* next_ptr = ptr_on_head_;
    Node* prev_ptr = ptr_on_head_;
    for (size_t i = 1; i <= count; ++i) {
      Node* allocated_memory =
          node_alloc_traits::allocate(common_allocator_, 1);
      try {
        node_alloc_traits::construct(common_allocator_, allocated_memory, value,
                                     next_ptr, prev_ptr);
        prev_ptr->next = allocated_memory;
        next_ptr->prev = allocated_memory;
        prev_ptr = allocated_memory;
      } catch (...) {
        node_alloc_traits::deallocate(common_allocator_, allocated_memory, 1);
        this->~List();
        throw;
      }
    }
    size_of_list_ = count;
  }

  explicit List(size_t count, const Allocator& alloc = Allocator()) {
    common_allocator_ = alloc;
    Node* ptr = node_alloc_traits::allocate(common_allocator_, 1);
    ptr_on_head_ = ptr;
    Node* next_ptr = ptr_on_head_;
    Node* prev_ptr = ptr_on_head_;
    for (size_t i = 1; i <= count; ++i) {
      Node* allocated_memory =
          node_alloc_traits::allocate(common_allocator_, 1);
      try {
        node_alloc_traits::construct(common_allocator_, allocated_memory,
                                     next_ptr, prev_ptr);
        prev_ptr->next = allocated_memory;
        next_ptr->prev = allocated_memory;
        prev_ptr = allocated_memory;
      } catch (...) {
        node_alloc_traits::deallocate(common_allocator_, allocated_memory, 1);
        this->~List();
        throw;
      }
    }
    size_of_list_ = count;
  }
  List(std::initializer_list<T> init, const Allocator& alloc = Allocator()) {
    size_of_list_ = init.size();
    common_allocator_ = alloc;
    Node* ptr = node_alloc_traits::allocate(common_allocator_, 1);
    ptr_on_head_ = ptr;
    Node* next_ptr = ptr_on_head_;
    Node* prev_ptr = ptr_on_head_;
    for (const auto& iter : init) {
      Node* allocated_memory =
          node_alloc_traits::allocate(common_allocator_, 1);
      try {
        node_alloc_traits::construct(common_allocator_, allocated_memory, iter,
                                     next_ptr, prev_ptr);
        prev_ptr->next = allocated_memory;
        next_ptr->prev = allocated_memory;
        prev_ptr = allocated_memory;
      } catch (...) {
        node_alloc_traits::deallocate(common_allocator_, allocated_memory, 1);
        this->~List();
        throw;
      }
    }
  }

  List(const List& another_list) {
    size_of_list_ = another_list.size_of_list_;
    common_allocator_ =
        node_alloc_traits::select_on_container_copy_construction(
            another_list.common_allocator_);
    ptr_on_head_ = node_alloc_traits::allocate(common_allocator_, 1);
    ptr_on_head_->prev = ptr_on_head_->next = ptr_on_head_;
    Node* next_ptr = ptr_on_head_;
    Node* prev_ptr = ptr_on_head_;
    Node* temp_ptr = another_list.ptr_on_head_->next;
    for (size_t i = 1; i <= another_list.size_of_list_; ++i) {
      Node* allocated_memory =
          node_alloc_traits::allocate(common_allocator_, 1);
      try {
        node_alloc_traits::construct(common_allocator_, allocated_memory,
                                     temp_ptr->value, next_ptr, prev_ptr);
        prev_ptr->next = allocated_memory;
        next_ptr->prev = allocated_memory;
        prev_ptr = allocated_memory;
        temp_ptr = temp_ptr->next;
      } catch (...) {
        node_alloc_traits::deallocate(common_allocator_, allocated_memory, 1);
        this->~List();
        throw;
      }
    }
  }
  void swap(const List<T, Allocator>& another_list) {
    auto copy = another_list;
    std::swap(this->ptr_on_head_, copy.ptr_on_head_);
    std::swap(this->size_of_list_, copy.size_of_list_);
    if (node_alloc_traits::propagate_on_container_copy_assignment::value) {
      common_allocator_ = another_list.common_allocator_;
    }
  }

  List<T, Allocator>& operator=(const List<T, Allocator>& another_list) {
    try {
      swap(another_list);
    } catch (...) {
      throw;
    }
    return *this;
  }

  ~List() { destroy_list(); }

  Allocator get_allocator() { return common_allocator_; }

  Node* get_ptr_on_head() const { return ptr_on_head_; }

  size_t size() const { return size_of_list_; }
  bool empty() const {
    bool loc = true;
    if (size_of_list_ == 0) {
      return loc;
    }
    return !loc;
  }

  // iterator part
  template <bool IsConst, bool Reversed>
  class CommonIterator {
   public:
    using value_type = std::conditional_t<IsConst, const T, T>;
    using pointer = std::conditional_t<IsConst, const T*, T*>;
    using iterator_category = std::bidirectional_iterator_tag;
    using reference = std::conditional_t<IsConst, const T&, T&>;
    using difference_type = std::ptrdiff_t;

   private:
    Node* node_ = nullptr;

   public:
    CommonIterator() = default;

    CommonIterator(const CommonIterator<false, Reversed>& cur) {
      this->node_ = cur.get_node();
    }

    CommonIterator(Node* new_node) { node_ = new_node; }

    Node* get_node() const { return this->node_; }

    CommonIterator<IsConst, Reversed>& operator++() {
      if (Reversed) {
        node_ = node_->prev;
      } else {
        node_ = node_->next;
      }
      return *this;
    }

    CommonIterator<IsConst, Reversed>& operator--() {  // prefix
      if (Reversed) {
        node_ = node_->next;
      } else {
        node_ = node_->prev;
      }
      return *this;
    }

    CommonIterator<IsConst, Reversed> operator--(int) {
      CommonIterator<false, Reversed> copy(*this);
      --*this;
      return copy;
    }

    CommonIterator<IsConst, Reversed> operator++(int) {
      CommonIterator<false, Reversed> copy(*this);
      ++*this;
      return copy;
    }

    CommonIterator<false, false> base() {
      CommonIterator<false, false> iter(node_->next);
      return iter;
    }

    template <bool AnotherConst>
    bool operator==(
        const CommonIterator<AnotherConst, Reversed>& another) const {
      assert(node_ != nullptr);
      return this->node_ == another.node_;
    }

    template <bool AnotherConst>
    bool operator!=(
        const CommonIterator<AnotherConst, Reversed>& another) const {
      assert(node_ != nullptr);
      return this->node_ != another.node_;
    }

    reference operator*() const {
      assert(node_ != nullptr);
      return this->node_->value;
    }

    reference operator*() {
      assert(node_ != nullptr);
      return this->node_->value;
    }

    pointer operator->() const {
      assert(node_ != nullptr);
      return &this->node_->value;
    }

    pointer operator->() {
      assert(node_ != nullptr);
      return &this->node_->value;
    }

    int operator-(const CommonIterator<IsConst, Reversed>& another_iter) const {
      assert(another_iter.node_ != nullptr);
      auto cur = another_iter;
      int difference = 0;
      if (*this < another_iter) {
        while (cur != *this) {
          --cur;
          ++difference;
        }
      } else {
        while (cur != *this) {
          ++cur;
          ++difference;
        }
      }
      return difference;
    }
  };

  using iterator = CommonIterator<false, false>;
  using const_iterator = CommonIterator<true, false>;
  using reverse_iterator = CommonIterator<false, true>;
  using const_reverse_iterator = CommonIterator<true, true>;

  iterator begin() {
    CommonIterator<false, false> copy_iter(ptr_on_head_->next);
    return copy_iter;
  }

  const_iterator begin() const { return const_iterator(ptr_on_head_->next); }

  const_iterator cbegin() const { return const_iterator(ptr_on_head_->next); }

  iterator end() {
    CommonIterator<false, false> copy_of_end(ptr_on_head_);
    return copy_of_end;
  }

  const_iterator end() const {
    CommonIterator<true, false> copy_of_end(ptr_on_head_);
    return copy_of_end;
  }

  const_iterator cend() const {
    CommonIterator<false, false> copy_of_end(ptr_on_head_);
    return copy_of_end;
  }

  const_reverse_iterator crbegin() const {
    CommonIterator<true, true> iter(ptr_on_head_->next);
    return iter;
  }

  const_reverse_iterator crend() const {
    CommonIterator<true, true> iter(ptr_on_head_);
    return iter;
  }

  reverse_iterator rend() {
    CommonIterator<false, true> iter(ptr_on_head_);
    return iter;
  }

  reverse_iterator rbegin() {
    return CommonIterator<false, true>(ptr_on_head_->prev);
  }

  const_reverse_iterator rend() const {
    CommonIterator<true, true> iter(ptr_on_head_);
    return iter;
  }

  const_reverse_iterator rbegin() const {
    CommonIterator<true, true> iter(ptr_on_head_->prev);
    return iter;
  }
  // end of iterator's part
  void push_back(const T& val) {
    Node* new_node = node_alloc_traits::allocate(common_allocator_, 1);
    if (size_of_list_ != 0) {
      try {
        node_alloc_traits::construct(common_allocator_, new_node, val,
                                     ptr_on_head_, ptr_on_head_->prev);
        ptr_on_head_->prev->next = new_node;
        ptr_on_head_->prev = new_node;
      } catch (...) {
        node_alloc_traits::destroy(common_allocator_, new_node);
        node_alloc_traits::deallocate(common_allocator_, new_node, 1);
        this->~List();
        throw;
      }
    } else {
      try {
        node_alloc_traits::construct(common_allocator_, new_node, val,
                                     ptr_on_head_, ptr_on_head_);
        ptr_on_head_ = node_alloc_traits::allocate(common_allocator_, 1);
        ptr_on_head_->next = new_node;
        ptr_on_head_->prev = new_node;
      } catch (...) {
        node_alloc_traits::destroy(common_allocator_, new_node);
        node_alloc_traits::deallocate(common_allocator_, new_node, 1);
        this->~List();
        throw;
      }
    }
    ++size_of_list_;
  }

  void push_front(T&& val) {
    Node* new_node = node_alloc_traits::allocate(common_allocator_, 1);
    try {
      node_alloc_traits::construct(common_allocator_, new_node, std::move(val),
                                   ptr_on_head_->next, ptr_on_head_);
      ptr_on_head_->next->prev = new_node;
      ptr_on_head_->next = new_node;
      ++size_of_list_;
    } catch (...) {
      node_alloc_traits::deallocate(common_allocator_, new_node, 1);
      this->~List();
      throw;
    }
  }

  void pop_front() {
    assert(size_of_list_ != 0);
    Node* deleted = ptr_on_head_->next;
    Node* temp_node = ptr_on_head_->next->next;
    ptr_on_head_->next->next->prev = ptr_on_head_;
    ptr_on_head_->next = temp_node;
    node_alloc_traits::destroy(common_allocator_, deleted);
    node_alloc_traits::deallocate(common_allocator_, deleted, 1);
    --size_of_list_;
  }

  void pop_back() {
    assert(size_of_list_ != 0);
    Node* deleted = ptr_on_head_->prev;
    Node* temp_node = ptr_on_head_->prev->prev;
    ptr_on_head_->prev->prev->next = ptr_on_head_;
    ptr_on_head_->prev = temp_node;
    node_alloc_traits::destroy(common_allocator_, deleted);
    node_alloc_traits::deallocate(common_allocator_, deleted, 1);
    --size_of_list_;
  }
};
