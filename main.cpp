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

    void PushFront(const Type & value) {
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

private:
    // ��������� ����, ������������ ��� ������� "����� ������ ���������"
    Node head_;
    size_t size_ = 0;
};

// ��� ������� ��������� ������ SingleLinkedList
void Test2() {
    // ������������ �� ������� ������
    {
        SingleLinkedList<int> list;
        // ����������� ������ ��� ������� � ����������� ������� begin()/end()
        const auto& const_list = list;

        // ��������� begine � end � ������� ��������� ����� ���� �����
        assert(list.begin() == list.end());
        assert(const_list.begin() == const_list.end());
        assert(list.cbegin() == list.cend());
        assert(list.cbegin() == const_list.begin());
        assert(list.cend() == const_list.end());
    }

    // ������������ �� ��������� ������
    {
        SingleLinkedList<int> list;
        const auto& const_list = list;

        list.PushFront(1);
        assert(list.GetSize() == 1u);
        assert(!list.IsEmpty());

        assert(const_list.begin() != const_list.end());
        assert(const_list.cbegin() != const_list.cend());
        assert(list.begin() != list.end());

        assert(const_list.begin() == const_list.cbegin());

        assert(*list.cbegin() == 1);
        *list.begin() = -1;
        assert(*list.cbegin() == -1);

        const auto old_begin = list.cbegin();
        list.PushFront(2);
        assert(list.GetSize() == 2);

        const auto new_begin = list.cbegin();
        assert(new_begin != old_begin);
        // �������� ��������������
        {
            auto new_begin_copy(new_begin);
            assert((++(new_begin_copy)) == old_begin);
        }
        // �������� ��������������
        {
            auto new_begin_copy(new_begin);
            assert(((new_begin_copy)++) == new_begin);
            assert(new_begin_copy == old_begin);
        }
        // ��������, ����������� �� ������� ����� ���������� �������� ����� ��������� end()
        {
            auto old_begin_copy(old_begin);
            assert((++old_begin_copy) == list.end());
        }
    }
    // �������������� ����������
    {
        SingleLinkedList<int> list;
        list.PushFront(1);
        // ��������������� ConstItrator �� Iterator
        SingleLinkedList<int>::ConstIterator const_it(list.begin());
        assert(const_it == list.cbegin());
        assert(*const_it == *list.cbegin());

        SingleLinkedList<int>::ConstIterator const_it1;
        // ������������ ConstIterator-� �������� Iterator
        const_it1 = list.begin();
        assert(const_it1 == const_it);
    }
    // �������� ��������� ->
    {
        using namespace std;
        SingleLinkedList<std::string> string_list;

        string_list.PushFront("one"s);
        assert(string_list.cbegin()->length() == 3u);
        string_list.begin()->push_back('!');
        assert(*string_list.begin() == "one!"s);
    }
}

int main() {
    Test2();
}