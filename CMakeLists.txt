cmake_minimum_required(VERSION 3.27.0)
project(exercise VERSION 1.0.0 LANGUAGES C)

#set(bin_name sdl_0)
#add_executable(${bin_name} src/sdl_0.c)

#set(bin_name mine_sweep)
#add_executable(${bin_name} src/mine_sweep.c)

set(bin_name web_0)
add_executable(${bin_name} src/web_0.c mongoose/mongoose.c)

# sdl header files
set(sdl_include /usr/local/Cellar/sdl2/2.0.20/include/SDL2)
target_include_directories(${bin_name} PRIVATE ${sdl_include})

# sdl lib files
set(sdl_lib /usr/local/Cellar/sdl2/2.0.20/lib)
target_link_libraries(${bin_name} PRIVATE ${sdl_lib}/libSDL2.dylib)

# sdl mongoose files
set(mongoose_include ./mongoose)
target_include_directories(${bin_name} PRIVATE ${mongoose_include})