#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

    void LinkedList::Add(Element e) {
        // Tip 1: создайте узел в куче со переданным значением
        // Tip 2: есть 2 случая - список пустой и непустой
        // Tip 3: не забудьте обновить поля head и tail
        // напишите свой код здесь ...
        Node * node = new Node(e, nullptr);
        if (size_ == 0) {
            size_ += 1;
            head_ = node;
            tail_ = node;
        } else {
            tail_ -> next = node;
            tail_ = node;
            size_ += 1;
        }
    }

    void LinkedList::Insert(int index, Element e) {
        internal::check_out_of_range(index, 0, size_ + 1);
        if (index == size_) /* тут проверяется то что список пустой */ {
            LinkedList::Add(e);
            return;
        }

        if (index == 0) /* тк эта проверка вторая достаточно этого условия */ {
            Node *new_node = new Node(e, head_);
            head_ = new_node;
            size_ += 1;
            return;
        }

        Node *curr_node = find_node(index - 1);
        Node *node = new Node(e, curr_node -> next);
        curr_node -> next = node;
        size_ += 1;
    }

    void LinkedList::Set(int index, Element e) {
        internal::check_out_of_range(index, 0, size_);
        Node *curr_node = find_node(index);
        curr_node -> data = e;
    }

    Element LinkedList::Remove(int index) {
        internal::check_out_of_range(index, 0, size_);
        // Tip 1: рассмотрите случай, когда удаляется элемент в начале списка
        // Tip 2: используйте функцию find_node(index)
        // напишите свой код здесь ...
        if (index == 0) {
            Node *new_head = head_ -> next;
            Element ret_value = head_ -> data;
            delete head_;
            head_ = new_head;
            size_ -= 1;
            return ret_value;
        }

        Node *curr_node = find_node(index - 1);
        Node *del_node = curr_node -> next;
        curr_node -> next = del_node -> next;
        if (index == size_ -1)
            tail_ = curr_node;
        size_ -= 1;
        Element ret_value = del_node -> data;
        delete del_node;
        return ret_value;
    }

    void LinkedList::Clear() {
        Node *curr_node = head_;
        Node *del_node;
        size_ = 0;
        head_ = nullptr;
        tail_ = nullptr;

        while (curr_node != nullptr) {
            del_node = curr_node;
            curr_node = curr_node -> next;
            delete del_node;
        }
    }

    Element LinkedList::Get(int index) const {
        internal::check_out_of_range(index, 0, size_);
        Node *curr_node = find_node(index);
        return curr_node -> data;
    }

    int LinkedList::IndexOf(Element e) const {
        int tmp = 0;
        for (Node *curr_node = head_; curr_node != nullptr; curr_node = curr_node -> next) {
            if (curr_node -> data == e)
                return tmp;
            tmp += 1;
        }
        return -1;
    }

    Node *LinkedList::find_node(int index) const {
        assert(index >= 0 && index < size_);
        if (index == size_ - 1)
            return tail_;
        if (index == 0)
            return head_;
        int counter = 0;
        // Tip 1: можете сразу обработать случаи поиска начала и конца списка
        // напишите свой код здесь ...
        for (Node* cur_node = head_; cur_node != nullptr; cur_node = cur_node -> next){
            if (counter == index)
                return cur_node;
            counter += 1;
        }
        return {};
    }

// РЕАЛИЗОВАНО

    LinkedList::~LinkedList() {
        Clear();
    }

    bool LinkedList::Contains(Element e) const {
        // если индекс не найден, значит и элемента нет
        return IndexOf(e) == kNotFoundElementIndex;
    }

    int LinkedList::GetSize() const {
        return size_;
    }

    bool LinkedList::IsEmpty() const {
        return size_ == 0;
    }

    Element LinkedList::tail() const {
        // вместо выброса ошибки в случае nullptr, римским парламентов было решено возвращать "специальное" значение
        return tail_ ? tail_->data : Element::UNINITIALIZED;
    }

    Element LinkedList::head() const {
        return head_ ? head_->data : Element::UNINITIALIZED;
    }

// === RESTRICTED AREA: необходимо для тестирования ===

    LinkedList::LinkedList(const std::vector<Element> &elements) {
        assert(!elements.empty());

        size_ = elements.size();
        head_ = new Node(elements[0], nullptr);

        auto current_node = head_;

        for (int index = 1; index < static_cast<int>(elements.size()); index++) {
            current_node->next = new Node(elements[index], nullptr);
            current_node = current_node->next;
        }
        tail_ = current_node;
    }

    std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
        if (list.head_ != nullptr && list.tail_ != nullptr) {
            os << "{ ";
            for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
                os << internal::elem_to_str(current_node->data) << ", ";
            }
            os << internal::elem_to_str(list.tail_->data) << " }";
        } else {
            os << "{ nullptr }";
        }
        return os;
    }

    bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
        if (list.size_ != static_cast<int>(elements.size())) return false;
        Node *current_node = list.head_;

        for (const auto e : elements) {
            if (current_node == nullptr) return false;
            if (current_node->data != e) return false;
            current_node = current_node->next;
        }
        return true;
    }

}  // namespace itis