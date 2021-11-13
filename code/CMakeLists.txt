cmake_minimum_required(VERSION 3.20)
project(game)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system audio)

add_executable(game main.cpp headers.h gui.h base_class.h game_class.h library.h main_menu.h buttons_function.h prototype.h game.h pause.h aniamation_functions.h game_function.h random.h game_over.h)

target_link_libraries(game sfml-graphics sfml-audio)

install(TARGETS game DESTINATION bin)
