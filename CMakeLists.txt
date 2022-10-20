cmake_policy(SET CMP0048 NEW)
PROJECT(idris2-rcpr VERSION 0.2.1)

string(REPLACE "." ";" IDRIS2_RCPR_VERSION_LIST "${CMAKE_PROJECT_VERSION}")
list(GET IDRIS2_RCPR_VERSION_LIST 0 IDRIS2_RCPR_VERSION_MAJOR)
list(GET IDRIS2_RCPR_VERSION_LIST 1 IDRIS2_RCPR_VERSION_MINOR)
list(GET IDRIS2_RCPR_VERSION_LIST 2 IDRIS2_RCPR_VERSION_REL)

cmake_minimum_required(VERSION 3.10)

INCLUDE(CheckSymbolExists)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

#minunit package
find_package(minunit REQUIRED)
find_package(rcpr REQUIRED)

#Build config.h
configure_file(config.h.cmake include/rcprhelper/config.h)

#includes
INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)

#source files
AUX_SOURCE_DIRECTORY(src/helper/psock IDRIS2_RCPR_PSOCK_SOURCES)
SET(IDRIS2_RCPR_SOURCES
    ${IDRIS2_RCPR_PSOCK_SOURCES})

#test source files
AUX_SOURCE_DIRECTORY(test/helper/psock IDRIS2_RCPR_TEST_PSOCK_SOURCES)
SET(IDRIS2_RCPR_TEST_SOURCES 
    ${IDRIS2_RCPR_TEST_PSOCK_SOURCES})

ADD_LIBRARY(rcprhelper-${CMAKE_PROJECT_VERSION} SHARED
    ${IDRIS2_RCPR_SOURCES})
TARGET_COMPILE_OPTIONS(
    rcprhelper-${CMAKE_PROJECT_VERSION} PRIVATE -fPIC -O2 ${RCPR_CFLAGS}
    -Wall -Werror -Wextra -Wpedantic -Wno-unused-command-line-argument)
TARGET_LINK_LIBRARIES(
    rcprhelper-${CMAKE_PROJECT_VERSION} PRIVATE ${RCPR_LDFLAGS})

ADD_EXECUTABLE(testidris2rcpr
    ${IDRIS2_RCPR_SOURCES} ${IDRIS2_RCPR_TEST_SOURCES})
TARGET_COMPILE_OPTIONS(
    testidris2rcpr PRIVATE -g -O0 --coverage ${MINUNIT_CFLAGS} ${RCPR_CFLAGS}
                     -Wall -Werror -Wextra -Wpedantic
                     -Wno-unused-command-line-argument)
TARGET_LINK_LIBRARIES(
    testidris2rcpr PRIVATE -g -O0 --coverage ${MINUNIT_LDFLAGS} ${RCPR_LDFLAGS})

set_source_files_properties(
    ${IDRIS2_RCPR_TEST_SOURCES} PROPERTIES
    COMPILE_FLAGS "${STD_CXX_20} ${USE_INTERN_ASSEMBLER}")

ADD_CUSTOM_TARGET(
    test
    COMMAND testidris2rcpr
    DEPENDS testidris2rcpr)

ADD_CUSTOM_TARGET(
    idris2-compile
    COMMAND $(CMAKE_SOURCE_DIR)/scripts/idris2_compile.sh)

ADD_CUSTOM_TARGET(
    idris2-docs
    COMMAND $(CMAKE_SOURCE_DIR)/scripts/idris2_docs.sh)

ADD_CUSTOM_TARGET(
    idris2-install
    COMMAND $(CMAKE_SOURCE_DIR)/scripts/idris2_install.sh
    DEPENDS idris2-compile idris2-docs)
