cmake_minimum_required(VERSION 3.30)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")  # or -stdlib=libstdc++

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0  # Use the latest stable version
)
FetchContent_MakeAvailable(Catch2)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Find all .cpp files in the current directory and its subdirectories
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Exclude Catch2 tests or external libraries
list(FILTER SOURCE_FILES EXCLUDE REGEX ".*Catch2/.*")

add_executable(tests ${SOURCE_FILES})

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)