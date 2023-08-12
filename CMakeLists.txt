# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(astro.fcgi)

# Spécifiez l'emplacement des en-têtes. Homebrew installe généralement les fichiers d'en-tête dans /usr/local/include
include_directories(/usr/local/include)

# Spécifiez la source à compiler
add_executable(astro.fcgi astro.c)

# Lien avec la bibliothèque libfcgi. Homebrew installe généralement les bibliothèques dans /usr/local/lib
link_directories(/usr/local/lib)
target_link_libraries(astro.fcgi fcgi)
