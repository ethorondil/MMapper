#pragma once
// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2019 The MMapper Authors
// Author: Nils Schimmelmann <nschimme@gmail.com> (Jahara)

#include "Array.h"
#include "Flags.h"
#include "macros.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>

template<typename T, typename E, size_t SIZE_ = enums::CountOf<E>::value>
class NODISCARD EnumIndexedArray : private MMapper::Array<T, SIZE_>
{
public:
    using index_type = E;
    using base = typename MMapper::Array<T, SIZE_>;
    static constexpr const size_t SIZE = SIZE_;

public:
    // inherits all constructors
    using base::base;

public:
    NODISCARD decltype(auto) at(E e) { return base::at(static_cast<uint32_t>(e)); }
    NODISCARD decltype(auto) at(E e) const { return base::at(static_cast<uint32_t>(e)); }
    NODISCARD decltype(auto) operator[](E e) { return at(e); }
    NODISCARD decltype(auto) operator[](E e) const { return at(e); }

public:
    using base::data;
    using base::size;

public:
    using base::begin;
    using base::cbegin;
    using base::cend;
    using base::end;

private:
    template<typename U>
    NODISCARD static bool my_equals(const U &a, const U &b)
    {
        return a == b;
    }
    template<typename U>
    NODISCARD static bool my_equals(U *const a, const std::unique_ptr<U> &b)
    {
        return a == b.get();
    }
    template<typename U>
    NODISCARD static bool my_equals(const std::unique_ptr<U> &a, U *const b)
    {
        return a.get() == b;
    }

public:
    template<typename U>
    NODISCARD std::optional<E> findIndexOf(const U element) const
    {
        const auto beg = this->begin();
        const auto end = this->end();
        const auto it = std::find_if(beg, end, [element](auto &x) -> bool {
            return my_equals(x, element);
        });
        if (it == end) {
            return std::nullopt;
        }

        return static_cast<E>(it - beg);
    }

    template<typename Callback>
    void for_each(Callback &&callback)
    {
        for (auto &x : *this) {
            callback(x);
        }
    }

public:
    NODISCARD bool operator==(const EnumIndexedArray &other) const
    {
        return static_cast<const base &>(*this) == static_cast<const base &>(other);
    }
    NODISCARD bool operator!=(const EnumIndexedArray &other) const { return !operator==(other); }
};
