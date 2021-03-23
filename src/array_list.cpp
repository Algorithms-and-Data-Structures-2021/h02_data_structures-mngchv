#include "array_list.hpp"  // подключаем заголовочный файл с объявлениями

#include <algorithm>  // copy, fill
#include <cassert>    // assert
#include <stdexcept>  // out_of_range, invalid_argument

#include "private/internal.hpp"  // вспомогательные функции

namespace itis {

    ArrayList::ArrayList(int capacity) : capacity_{capacity} {
        if (capacity <= 0) {
            throw std::invalid_argument("ArrayList::capacity must be positive");
        }
        data_ = new Element [capacity_];
        std::fill(data_, data_ + capacity_, Element::UNINITIALIZED);
    }

    ArrayList::~ArrayList() {
        if (data_ != nullptr) {
            delete[] data_;
            data_ = nullptr;
        }
        size_ = 0;
        capacity_ = 0;
    }

    void ArrayList::Add(Element e) {
        if (size_ == capacity_) {
            ArrayList::resize(capacity_ + kCapacityGrowthCoefficient);
        }

        assert(size_ < capacity_);
        data_[size_] = e;
        size_ += 1;
    }

    void ArrayList::Insert(int index, Element e) {
        if (index != size_) {
            // index = 0 и index == size это особые случаи, при которых всегда можно выполнить операцию вставки
            internal::check_out_of_range(index, 0, size_);
        } else {
            ArrayList::Add(e);
            return;
        }

        if (size_ == capacity_)
            ArrayList::resize (capacity_ + kCapacityGrowthCoefficient);
        assert(size_ < capacity_);
        std::copy(data_ + index, data_ + size_, data_ + index + 1);
        data_[index] = e;
        size_ += 1;
    }

    void ArrayList::Set(int index, Element value) {
        internal::check_out_of_range(index, 0, size_);
        data_[index] = value;
    }

    Element ArrayList::Remove(int index) {
        internal::check_out_of_range(index, 0, size_);
        Element elem = data_[index];
        std::copy(data_ + index + 1, data_ + size_, data_ + index);
        size_ -= 1;
        data_[size_] = Element::UNINITIALIZED;

        return elem;
    }

    void ArrayList::Clear() {
        std::fill(data_, data_ + size_, Element::UNINITIALIZED);
        size_ = 0;
    }

    Element ArrayList::Get(int index) const {
        internal::check_out_of_range(index, 0, size_);
        return data_[index];
    }

    int ArrayList::IndexOf(Element e) const {
        for (int i = 0; i < size_; i++) {
            if (data_[i] == e)
                return i;
        }
        return -1;
    }

// === РЕАЛИЗОВАНО ===

    bool ArrayList::Contains(Element e) const {
        // здесь был Рамиль
        return IndexOf(e) != kNotFoundElementIndex;
    }

// это делегирующий конструктор если что
    ArrayList::ArrayList() : ArrayList(kInitCapacity) {}

    int ArrayList::GetSize() const {
        return size_;
    }

    int ArrayList::GetCapacity() const {
        return capacity_;
    }

    bool ArrayList::IsEmpty() const {
        return size_ == 0;
    }

// Легенда: давным давно на планете под названием Земля жил да был Аватар...
// Аватар мог управлять четырьмя стихиями, но никак не мог совладать с C++ (фейспалм).
// Помогите найти непростительную ошибку Аватара,
// которая привела к гибели десятков тысяч котиков (плак-плак, шмыгание носом, втягивание соплей).
// P.S. кол-во ошибок может быть более одной, порядку операций можно верить
    void ArrayList::resize(int new_capacity) {
        assert(new_capacity > capacity_);  // не ошибается тот, кто ничего не делает ...

        // 1. выделяем новый участок памяти
        auto new_data = new Element[new_capacity];

        // 2. копируем данные на новый участок
        std::copy(data_, data_ + size_, new_data);

        // 3. заполняем "свободные" ячейки памяти значением Element::UNINITIALIZED
        std::fill(new_data + size_, new_data + new_capacity, Element::UNINITIALIZED);

        // 4. высвобождаем старый участок памяти меньшего размера
        delete[] data_;

        // 5. пересылаем указатель на новый участок памяти
        data_ = new_data;

        // 6. не забываем посолить ... кхм... обновить емкость массива
        capacity_ = new_capacity;
    }

// === ЗОНА 51: необходимо для тестирования ===

    ArrayList::ArrayList(Element *data, int size, int capacity) : size_{size}, capacity_{capacity} {
        assert(capacity > 0 && size >= 0 && size <= capacity);

        data_ = new Element[capacity];
        std::fill(data_, data_ + capacity, Element::UNINITIALIZED);

        if (data != nullptr) {
            std::copy(data, data + size, data_);
        }
    }

    std::ostream &operator<<(std::ostream &os, const ArrayList &list) {
        if (list.data_ != nullptr) {
            os << "{ ";
            for (int index = 0; index < list.capacity_ - 1; index++) {
                os << internal::elem_to_str(list.data_[index]) << ", ";
            }
            os << internal::elem_to_str(list.data_[list.capacity_ - 1]) << " }";
        } else {
            os << "{ nullptr }";
        }
        return os;
    }

    bool operator==(const ArrayList &list, const std::vector<Element> &elements) {
        if (list.data_ == nullptr) return false;
        if (list.capacity_ != static_cast<int>(elements.size())) return false;

        for (int index = 0; index < list.capacity_; index++) {
            if (list.data_[index] != elements.at(index)) return false;
        }
        return true;
    }

}  // namespace itis