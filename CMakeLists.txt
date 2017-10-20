cmake_minimum_required (VERSION 3.1)
project (vulkan_test)

# require c++17 since it is finished
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# find all source and header files
file(GLOB_RECURSE SOURCE_FILES "src/*.hpp" "src/*.cpp")

# organize the files according to the organisation in the file system
# from: http://cmake.3232098.n2.nabble.com/Keep-folder-structure-in-Visual-Studio-Project-generated-with-CMake-td7586044.html
macro(GroupSources curdir)
   file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
   foreach(child ${children})
          if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
                  GroupSources(${curdir}/${child})
          else()
                  string(REPLACE "/" "\\" groupname ${curdir})
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
          endif()
   endforeach()
endmacro()

# execute tha macro
GroupSources(src)

# faster compilation with multithreading
# cmake does not support vs compiler switches for std
if(MSVC)
	add_compile_options( /MP )
	add_compile_options( /std:c++17 )
	
	set(OPTIMIZATION_OPTIONS 
    $<$<CONFIG:Debug>:>
    $<$<CONFIG:RelWithDebInfo>:>
    $<$<CONFIG:Release>: /O2 /Ob2 /Oi /Ot /GL > 
    $<$<CONFIG:MinSizeRel>:>
    )
	
	add_compile_options( "${OPTIMIZATION_OPTIONS}" )
endif(MSVC)

# needs to be known to link with the libraries
add_executable(vulkan_test ${SOURCE_FILES})
# navigation should always start from root
include_directories("src")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory ("dependencies/glfw")
include_directories("dependencies/glfw/include")
target_link_libraries (vulkan_test glfw)

#glm is header only
include_directories("dependencies/glm")

find_package(Vulkan REQUIRED)
target_link_libraries(vulkan_test ${Vulkan_LIBRARY})
include_directories(${Vulkan_INCLUDE_DIR})