// This file was autogenerated using foonathan/compatibility.
// See https://github.com/foonathan/compatibility for further information.
// Do not edit manually!

#ifndef COMP_FOONATHAN_LITERAL_OP_HPP_INCLUDED
#define COMP_FOONATHAN_LITERAL_OP_HPP_INCLUDED

#include <cstddef>

#define FOONATHAN_HAS_LITERAL_OP 1


#if FOONATHAN_HAS_LITERAL_OP
    #if !defined(__cpp_user_defined_literals)
        #define __cpp_user_defined_literals 200809
    #elif 200809 > __cpp_user_defined_literals
        #undef __cpp_user_defined_literals
        #define __cpp_user_defined_literals 200809
    #elif defined(COMP_OVERRIDE_SD6)
        #undef __cpp_user_defined_literals
        #define __cpp_user_defined_literals 200809
    #endif
#endif



#endif
