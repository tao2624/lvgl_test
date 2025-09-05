cmake_minimum_required(VERSION 3.10)
project(lvgl)

foreach(BACKEND_NAME "SDL" "LINUX_DRM" "LINUX_FBDEV" "X11" "WAYLAND")
    execute_process(COMMAND "scripts/backend_conf.sh" ${BACKEND_NAME} OUTPUT_VARIABLE IS_BACKEND_ENABLED)
    set("LV_USE_${BACKEND_NAME}" ${IS_BACKEND_ENABLED})
endforeach()

# Uncomment if the program needs debugging
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb")
# Enable debugging support
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(CMAKE_C_STANDARD 99) # LVGL officially supports C99 and above
set(CMAKE_CXX_STANDARD 17) #C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE IMAGE_FILES ${SOURCE_DIR}/assets/images/*.c)
add_subdirectory(lvgl)
target_include_directories(lvgl PUBLIC ${PROJECT_SOURCE_DIR} ${SOURCE_DIR})

# Find and configure FreeType if enabled
find_package(PkgConfig REQUIRED)
pkg_check_modules(FREETYPE freetype2)
if(FREETYPE_FOUND)
    target_include_directories(lvgl PUBLIC ${FREETYPE_INCLUDE_DIRS})
    message(STATUS "FreeType found: ${FREETYPE_INCLUDE_DIRS}")
else()
    message(STATUS "FreeType not found via pkg-config")
endif()

# Check for FFmpeg
pkg_check_modules(LIBAVCODEC libavcodec)
pkg_check_modules(LIBAVFORMAT libavformat)
pkg_check_modules(LIBAVUTIL libavutil)
pkg_check_modules(LIBSWSCALE libswscale)
pkg_check_modules(LIBAVFILTER libavfilter)

if(LIBAVCODEC_FOUND AND LIBAVFORMAT_FOUND AND LIBAVUTIL_FOUND AND LIBSWSCALE_FOUND)
    message(STATUS "FFmpeg found")
    set(FFMPEG_LIBRARIES ${LIBAVCODEC_LIBRARIES} ${LIBAVFORMAT_LIBRARIES} ${LIBAVUTIL_LIBRARIES} ${LIBSWSCALE_LIBRARIES} ${LIBAVFILTER_LIBRARIES})
    set(FFMPEG_INCLUDE_DIRS ${LIBAVCODEC_INCLUDE_DIRS} ${LIBAVFORMAT_INCLUDE_DIRS} ${LIBAVUTIL_INCLUDE_DIRS} ${LIBSWSCALE_INCLUDE_DIRS} ${LIBAVFILTER_INCLUDE_DIRS})
    target_include_directories(lvgl PUBLIC ${FFMPEG_INCLUDE_DIRS})
else()
    message(STATUS "FFmpeg not found - some libraries may be missing")
    set(FFMPEG_LIBRARIES "")
endif()

message(STATUS "SOURCE_DIR = ${SOURCE_DIR}")
message(STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message(STATUS "SOURCE_FILES = ${SOURCE_FILES}")
message(STATUS "Found ${SOURCE_FILES} source files")
if (LV_USE_LINUX_DRM)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBDRM REQUIRED libdrm)

    target_include_directories(lvgl PUBLIC ${LIBDRM_INCLUDE_DIRS})
    add_executable(lvglsim ${SOURCE_FILES} mouse_cursor_icon.c)
    target_link_libraries(lvglsim lvgl lvgl::examples lvgl::demos lvgl::thorvg ${LIBDRM_LIBRARIES} ${FREETYPE_LIBRARIES} ${FFMPEG_LIBRARIES} m pthread)

elseif (LV_USE_SDL)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_image REQUIRED SDL2_image)

    target_include_directories(lvgl PRIVATE ${SDL2_INCLUDE_DIRS})
    target_include_directories(lvgl PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
    target_include_directories(lvgl PUBLIC ${SOURCE_DIR}/include ${SOURCE_DIR}/library)
    add_executable(lvglsim ${SOURCE_FILES} mouse_cursor_icon.c ${IMAGE_FILES})
    target_link_libraries(lvglsim lvgl lvgl::examples lvgl::demos lvgl::thorvg ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${FREETYPE_LIBRARIES} ${FFMPEG_LIBRARIES} m pthread)

elseif (LV_USE_X11)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(X11 REQUIRED x11)

    target_include_directories(lvgl PRIVATE ${X11_INCLUDE_DIRS})
    add_executable(lvglsim ${SOURCE_FILES} mouse_cursor_icon.c)
    target_link_libraries(lvglsim lvgl lvgl::examples lvgl::demos lvgl::thorvg ${X11_LIBRARIES} ${FREETYPE_LIBRARIES} ${FFMPEG_LIBRARIES} m pthread)

elseif (LV_USE_WAYLAND)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(wayland-client REQUIRED wayland-client)
    pkg_check_modules(wayland-cursor REQUIRED wayland-cursor)
    pkg_check_modules(xkbcommon REQUIRED xkbcommon)

    # Wayland protocols
    pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols>=1.25)
    pkg_get_variable(WAYLAND_PROTOCOLS_BASE wayland-protocols pkgdatadir)

    execute_process(COMMAND "scripts/gen_wl_protocols.sh" OUTPUT_VARIABLE WAYLAND_PROTOCOLS_SRC)

    target_include_directories(lvgl PRIVATE ${PROJECT_SOURCE_DIR}
                                            "${PROJECT_SOURCE_DIR}/wl_protocols")
    add_executable(lvglsim ${SOURCE_FILES} ${WAYLAND_PROTOCOLS_SRC} 
        mouse_cursor_icon.c backends/wayland.c)
    target_compile_definitions(lvglsim PRIVATE LV_CONF_INCLUDE_SIMPLE)
    target_link_libraries(lvglsim lvgl lvgl::examples lvgl::demos lvgl::thorvg m
        wayland-client wayland-cursor xkbcommon ${FREETYPE_LIBRARIES} ${FFMPEG_LIBRARIES})

else()

    # No specific build steps required for FBDEV
    add_executable(lvglsim ${SOURCE_FILES} mouse_cursor_icon.c)
    target_link_libraries(lvglsim lvgl lvgl::examples lvgl::demos lvgl::thorvg ${FREETYPE_LIBRARIES} m pthread)
endif()

add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/lvglsim DEPENDS lvglsim)
