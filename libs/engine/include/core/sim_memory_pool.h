
/*-
 * Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

 /*
 License
 This code is distributed under the MIT License, which is reproduced below and at the top of the project files.
 This pretty much means you can do whatever you want with the code, but I will not be liable for ANY kind of damage
 that this code might cause.Here is the full license which you should read before using the code :

 Copyright(c) 2013 Cosku Acay, http ://www.coskuacay.com

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 documentation files(the "Software"), to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software,
 and to permit persons to whom the Software is furnished to do so, subject to the following conditions :

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef SIM_MEMORY_POOL_H
#define SIM_MEMORY_POOL_H

#include <climits>
#include <cstddef>
#include <type_traits>

namespace sim
{
namespace stl
{
// ----------------------------------------------------------------------//
template <typename T, size_t BlockSize = 4096>
class CMemoryPool
{
  public:
	  // ----------------------------------------------------------------------//
    /* Member types */
    typedef T               value_type;
    typedef T*              pointer;
    typedef T&              reference;
    typedef const T*        const_pointer;
    typedef const T&        const_reference;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;
    typedef std::false_type propagate_on_container_copy_assignment;
    typedef std::true_type  propagate_on_container_move_assignment;
    typedef std::true_type  propagate_on_container_swap;

    template <typename U> struct rebind {
      typedef CMemoryPool<U> other;
    };

	// ----------------------------------------------------------------------//

    /* Member functions */
    CMemoryPool();
    CMemoryPool(const CMemoryPool& memoryPool);
    CMemoryPool(CMemoryPool&& memoryPool);
    template <class U> CMemoryPool(const CMemoryPool<U>& memoryPool);

    ~CMemoryPool();

    CMemoryPool& operator=(const CMemoryPool& memoryPool) = delete;
    CMemoryPool& operator=(CMemoryPool&& memoryPool);

    pointer address(reference x) const;
    const_pointer address(const_reference x) const;

    // Can only allocate one object at a time. n and hint are ignored
    pointer allocate(size_type n = 1, const_pointer hint = 0);
    void deallocate(pointer p, size_type n = 1);

    size_type max_size() const;

    template <class U, class... Args> void construct(U* p, Args&&... args);
    template <class U> void destroy(U* p);

    template <class... Args> pointer New(Args&&... args);
    void Delete(pointer p);

	// ----------------------------------------------------------------------//

  private:
    union Slot_ {
      value_type element;
      Slot_* next;
    };

    typedef char* data_pointer_;
    typedef Slot_ slot_type_;
    typedef Slot_* slot_pointer_;

    slot_pointer_ currentBlock_;
    slot_pointer_ currentSlot_;
    slot_pointer_ lastSlot_;
    slot_pointer_ freeSlots_;

    size_type padPointer(data_pointer_ p, size_type align) const;
    void allocateBlock();

    static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize too small.");

	// ----------------------------------------------------------------------//
};

// ----------------------------------------------------------------------//
template <typename T, size_t BlockSize>
inline typename CMemoryPool<T, BlockSize>::size_type
CMemoryPool<T, BlockSize>::padPointer(data_pointer_ p, size_type align)
const
{
	uintptr_t result = reinterpret_cast<uintptr_t>(p);
	return ((align - result) % align);
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
CMemoryPool<T, BlockSize>::CMemoryPool()

{
	currentBlock_ = nullptr;
	currentSlot_ = nullptr;
	lastSlot_ = nullptr;
	freeSlots_ = nullptr;
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
CMemoryPool<T, BlockSize>::CMemoryPool(const CMemoryPool& memoryPool)
 :
CMemoryPool()
{}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
CMemoryPool<T, BlockSize>::CMemoryPool(CMemoryPool&& memoryPool)

{
	currentBlock_ = memoryPool.currentBlock_;
	memoryPool.currentBlock_ = nullptr;
	currentSlot_ = memoryPool.currentSlot_;
	lastSlot_ = memoryPool.lastSlot_;
	freeSlots_ = memoryPool.freeSlots;
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
template<class U>
CMemoryPool<T, BlockSize>::CMemoryPool(const CMemoryPool<U>& memoryPool)
 :
CMemoryPool()
{}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
CMemoryPool<T, BlockSize>&
CMemoryPool<T, BlockSize>::operator=(CMemoryPool&& memoryPool)
{
	if (this != &memoryPool)
	{
		std::swap(currentBlock_, memoryPool.currentBlock_);
		currentSlot_ = memoryPool.currentSlot_;
		lastSlot_ = memoryPool.lastSlot_;
		freeSlots_ = memoryPool.freeSlots;
	}
	return *this;
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
CMemoryPool<T, BlockSize>::~CMemoryPool()

{
	slot_pointer_ curr = currentBlock_;
	while (curr != nullptr) {
		slot_pointer_ prev = curr->next;
		operator delete(reinterpret_cast<void*>(curr));
		curr = prev;
	}
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
inline typename CMemoryPool<T, BlockSize>::pointer
CMemoryPool<T, BlockSize>::address(reference x)
const
{
	return &x;
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
inline typename CMemoryPool<T, BlockSize>::const_pointer
CMemoryPool<T, BlockSize>::address(const_reference x)
const
{
	return &x;
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
void
CMemoryPool<T, BlockSize>::allocateBlock()
{
	// Allocate space for the new block and store a pointer to the previous one
	data_pointer_ newBlock = reinterpret_cast<data_pointer_>
		(operator new(BlockSize));
	reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
	currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);
	// Pad block body to staisfy the alignment requirements for elements
	data_pointer_ body = newBlock + sizeof(slot_pointer_);
	size_type bodyPadding = padPointer(body, alignof(slot_type_));
	currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
	lastSlot_ = reinterpret_cast<slot_pointer_>
		(newBlock + BlockSize - sizeof(slot_type_) + 1);
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
inline typename CMemoryPool<T, BlockSize>::pointer
CMemoryPool<T, BlockSize>::allocate(size_type n, const_pointer hint)
{
	if (freeSlots_ != nullptr) {
		pointer result = reinterpret_cast<pointer>(freeSlots_);
		freeSlots_ = freeSlots_->next;
		return result;
	}
	else {
		if (currentSlot_ >= lastSlot_)
			allocateBlock();
		return reinterpret_cast<pointer>(currentSlot_++);
	}
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
inline void
CMemoryPool<T, BlockSize>::deallocate(pointer p, size_type n)
{
	if (p != nullptr) {
		reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
		freeSlots_ = reinterpret_cast<slot_pointer_>(p);
	}
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
inline typename CMemoryPool<T, BlockSize>::size_type
CMemoryPool<T, BlockSize>::max_size()
const
{
	size_type maxBlocks = -1 / BlockSize;
	return (BlockSize - sizeof(data_pointer_)) / sizeof(slot_type_) * maxBlocks;
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
template <class U, class... Args>
inline void
CMemoryPool<T, BlockSize>::construct(U* p, Args&&... args)
{
	new (p) U(std::forward<Args>(args)...);
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
template <class U>
inline void
CMemoryPool<T, BlockSize>::destroy(U* p)
{
	p->~U();
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
template <class... Args>
inline typename CMemoryPool<T, BlockSize>::pointer
CMemoryPool<T, BlockSize>::New(Args&&... args)
{
	pointer result = allocate();
	construct<value_type>(result, std::forward<Args>(args)...);
	return result;
}

// ----------------------------------------------------------------------//

template <typename T, size_t BlockSize>
inline void
CMemoryPool<T, BlockSize>::Delete(pointer p)
{
	if (p != nullptr) {
		p->~value_type();
		deallocate(p);
	}
}

// ----------------------------------------------------------------------//

// ----------------------------------------------------------------------//
} // namespace stl;
} // namespace sim


#endif // SIM_MEMORY_POOL_H
