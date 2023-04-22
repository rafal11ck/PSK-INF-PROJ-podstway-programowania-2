add_library(mmenu
  carsmenu.c
  carsmenu.h
  clientsmenu.c
  clientsmenu.h
  menuutil.c
  menuutil.h
  mmenu.c
  mmenu.h
  rentalsmenu.c
  rentalsmenu.h
)


target_link_libraries(mmenu ncurses dbhandle panel menu form client)
include_directories(../)
