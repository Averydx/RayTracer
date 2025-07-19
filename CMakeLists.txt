cmake_minimum_required(VERSION 3.20)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
add_library(raytracer_core STATIC ${SOURCE_FILES})
target_include_directories(raytracer_core PUBLIC ${PROJECT_SOURCE_DIR}/include)

# # Exclude Catch2 tests or external libraries
# list(FILTER SOURCE_FILES EXCLUDE REGEX ".*Catch2/.*")

# Tests
file(GLOB_RECURSE TEST_FILES "tests/*.cpp")
add_executable(tests ${TEST_FILES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain raytracer_core)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Demo1
file(GLOB_RECURSE DEMO_FILES "demo1/*.cpp")
add_executable(demo1 ${DEMO_FILES})
target_link_libraries(demo1 PRIVATE raytracer_core)
target_include_directories(demo1 PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Demo2
file(GLOB_RECURSE DEMO_FILES "demo2/*.cpp")
add_executable(demo2 ${DEMO_FILES})
target_link_libraries(demo2 PRIVATE raytracer_core)
target_include_directories(demo2 PRIVATE ${PROJECT_SOURCE_DIR}/include)


