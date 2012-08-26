/*
 * LibSylph Class Library
 * Copyright (C) 2012 Frank "SeySayux" Erens <seysayux@gmail.com>
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
 *  Created on: 12 sep. 2011
 */

#ifndef SYLPH_CORE_LIST_H_
#define SYLPH_CORE_LIST_H_

#include "Object.h"
#include "Iterator.h"
#include "Primitives.h"

SYLPH_BEGIN_NAMESPACE

template<class T>
class List {
public:
    struct ListNode {
        ListNode* prev;
        ListNode* next;
        T value;
    };

    template<class C, class V>
    class S_ITERATOR : public BidirectionalIterator<V, S_ITERATOR<C,V> > {
        typedef BidirectionalIterator<V, S_ITERATOR<C,V> > super;
    public:
        S_ITERATOR(bool begin = false, C* obj = null) : list(obj), super(begin),
                cur(begin? obj->first : obj->last){
        }

        template<class C1, class V1>
        S_ITERATOR(const S_ITERATOR<C1,V1>& other) : cur(other.cur) {}

        template<class C1, class V1>
        bool equals(const S_ITERATOR<C1,V1>& other) const {
            return cur == other.cur;
        }

        typename super::value_type& dereference() {
            return cur->value;
        }

        typename super::const_reference dereference() const {
            return cur->value;
        }

        void next() {
            cur = cur->next;
        }

        bool hasNext() const {
            return cur->next != null;
        }

        void previous() {
            cur = cur->prev;
        }

        bool hasPrevious() const {
            return cur->prev != null;
        }
    public:
        C* list;
        ListNode* cur;
    };

    S_ITERABLE(List,T)
    S_REVERSE_ITERABLE(List,T)

public:
    List() : mFirst(null), mLast(null) {}
    List(const List& other) : mFirst(null), mLast(null) {
        appendAll(other);
    }

    List(const std::initializer_list<T>& il) : mFirst(null), mLast(null) {
        sforeach(T& t, il) {
            append(t);
        }
    }

    List(List&& other) {
        mFirst = other.mFirst;
        mLast = other.mLast;
        other.mFirst = null;
        other.mLast = null;
    }

    virtual ~List() {
        if_nullptr(mFirst) return;
        for(idx_t i = 0; i < mSize; ++i) {
            ListNode* n = mFirst;
            mFirst = mFirst->next;
            delete n;
        }
    }

    size_t size() const {
        return mSize;
    }

    void append(const T& t) {
        if_nullptr(mFirst) {
            mFirst = new ListNode;
            mLast = mFirst;
        } else {
            mLast->next = new ListNode;
            mLast->next->prev = mLast;
            mLast = mLast->next;
        }
        mLast->value = t;
        ++mSize;
    }

    void appendAll(const List<T>& c) {
        for(const_iterator it = c.begin(); it != c.end(); ++it) {
            append(*it);
        }
    }

    void appendAllReverse(const List<T>& c) {
        for(const_reverse_iterator it = c.rbegin(); it != c.rend(); ++it) {
            append(*it);
        }
    }

    void prepend(const T& t) {
        if_nullptr(mFirst) {
            mFirst = new ListNode;
            mLast = mFirst;
        } else {
            mFirst->prev = new ListNode;
            mFirst->prev->next = mFirst;
            mFirst = mFirst->prev;
        }
        mFirst->value = t;
        ++mSize;
    }

    void prependAll(const List<T>& c) {
        for(const_reverse_iterator it = c.rbegin(); it != c.rend(); ++it) {
            prepend(*it);
        }
    }

    void prependAllReverse(const List<T>& c) {
        for(const_iterator it = c.begin(); it != c.end(); ++it) {
            prepend(*it);
        }
    }

    List<T> reverse() const {
        List<T> toReturn;
        for(const_iterator it = begin(); it != end(); ++it) {
            toReturn.prepend(*it);
        }
        return toReturn;
    }

    T& first() {
        return mFirst->value;
    }
    const T& first() const {
        return mFirst->value;
    }

    T& last() {
        return mLast->value;
    }

    const T& last() const {
        return mLast->value;
    }

    void insertAfter(idx_t i, const T& t) {
        if(i > size()) sthrow(IndexException,"List out of bounds!");
        ListNode* n = mFirst;
        for(idx_t j = 0; j < i; ++j) {
            n = n->next;
        }
        ListNode* newNode = new ListNode;
        newNode->value = t;
        newNode->prev = n;
        newNode->next = n->next;
        if(n->next) {
            n->next->prev = newNode;
        }
        n->next = newNode;
    }
    void insertBefore(idx_t i, const T& t) {
        if(i > size()) sthrow(IndexException,"List out of bounds!");
        ListNode* n = mFirst;
        for(idx_t j = 0; j < i; ++j) {
            n = n->next;
        }
        ListNode* newNode = new ListNode;
        newNode->value = t;
        newNode->next = n;
        newNode->prev = n->prev;
        if(n->prev) {
            n->prev->next = newNode;
        }
        n->prev = newNode;
    }

    // TODO implement List::insertAfter(iterator, const T&)
    void insertAfter(iterator i, const T& t) {
        (void)i;
        (void)t;
    }
    // TODO implement List::insertBefore(iterator, const T&)
    void insertBefore(iterator i, const T& t) {
        (void)i;
        (void)t;
    }

    // TODO implement List::remove(idx_t)
    void remove(idx_t i) {
        (void)i;
    }
    // TODO implement List::iterator(iterator)
    void remove(iterator i) {
        (void)i;
    }

    T& at(idx_t i) {
        if(i > size()) sthrow(IndexException,"List out of bounds!");
        ListNode* n = mFirst;
        for(idx_t j = 0; j < i; ++j) {
            n = n->next;
        }
        return n->value;
    }
    const T& at(idx_t i) const {
        if(i > size()) sthrow(IndexException,"List out of bounds!");
        ListNode* n = mFirst;
        for(idx_t j = 0; j < i; ++j) {
            n = n->next;
        }
    }

    sidx_t indexOf(const T& t, idx_t off = 0) const {
        if(off > size()) sthrow(IndexException, "List out of bounds!");
        idx_t i = 0;
        for(const_iterator it = begin(); it != end(); ++it) {
            while(off--) continue;
            if(*it == t) return i;
            ++i;
        }
        return -1;
    }

    sidx_t lastIndexOf(const T& t) const {
        return lastIndexOf(t, size() - 1);
    }

    sidx_t lastIndexOf(const T& t, idx_t off) const {
        if(off > size()) sthrow(IndexException, "List out of bounds!");
        idx_t i = size() - 1;
        for(const_reverse_iterator it = rbegin(); it != rend(); ++it) {
            while(off++ != size() - 1) continue;
            if(*it == t) return i;
            --i;
        }
        return -1;
    }

    bool contains(const T& t) const {
        return indexOf(t) != -1;
    }

private:
    std::size_t mSize;

    ListNode* mFirst;
    ListNode* mLast;
};

// TODO finish implementation of List
SYLPH_END_NAMESPACE

#endif /* SYLPH_CORE_LIST_H_ */

// vim: syntax=cpp11:ts=4:sts=4:sw=4:sta:et:tw=80:nobk:pa+=,../../src