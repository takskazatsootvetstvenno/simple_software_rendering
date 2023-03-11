cmake_minimum_required(VERSION 3.12)

set(RENDER_PROJECT_NAME Render)

set(RENDER_INCLUDES 
)
set(RENDER_SOURCES
	sources/main.cpp
)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

add_executable(${RENDER_PROJECT_NAME} ${RENDER_INCLUDES} ${RENDER_SOURCES})

target_compile_features(${RENDER_PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${RENDER_PROJECT_NAME} PUBLIC includes)
target_include_directories(${RENDER_PROJECT_NAME} PRIVATE src)

install(TARGETS ${RENDER_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/install)