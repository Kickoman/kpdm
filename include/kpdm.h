#pragma once

#include <iostream>
#include <iterator>

namespace Kpdm {

template<class T>
class krange {
public:
    explicit krange(const T finish)
        : start(0)
        , finish(finish)
        , step(1)
    {}

    krange(const T start, const T finish, const T step = 1)
        : start(start)
        , finish(finish)
        , step(step)
    {}

    class Iterator {
    public:
        explicit Iterator(const T value, const T step) : step(step), current(value) {}
        T operator*() const {
            return current;
        }
        Iterator& operator++() {
            current += step;
            return *this;
        }
        Iterator& operator++(int) {
            const auto old = *this;
            current += step;
            return old;
        }
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
        size_t operator-(const Iterator& other) const {
            return current - other.current;
        }
    private:
        const T step;
        T current;
    };
    using iterator = Iterator;

    iterator begin() const { return Iterator(start, step); }
    iterator end() const { return Iterator(finish, step); }

    size_t size() const { return (finish - start) / step; }

private:
    const T start;
    const T finish;
    const T step;
};

template<class T>
class kpdm_t {
public:
    using underlying_iterator_type = decltype(std::begin(std::declval<T&>()));
    using underlying_value_type = decltype(*std::begin(std::declval<T&>()));

    class printer {
    public:
        printer(int minValue, int maxValue) : minValue(minValue), maxValue((maxValue)) {}

        void start() const {
            update(minValue);
        }
        void update(const int value) const {
            const auto percent = calcPecents(value);
            const auto currentLength = calcLength(percent);
            std::cout << "\r[";
            for (size_t i = 0; i < length; ++i) {
                if (i < currentLength) {
                    std::cout << "=";
                } else if (i == currentLength) {
                    std::cout << ">";
                } else {
                    std::cout << " ";
                }
            }
            std::cout << "] " << int(percent * 100) << "% Complete" << std::flush;
        }
        void finish() const {
            update(maxValue);
            std::cout << std::endl;
        }
    private:
        double calcPecents(const int value) const {
            const auto totalRange = maxValue - minValue;
            const auto goneRange = value - minValue;
            return 1. * goneRange / totalRange;
        }
        size_t calcLength(const double percents) const {
            return static_cast<size_t>(length * percents);
        }

        const int minValue;
        const int maxValue;

        size_t length = 50;
    };

    class iterator {
    public:
        iterator(
            underlying_iterator_type underlyingIterator,
            const printer& printer
        )   : underlyingIterator(underlyingIterator)
            , p_device(printer)
        {}

        underlying_value_type operator*() const {
            return *underlyingIterator;
        }

        iterator& operator++() {
            ++iterations;
            ++underlyingIterator;
            p_device.update(iterations);
            return *this;
        }
        iterator operator++(int) {
            iterator old = *this;
            ++iterations;
            ++underlyingIterator;
            return old;
        }

        bool operator!=(const iterator& other) const {
            return underlyingIterator != other.underlyingIterator;
        }
    private:
        const printer& p_device;
        underlying_iterator_type underlyingIterator;
        unsigned long long iterations = 0;
    };

    explicit kpdm_t(T& iterable)
        : iterable(iterable)
        , p_device(0, std::size(iterable))
    {
        p_device.start();
    }

    explicit kpdm_t(T&& iterable)
        : localIterable(iterable)
        , iterable(localIterable)
        , p_device(0, std::size(iterable))
    {
        p_device.start();
    }

    ~kpdm_t() {
        p_device.finish();
    }

    iterator begin() const { return iterator(std::begin(iterable), p_device); }
    iterator end() const { return iterator(std::end(iterable), p_device); }

private:
    // dirty hack to store temp iterables
    T localIterable;
    T& iterable;
    printer p_device;
};

} // namespace Kpdm

template<class T>
auto kpdm(T&& iterable) {
    return Kpdm::kpdm_t(std::forward<T>(iterable));
}

template<class T>
auto kpdm(const T a, const T b) {
    return Kpdm::kpdm_t(Kpdm::krange(a, b));
}
