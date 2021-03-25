set(OPT_FLAGS
    "-O3"
)

# END OPTIMIZATION FLAGS
# SOURCES : FIXME
# Put all your sources here EXCEPT YOUR MAIN.CC
set(SRC_ENGINE
    "src/batch_gcd.cc"
)

# Directories containing headers must be specified in the include_directories()
include_directories(
    "src/"
)

# Put your test files here
set(SRC_GTEST
    "tests/unit/test_main.cc"
    "tests/unit/product_tree.cc"
)

# END SOURCES


# SETUP
cmake_minimum_required(VERSION 3.12)
project(Cracker)

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -fsanitize=address")

# LIBRARIES


# CRACKER
add_executable(cracker)
target_sources(cracker PRIVATE "src/main.cc" ${SRC_ENGINE})
target_link_libraries(cracker PRIVATE ${LIBRARIES})


# TESTS
# Check if GTest is installed
find_package(GTest)

# If it is, create a dedicated target "tests"
if(GTest_FOUND)
  include(GoogleTest)

  add_executable(tests)
  target_sources(tests PRIVATE
    ${SRC_GTEST}
    ${SRC_ENGINE}
  )
  target_compile_options(tests PRIVATE -g -O0 -Wall -Werror -Wextra)
  target_link_libraries(tests PRIVATE GTest::GTest ${LIBRARIES})
endif()
