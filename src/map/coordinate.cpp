// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2019 The MMapper Authors
// Author: Ulf Hermann <ulfonk_mennhar@gmx.de> (Alve)
// Author: Marek Krejza <krejza@gmail.com> (Caligor)
// Author: Nils Schimmelmann <nschimme@gmail.com> (Jahara)

#include "coordinate.h"

#include <cmath>
#include <cstdlib>
#include <stdexcept>

Coordinate2i Coordinate2f::truncate() const
{
    return Coordinate2i{utils::round_ftoi(std::floor(x)), utils::round_ftoi(std::floor(y))};
}

Coordinate2f Coordinate2f::operator/(const float f) const
{
    if (utils::equals(f, 0.f)) {
        throw std::runtime_error("division by zero");
    }

    if (std::isnan(f)) {
        throw std::runtime_error("division by NaN");
    }

    return operator*(1.f / f);
}

bool Coordinate::operator==(const Coordinate &other) const
{
    return (other.x == x && other.y == y && other.z == z);
}

bool Coordinate::operator!=(const Coordinate &other) const
{
    return (other.x != x || other.y != y || other.z != z);
}

int Coordinate::distance(const Coordinate &other) const
{
    return std::abs(x - other.x) + std::abs(y - other.y) + std::abs(z - other.z);
}

void Coordinate::clear()
{
    x = 0;
    y = 0;
    z = 0;
}

void Coordinate::operator+=(const Coordinate &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
}

void Coordinate::operator-=(const Coordinate &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

Coordinate Coordinate::operator+(const Coordinate &other) const
{
    Coordinate ret = *this;
    ret += other;
    return ret;
}

Coordinate Coordinate::operator-(const Coordinate &other) const
{
    Coordinate ret = *this;
    ret -= other;
    return ret;
}

Coordinate Coordinate::operator*(const int scalar) const
{
    Coordinate ret = *this;
    ret.x *= scalar;
    ret.y *= scalar;
    ret.z *= scalar;
    return ret;
}

Coordinate Coordinate::operator/(const int scalar) const
{
    if (scalar == 0) {
        throw std::runtime_error("division by zero");
    }
    Coordinate ret = *this;
    ret.x /= scalar;
    ret.y /= scalar;
    ret.z /= scalar;
    return ret;
}

bool Coordinate::operator<(const Coordinate &rhs) const
{
    if (z < rhs.z) {
        return true;
    }
    if (rhs.z < z) {
        return false;
    }

    // REVISIT: use morton "z-ordering" (interleaved bits) for x/y axes?
    if (y < rhs.y) {
        return true;
    }
    if (rhs.y < y) {
        return false;
    }
    return x < rhs.x;
}
bool Coordinate::operator>(const Coordinate &rhs) const
{
    return rhs < *this;
}
bool Coordinate::operator<=(const Coordinate &rhs) const
{
    return !(rhs < *this);
}
bool Coordinate::operator>=(const Coordinate &rhs) const
{
    return !(*this < rhs);
}

// Technically we could init a, and then insert b.
Bounds::Bounds(const Coordinate &a, const Coordinate &b)
    : min{Coordinate::min(a, b)}
    , max{Coordinate::max(a, b)}
{}

void Bounds::insert(const Coordinate &c)
{
#define ONE(_type, _xyz) ((_type)._xyz) = std::_type(((_type)._xyz), (c._xyz))
#define THREE(_type) \
    ONE(_type, x); \
    ONE(_type, y); \
    ONE(_type, z)
    THREE(min);
    THREE(max);
#undef THREE
#undef ONE
}
