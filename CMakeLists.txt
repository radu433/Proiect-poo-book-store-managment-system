cmake_minimum_required(VERSION 3.26)

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing executable name in this file
# for now, the project name is used as the executable name
set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")


project(${MAIN_PROJECT_NAME})
# set(CMAKE_PROJECT_VERSION_MAJOR 0)
# set(CMAKE_PROJECT_VERSION_MINOR 0)
# set(CMAKE_PROJECT_VERSION_PATCH 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

###############################################################################

# external dependencies with FetchContent
# include(FetchContent)
#
# set(FETCHCONTENT_QUIET OFF)
# set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
#
# NOTE: Also update env vars used for caching in
#         - .github/actions/configure-cmake/action.yml
#         - .github/workflows/cmake.yml
# FetchContent_Declare(
#         SomeLib
#         GIT_REPOSITORY https://github.com/<SomeUser>/<SomeLib>.git
#         GIT_TAG        <some_git_hash> # <which tag/branch @ <some_date>>
#         GIT_SHALLOW    1  # works only with branches or tags, not with arbitrary commit hashes
# )
#
# FetchContent_MakeAvailable(SomeLib)

###############################################################################

# external dependencies with find_package

# find_package(Threads REQUIRED)

###############################################################################

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
add_executable(${MAIN_EXECUTABLE_NAME}
        "main.cpp"
        "files/fisiere_executabile/Carte.cpp"
        "files/fisiere_executabile/Autor.cpp"
        "files/fisiere_executabile/Client.cpp"
        "files/fisiere_executabile/adresa.cpp"
        "files/fisiere_executabile/Manual.cpp"
        "files/fisiere_executabile/CarteStiintifica.cpp"
        "files/fisiere_executabile/Revista.cpp"
        "files/fisiere_executabile/UnitateVanzare.cpp"
        "files/fisiere_executabile/Comanda.cpp"
        "files/fisiere_executabile/CarteIndividuala.cpp"
        "files/fisiere_executabile/PachetSerie.cpp"
        "files/exceptii/exceptii_headere/LibrarieException.h"
        "files/exceptii/exceptii_executabile/LibrarieException.cpp"
        "files/exceptii/exceptii_headere/StocException.h"
        "files/exceptii/exceptii_executabile/StocException.cpp"
        "files/exceptii/exceptii_executabile/ComandaExceptions.cpp"
        "files/exceptii/exceptii_executabile/ClientExceptions.cpp"
        "files/exceptii/exceptii_executabile/DateInvalideException.cpp"
        "files/exceptii/exceptii_executabile/AutorException.cpp"
        "files/headere/Publicatie.h"
        "files/fisiere_executabile/Publicatie.cpp"
        "files/exceptii/exceptii_executabile/PaginareException.cpp"
        "files/exceptii/exceptii_executabile/PublicatieException.cpp"
        "files/fisiere_executabile/Reducere.cpp"
        "files/fisiere_executabile/RevistaIndividuala.cpp"
        "files/fisiere_executabile/AppState.cpp"
        "files/fisiere_executabile/Book_Store_manager.cpp"
        "files/fisiere_executabile/publicatieService.cpp"
        "files/fisiere_executabile/SerchService.cpp"
        "files/fisiere_executabile/InventoryService.cpp"
        "files/headere/ReducereService.h"
        "files/fisiere_executabile/AuthenticationService.cpp"
        "files/fisiere_executabile/ReviewService.cpp"
        "files/fisiere_executabile/AutorService.cpp"
        "files/headere/CosService.h"
        "files/fisiere_executabile/CosService.cpp"
        "files/fisiere_executabile/TradeInService.cpp"

)

# NOTE: Add all defined targets (e.g. executables, libraries, etc. )
# NOTE: RUN_SANITIZERS is optional, if it's not present it will default to true
set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
# set_compiler_flags(TARGET_NAMES ${MAIN_EXECUTABLE_NAME} ${FOO} ${BAR})
# where ${FOO} and ${BAR} represent additional executables or libraries
# you want to compile with the set compiler flags

###############################################################################

# use SYSTEM so cppcheck and clang-tidy do not report warnings from these directories
# target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ext/<SomeHppLib>/include)
# target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${<SomeLib>_SOURCE_DIR}/include)
# target_link_directories(${MAIN_EXECUTABLE_NAME} PRIVATE ${<SomeLib>_BINARY_DIR}/lib)
# target_link_libraries(${MAIN_EXECUTABLE_NAME} <SomeLib>)

###############################################################################

# copy binaries to "bin" folder; these are uploaded as artifacts on each release
# DESTINATION_DIR is set as "bin" in cmake/Options.cmake:6
install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# copy_files(FILES tastatura.txt config.json DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# copy_files(DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
