// This file was autogenerated using foonathan/compatibility.
// See https://github.com/foonathan/compatibility for further information.
// Do not edit manually!

#ifndef COMP_FOONATHAN_PMR_HPP_INCLUDED
#define COMP_FOONATHAN_PMR_HPP_INCLUDED

#include <cstddef>

#define FOONATHAN_HAS_PMR 0


#include "alignof.hpp"
#include "noexcept.hpp"
#include "max_align_t.hpp"

#include <cstddef>

#if FOONATHAN_HAS_PMR
    #include <experimental/memory_resource>
#endif

namespace foonathan_comp
{
#if FOONATHAN_HAS_PMR
    using std::experimental::pmr::memory_resource;
#else
    // see N3916 for documentation
    class memory_resource
    {
        static const std::size_t max_alignment = FOONATHAN_ALIGNOF(max_align_t);
    public:
        virtual ~memory_resource() FOONATHAN_NOEXCEPT {}

        void* allocate(std::size_t bytes, std::size_t alignment = max_alignment)
        {
            return do_allocate(bytes, alignment);
        }

        void deallocate(void* p, std::size_t bytes, std::size_t alignment = max_alignment)
        {
            do_deallocate(p, bytes, alignment);
        }

        bool is_equal(const memory_resource& other) const FOONATHAN_NOEXCEPT
        {
            return do_is_equal(other);
        }

    protected:
        virtual void* do_allocate(std::size_t bytes, std::size_t alignment) = 0;

        virtual void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) = 0;

        virtual bool do_is_equal(const memory_resource& other) const FOONATHAN_NOEXCEPT = 0;
    };

    inline bool operator==(const memory_resource& a, const memory_resource& b) FOONATHAN_NOEXCEPT
    {
        return &a == &b || a.is_equal(b);
    }

    inline bool operator!=(const memory_resource& a, const memory_resource& b) FOONATHAN_NOEXCEPT
    {
        return !(a == b);
    }
#endif
}

#endif
