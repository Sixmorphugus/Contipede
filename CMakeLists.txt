# I wrote a cmake file to eliminate troubles with different IDEs and operating systems
# if you have cmake just make a directory called "build" and do "cmake .."
cmake_minimum_required(VERSION 3.6)

project(Contipede)

# Contipede's files
file(GLOB CONT_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(Contipede ${CONT_SRC})

# This project will need ncurses/pdcurses depending on whether you're on windows or linux. This finds both:
if(WIN32)
	set(CURSES_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extlibs/pdcurses")
	set(CURSES_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlibs/pdcurses/pdcurses.lib")
	
	# do the normal trick to ensure that the dll is autocopied
	add_custom_command(TARGET Contipede POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/pdcurses/pdcurses.dll $<TARGET_FILE_DIR:Contipede>/pdcurses.dll
	)
else()
	find_package(Curses REQUIRED)
endif()

include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(Contipede ${CURSES_LIBRARIES})

# # # # # # # # # # # # # # # # # # # # # # # # # # 
# pdCURSES: https://pdcurses.sourceforge.io/      #
# nCURSES: https://www.gnu.org/software/ncurses/  #
# # # # # # # # # # # # # # # # # # # # # # # # # #
