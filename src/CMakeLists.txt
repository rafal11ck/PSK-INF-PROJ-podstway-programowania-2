cmake_minimum_required(VERSION 3.22.1)
project(Podstway_Programowania_2)

add_library(car car.h car.c)
add_library(client car.h client.c)
add_library(retnal rental.h rental.c)
add_library(list list.h list.c)

add_library(menu menu.h menu.c)
target_link_libraries(menu ncurses)

add_executable(wypozyczalnia main.c)
target_link_libraries(wypozyczalnia menu)


add_executable(testlist testlist.c)
target_link_libraries(testlist list)
