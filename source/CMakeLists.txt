cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm64)

file(GLOB_RECURSE SRCS
    "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/*.c"
)

set(INCS
    "${CMAKE_CURRENT_LIST_DIR}"
)

add_executable(irisHelper ${SRCS})
target_link_libraries(irisHelper PRIVATE spdlog absl scn)

set(THIS_COMPILE_FLAGS
    -Wall -Werror -fdata-sections -ffunction-sections -fno-threadsafe-statics -fno-omit-frame-pointer -fno-rtti -fomit-frame-pointer -Os -flto -fvisibility=hidden -fshort-enums -fmerge-all-constants -Bsymbolic -fdata-sections -ffunction-sections -fno-stack-protector -falign-functions -finline-functions
    -D_GNU_SOURCE
)

set(THIS_LINK_FLAGS
    -ffixed-x18 -Wl,--hash-style=both -fPIE -Wl,-exclude-libs,ALL -Wl,--gc-sections
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(irisHelper PRIVATE c++_static dl)
    list(APPEND THIS_COMPILE_FLAGS -O3 -fvisibility=hidden -fvisibility-inlines-hidden -flto)
    list(APPEND THIS_LINK_FLAGS -static-libgcc -static-libstdc++ -L/system/lib64 -lc++ -ldl -lc -lm -O3 -fvisibility=hidden -fvisibility-inlines-hidden -flto -Wl,-O3,--lto-O3,--gc-sections,--as-needed,--icf=all,-z,norelro,--pack-dyn-relocs=android+relr,-x,-s,--strip-all)
endif()

target_compile_options(irisHelper PRIVATE ${THIS_COMPILE_FLAGS} -D_GNU_SOURCE)
target_link_options(irisHelper PRIVATE ${THIS_LINK_FLAGS})
