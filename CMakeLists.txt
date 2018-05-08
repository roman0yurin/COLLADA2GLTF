cmake_minimum_required (VERSION 3.5)

set(PROJECT_NAME COLLADA2GLTF)
set( CMAKE_CXX_FLAGS "-fpic ${CMAKE_CXX_FLAGS}" )
set( CMAKE_C_FLAGS   "-fpic ${CMAKE_C_FLAGS}" )
#Переключаем библиотеку draco в нужный режим
set(BUILD_FOR_GLTF ON)
#По какой то причине опции компилятора в draco BUILD_FOR_GLTF срабатыват через раз, дублируем их здесь.
ADD_DEFINITIONS("-DDRACO_MESH_COMPRESSION_SUPPORTED -DDRACO_STANDARD_EDGEBREAKER_SUPPORTED")

include(ExternalProject)

project(${PROJECT_NAME})
if(MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-lstdc++fs)
endif()

# cmake -Dtest=ON to build with tests
option(test "Build all tests." ON)

# GLTF
set(GLTF_INCLUDES GLTF/include)
add_subdirectory(GLTF)

# Disable for other modules
set(TEST_ENABLED ${test})
set(test OFF)

# RapidJSON
set(GLTF_INCLUDES GLTF/dependencies/rapidjson/include ${GLTF_INCLUDES})

# Draco
set(GLTF_INCLUDES GLTF/dependencies/draco/src ${GLTF_INCLUDES})

# Pre-compiled
if(precompiled MATCHES "X86")
  if(precompiled MATCHES "DEBUG")
    if(MSVC)
      ExternalProject_Add(
        precompiled
        URL https://github.com/KhronosGroup/COLLADA2GLTF/releases/download/dependencies/dependencies-windows-Debug-Win32.zip
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
      )
      set(PRECOMPILED_DIR ${CMAKE_CURRENT_BINARY_DIR}/precompiled-prefix/src/precompiled)
    endif()
  elseif(precompiled MATCHES "RELEASE")
    if(MSVC)
      ExternalProject_Add(
        precompiled
        URL https://github.com/KhronosGroup/COLLADA2GLTF/releases/download/dependencies/dependencies-windows-Release-Win32.zip
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
      )
      set(PRECOMPILED_DIR ${CMAKE_CURRENT_BINARY_DIR}/precompiled-prefix/src/precompiled)
    endif()
  endif()
elseif(precompiled MATCHES "X64")
  if(precompiled MATCHES "DEBUG")
    if(MSVC)
      ExternalProject_Add(
        precompiled
        URL https://github.com/KhronosGroup/COLLADA2GLTF/releases/download/dependencies/dependencies-windows-Debug-x64.zip
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
      )
      set(PRECOMPILED_DIR ${CMAKE_CURRENT_BINARY_DIR}/precompiled-prefix/src/precompiled)
    endif()
  elseif(precompiled MATCHES "RELEASE")
    if(MSVC)
      ExternalProject_Add(
        precompiled
        URL https://github.com/KhronosGroup/COLLADA2GLTF/releases/download/dependencies/dependencies-windows-Release-x64.zip
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
      )
      set(PRECOMPILED_DIR ${CMAKE_CURRENT_BINARY_DIR}/precompiled-prefix/src/precompiled)
    endif()
  endif()
endif()

if(PRECOMPILED_DIR)
  set(OpenCOLLADA
    ${PRECOMPILED_DIR}/COLLADABaseUtils.lib
    ${PRECOMPILED_DIR}/COLLADAFramework.lib
    ${PRECOMPILED_DIR}/COLLADASaxFrameworkLoader.lib
    ${PRECOMPILED_DIR}/expat.lib
    ${PRECOMPILED_DIR}/GeneratedSaxParser.lib
    ${PRECOMPILED_DIR}/LibXML.lib
    ${PRECOMPILED_DIR}/MathMLSolver.lib
    ${PRECOMPILED_DIR}/pcre.lib)
endif()

# COLLADASaxFrameworkLoader/BaseUtils/Framework, GeneratedSaxParser
set(GLTF_INCLUDES
        dependencies/OpenCOLLADA/OpenCOLLADA/COLLADASaxFrameworkLoader/include
        dependencies/OpenCOLLADA/OpenCOLLADA/COLLADABaseUtils/include
				dependencies/OpenCOLLADA/OpenCOLLADA/COLLADAFramework/include
        dependencies/OpenCOLLADA/OpenCOLLADA/GeneratedSaxParser/include
        ${GLTF_INCLUDES}
)
if(NOT OpenCOLLADA)
  add_subdirectory(dependencies/OpenCOLLADA/modules/COLLADASaxFrameworkLoader)
  set(OpenCOLLADA COLLADASaxFrameworkLoader)
endif()

# COLLADA2GLTF
set(GLTF_INCLUDES include ${GLTF_INCLUDES})

file(GLOB LIB_HEADERS "include/*.h")
set(LIB_SOURCES
        src/COLLADA2GLTFWriter.cpp
        src/COLLADA2GLTFExtrasHandler.cpp
        src/Utils.cpp
  )
add_library(${PROJECT_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})
if(MSVC)
   target_link_libraries(${PROJECT_NAME} GLTF ${OpenCOLLADA})
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
   target_link_libraries(${PROJECT_NAME} GLTF ${OpenCOLLADA} stdc++fs)
endif()

# ahoy
set(GLTF_INCLUDES dependencies/ahoy/include ${GLTF_INCLUDES})
add_subdirectory(dependencies/ahoy)

add_executable(${PROJECT_NAME}-bin src/main.cpp src/Utils)
if(MSVC)
   target_link_libraries(${PROJECT_NAME}-bin ${PROJECT_NAME} ahoy)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
   target_link_libraries(${PROJECT_NAME}-bin ${PROJECT_NAME} ahoy stdc++fs)
endif()

target_link_libraries(${PROJECT_NAME}-bin draco)

if(TEST_ENABLED)
  enable_testing()

  # gtest
  set(GTEST_LINKED 1)
  set(GLTF_INCLUDES GLTF/dependencies/googletest/googletest/include ${GLTF_INCLUDES})
  add_subdirectory(GLTF/dependencies/googletest/googletest)

  # Unit Tests
  set(GLTF_INCLUDES test/include ${GLTF_INCLUDES})
  file(GLOB TEST_HEADERS "test/include/*.h")
  file(GLOB TEST_SOURCES "test/src/*.cpp")

  add_executable(${PROJECT_NAME}-test ${TEST_HEADERS} ${TEST_SOURCES})
  target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME} GLTF gtest)

  add_test(COLLADA2GLTFWriterTest ${PROJECT_NAME}-test)
endif()

include_directories(${GLTF_INCLUDES})

FOREACH(incl ${GLTF_INCLUDES})
  LIST(APPEND GLTF_INCLUDES_FULL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${incl}")
ENDFOREACH()
set_property(GLOBAL PROPERTY GLTF_INCLUDES ${GLTF_INCLUDES_FULL_PATH})

