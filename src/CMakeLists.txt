cmake_minimum_required(VERSION 3.22.1)
project(Podstway_Programowania_2)


add_subdirectory(mmenu)

add_library(car car.h car.c)
add_library(client car.h client.c)
add_library(retnal rental.h rental.c)
add_library(list list.h list.c)
add_library(dbhandle dbhandle.h dbhandle.c)
target_link_libraries(dbhandle sqlite3)

add_library(mmenu mmenu.h mmenu.c)
target_link_libraries(mmenu ncurses dbhandle panel menu)

add_executable(wypozyczalnia main.c)
target_link_libraries(wypozyczalnia mmenu)


add_executable(testlist testlist.c)
target_link_libraries(testlist list)
