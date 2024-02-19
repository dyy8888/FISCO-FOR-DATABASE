# Install script for directory: /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/jsonrpccpp" TYPE FILE FILES "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/gen/jsonrpccpp/version.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/jsonrpccpp/common" TYPE FILE FILES "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/gen/jsonrpccpp/common/jsonparser.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/jsonrpccpp" TYPE FILE FILES
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/client.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/jsonrpccpp/common" TYPE FILE FILES
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/common/errors.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/common/exception.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/common/procedure.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/common/specification.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/common/specificationparser.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/common/specificationwriter.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/jsonrpccpp/client" TYPE FILE FILES
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/client/batchcall.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/client/batchresponse.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/client/client.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/client/iclientconnector.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/jsonrpccpp/server" TYPE FILE FILES
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/abstractserver.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/abstractserverconnector.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/iclientconnectionhandler.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/iprocedureinvokationhandler.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/requesthandlerfactory.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/jsonrpccpp/server/connectors" TYPE FILE FILES "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/connectors/httpserver.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/lib/libjsonrpccpp-common.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/lib/libjsonrpccpp-client.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/lib/libjsonrpccpp-server.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/libjsonrpccpp-server.pc"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/libjsonrpccpp-client.pc"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/libjsonrpccpp-common.pc"
    )
endif()

