cmake_minimum_required(VERSION 3.1...3.18)

if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}})
endif()


project(calc VERSION 0.1.0
             DESCRIPTION "A Simple Calculator"
             LANGUAGES CXX)

# set your cmake variables

set(CMAKE_CXX_STANDARD_17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/libs/fruit/cmake-modules ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_BUILD_TYPE Release)
set(FRUIT_TESTS_USE_PRECOMPILED_HEADERS OFF)
set(SRCS
${PROJECT_SOURCE_DIR}/srcs/calcdfafactory.cc
${PROJECT_SOURCE_DIR}/srcs/dfa.cc
${PROJECT_SOURCE_DIR}/srcs/token.cc
${PROJECT_SOURCE_DIR}/srcs/tokenimpl.cc
${PROJECT_SOURCE_DIR}/srcs/visitor.cc
${PROJECT_SOURCE_DIR}/srcs/dfabuilder.cc
${PROJECT_SOURCE_DIR}/srcs/parser.cc
${PROJECT_SOURCE_DIR}/srcs/tokengen.cc
${PROJECT_SOURCE_DIR}/srcs/validator.cc
)
set(ALL_CXX_SOURCE_FILES ${SRCS} ${PROJECT_SOURCE_DIR}/srcs/main.cc)
set(LIB_HEADER_DIR "") # placeholder for now
# Find packages go here
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()


if(NOT EXISTS "${PROJECT_SOURCE_DIR}/libs/fruit/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()





add_subdirectory("${PROJECT_SOURCE_DIR}/libs/fruit")


set(FRUIT_HEADER_DIR "${PROJECT_SOURCE_DIR}/libs/fruit/include/" "${Fruit_BINARY_DIR}/include/")
set(INCLUDE_HEADER_DIR)

# Only Available in CMake3.12+
if (${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.12)
    set(INCLUDE_HEADER_DIR ${FRUIT_HEADER_DIR})
    list(TRANSFORM INCLUDE_HEADER_DIR PREPEND -I)
else()
    foreach(DIR in FRUIT_HEADER_DIR)
        list(APPEND INCLUDE_HEADER_DIR -I${DIR})
    endforeach(DIR in FRUIT_HEADER_DIR)
endif()

# Adding clang-tidy target if executable is found
find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
  add_custom_target(
    tidy
    COMMAND ${CLANG_TIDY}
    ${ALL_CXX_SOURCE_FILES}
    --header-filter=.*
    --
    --std=c++17
    ${INCLUDE_HEADER_DIR}
    )
endif()

add_executable(calc ${PROJECT_SOURCE_DIR}/srcs/main.cc ${SRCS})
add_executable(testToken ${PROJECT_SOURCE_DIR}/tests/testToken.cc ${SRCS})
add_executable(testParser ${PROJECT_SOURCE_DIR}/tests/testParser.cc ${SRCS})
set_target_properties(calc testToken testParser PROPERTIES CXX_STANDARD 17)
target_include_directories(calc PRIVATE "${FRUIT_HEADER_DIR}" PUBLIC "${LIB_HEADER_DIR}")
target_include_directories(testToken PRIVATE "${FRUIT_HEADER_DIR}" PUBLIC "${LIB_HEADER_DIR}")
target_include_directories(testParser PRIVATE "${FRUIT_HEADER_DIR}" PUBLIC "${LIB_HEADER_DIR}")
target_link_libraries(calc PRIVATE fruit)
target_link_libraries(testToken PRIVATE fruit)
target_link_libraries(testParser PRIVATE fruit)


