cmake_minimum_required(VERSION 3.22.1)
project("Wypożyczalnia Samochodów")
add_subdirectory(mmenu)

add_library(car car.h car.c)
add_library(client car.h client.c)
add_library(retnal rental.h rental.c)
add_library(list list.h list.c)
add_library(dbhandle dbhandle.h dbhandle.c)
target_link_libraries(dbhandle sqlite3)

add_executable(wypozyczalnia main.c)
target_link_libraries(wypozyczalnia mmenu)

add_executable(testlist testlist.c)
target_link_libraries(testlist list)


# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
