cmake_minimum_required(VERSION 3.12)
project(pprintpp CXX)

set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)

enable_testing()

add_executable(${PROJECT_NAME}-test test/main.cpp)
add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-test)
target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
target_compile_options(${PROJECT_NAME}-test PRIVATE -Wall -Wextra -Werror)

add_executable(${PROJECT_NAME}-example example/main.cpp)
target_link_libraries(${PROJECT_NAME}-example ${PROJECT_NAME})
target_compile_options(${PROJECT_NAME}-example PRIVATE -Wall -Wextra -Werror)

include(GNUInstallDirs)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-config)

install(
  EXPORT ${PROJECT_NAME}-config
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  )

install(
  DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
  )

set(CPACK_PACKAGE_NAME "pprintpp")
set(CPACK_PACKAGE_VENDOR "galowicz.de")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "pprintpp - Typesafe Python Style Printf Formatting for C++")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "pprintpp")
set(CPACK_PACKAGE_CONTACT "Jacek Galowicz <jacek@galowicz.de>")

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST /usr/lib64/)

include(CPack)
