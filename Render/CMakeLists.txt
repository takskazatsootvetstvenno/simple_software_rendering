cmake_minimum_required(VERSION 3.25)

set(RENDER_PROJECT_NAME Render)

set(RENDER_INCLUDES
	includes/Window.hpp 
)
set(RENDER_SOURCES
	sources/main.cpp
	sources/Window.cpp
)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

add_executable(${RENDER_PROJECT_NAME} ${RENDER_INCLUDES} ${RENDER_SOURCES})
target_compile_features(${RENDER_PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${RENDER_PROJECT_NAME} PUBLIC includes)
target_include_directories(${RENDER_PROJECT_NAME} PRIVATE src)
target_include_directories(${RENDER_PROJECT_NAME} PRIVATE ${MODULE_INSTALL_DIR}/include)
target_link_directories(${RENDER_PROJECT_NAME} PRIVATE ${MODULE_INSTALL_DIR}/lib)

message(FROM: ${MODULE_INSTALL_DIR}/bin)
target_link_libraries(${RENDER_PROJECT_NAME} $<IF:$<CONFIG:Debug>, SDL2D, SDL2>)
message(TO: ${CMAKE_BINARY_DIR}/$<CONFIG>)

set(postfix_debug  "$<IF:$<CONFIG:Debug>,d,>")
if(MSVC)
set(shared_library_extention "dll")
else()
set(shared_library_extention "so")
endif()

add_custom_command(TARGET ${RENDER_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E echo "ECHOECHOECHO: ${MODULE_INSTALL_DIR}/bin/SDL2${postfix_debug}.${shared_library_extention}"
)

add_custom_command(TARGET ${RENDER_PROJECT_NAME} POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy ${MODULE_INSTALL_DIR}/bin/SDL2${postfix_debug}.${shared_library_extention} ${CMAKE_BINARY_DIR}/$<CONFIG>/SDL2${postfix_debug}.${shared_library_extention}
	)

install(TARGETS ${RENDER_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/install)
message(${MODULE_INSTALL_DIR})
message(${CMAKE_BINARY_DIR}/)
