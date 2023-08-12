# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Nom du projet
project(astro.fcgi)

# Spécifiez l'emplacement des en-têtes. Homebrew installe généralement les fichiers d'en-tête dans /usr/local/include
include_directories(/usr/local/include)

include(FetchContent)

FetchContent_Declare(
        swe_bressani_dev
        GIT_REPOSITORY https://github.com/stephaneworkspace/SweBressaniDev.git
        GIT_TAG main
)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)

FetchContent_GetProperties(swe_bressani_dev)
if(NOT swe_bressani_dev_POPULATED)
    FetchContent_Populate(swe_bressani_dev)
    include_directories(${swe_bressani_dev_SOURCE_DIR}/Sources/swelib/include)
    include_directories(${swe_bressani_dev_SOURCE_DIR}/Sources/svglib/include)
    include_directories(${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/include)
endif()

FetchContent_MakeAvailable(json)

set(SWEBRESSANIDEVLIB_SOURCES
        ${swe_bressani_dev_SOURCE_DIR}/Sources/swelib/swecl.c
        ${swe_bressani_dev_SOURCE_DIR}/Sources/swelib/swedate.c
        ${swe_bressani_dev_SOURCE_DIR}/Sources/swelib/swehel.c
        ${swe_bressani_dev_SOURCE_DIR}/Sources/swelib/swehouse.c
        ${swe_bressani_dev_SOURCE_DIR}/Sources/swelib/swejpl.c
        ${swe_bressani_dev_SOURCE_DIR}/Sources/swelib/swemmoon.c
        ${swe_bressani_dev_SOURCE_DIR}/Sources/swelib/swemplan.c
        ${swe_bressani_dev_SOURCE_DIR}/Sources/swelib/swepcalc.c
        ${swe_bressani_dev_SOURCE_DIR}/Sources/swelib/sweph.c
        ${swe_bressani_dev_SOURCE_DIR}/Sources/swelib/swephlib.c
        ${swe_bressani_dev_SOURCE_DIR}/Sources/svglib/circle.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/svglib/data.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/svglib/document.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/svglib/line.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/svglib/image.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/angle.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/aspect.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/astre.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/draw.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/draw_aspect_line.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/draw_bodie_astre.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/draw_bodie_line.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/draw_house_angle.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/draw_house_line.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/draw_house_number.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/draw_zodiac_circle.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/draw_zodiac_line.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/draw_zodiac_sign.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/swe02.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/swe03.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/swe08.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/swe14.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/swe17.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/house.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/sign.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/timezone.cpp
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/SweInterface.cpp
        )

set(SWEBRESSANIDEVLIB_HEADERS
        ${swe_bressani_dev_SOURCE_DIR}/Sources/swelib/include/swelib.h
        ${swe_bressani_dev_SOURCE_DIR}/Sources/svglib/include/svglib.h
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/include/base64.h
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib/include/sweinterfacelib.h
        )


# Spécifiez la source à compiler
add_executable(astro.fcgi
        Sources/astro.cpp
        ${SWEBRESSANIDEVLIB_HEADERS}
        ${SWEBRESSANIDEVLIB_SOURCES}
        Sources/Util.h
        Sources/Util.cpp)

# Lien avec la bibliothèque libfcgi. Homebrew installe généralement les bibliothèques dans /usr/local/lib
link_directories(/usr/local/lib)

target_include_directories(astro.fcgi PRIVATE
        ${swe_bressani_dev_SOURCE_DIR}/Sources/swelib
        ${swe_bressani_dev_SOURCE_DIR}/Sources/svglib
        ${swe_bressani_dev_SOURCE_DIR}/Sources/sweinterfacelib
        )

target_link_libraries(astro.fcgi PRIVATE
        fcgi
        nlohmann_json::nlohmann_json
        )