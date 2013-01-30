/*
 * LibSylph Class Library
 * Copyright (C) 2013 Frank "SeySayux" Erens <seysayux@gmail.com>
 * Copyright (C) 2010 Scott "ernieballsep" Philbrook <ernieballsep@gmail.com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software
 *   in a product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be
 *   misrepresented as being the original software.
 *
 *   3. This notice may not be removed or altered from any source
 *   distribution.
 *
 * Created on 6 december 2008, 12:07
 */

#ifndef SYLPH_CORE_VECTOR_H_
#define	SYLPH_CORE_VECTOR_H_

#include "Array.h"
#include "Util.h"
#include "Equals.h"
#include "Iterator.h"

#include "Debug.h"

#include <vector>
#include <initializer_list>

SYLPH_BEGIN_NAMESPACE


/**
 * @todo Write documentation!
 * @tplreqs T DefaultConstructible, CopyConstructible, Assignable
 */
template<class T>
class Vector : public Object {
public:

    template<class C, class V>
    class S_ITERATOR : public RandomAccessIterator<V, S_ITERATOR<C,V> > {
    public:
        typedef RandomAccessIterator<V, S_ITERATOR<C,V> > super;

        S_ITERATOR(bool begin = false, C* obj = null) : super(begin),
                _obj(obj) {
            _currentIndex = begin ? 0 : _obj->size() - 1;
        }

        template<class C1, class V1>
        bool equals(const S_ITERATOR<C1,V1>& other) const {
            return _currentIndex == other._currentIndex &&
                    _obj == other._obj;
        }

        template<class C1, class V1>
        S_ITERATOR(const S_ITERATOR<C1,V1>& other) {
            _currentIndex = other._currentIndex;
            _obj = other._obj;
        }

        typename super::value_type& current() {
            return (*_obj)[_currentIndex];
        }

        typename super::const_reference current() const {
            return (*_obj)[_currentIndex];
        }

        bool hasNext() const {
            return _currentIndex < _obj->size() - 1;
        }

        void next() {
            _currentIndex++;
        }

        bool hasPrevious() const {
            return _currentIndex > 0;
        }

        void previous() {
            _currentIndex--;
        }

        idx_t currentIndex() const {
            return _currentIndex;
        }

        size_t length() const {
            return _obj->size() - 1;
        }
    //private:
        idx_t _currentIndex;
        C* _obj;
    };

    S_ITERABLE(Vector<T>,T)
    S_REVERSE_ITERABLE(Vector<T>,T)


    /**
     * Creates an empty vector. By default, the initial capacity is 16, the
     * size is 0.
     * @param initialCount The initial capacity of the vector, 16 if none is
     * provided.
     */
    explicit Vector(size_t initialCount = 16) :
            elements((size_t)initialCount), _size(0) {
    }

    /**
     * Creates a copy of the vector. 
     *
     * All elements of the other vector will be copied into this vector.
     *
     * TODO implement this!
     *
     * @param other The other Vector.
     */
    Vector(const Vector<T>& other) {
        (void)other;
        SYLPH_STUB;
    }

    /**
     * Destroys this vector. If this Vector is holding pointers, this destructor
     * does not deallocate their memory.
     */
    virtual ~Vector() {
    }

    /**
     * Appends given element to the Vector. The element will be copied to the
     * end of the Vector. If the capacity is not sufficient, a new elements
     * array will be allocated with twice the size of the original array
     * and all existing data will be copied into the new array. The size
     * of this Vector will increase by 1.
     * @param t The element to append.
     * @complexity O(1)
     */
    void add(const T & t) {
        ensureCapacity(_size + 1);
        _size++;
        set(_size - 1, t);
    }

    /**
     * Appends all elements of given Vector to this Vector, in order they
     * have been inserted in the original Vector. This is the equivalent of
     * calling add() for each of the elements in the given Vector in the order
     * they have been inserted.
     * The size of this Vector will increase by the size of the given Vector.
     * @param c Another Vector whose elements to append to this one.
     * @complexity O(n)
     */
    void addAll(Vector<T> & c) {
        ensureCapacity(_size + c._size);
        for (Vector<T>::iterator it = c.begin();
                it != c.end(); ++it) {
            add(*it);
        }
    }

    /**
     * Returns the current capacity of this Vector. The capacity represents
     * the maximum size this Vector can have without needing to expand and
     * copy all elements to a new, bigger array. Therefore, the capacity must
     * always be greater than or equal to the size of this Vector.
     * @return The current capacity of this Vector.
     * @complexity O(0)
     */
    size_t capacity() const {
        return elements.length;
    }

    /**
     * Removes all elements from this vector. This will clear the underlying
     * array of the vector and reduces the sice effectively to 0.
     * @complexity O(n)
     */
    void clear() {
        _size = 0;
        elements.clear();
    }

    /**
     * @param An object @c t
     * @returns if this Vector has an element equal to @c t
     * @complexity O(n)
     */
    bool contains(const T & t) const {
        return indexOf(t) != -1;
    }

    /**
     * @complexity O(1)
     */
    bool empty() const {
        return _size == 0;
    }

    /**
     * @complexity O(1)
     */
    const T & get(size_t idx) const throw(ArrayException) {
        try {
            checkIfOutOfBounds(idx);
            return elements[idx];
        }
        straced;
    }

    /**
     * @complexity O(1)
     */
    T & get(size_t idx) throw(ArrayException) {
        try {
            checkIfOutOfBounds(idx);
            return elements[idx];
        }
        straced;
    }

    /**
     * @complexity O(n)
     */
    sidx_t indexOf(const T & t, idx_t idx = 0) const throw(ArrayException) {
        try {
            checkIfOutOfBounds(idx);
        } straced;
        static Equals<T> equf;
        for (size_t i = idx; i < _size; i++) {
            if (equf(get(i), t)) {
                return i;
            }
        }
        return -1;
    }

    /**
     * @complexity O(n)
     */
    sidx_t lastIndexOf(const T & t) const {
        return lastIndexOf(t, _size - 1);
    }

    /**
     * @complexity O(n)
     */
    sidx_t lastIndexOf(const T & t, size_t idx) const 
            throw(ArrayException) {
        static Equals<T> equf;
        try {
            checkIfOutOfBounds(idx);
        } straced;
        for (size_t i = idx; (signed)i >= 0; --i) {
            if (equf(get(i), t)) {
                return i;
            }
        }
        return -1;
    }

    /**
     * @complexity O(n)
     */
    void remove(const T & t) {
        removeAt(indexOf(t));
    }

    /**
     * @complexity O(n)
     */
    void removeAt(size_t idx) throw(ArrayException) {
        try {
            checkIfOutOfBounds(idx);
        } straced;
        _size--;
        if (idx < (elements.length - 1))
            arraycopy(elements, idx + 1, elements, idx,
                elements.length - 1 - idx);
    }

    /**
     * @complexity O(1)
     */
    void set(size_t idx, const T & t) throw(ArrayException) {
        try {
            checkIfOutOfBounds(idx);
            elements[idx] = t;
        }
        straced;
    }

    /**
     * @complexity O(0)
     */
    size_t size() const {
        return _size;
    }

    /**
     * @complexity O(n)
     */
    Array<T> toArray() const {
        Array<T> toReturn(size());
        for(idx_t i = 0; i < size(); ++i) {
            toReturn[i] = elements[i];
        }
        return toReturn;
    }

    /**
     * @complexity O(n)
     */
    bool operator==(const Vector<T> & c) const {
        if (_size != c.size()) return false;
        else {
            for (size_t x = 0; x < c.size(); x++) {
                if (get(x) != c.get(x)) return false;
            }
            return true;
        }
    }

    /**
     * @complexity O(1)
     */
    T & operator[](idx_t idx) throw(ArrayException) {
        try {
            return get(idx);
        } straced;
    }

    /**
     * @complexity O(1)
     */
    const T & operator[](idx_t idx) const throw(ArrayException) {
        try {
            return get(idx);
        } straced;
    }

    /**
     * @complexity O(n)
     */
    Vector& operator=(const Vector<T> & rhs) {
        elements = Array<T > (rhs.elements.length);
        _size = rhs.size();
        arraycopy(rhs.elements, 0, elements, 0, rhs.elements.length);
        return *this;
    }

private:
    Array<T> elements;
    size_t _size;

    void ensureCapacity(size_t capacity) {
        if (capacity > elements.length) {
            size_t newsize;
            newsize = elements.length << 1;
            Array<T> oldElements = elements;
            elements = Array<T > (newsize);
            arraycopy(oldElements, 0, elements, 0, oldElements.length);
        }
    }

    inline void checkIfOutOfBounds(size_t idx) const 
            throw(ArrayException) {
        if (idx >= _size) sthrow(ArrayException, "Vector out of bounds");
    }

};



SYLPH_END_NAMESPACE
#endif	/* SYLPH_CORE_VECTOR_H_ */

// vim: ts=4:sts=4:sw=4:sta:et:tw=80:nobk
