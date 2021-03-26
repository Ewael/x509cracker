set(OPT_FLAGS
    "-march=native -flto"
)

# END OPTIMIZATION FLAGS
# SOURCES : FIXME
# Put all your sources here EXCEPT YOUR MAIN.CC
set(SRC_ENGINE
    "src/batch_gcd.cc"
    "src/database.cc"
)

# Directories containing headers must be specified in the include_directories()
include_directories(
    "src/"
)

# Put your test files here
set(SRC_GTEST
    "tests/unit/gcds.cc"
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

if(APPLE)
   add_definitions("-I/usr/local/include -I/opt/local/include")
endif()
if(MSVC)
  find_package(GMPXX REQUIRED)
  set(GMP_LIB "${GMPXX_LIBRARIES}")
  set(GMPXX_LIB "${GMPXX_LIBRARIES}")
  include_directories("${VCPKG_INCLUDE_DIRS}")
else()
  find_library(GMP_LIB gmp REQUIRED)
  find_library(GMPXX_LIB gmpxx REQUIRED)
  message(STATUS "Found gmp: ${GMP_LIB}")
  message(STATUS "Found gmpxx: ${GMPXX_LIB}")
endif()

find_package (SQLite3)
if (SQLITE3_FOUND)
  message(STATUS "Found sqlite3: ${SQLITE3_LIBRARIES}")
else()
  message(STATUS "Sqlite3 not found")
endif (SQLITE3_FOUND)

set(LIBRARIES ${GMPXX_LIB} ${GMP_LIB} sqlite3)

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
