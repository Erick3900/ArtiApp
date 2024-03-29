//
// Created by Alcachofa
//

#pragma once
#pragma once

#include <cmath>
#include <string>
#include <ostream>
#include <iostream>
#include <type_traits>

#include <SFML/System/Vector2.hpp>

#include <utils/utils.hpp>

#include <constants/math.hpp>

namespace arti::math {

    template <typename T>
    class vec2d {

    public:
        typedef T value_type;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef T &reference;
        typedef const T &const_reference;
        typedef std::size_t size_type;

        static_assert(std::is_arithmetic_v<value_type>);

        value_type x = value_type{};
        value_type y = value_type{};

        vec2d() : x(to<value_type>(0)),
                  y(to<value_type>(0)) {}

        vec2d(const_reference x, const_reference y)
                : x(x),
                  y(y) {}

        vec2d(const vec2d& other)
                : x(other.x),
                  y(other.y) {}

        template<typename U>
        vec2d(const vec2d<U>& other)
                : x(to<value_type>(other.x)),
                  y(to<value_type>(other.y)) {}

        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, value_type>>>
        vec2d(const sf::Vector2<U>& other)
                : x(to<value_type>(other.x)),
                  y(to<value_type>(other.y)) {}

        template <typename U, typename = std::enable_if_t<std::is_convertible_v<value_type, U>>>
        operator sf::Vector2<U>() const {
            return sf::Vector2<U>(to<U>(this->x), to<U>(this->y));
        }

        vec2d& operator=(const vec2d& other) {
            this->x = other.x;
            this->y = other.y;
            return *this;
        }

        vec2d& operator=(vec2d&& other) {
            this->x = std::move(other.x);
            this->y = std::move(other.y);
            return *this;
        }

        void swap(vec2d& other) {
            std::swap(this->x, other.x);
            std::swap(this->y, other.y);
        }

        template <typename RealType_ = real>
        RealType_ length() const {
            return std::sqrt(x * x + y * y);
        }

        vec2d normalize() const {
            real il = 1.0 / this->length();

            return vec2d(
                to<real>(x) * il,
                to<real>(y) * il
            );
        }

        vec2d perpendicular() const {
            return vec2d(-y, x);
        }

        template <typename T_ = value_type>
        std::enable_if_t<std::is_floating_point_v<T_>, vec2d>
        floor() const {
            static_assert(std::is_same_v<T_, value_type>);
            return vec2d(std::floor(x), std::floor(y));
        }

        template <typename T_ = value_type>
        std::enable_if_t<std::is_floating_point_v<T_>, vec2d>
        ceil() const {
            static_assert(std::is_same_v<T_, value_type>);
            return vec2d(std::ceil(x), std::ceil(y));
        }

        value_type dot(const vec2d& rhs) const {
            return x * rhs.x + y * rhs.y;
        }

        value_type cross(const vec2d& rhs) const {
            return x * rhs.y - y * rhs.x;
        }

        vec2d operator+(const vec2d& rhs) const {
            return vec2d(x + rhs.x, y + rhs.y);
        }

        vec2d operator-(const vec2d& rhs) const {
            return vec2d(x - rhs.x, y - rhs.y);
        }

        template <typename U>
        std::enable_if_t<std::is_arithmetic_v<U>, vec2d>
        operator+(const U &rhs) const {
            return vec2d(x + rhs, y + rhs);
        }

        template <typename U>
        std::enable_if_t<std::is_arithmetic_v<U>, vec2d>
        operator-(const U &rhs) const {
            return vec2d(x - rhs, y - rhs);
        }

        template <typename U>
        std::enable_if_t<std::is_arithmetic_v<U>, vec2d>
        operator*(const U &rhs) const {
            return vec2d(x * rhs, y * rhs);
        }

        template <typename U>
        std::enable_if_t<std::is_arithmetic_v<U>, vec2d>
        operator/(const U &rhs) const {
            return vec2d(x / rhs, y / rhs);
        }

        vec2d &operator+=(const vec2d &rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        vec2d &operator-=(const vec2d &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        template <typename U>
        std::enable_if_t<std::is_arithmetic_v<U>, vec2d&>
        operator+=(const U &rhs) {
            x += rhs;
            y += rhs;
            return *this;
        }

        template <typename U>
        std::enable_if_t<std::is_arithmetic_v<U>, vec2d&>
        operator-=(const U &rhs) {
            x -= rhs;
            y -= rhs;
            return *this;
        }

        template <typename U>
        std::enable_if_t<std::is_arithmetic_v<U>, vec2d&>
        operator*=(const U &rhs) {
            x *= rhs;
            y *= rhs;
            return *this;
        }
        template <typename U>
        std::enable_if_t<std::is_arithmetic_v<U>, vec2d&>
        operator/=(const U &rhs) {
            x /= rhs;
            y /= rhs;
            return *this;
        }

        vec2d operator+() const {
            return vec2d(+x, +y);
        }

        vec2d operator-() const {
            return vec2d(-x, -y);
        }

        template <typename U>
        bool operator==(const vec2d<U> &rhs) const {
            return (x == rhs.x && y == rhs.y);
        }

        bool operator==(const vec2d<float> &rhs) const {
            return (
                    std::abs(to<float>(x) - rhs.x) <= math::EPS &&
                    std::abs(to<float>(y) - rhs.y) <= math::EPS);
        }

        bool operator==(const vec2d<double> &rhs) const {
            return (
                    std::abs(to<double>(x) - rhs.x) <= math::EPS &&
                    std::abs(to<double>(y) - rhs.y) <= math::EPS);
        }

        template <typename U>
        bool operator!=(const vec2d<U> &rhs) const {
            return !(*this == rhs);
        }

        std::string to_string() const {
            using namespace std::string_literals;

            return "{ "s + std::to_string(x) + ", "s + std::to_string(y) + "}";
        }

        template <typename U, typename = std::enable_if<!std::is_same_v<U, T> && std::is_arithmetic_v<U>>>
#ifdef VEC_EXPLICIT_CONVERSIONS
        explicit
#endif
        operator vec2d<U>() const {
            return vec2d<U>(to<U>(x), to<U>(y));
        }
    };

    template <class T, class U>
    inline bool operator<(const vec2d<T> &lhs, const vec2d<U> &rhs) {
        return ((lhs.y <= rhs.y) && (lhs.x < rhs.x));
    }

    template <class T, class U>
    inline bool operator>(const vec2d<T> &lhs, const vec2d<U> &rhs) {
        return ((lhs.y >= rhs.y) && (lhs.x > rhs.x));
    }

    template <class T, class U>
    inline bool operator<=(const vec2d<T> &lhs, const vec2d<U> &rhs) {
        return ((lhs < rhs) || (lhs == rhs));
    }

    template <class T, class U>
    inline bool operator>=(const vec2d<T> &lhs, const vec2d<U> &rhs) {
        return ((lhs > rhs) || (lhs == rhs));
    }

    template <typename T, typename U, typename = std::enable_if<!std::is_same_v<T, U> && std::is_arithmetic_v<U>>>
    inline vec2d<T> operator*(const U &lhs, const vec2d<T> &rhs) {
        return vec2d<T>((T)(lhs * (U)rhs.x), (T)(lhs * (U)rhs.y));
    }

    template <typename T, typename U, typename = std::enable_if<!std::is_same_v<T, U> && std::is_arithmetic_v<U>>>
    inline vec2d<T> operator/(const U &lhs, const vec2d<T> &rhs) {
        return vec2d<T>((T)(lhs / (U)rhs.x), (T)(lhs / (U)rhs.y));
    }

    typedef vec2d<int64_t> vec2dl;
    typedef vec2d<uint64_t> vec2dul;

    typedef vec2d<int32_t> vec2di;
    typedef vec2d<uint32_t> vec2du;

    typedef vec2d<int16_t> vec2ds;
    typedef vec2d<uint16_t> vec2dus;

    typedef vec2d<int8_t> vec2db;
    typedef vec2d<uint8_t> vec2dub;

    typedef vec2d<double> vec2dd;
    typedef vec2d<float> vec2df;
}