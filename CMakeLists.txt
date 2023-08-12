# CMakeLists.txt

# Nom du projet
project(astro.fcgi)

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "clang++")
if(DEFINED SW_DEBUG)
    add_definitions(-DSW_DEBUG=${SW_DEBUG})
else ()
    add_definitions(-DSW_DEBUG=1)
endif()


# Spécifiez l'emplacement des en-têtes. Homebrew installe généralement les fichiers d'en-tête dans /usr/local/include
include_directories(/usr/local/include)

include(FetchContent)

FetchContent_Declare(
        swebressanidevcpp
        GIT_REPOSITORY https://github.com/stephaneworkspace/swebressanidevcpp.git
        GIT_TAG main
)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)

FetchContent_GetProperties(swebressanidevcpp)
if(NOT swebressanidevcpp_POPULATED)
    FetchContent_Populate(swebressanidevcpp)
    include_directories(${swebressanidevcpp_SOURCE_DIR}/Sources/swelib/include)
    include_directories(${swebressanidevcpp_SOURCE_DIR}/Sources/svglib/include)
    include_directories(${swebressanidevcpp_SOURCE_DIR}/Sources/sweinterfacelib/include)
    add_subdirectory(${swebressanidevcpp_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/swebressanidevcpp)
endif()

FetchContent_MakeAvailable(json)

# Spécifiez la source à compiler
add_executable(astro.fcgi Sources/astro.cpp)

# Lien avec la bibliothèque libfcgi. Homebrew installe généralement les bibliothèques dans /usr/local/lib
link_directories(/usr/local/lib)

target_include_directories(astro.fcgi PRIVATE
        ${swebressanidevcpp_SOURCE_DIR}/Sources/
        )

target_link_libraries(astro.fcgi PRIVATE
        fcgi
        nlohmann_json::nlohmann_json
        SweBressaniDevCpp
        )