/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   ENS_String.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _ENS_STRING_H_
#define _ENS_STRING_H_

/// @defgroup ens_string_class ENS String
/// @ingroup ens_utility_classes
/// utility class for handling strings in ENS
/// @{

/// Concrete class for a limited string (characters string, NULL-terminated)
/// characters are statically allocated through template definition
template<int size>
class ENS_String {
public:

    ENS_String() { mData[0] = 0; }
    ENS_String(const ENS_String &string) { ENS_String::copy(string); }
    ENS_String(const char *pData) { ENS_String::copy(pData); }

    ~ENS_String() {}

    void clear() { mData[0] = 0; }

    int length() const;

    inline char getcharacter(int i) const {
        return mData[i];
    }

    int put(char *pData) const;

    ENS_String&copy(const ENS_String &string);
    ENS_String&copy(const char *pData);

    ENS_String&concat(const ENS_String &string);
    ENS_String&concat(const char *pData);

    int compare(const ENS_String &string) const;
    int compare(const char *pData) const;

    inline char operator[](int i) const {
        return mData[i];
    }

    inline operator const char *() const {
        return mData;
    }
    
    inline const char * c_str() const {
        return mData;
    }

    ENS_String &operator=(const ENS_String &string);
    ENS_String &operator=(const char *pData);

    ENS_String &operator+(const ENS_String &string);
    ENS_String &operator+(const char *pData);

    int operator==(const ENS_String &string) const;
    int operator==(const char *pData) const;
private:

    char mData[size + 1];
};

template<int size>
int ENS_String<size>::length() const {
    int         length = 0;
    const char *p      = mData;

    while (*p++) {
        length++;
    }

    return length;
}

template<int size>
int ENS_String<size>::put(char *pData) const {
    const char *p  = mData;
    char *      pc = pData;

    while (*p) {
        *pc++ = *p++;
    }

    *pc = 0;

    return p - mData;
}

template<int size>
ENS_String<size> &ENS_String<size>::copy(const ENS_String<size> &string) {
    if (this != &string) {
        char *      p   = mData;
        char *      end = mData + size;
        const char *pc  = (const char *)string;

        while (*pc && p < end) {
            *p++ = *pc++;
        }

        if (p <= end) {
            *p = 0;
        }
    }

    return *this;
}

template<int size>
ENS_String<size> &ENS_String<size>::copy(const char *pData) {
    char *      p   = mData;
    char *      end = mData + size;
    const char *pc  = pData;

    while (*pc && p < end) {
        *p++ = *pc++;
    }

    if (p <= end) {
        *p = 0;
    }

    return *this;
}

template<int size>
ENS_String<size> &ENS_String<size>::concat(const ENS_String<size> &string) {
    char *      p   = mData;
    char *      end = mData + size;
    const char *pc  = (const char *)string;

    while (*p) {
        p++;
    }

    while (*pc && p < end) {
        *p++ = *pc++;
    }

    if (p <= end) {
        *p = 0;
    }

    return *this;
}

template<int size>
ENS_String<size> &ENS_String<size>::concat(const char *pData) {
    char *      p   = mData;
    char *      end = mData + size;
    const char *pc  = pData;

    while (*p) {
        p++;
    }

    while (*pc && p < end) {
        *p++ = *pc++;
    }

    if (p <= end) {
        *p = 0;
    }

    return *this;
}

template<int size>
int ENS_String<size>::compare(const ENS_String<size> &string) const {
    int length1 = ENS_String::length();
    int length2 = string.length();
    int length  = (length1 <= length2 ? length1 : length2);
    int cmp;

    for (int i = 0; i < length; i++) {
        cmp = mData[i] - string.getcharacter(i);
        if (cmp) {
            return cmp;
        }
    }

    return length1 - length2;
}

template<int size>
int ENS_String<size>::compare(const char *pData) const {
    const char *p       = pData;
    int         length1 = ENS_String::length();
    int         length2 = 0;
    int         length;
    int         cmp;

    while (*p++) {
        length2++;
    }

    length = (length1 <= length2 ? length1 : length2);

    for (int i = 0; i < length; i++) {
        cmp = mData[i] - pData[i];
        if (cmp) {
            return cmp;
        }
    }

    return length1 - length2;
}

template<int size>
ENS_String<size> &ENS_String<size>::operator=(const ENS_String<size> &string) {
    return ENS_String::copy(string);
}

template<int size>
ENS_String<size> &ENS_String<size>::operator=(const char *pData) {
    return ENS_String::copy(pData);
}

template<int size>
ENS_String<size> &ENS_String<size>::operator+(const ENS_String<size> &string) {
    return ENS_String::concat(string);
}

template<int size>
ENS_String<size> &ENS_String<size>::operator+(const char *pData) {
    return ENS_String::concat(pData);
}

template<int size>
int ENS_String<size>::operator==(const ENS_String<size> &string) const {
    return !ENS_String::compare(string);
}

template<int size>
int ENS_String<size>::operator==(const char *pData) const {
    return !ENS_String::compare(pData);
}

/// @}

#endif // _ENS_STRING_H_
