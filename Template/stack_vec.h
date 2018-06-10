//          Copyright LibreMaple Team 2018 - 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)
#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

namespace jrc
{
template<typename T, int N>
class stack_vec
{
public:
    using size_type = int;
    template<typename U>
    using optional_ref = std::optional<std::reference_wrapper<U>>;

    class iterator
    {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::random_access_iterator_tag iterator_category;

        constexpr iterator() noexcept : p(nullptr)
        {
        }
        constexpr iterator(const iterator& other) noexcept : p(other.p)
        {
        }
        constexpr iterator(stack_vec<T, N>& sv) noexcept : p(sv.data())
        {
        }
        constexpr iterator(stack_vec<T, N>& sv, size_type pos) noexcept
            : p(&sv[pos])
        {
        }
        explicit constexpr iterator(T* other_p) noexcept : p(other_p)
        {
        }

        ~iterator() noexcept = default;

        constexpr iterator& operator=(const iterator& other) noexcept
        {
            p = other.p;
            return *this;
        }

        constexpr bool operator==(const iterator& other) const noexcept
        {
            return p == other.p;
        }

        constexpr bool operator!=(const iterator& other) const noexcept
        {
            return p != other.p;
        }

        constexpr bool operator<(const iterator& other) const noexcept
        {
            return p < other.p;
        }

        constexpr bool operator>(const iterator& other) const noexcept
        {
            return p > other.p;
        }

        constexpr bool operator<=(const iterator& other) const noexcept
        {
            return p <= other.p;
        }

        constexpr bool operator>=(const iterator& other) const noexcept
        {
            return p >= other.p;
        }

        constexpr iterator& operator++() noexcept
        {
            ++p;
            return *this;
        }
        constexpr iterator operator++(int) noexcept
        {
            iterator copy{*this};
            ++(*this);
            return copy;
        }

        constexpr iterator& operator--() noexcept
        {
            --p;
            return *this;
        }
        constexpr iterator operator--(int) noexcept
        {
            iterator copy{*this};
            --(*this);
            return copy;
        }

        constexpr iterator& operator+=(size_type increment) noexcept
        {
            p += increment;
            return *this;
        }

        constexpr iterator operator+(size_type increment) const noexcept
        {
            return iterator{p + increment};
        }

        constexpr iterator& operator-=(size_type decrement) noexcept
        {
            p -= decrement;
            return *this;
        }

        constexpr iterator operator-(size_type decrement) const noexcept
        {
            return iterator{p - decrement};
        }
        constexpr difference_type operator-(iterator other) const noexcept
        {
            return p - other.p;
        }
        constexpr difference_type operator-(T* other_p) const noexcept
        {
            return p - other_p;
        }

        constexpr reference operator*() const noexcept
        {
            return *p;
        }

        constexpr pointer operator->() const noexcept
        {
            return p;
        }

        constexpr reference operator[](size_type pos) const noexcept
        {
            return p[pos];
        }

    private:
        T* p;
    };

    class const_iterator
    {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T& reference;
        typedef const T* pointer;
        typedef std::random_access_iterator_tag iterator_category;

        constexpr const_iterator() noexcept : p(nullptr)
        {
        }
        constexpr const_iterator(const const_iterator& other) noexcept
            : p(other.p)
        {
        }
        constexpr const_iterator(const iterator& other) noexcept
            : p(other.operator->())
        {
        }
        constexpr const_iterator(const stack_vec<T, N>& sv) noexcept
            : p(sv.data())
        {
        }
        constexpr const_iterator(const stack_vec<T, N>& sv,
                                 size_type pos) noexcept
            : p(&sv[pos])
        {
        }
        explicit constexpr const_iterator(T* other_p) noexcept : p(other_p)
        {
        }

        ~const_iterator() noexcept = default;

        constexpr const_iterator&
        operator=(const const_iterator& other) noexcept
        {
            p = other.p;
            return *this;
        }

        constexpr bool operator==(const const_iterator& other) const noexcept
        {
            return p == other.p;
        }

        constexpr bool operator!=(const const_iterator& other) const noexcept
        {
            return p != other.p;
        }

        constexpr bool operator<(const const_iterator& other) const noexcept
        {
            return p < other.p;
        }

        constexpr bool operator>(const const_iterator& other) const noexcept
        {
            return p > other.p;
        }

        constexpr bool operator<=(const const_iterator& other) const noexcept
        {
            return p <= other.p;
        }

        constexpr bool operator>=(const const_iterator& other) const noexcept
        {
            return p >= other.p;
        }

        constexpr const_iterator& operator++() noexcept
        {
            ++p;
            return *this;
        }
        constexpr const_iterator operator++(int) noexcept
        {
            const_iterator copy{*this};
            ++(*this);
            return copy;
        }

        constexpr const_iterator& operator--() noexcept
        {
            --p;
            return *this;
        }
        constexpr const_iterator operator--(int) noexcept
        {
            const_iterator copy{*this};
            --(*this);
            return copy;
        }

        constexpr const_iterator& operator+=(size_type increment) noexcept
        {
            p += increment;
            return *this;
        }

        constexpr const_iterator operator+(size_type increment) const noexcept
        {
            return const_iterator{p + increment};
        }

        constexpr const_iterator& operator-=(size_type decrement) noexcept
        {
            p -= decrement;
            return *this;
        }

        constexpr const_iterator operator-(size_type decrement) const noexcept
        {
            return const_iterator{p - decrement};
        }
        constexpr difference_type operator-(const_iterator other) const
            noexcept
        {
            return p - other.p;
        }
        constexpr difference_type operator-(T* other_p) const noexcept
        {
            return p - other_p;
        }

        constexpr reference operator*() const noexcept
        {
            return *p;
        }

        constexpr pointer operator->() const noexcept
        {
            return p;
        }

        constexpr reference operator[](size_type pos) const noexcept
        {
            return p[pos];
        }

    private:
        const T* p;
    };

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    constexpr stack_vec() noexcept : len(0)
    {
    }
    constexpr stack_vec(const T& value) noexcept : len(N)
    {
        a.fill(value);
    }
    constexpr stack_vec(size_type count, const T& value) noexcept : len(count)
    {
        for (size_type i = 0; i < count; ++i) {
            a[i] = value;
        }
    }
    explicit constexpr stack_vec(size_type count) noexcept : len(count)
    {
    }
    constexpr stack_vec(const stack_vec<T, N>& other) noexcept : len(other.len)
    {
        std::memcpy(a.data(), other.data(), other.len * sizeof(T));
    }
    constexpr stack_vec(stack_vec<T, N>&& other) noexcept = default;
    constexpr stack_vec(std::initializer_list<T> init) noexcept
        : len(static_cast<size_type>(init.size()))
    {
        std::copy(init.begin(), init.end(), a.begin());
    }
    stack_vec(const std::vector<T>& vec) noexcept
    {
        len = std::min(N, static_cast<size_type>(vec.size()));
        std::memcpy(a.data(), vec.data(), len * sizeof(T));
    }

    ~stack_vec() noexcept = default;

    constexpr stack_vec<T, N>& operator=(const stack_vec<T, N>& other) noexcept
    {
        len = other.len;
        std::memcpy(a.data(), other.data(), other.len * sizeof(T));
        return *this;
    }
    constexpr stack_vec<T, N>& operator=(stack_vec<T, N>&& other) noexcept
    {
        len = std::move(other.len);
        a = std::move(other.a);
        return *this;
    }
    constexpr stack_vec<T, N>&
    operator=(std::initializer_list<T> ilist) noexcept
    {
        len = static_cast<size_type>(ilist.size());
        std::copy(ilist.begin(), ilist.end(), a.begin());
        return *this;
    }

    constexpr void assign(size_type count, const T& value) noexcept
    {
        len = count;
        for (size_type i = 0; i < count; ++i) {
            a[i] = value;
        }
    }
    constexpr void assign(std::initializer_list<T> ilist) noexcept
    {
        len = static_cast<size_type>(ilist.size());
        std::copy(ilist.begin(), ilist.end(), a.begin());
    }

    template<int M>
    constexpr bool operator==(const stack_vec<T, M>& other) const noexcept
    {
        return std::equal(begin(), end(), other.begin(), other.end());
    }
    template<int M>
    constexpr bool operator!=(const stack_vec<T, M>& other) const noexcept
    {
        return !std::equal(begin(), end(), other.begin(), other.end());
    }

    constexpr optional_ref<T> at(size_type pos) noexcept
    {
        if (pos >= 0 && pos < len) {
            return {a[pos]};
        } else {
            return {};
        }
    }
    constexpr const optional_ref<T> at(size_type pos) const noexcept
    {
        if (pos >= 0 && pos < len) {
            return {a[pos]};
        } else {
            return {};
        }
    }

    constexpr T& operator[](size_type pos) noexcept
    {
        return a[pos];
    }
    constexpr const T& operator[](size_type pos) const noexcept
    {
        return a[pos];
    }

    constexpr T& front() noexcept
    {
        return a.front();
    }
    constexpr const T& front() const noexcept
    {
        return a.front();
    }

    constexpr T& back() noexcept
    {
        return a[len - 1];
    }
    constexpr const T& back() const noexcept
    {
        return a[len - 1];
    }

    constexpr T* data() noexcept
    {
        return a.data();
    }
    constexpr const T* data() const noexcept
    {
        return a.data();
    }

    constexpr iterator begin() noexcept
    {
        return {*this};
    }
    constexpr const_iterator begin() const noexcept
    {
        return {*this};
    }
    constexpr const_iterator cbegin() const noexcept
    {
        return {*this};
    }

    constexpr iterator end() noexcept
    {
        return {*this, len};
    }
    constexpr const_iterator end() const noexcept
    {
        return {*this, len};
    }
    constexpr const_iterator cend() const noexcept
    {
        return {*this, len};
    }

    constexpr reverse_iterator rbegin() noexcept
    {
        return {begin()};
    }
    constexpr const_reverse_iterator rbegin() const noexcept
    {
        return {begin()};
    }
    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return {cbegin()};
    }

    constexpr reverse_iterator rend() noexcept
    {
        return {end()};
    }
    constexpr const_reverse_iterator rend() const noexcept
    {
        return {end()};
    }
    constexpr const_reverse_iterator crend() const noexcept
    {
        return {cend()};
    }

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return len == 0;
    }

    constexpr size_type size() const noexcept
    {
        return len;
    }

    constexpr size_type max_size() const noexcept
    {
        return N;
    }

    constexpr size_type capacity() const noexcept
    {
        return N;
    }

    constexpr bool full() const noexcept
    {
        return len == N;
    }

    constexpr void clear() noexcept
    {
        if constexpr (std::is_destructible_v<T>) {
            for (size_type i = 0; i < len; ++i) {
                a[i].~T();
            }
        }
        len = 0;
    }

    //! Inserts an element at the specified location in the `stack_vec`,
    //! shifting everything to the right to make room. Returns an iterator
    //! pointing to the newly inserted value.
    //!
    //! If there is no more room left (`size() == N`), this does nothing and
    //! returns the `end()` iterator.
    constexpr iterator insert(const_iterator pos, const T& value) noexcept
    {
        if (!full()) {
            T* p = const_cast<T*>(
                pos.operator->()); // Evil nonsense. Don't do this.
            iterator it{p};
            std::memmove(p + 1, p, (len - (pos - data())) * sizeof(T));

            *it = value;
            ++len;

            return it;
        } else {
            return end();
        }
    }
    //! Inserts an element at the specified location in the `stack_vec`,
    //! shifting everything to the right to make room. Returns an iterator
    //! pointing to the newly inserted value.
    //!
    //! If there is no more room left (`size() == N`), this does nothing and
    //! returns the `end()` iterator.
    constexpr iterator insert(const_iterator pos, T&& value) noexcept
    {
        if (!full()) {
            T* p = const_cast<T*>(
                pos.operator->()); // Evil nonsense. Don't do this.
            iterator it{p};
            std::memmove(p + 1, p, (len - (pos - data())) * sizeof(T));

            *it = std::move(value);
            ++len;

            return it;
        } else {
            return end();
        }
    }
    //! Inserts `count` copies of the element `value` at the specified location
    //! in the `stack_vec`, shifting everything to the right to make room.
    //! Returns an iterator pointing to the first newly inserted value, or
    //! `pos` if `count == 0`.
    //!
    //! If there is no more room left (`size() > N - count`), this does nothing
    //! and returns the `end()` iterator.
    constexpr iterator
    insert(const_iterator pos, size_type count, const T& value) noexcept
    {
        if (len <= N - count) {
            T* p = const_cast<T*>(
                pos.operator->()); // Evil nonsense. Don't do this.
            iterator it{p};
            if (count == 0) {
                return it;
            }

            std::memmove(p + count, p, (len - (pos - data())) * sizeof(T));
            len += count;
            for (; count--; ++p) {
                *p = value;
            }

            return it;
        } else {
            return end();
        }
    }
    //! Inserts the values of the initializer list at the specified location
    //! into the `stack_vec`, shifting everything to the right to make room.
    //! Returns an iterator pointing to the first newly inserted value, or
    //! `pos` if the initializer list is empty.
    //!
    //! If there is no more room left (`size() > N - ilist.size()`), this does
    //! nothing and returns the `end()` iterator.
    constexpr iterator insert(const_iterator pos,
                              std::initializer_list<T> ilist) noexcept
    {
        auto ilist_size = ilist.size();
        if (len <= N - static_cast<size_type>(ilist_size)) {
            T* p = const_cast<T*>(
                pos.operator->()); // Evil nonsense. Don't do this.
            iterator it{p};
            if (ilist_size == 0) {
                return it;
            }

            std::memmove(
                p + ilist_size, p, (len - (pos - data())) * sizeof(T));
            len += ilist_size;

            for (auto& item : ilist) {
                *p = item;
                ++p;
            }

            return it;
        } else {
            return end();
        }
    }

    //! Constructs an element in-place at the specified location in the
    //! `stack_vec`, shifting everything to the right to make room. Returns an
    //! iterator pointing to the newly inserted value.
    //!
    //! If there is no more room left (`size() == N`), this does nothing and
    //! returns the `end()` iterator.
    template<typename... Args>
    constexpr iterator
    emplace(const_iterator pos,
            Args&&... args) noexcept(noexcept(T{std::forward<Args>(args)...}))
    {
        if (!full()) {
            T* p = const_cast<T*>(
                pos.operator->()); // Evil nonsense. Don't do this.
            iterator it{p};
            std::memmove(p + 1, p, (len - (pos - data())) * sizeof(T));

            *it = std::move(T{std::forward<Args>(args)...});
            ++len;

            return it;
        } else {
            return end();
        }
    }

    constexpr iterator erase(const_iterator pos) noexcept
    {
        T* p =
            const_cast<T*>(pos.operator->()); // Evil nonsense. Don't do this.
        iterator it{p};
        if constexpr (std::is_destructible_v<T>) {
            it->~T();
        }
        std::memmove(p, p + 1, (len - (pos - data()) - 1) * sizeof(T));
        --len;
        return it;
    }
    constexpr iterator erase(const_iterator first,
                             const_iterator last) noexcept
    {
        T* first_p = const_cast<T*>(
            first.operator->()); // Evil nonsense. Don't do this.
        iterator it{first_p};
        if (first == last) {
            return it;
        }

        if constexpr (std::is_destructible_v<T>) {
            for (; first != last; ++first) {
                first->~T();
            }
        }
        std::memmove(
            first_p, last.operator->(), (len - (last - data())) * sizeof(T));
        len -= last - first_p;

        return it;
    }

    //! Pushes a copy of `value` to the back of this `stack_vec`. If the
    //! `stack_vec` is already `full()`, then this function does nothing and
    //! returns `false`. Otherwise it returns `true`.
    constexpr bool push_back(const T& value) noexcept
    {
        if (full()) {
            return false;
        }

        a[len] = value;
        ++len;

        return true;
    }
    //! Moves `value` into the back of this `stack_vec`. If the
    //! `stack_vec` is already `full()`, then this function does nothing and
    //! returns `false`. Otherwise it returns `true`.
    constexpr bool push_back(T&& value) noexcept
    {
        if (full()) {
            return false;
        }

        a[len] = std::move(value);
        ++len;

        return true;
    }

    //! In-place constructs a new value into the back of this `stack_vec`. If
    //! the `stack_vec` is already `full()`, then this function does nothing
    //! and returns an empty `optional`. Otherwise it returns an `optional`
    //! containing a reference to the newly-constructed object.
    template<typename... Args>
    constexpr optional_ref<T> emplace_back(Args&&... args) noexcept(noexcept(T{
        std::forward<Args>(args)...}))
    {
        if (full()) {
            return {};
        }

        a[len] = std::move(T{std::forward<Args>(args)...});
        ++len;

        return {a[len - 1]};
    }

    constexpr void pop_back() noexcept
    {
        --len;
        if constexpr (std::is_destructible_v<T>) {
            a[len].~T();
        }
    }

    //! Resizes the `stack_vec` to contain `count` elements.
    //!
    //! If `count` is greater than `N`, this function is a no-op and returns
    //! `false`. Otherwise, it will return `true`.
    //!
    //! If the current `size()` is greater than `count`, the `stack_vec` is
    //! reduced to its first `count` elements.
    //!
    //! If the current `size()` is less than `count`, the uninitialized slots
    //! are filled with default-constructed values.
    constexpr bool resize(size_type count) noexcept
    {
        if (count > N) {
            return false;
        }

        if (count > len) {
            for (size_type i = len; i < count; ++i) {
                a[i] = std::move(T{});
            }
        } else {
            if constexpr (std::is_destructible_v<T>) {
                for (size_type i = count; i < len; ++i) {
                    a[i].~T();
                }
            }
        }

        len = count;
        return true;
    }
    //! Resizes the `stack_vec` to contain `count` elements.
    //!
    //! If `count` is greater than `N`, this function is a no-op and returns
    //! `false`. Otherwise, it will return `true`.
    //!
    //! If the current `size()` is greater than `count`, the `stack_vec` is
    //! reduced to its first `count` elements.
    //!
    //! If the current `size()` is less than `count`, the uninitialized slots
    //! are filled with copies of `value`.
    constexpr bool resize(size_type count, const T& value) noexcept
    {
        if (count > N) {
            return false;
        }

        if (count > len) {
            for (size_type i = len; i < count; ++i) {
                a[i] = value;
            }
        } else {
            if constexpr (std::is_destructible_v<T>) {
                for (size_type i = count; i < len; ++i) {
                    a[i].~T();
                }
            }
        }

        len = count;
        return true;
    }

    constexpr void swap(stack_vec<T, N>& other) noexcept
    {
        std::swap(a, other.a);
        std::swap(len, other.len);
    }

    std::vector<T> as_vector() const noexcept(noexcept(std::vector<T>{}))
    {
        return {begin(), end()};
    }

private:
    std::array<T, N> a;
    size_type len;
};
} // namespace jrc
