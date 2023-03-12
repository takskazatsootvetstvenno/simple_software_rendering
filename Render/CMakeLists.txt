cmake_minimum_required(VERSION 3.21)

set(RENDER_PROJECT_NAME SoftwareRender)
project(${RENDER_PROJECT_NAME})

set(RENDER_INCLUDES
	includes/Window.hpp
	includes/Application.hpp 
)
set(RENDER_SOURCES
	sources/main.cpp
	sources/Window.cpp
	sources/Application.cpp
)

if(MSVC)
	 add_compile_options(/MP)
endif()

add_executable(${RENDER_PROJECT_NAME} ${RENDER_INCLUDES} ${RENDER_SOURCES})
target_compile_features(${RENDER_PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${RENDER_PROJECT_NAME} PUBLIC includes)
target_include_directories(${RENDER_PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
