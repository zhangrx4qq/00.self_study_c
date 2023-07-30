cmake_minimum_required(VERSION 3.27.0)
project(exercise VERSION 1.0.0 LANGUAGES C)

# sdl_0
# set(bin_name sdl_0)
# add_executable(${bin_name} src/sdl_0.c)
# set(sdl_include /usr/local/Cellar/sdl2/2.0.20/include/SDL2)
# target_include_directories(${bin_name} PRIVATE ${sdl_include})
# set(sdl_lib /usr/local/Cellar/sdl2/2.0.20/lib)
# target_link_libraries(${bin_name} PRIVATE ${sdl_lib}/libSDL2.dylib)

# sdl_1
# set(bin_name sdl_1)
# add_executable(${bin_name} src/sdl_1.c)
# set(sdl_include /usr/local/Cellar/sdl2/2.0.20/include/SDL2)
# target_include_directories(${bin_name} PRIVATE ${sdl_include})
# set(sdl_lib /usr/local/Cellar/sdl2/2.0.20/lib)
# target_link_libraries(${bin_name} PRIVATE ${sdl_lib}/libSDL2.dylib)

# sdl_2
# set(bin_name sdl_2)
# add_executable(${bin_name} src/sdl_2.c)
# set(sdl_include /usr/local/Cellar/sdl2/2.0.20/include/SDL2)
# target_include_directories(${bin_name} PRIVATE ${sdl_include})
# set(sdl_lib /usr/local/Cellar/sdl2/2.0.20/lib)
# target_link_libraries(${bin_name} PRIVATE ${sdl_lib}/libSDL2.dylib)

# sdl_3
# set(bin_name sdl_3)
# add_executable(${bin_name} src/sdl_3.c)
# set(sdl_include /usr/local/Cellar/sdl2/2.0.20/include/SDL2)
# set(sdl_image_include /usr/local/Cellar/sdl2_image/2.0.5/include/SDL2)
# target_include_directories(${bin_name} PRIVATE ${sdl_include})
# target_include_directories(${bin_name} PRIVATE ${sdl_image_include})
# set(sdl_lib /usr/local/Cellar/sdl2/2.0.20/lib)
# set(sdl_image_lib /usr/local/Cellar/sdl2_image/2.0.5/lib)
# target_link_libraries(${bin_name} PRIVATE ${sdl_lib}/libSDL2.dylib)
# target_link_libraries(${bin_name} PRIVATE ${sdl_image_lib}/libSDL2_image.dylib)

# sdl_4
# set(bin_name sdl_4)
# add_executable(${bin_name} src/sdl_4.c)
# set(sdl_include /usr/local/Cellar/sdl2/2.0.20/include/SDL2)
# set(sdl_ttf_include /usr/local/Cellar/sdl2_ttf/2.0.18_1/include/SDL2)
# target_include_directories(${bin_name} PRIVATE ${sdl_include})
# target_include_directories(${bin_name} PRIVATE ${sdl_ttf_include})
# set(sdl_lib /usr/local/Cellar/sdl2/2.0.20/lib)
# set(sdl_ttf_lib /usr/local/Cellar/sdl2_ttf/2.0.18_1/lib)
# target_link_libraries(${bin_name} PRIVATE ${sdl_lib}/libSDL2.dylib)
# target_link_libraries(${bin_name} PRIVATE ${sdl_ttf_lib}/libSDL2_ttf.dylib)

# sdl_5
set(bin_name sdl_5)
add_executable(${bin_name} src/sdl_5.c)
set(sdl_include /usr/local/Cellar/sdl2/2.0.20/include/SDL2)
target_include_directories(${bin_name} PRIVATE ${sdl_include})
set(sdl_lib /usr/local/Cellar/sdl2/2.0.20/lib)
target_link_libraries(${bin_name} PRIVATE ${sdl_lib}/libSDL2.dylib)