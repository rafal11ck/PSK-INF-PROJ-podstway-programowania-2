cmake_minimum_required(VERSION 3.26.2)
project(Podstway_Programowania_2)

add_executable(wypozyczalnia main.c)


add_library(car car.h car.c)
add_library(client car.h client.c)
add_library(retnal rental.h rental.c)
