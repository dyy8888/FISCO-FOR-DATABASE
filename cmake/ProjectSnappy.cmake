include(ExternalProject)
include(GNUInstallDirs)

set(prefix "${CMAKE_CURRENT_SOURCE_DIR}/deps")
set(SNAPPY_INCLUDE_DIR ${prefix}/include/)
set(SNAPPY_LIBRARY ${prefix}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}snappy${CMAKE_STATIC_LIBRARY_SUFFIX})

ExternalProject_Add(snappy
    PREFIX ${prefix}
    DOWNLOAD_NAME snappy-1.1.7.tar.gz
    DOWNLOAD_NO_PROGRESS 1
    URL https://codeload.github.com/google/snappy/tar.gz/1.1.7
        https://osp-1257653870.cos.ap-guangzhou.myqcloud.com/FISCO-BCOS/FISCO-BCOS/deps/snappy-1.1.7.tar.gz
        https://raw.githubusercontent.com/FISCO-BCOS/LargeFiles/master/libs/snappy-1.1.7.tar.gz
    URL_HASH SHA256=3dfa02e873ff51a11ee02b9ca391807f0c8ea0529a4924afa645fbf97163f9d4
    CMAKE_COMMAND ${CMAKE_COMMAND}
    CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    -DCMAKE_INSTALL_LIBDIR=lib
    -DCMAKE_POSITION_INDEPENDENT_CODE=${BUILD_SHARED_LIBS}
    ${_only_release_configuration}
    -DSNAPPY_BUILD_TESTS=OFF
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    BUILD_IN_SOURCE 1
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
    # BUILD_COMMAND make
    ${_overwrite_install_command}
    BUILD_BYPRODUCTS "${SNAPPY_LIBRARY}"
)

ExternalProject_Get_Property(snappy INSTALL_DIR)
add_library(Snappy STATIC IMPORTED)

file(MAKE_DIRECTORY ${SNAPPY_INCLUDE_DIR})  # Must exist.

set_property(TARGET Snappy PROPERTY IMPORTED_LOCATION ${SNAPPY_LIBRARY})
set_property(TARGET Snappy PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${SNAPPY_INCLUDE_DIR})
add_dependencies(Snappy snappy)

unset(INSTALL_DIR)
