#pragma once

#include <iostream>

class kpdm {
public:
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
        iterator(int current, const printer& p) : p_device(p), current(current) {}
        int operator*() const {
            return current;
        }
        iterator& operator++() {
            ++current;
            p_device.update(current);
            return *this;
        }
        iterator operator++(int) {
            iterator old = *this;
            ++current;
            return old;
        }
        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
    private:
        const printer& p_device;
        int current;
    };

    explicit kpdm(int finish) : p_device(0, finish), start(0), finish(finish) {
        p_device.start();
    }
    kpdm(int start, int finish) : p_device(start, finish), start(start), finish(finish) {
        p_device.start();
    }
    ~kpdm() {
        p_device.finish();
    }

    iterator begin() const { return iterator(start, p_device); }
    iterator end() const { return iterator(finish, p_device); }

    static void SetStream(std::ostream& s) {
        stream = &s;
    }
private:
    static std::ostream* stream;

    printer p_device;
    int start;
    int finish;
};
