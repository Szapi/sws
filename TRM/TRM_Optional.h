/******************************************************************************
/ TRM_Optional.h
/
/ Copyright (c) 2023 
/
/
/ Permission is hereby granted, free of charge, to any person obtaining a copy
/ of this software and associated documentation files (the "Software"), to deal
/ in the Software without restriction, including without limitation the rights to
/ use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
/ of the Software, and to permit persons to whom the Software is furnished to
/ do so, subject to the following conditions:
/ 
/ The above copyright notice and this permission notice shall be included in all
/ copies or substantial portions of the Software.
/ 
/ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
/ EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
/ OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
/ NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
/ HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
/ WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/ FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
/ OTHER DEALINGS IN THE SOFTWARE.
/
******************************************************************************/
#pragma once

#include "TRM_TypeTraits.h"


namespace TRM {

template<class Type>
class Optional final {
    static_assert(is_destructible<Type>::value, "Type must be a destructible non-reference type.");

    char buf[sizeof(Type)] = {};
    bool hasValue          = false;

public:
    Optional() = default;
    ~Optional() { clear(); }

    bool has_value() const { return hasValue; }

    template<class... Args>
    void emplace(Args&&... args) {
        clear();
        new (buf) Type(forward<Args>(args)...);
        hasValue = true;
    }

    void clear() {
        if (has_value())
            (*this)->~Type();
        hasValue = false;
    }

    Type& operator*()  { return *reinterpret_cast<Type*>(buf); }
    Type* operator->() { return reinterpret_cast<Type*>(buf);  }
};

}   // namespace TRM