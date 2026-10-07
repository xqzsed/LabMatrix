#pragma once
#include <iostream>
#include <stdexcept>
#include <random>
#include <algorithm>
#include "memdata.h"

template<typename T>
class TVector {
    MemData<T> _mem;
    size_t _front = 0;
    size_t _back = 0;

    size_t normalize_index(size_t index) const {
        return (_front + index) % _mem._capacity;
    }

    void copy_to_linear(T* dest) const {
        for (size_t i = 0; i < _mem._size; i++) {
            dest[i] = _mem._data[(_front + i) % _mem._capacity];
        }
    }

    void realloc_for_insert() {
        if (!_mem.is_full()) return;

        size_t new_capacity = calculate_capacity(_mem._capacity + 1);

        MemData<T> new_mem;
        new_mem.reset_memory(new_capacity);

        copy_to_linear(new_mem._data);
        new_mem._size = _mem._size;

        _mem = std::move(new_mem);
        _front = 0;
        _back = _mem._size ? _mem._size - 1 : 0;
    }

    void realloc_for_delete() {
        if (_mem._capacity <= MEM_STEP) return;

        if (_mem._size <= _mem._capacity - MEM_STEP) {
            size_t new_capacity = calculate_capacity(_mem._size);

            MemData<T> new_mem;
            new_mem.reset_memory(new_capacity);

            copy_to_linear(new_mem._data);
            new_mem._size = _mem._size;

            _mem = std::move(new_mem);
            _front = 0;
            _back = _mem._size ? _mem._size - 1 : 0;
        }
    }

public:
    TVector() = default;

    TVector(size_t size) : _mem(size), _front(0), _back(size ? size - 1 : 0) {}

    TVector(std::initializer_list<T> list)
        : _mem(list), _front(0), _back(list.size() ? list.size() - 1 : 0) {
    }

    TVector(T* arr, size_t size)
        : _mem(arr, size), _front(0), _back(size ? size - 1 : 0) {
    }

    TVector(const TVector& other)
        : _mem(other._mem), _front(other._front), _back(other._back) {
    }

    TVector(TVector&& other) noexcept
        : _mem(std::move(other._mem)), _front(other._front), _back(other._back) {
        other._front = other._back = 0;
    }


    bool is_empty() const noexcept { return _mem._size == 0; }
    bool is_full() const noexcept { return _mem._size == _mem._capacity; }

    size_t size() const noexcept { return _mem._size; }
    size_t capacity() const noexcept { return _mem._capacity; }


    T& front() {
        if (is_empty()) throw std::out_of_range("empty");
        return _mem._data[_front];
    }

    T& back() {
        if (is_empty()) throw std::out_of_range("empty");
        return _mem._data[_back];
    }

    T front() const {
        if (is_empty()) throw std::out_of_range("empty");
        return _mem._data[_front];
    }

    T back() const {
        if (is_empty()) throw std::out_of_range("empty");
        return _mem._data[_back];
    }

    T& operator[](size_t index) {
        if (index >= _mem._size) throw std::out_of_range("index");
        return _mem._data[normalize_index(index)];
    }

    T operator[](size_t index) const {
        if (index >= _mem._size) throw std::out_of_range("index");
        return _mem._data[normalize_index(index)];
    }

    void push_front(double value) {
        realloc_for_insert();

        if (is_empty()) {
            _front = _back = 0;
        }
        else {
            _front = (_front == 0 ? _mem._capacity - 1 : _front - 1);
        }

        _mem._data[_front] = value;
        _mem._size++;
    }

    void insert(double value, size_t pos) {
        if (pos > _mem._size) throw std::out_of_range("pos");

        if (pos == 0) return push_front(value);
        if (pos == _mem._size) return push_back(value);

        realloc_for_insert();

        if (pos < _mem._size / 2) {
            _front = (_front == 0 ? _mem._capacity - 1 : _front - 1);

            for (size_t i = 0; i < pos; i++) {
                size_t src = normalize_index(i + 1);
                size_t dst = normalize_index(i);
                _mem._data[dst] = _mem._data[src];
            }
        }
        else {
            _back = (_back + 1) % _mem._capacity;

            for (size_t i = _mem._size; i > pos; i--) {
                size_t src = normalize_index(i - 1);
                size_t dst = normalize_index(i);
                _mem._data[dst] = _mem._data[src];
            }
        }

        _mem._data[normalize_index(pos)] = value;
        _mem._size++;
    }

    void pop_back() {
        if (is_empty()) throw std::out_of_range("empty");

        _back = (_back == 0 ? _mem._capacity - 1 : _back - 1);
        _mem._size--;

        if (is_empty()) _front = _back = 0;

        realloc_for_delete();
    }

    void pop_front() {
        if (is_empty()) throw std::out_of_range("empty");

        _front = (_front + 1) % _mem._capacity;
        _mem._size--;

        if (is_empty()) _front = _back = 0;

        realloc_for_delete();
    }

    void erase(size_t pos) {
        if (pos >= _mem._size) throw std::out_of_range("pos");

        if (pos == 0) return pop_front();
        if (pos == _mem._size - 1) return pop_back();

        if (pos < _mem._size / 2) {
            for (size_t i = pos; i > 0; i--) {
                (*this)[i] = (*this)[i - 1];
            }
            pop_front();
        }
        else {
            for (size_t i = pos; i < _mem._size - 1; i++) {
                (*this)[i] = (*this)[i + 1];
            }
            pop_back();
        }
    }

    void sort() {
        if (_mem._size <= 1) return;

        std::sort(_mem._data, _mem._data + _mem._size);

        _front = 0;
        _back = _mem._size - 1;
    }

    void shuffle() {
        static std::mt19937 gen(std::random_device{}());

        for (size_t i = _mem._size - 1; i > 0; i--) {
            std::uniform_int_distribution<size_t> d(0, i);
            size_t j = d(gen);
            std::swap((*this)[i], (*this)[j]);
        }
    }

    TVector& operator=(const TVector& other) {
        if (this != &other) {
            _mem = other._mem;
            _front = other._front;
            _back = other._back;
        }
        return *this;
    }

    TVector& operator=(TVector&& other) noexcept {
        if (this != &other) {
            _mem = std::move(other._mem);
            _front = other._front;
            _back = other._back;
            other._front = other._back = 0;
        }
        return *this;
    }

    void push_back(T value) {
        realloc_for_insert();

        if (is_empty()) {
            _front = _back = 0;
        }
        else {
            _back = (_back + 1) % _mem._capacity;
        }

        _mem._data[_back] = value;
        _mem._size++;
    }

    friend std::ostream& operator<<(std::ostream& os, const TVector& v) {
        os << "{ ";
        for (size_t i = 0; i < v.size(); i++) {
            os << v[i];
            if (i + 1 < v.size()) os << ", ";
        }
        os << " }";
        return os;
    }


    friend std::istream& operator>>(std::istream& is, TVector& v) {
        size_t n;
        is >> n;
        v = TVector();
        for (size_t i = 0; i < n; i++) {
            T x;
            is >> x;
            v.push_back(x);
        }
        return is;
    }

    template <class Type> class Iterator;
    typedef Iterator<T> iterator;

    template <class T>
    class Iterator {
    private:
        T* p_cur;

    public:
        Iterator() {
            p_cur = nullptr;
        }
        Iterator(T* ptr) {
            p_cur = ptr;
        }
        Iterator(const Iterator& other) {
            p_cur = other.p_cur;
        }

        Iterator& operator=(const Iterator& other) noexcept {
            if (this != &other) {
                p_cur = other.p_cur;
                _mem = other._mem;
            }
            return *this;
        }

        bool operator==(const Iterator& other) const noexcept {
            return p_cur == other.p_cur;
        }
        bool operator!=(const Iterator& other) const noexcept {
            return p_cur != other.p_cur;
        }

        Iterator& operator++() noexcept {
            p_cur++;
            return *this;
        }
        Iterator operator++(int) noexcept {
            Iterator temp = *this;
            p_cur++;
            return temp;
        }

        Iterator& operator--() noexcept {
            p_cur--;
            return *this;
        }
        Iterator operator--(int) noexcept {
            Iterator temp = *this;
            p_cur--;
            return temp;
        }

        T& operator*() noexcept {
            return *p_cur;
        }
        T& operator*() const noexcept {
            return *p_cur;
        }
    };

    Iterator begin() noexcept;
    Iterator end() noexcept;

    Iterator begin() const noexcept;
    Iterator end() const noexcept;

};