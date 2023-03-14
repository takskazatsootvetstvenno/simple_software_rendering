cmake_minimum_required(VERSION 3.21)

set(RENDER_PROJECT_NAME SoftwareRender)
project(${RENDER_PROJECT_NAME})

set(RENDER_INCLUDES
	includes/Window.hpp
	includes/Application.hpp
	includes/Camera.hpp
	includes/Mesh.hpp
	includes/OBJ_Loader.h  
)
set(RENDER_SOURCES
	sources/main.cpp
	sources/Window.cpp
	sources/Application.cpp
	sources/Camera.cpp
	sources/Mesh.cpp
)

if(MSVC)
	 add_compile_options(/MP)
endif()

add_executable(${RENDER_PROJECT_NAME} ${RENDER_INCLUDES} ${RENDER_SOURCES})
target_compile_features(${RENDER_PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${RENDER_PROJECT_NAME} PUBLIC includes)
target_include_directories(${RENDER_PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)


cmake_path(GET PROJECT_SOURCE_DIR PARENT_PATH ROOT_DIR)
cmake_path(GET PROJECT_BINARY_DIR PARENT_PATH ROOT_BINARY_DIR)
file(COPY ${ROOT_DIR}/Models DESTINATION ${ROOT_BINARY_DIR})