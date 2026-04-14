cmake_minimum_required(VERSION 3.16)
project(krc-overlay C)

# set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD 99)

set(SDL_TEST OFF CACHE BOOL "" FORCE)

add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)

add_executable(${PROJECT_NAME} src/main.c)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${SDL2_INCLUDE_DIRS} 
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

# -mwindows es para ocultar la ventana de comandos
target_link_libraries(${PROJECT_NAME} PRIVATE -mwindows SDL2main SDL2 SDL2_image)

# crea el ejecutable en una carpeta /bin
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# copia los recursos necesarios para el ejecutable a la misma carpeta /bin que lo contiene
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/img
        ${CMAKE_CURRENT_BINARY_DIR}/bin/img
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/external/SDL/SDL2d.dll
        ${CMAKE_CURRENT_BINARY_DIR}/bin/SDL2d.dll
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/external/SDL_image/SDL2_imaged.dll
        ${CMAKE_CURRENT_BINARY_DIR}/bin/SDL2_imaged.dll
)

