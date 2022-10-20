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
    idris2-compile ALL
    COMMAND $(CMAKE_SOURCE_DIR)/scripts/idris2_compile.sh)

ADD_CUSTOM_TARGET(
    idris2-docs ALL
    COMMAND $(CMAKE_SOURCE_DIR)/scripts/idris2_docs.sh)

ADD_CUSTOM_TARGET(
    idris2-install INSTALL
    COMMAND $(CMAKE_SOURCE_DIR)/scripts/idris2_install.sh
    DEPENDS idris2-compile idris2-docs)

#Build a pkg-config file
SET(RCPR_PC "${CMAKE_BINARY_DIR}/rcprhelper.pc")
FILE(WRITE  ${RCPR_PC} "Name: rcprhelper")
FILE(APPEND ${RCPR_PC} "\nDescription: Idris2 RCPR helper library")
FILE(APPEND ${RCPR_PC} "\nVersion: ${CMAKE_PROJECT_VERSION}")
FILE(APPEND ${RCPR_PC} "\nURL: https://github.com/nanolith/idris2-rcpr")
FILE(APPEND ${RCPR_PC} "\nprefix=${CMAKE_INSTALL_PREFIX}")
FILE(APPEND ${RCPR_PC} "\nlibdir=\${prefix}/lib")
FILE(APPEND ${RCPR_PC} "\nincludedir=\${prefix}/include")
FILE(APPEND ${RCPR_PC} "\nLibs: -L\${libdir} ")
FILE(APPEND ${RCPR_PC}     "-lrcprhelper-${CMAKE_PROJECT_VERSION}")
FILE(APPEND ${RCPR_PC} "\nCflags: -I\${includedir}")
INSTALL(FILES ${RCPR_PC} DESTINATION lib/pkgconfig)

#Install headers
FILE(GLOB RCPR_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include/rcprhelper/*.h")
INSTALL(FILES ${RCPR_INCLUDES} DESTINATION include/rcprhelper)
INSTALL(
    FILES ${CMAKE_BINARY_DIR}/include/rcprhelper/config.h
    DESTINATION include/rcprhelper)

#Install library
INSTALL(TARGETS rcprhelper-${CMAKE_PROJECT_VERSION}
        LIBRARY DESTINATION lib)
