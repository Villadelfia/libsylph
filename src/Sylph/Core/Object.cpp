/*
 * LibSylph Class Library
 * Copyright (C) 2013 Frank "SeySayux" Erens <seysayux@gmail.com>
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
 */

#include "Object.h"

#include <gc/gc.h>

#include <new>

#ifndef SYLPH_LIKELY
    #if __GNUC__ - 0 >= 3
        #define SYLPH_LIKELY(x) __builtin_expect(!!(x),1)
        #define SYLPH_UNLIKELY(x) __builtin_expect(!!(x),0)
    #else
        #define SYLPH_LIKELY(x) (x)
        #define SYLPH_UNLIKELY(x) (x)
    #endif
#endif

SYLPH_BEGIN_NAMESPACE

bool Object::gc_inited = false;

void* Object::operator new(size_t size) {
    if(SYLPH_UNLIKELY(!Object::gc_inited)) {
        GC_INIT();
        Object::gc_inited = true;
    }

    void* toReturn = GC_MALLOC_UNCOLLECTABLE(size);

    if (!toReturn) throw std::bad_alloc();
    else return toReturn;
}

void* Object::operator new(size_t size, GCPlacement gcp) {
    if(SYLPH_UNLIKELY(!Object::gc_inited)) {
        GC_INIT();
        Object::gc_inited = true;
    }

    void* toReturn;

    if (gcp == GC) toReturn = GC_MALLOC(size);
    else if (gcp == PointerFreeGC) toReturn = GC_MALLOC_ATOMIC(size);
    else toReturn = GC_MALLOC_UNCOLLECTABLE(size);

    if (!toReturn) throw std::bad_alloc();
    else return toReturn;
}

void* Object::operator new(size_t, void *p) {
    return p;
}

void Object::operator delete(void* obj) {
    GC_FREE(obj);
}

void Object::operator delete(void*, void*) { }

void Object::operator delete(void* p, GCPlacement) {
    GC_FREE(p);
}

void* Object::operator new[](size_t size) {
    return Object::operator new(size, NoGC);
}

void* Object::operator new[](size_t, void *p) {
    return p;
}

void Object::operator delete[](void* obj) {
    Object::operator delete(obj);
}

void Object::operator delete[](void*, void*) { }

Object::~Object() {
    GC_register_finalizer_ignore_self(GC_base(this), 0, 0, 0, 0);
}

void Object::cleanup(void* obj, void* displ) {
    ((Object*) ((char*) obj + (ptrdiff_t) displ))->~Object();
}

Object::Object() {
    GC_finalization_proc oldProc;
    void* oldData;
    void* base = GC_base((void*)this);

    if(0 != base) {
        GC_register_finalizer_ignore_self(
                base, (GC_finalization_proc)cleanup, 
                (void*) ((char*)this - (char*)base),
                &oldProc, &oldData);

        if(0 != oldProc) {
            GC_register_finalizer_ignore_self(base, oldProc, oldData, 0, 0);
        }
    }
}

namespace GCInternal {
    void* gc_malloc(size_t s) {
        return GC_MALLOC(s); 
    }

    void gc_register_finalizer(void* data, void(*f)(void*, void*)) {
        GC_finalization_proc oldProc;
        void* oldData;
        void* base = GC_base((void*)data);

        if (0 != base) {
            // Don't call the debug version, since this is a real base address.
            GC_register_finalizer_ignore_self(
                    base, (GC_finalization_proc)f, 
                    (void*) ((char*)data - (char*)base),
                    &oldProc, &oldData);

            if (0 != oldProc) {
                GC_register_finalizer_ignore_self(base, oldProc, oldData, 0, 0);
            }
        }
    }

    void gc_free(void* data) {
        GC_FREE(data);
    }
}

SYLPH_END_NAMESPACE

// vim: ts=4:sts=4:sw=4:sta:et:tw=80:nobk
