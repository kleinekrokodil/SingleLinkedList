#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    // ���� ������
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value{};
        Node* next_node = nullptr;
    };

    // ������ ������ ������� ��������.
    // ���������� ��������� ��������� �� �������� ������������ ������
    // ValueType - ��������� � Type (��� Iterator) ���� � const Type (��� ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // ����� ������ ����������� �������������, ����� �� ������� ������
        // ��� ������ � ��������� ������� ���������
        friend class SingleLinkedList;

        // �������������� ����������� ��������� �� ��������� �� ���� ������
        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:
        // ����������� ���� ���� �������� ����������� ���������� � ��������� ����� ���������

        // ��������� ��������� - forward iterator
        // (��������, ������� ������������ �������� ���������� � ������������ �������������)
        using iterator_category = std::forward_iterator_tag;
        // ��� ���������, �� ������� ������������ ��������
        using value_type = Type;
        // ���, ������������ ��� �������� �������� ����� �����������
        using difference_type = std::ptrdiff_t;
        // ��� ��������� �� ����������� ��������
        using pointer = ValueType*;
        // ��� ������ �� ����������� ��������
        using reference = ValueType&;

        BasicIterator() = default;

        // �������������� �����������/����������� �����������
        // ��� ValueType, ����������� � Type, ������ ���� ����������� ������������
        // ��� ValueType, ����������� � const Type, ������ ���� ��������������� ������������
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        // ����� ���������� �� ������� �������������� �� ���������� ��������� = ��� �������
        // ����������������� ������������ �����������, ���� ������� �������� = �
        // �������� ���������� ������������� ��� �� ���.
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // �������� ��������� ���������� (� ���� ������� ��������� ��������� ����������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������, ���� �� end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // �������� �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        // �������� ��������� ���������� (� ���� ������� ��������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������, ���� �� end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // �������� �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        // �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������
        // ���������� ������ �� ������ ����
        // ��������� ���������, �� ������������ �� ������������ ������� ������, �������� � �������������� ���������
        BasicIterator& operator++() noexcept {
            if (node_ != nullptr) {
                node_ = node_->next_node;
            }
            return *this;
        }

        // �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������.
        // ���������� ������� �������� ���������
        // ��������� ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        BasicIterator operator++(int) noexcept {
            auto old_value(*this); // ��������� ������� �������� ������� ��� ������������ ��������
            ++(*this); // ���������� ������ ���������� ����� ����������
            return old_value;
        }

        // �������� �������������. ���������� ������ �� ������� �������
        // ����� ����� ��������� � ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        [[nodiscard]] reference operator*() const noexcept {
            ValueType& val = node_->value;
            reference val_ref = val;
            return val_ref;
        }

        // �������� ������� � ����� ������. ���������� ��������� �� ������� ������� ������.
        // ����� ����� ��������� � ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        [[nodiscard]] pointer operator->() const noexcept {
            ValueType& val = node_->value;
            pointer val_ptr = &val;
            return val_ptr;
        }

    private:
        Node* node_ = nullptr;
    };


public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // ��������, ����������� ��������� ��������� ������
    using Iterator = BasicIterator<Type>;
    // ����������� ��������, ��������������� ������ ��� ������ � ��������� ������
    using ConstIterator = BasicIterator<const Type>;

    // ���������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    // ���������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] Iterator end() noexcept {
        return Iterator{ nullptr };
    }

    // ���������� ����������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� end()
    // ��������� ������ ������������ ������ ������ cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    // ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    // ��������� ������ ������������ ������ ������ cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{ nullptr };
    }

    // ���������� ����������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    // ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{ nullptr };
    }
    SingleLinkedList<Type>() = default;
    // ���������� ���������� ��������� � ������ �� ����� O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // ��������, ������ �� ������ �� ����� O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        if (size_ == 0) {
            return true;
        }
        else {
            return false;
        }
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            Node* prev = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete prev;
        }
        size_ = 0;
    }

    ~SingleLinkedList() {
        Clear();
    }

    template <typename Iterator>
    void FillSingleLinkedList(Iterator begin, Iterator end) {
        SingleLinkedList temp;
        if (begin == end) {
            return;
        }
        Node* next = &temp.head_;
        Iterator it = begin;
        while (it != end) {
            next->next_node = new Node(*it, nullptr);
            next = next->next_node;
            ++it;
            ++temp.size_;
        }
        Clear();
        swap(temp);
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        FillSingleLinkedList(values.begin(), values.end());

    }

    SingleLinkedList(const SingleLinkedList& other) {
        if (head_.next_node != other.head_.next_node) {
            FillSingleLinkedList(other.begin(), other.end());
    
        }
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        // ������ �� ����������������
        if (this != &rhs) {
            if (rhs.IsEmpty()) {  // ������������ ������������ ������� ������
                Clear();
            }
            else {
                auto rhs_copy(rhs);  // ����� ������� ����������
                swap(rhs_copy);
            }
        }
        return *this;
    }

    // ���������� ���������� ������� �� ����� O(1)
    void swap(SingleLinkedList& other) noexcept {
        if (head_.next_node != other.head_.next_node) {
            Node* temp = head_.next_node;
            size_t temp_size = size_;
            head_.next_node = other.head_.next_node;
            size_ = other.size_;
            other.head_.next_node = temp;
            other.size_ = temp_size;
        }
    }

private:
    // ��������� ����, ������������ ��� ������� "����� ������ ���������"
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), [](Type x, Type y) {return x == y; });
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), [](Type x, Type y) {return x < y; });
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}

// ��� ������� ��������� ������ ������ SingleLinkedList
void Test3() {
    // �������� ������� �� ��������� � �����������
    {
        SingleLinkedList<int> list_1;
        list_1.PushFront(1);
        list_1.PushFront(2);

        SingleLinkedList<int> list_2;
        list_2.PushFront(1);
        list_2.PushFront(2);
        list_2.PushFront(3);

        SingleLinkedList<int> list_1_copy;
        list_1_copy.PushFront(1);
        list_1_copy.PushFront(2);

        SingleLinkedList<int> empty_list;
        SingleLinkedList<int> another_empty_list;

        // ������ ����� ������ ����
        assert(list_1 == list_1);
        assert(empty_list == empty_list);

        // ������ � ���������� ���������� �����, � � ������ - �� �����
        assert(list_1 == list_1_copy);
        assert(list_1 != list_2);
        assert(list_2 != list_1);
        assert(empty_list == another_empty_list);
    }

    // ����� ����������� �������
    {
        SingleLinkedList<int> first;
        first.PushFront(1);
        first.PushFront(2);

        SingleLinkedList<int> second;
        second.PushFront(10);
        second.PushFront(11);
        second.PushFront(15);

        const auto old_first_begin = first.begin();
        const auto old_second_begin = second.begin();
        const auto old_first_size = first.GetSize();
        const auto old_second_size = second.GetSize();

        first.swap(second);

        assert(second.begin() == old_first_begin);
        assert(first.begin() == old_second_begin);
        assert(second.GetSize() == old_first_size);
        assert(first.GetSize() == old_second_size);

        // ����� ��� ������ ������� swap
        {
            using std::swap;

            // � ���������� ���������������� ���������� ����� ������� ������� std::swap, �������
            // �������� ����� ����� �������� ��������� �����
            swap(first, second);

            // ����������, ��� ������������ �� std::swap, � ���������������� ����������

            // ���� �� ����� ��� �������� � ��������� ��������� �����,
            // �� �������� first.begin() �� ����� ����� ����� ����������� ��������,
            // ��� ��� ����� ����� ������� ���� ���� �� ���� �������
            assert(first.begin() == old_first_begin);
            assert(second.begin() == old_second_begin);
            assert(first.GetSize() == old_first_size);
            assert(second.GetSize() == old_second_size);
        }
    }

    // ������������� ������ ��� ������ std::initializer_list
    {
        SingleLinkedList<int> list{ 1, 2, 3, 4, 5 };
        assert(list.GetSize() == 5);
        assert(!list.IsEmpty());
        assert(std::equal(list.begin(), list.end(), std::begin({ 1, 2, 3, 4, 5 })));
    }

    // ������������������ ��������� �������
    {
        using IntList = SingleLinkedList<int>;

        assert((IntList{ 1, 2, 3 } < IntList{ 1, 2, 3, 1 }));
        assert((IntList{ 1, 2, 3 } <= IntList{ 1, 2, 3 }));
        assert((IntList{ 1, 2, 4 } > IntList{ 1, 2, 3 }));
        assert((IntList{ 1, 2, 3 } >= IntList{ 1, 2, 3 }));
    }

    // ����������� �������
    {
        const SingleLinkedList<int> empty_list{};
        // ����������� ������� ������
        {
            auto list_copy(empty_list);
            assert(list_copy.IsEmpty());
        }

        SingleLinkedList<int> non_empty_list{ 1, 2, 3, 4 };
        // ����������� ��������� ������
        {
            auto list_copy(non_empty_list);

            assert(non_empty_list.begin() != list_copy.begin());
            assert(list_copy == non_empty_list);
        }
    }

    // ������������ �������
    {
        const SingleLinkedList<int> source_list{ 1, 2, 3, 4 };

        SingleLinkedList<int> receiver{ 5, 4, 3, 2, 1 };
        receiver = source_list;
        assert(receiver.begin() != source_list.begin());
        assert(receiver == source_list);
    }

    // ��������������� �����, ��������� ���������� ����� �������� N-�����
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                }
                else {
                    --(*countdown_ptr);
                }
            }
        }
        // ������������ ��������� ����� ���� �� ���������
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // ����� �������� ��������� �������. ���� �� ����� nullptr, �� ����������� ��� ������ �����������.
        // ��� ������ ���������, ����������� ����������� �������� ����������
        int* countdown_ptr = nullptr;
    };

    // ���������� ������������ �������
    {
        SingleLinkedList<ThrowOnCopy> src_list;
        src_list.PushFront(ThrowOnCopy{});
        src_list.PushFront(ThrowOnCopy{});
        auto thrower = src_list.begin();
        src_list.PushFront(ThrowOnCopy{});

        int copy_counter = 0;  // ��� ������ �� ����������� ����� ��������� ����������
        thrower->countdown_ptr = &copy_counter;

        SingleLinkedList<ThrowOnCopy> dst_list;
        dst_list.PushFront(ThrowOnCopy{});
        int dst_counter = 10;
        dst_list.begin()->countdown_ptr = &dst_counter;
        dst_list.PushFront(ThrowOnCopy{});

        try {
            dst_list = src_list;
            // ��������� ���������� ��� ������������
            assert(false);
        }
        catch (const std::bad_alloc&) {
            // ���������, ��� ��������� ������-�������� �� ����������
            // ��� ������������ ����������
            assert(dst_list.GetSize() == 2);
            auto it = dst_list.begin();
            assert(it != dst_list.end());
            assert(it->countdown_ptr == nullptr);
            ++it;
            assert(it != dst_list.end());
            assert(it->countdown_ptr == &dst_counter);
            assert(dst_counter == 10);
        }
        catch (...) {
            // ������ ����� ���������� �� ���������
            assert(false);
        }
    }
}

int main() {
    Test3();
}