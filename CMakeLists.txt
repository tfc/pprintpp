cmake_minimum_required(VERSION 3.12)
project(pprintpp CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)

option(PPRINTPP_BUILD_TESTING "Enable building tests." OFF)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(PPRINTPP_BUILD_TESTING ON)
endif()

if(PPRINTPP_BUILD_TESTING)
  enable_testing()

  add_executable(${PROJECT_NAME}-test test/main.cpp)
  add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-test)
  target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
  target_compile_options(${PROJECT_NAME}-test PRIVATE -Wall -Wextra -Werror)
endif()

set(CPACK_PACKAGE_NAME "pprintpp")
set(CPACK_PACKAGE_VENDOR "galowicz.de")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Typesafe Python Style Printf Formatting for C++")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "pprintpp")
set(CPACK_PACKAGE_CONTACT "Jacek Galowicz <jacek@galowicz.de>")

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_SET_DESTDIR true)

set(PKG_CONFIG_REQUIRES)
set(PKG_CONFIG_LIBDIR "\${prefix}/lib")
set(PKG_CONFIG_INCLUDEDIR "\${prefix}/include")
set(PKG_CONFIG_LIBS)
set(PKG_CONFIG_CFLAGS "-I\${includedir}")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/pkg-config.pc.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc")

include(CPack)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config)

install(
  EXPORT ${PROJECT_NAME}-config
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/)

install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
