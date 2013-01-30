#########################################################################
#
# LibSylph Class Library (build script)
# Copyright (C) 2013 Frank "SeySayux" Erens <seysayux@gmail.com>
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
#     1. The origin of this software must not be misrepresented; you must not
#     claim that you wrote the original software. If you use this software
#     in a product, an acknowledgment in the product documentation would be
#     appreciated but is not required.
#
#     2. Altered source versions must be plainly marked as such, and must not be
#     misrepresented as being the original software.
#
#     3. This notice may not be removed or altered from any source
#     distribution.
#
#########################################################################

# Flags to enable C++11
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang
    set(CMAKE_CXX_FLAGS 
        "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++" )
    set(CMAKE_EXE_LINKER_FLAGS 
        "-std=c++11 -stdlib=libc++")
    set(CMAKE_SHARED_LINKER_FLAGS 
        "-std=c++11 -stdlib=libc++")
    set(CMAKE_MODULE_LINKER_FLAGS 
        "-std=c++11 -stdlib=libc++")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC >= 4.5.3
    # GCC is pretty schizophrenic when it comes to the name of the new C++
    # standard...
    if(CMAKE_CXX_COMPILER_VERSION STRGREATER "4.7./")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else(CMAKE_CXX_COMPILER_VERSION STRGREATER "4.5.2")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    endif()
endif()

