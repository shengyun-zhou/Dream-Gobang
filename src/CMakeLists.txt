cmake_minimum_required (VERSION 2.6)

project (Simple-Gobang)

set(PROJECT_SOURCE_FILE
    Chess.h
    Chess.cpp
	ComputerPlayer.h
	ComputerPlayer.cpp
	Image.h
	Image.cpp
	Interface.h
	Interface.cpp
	Music.h
	Music.cpp
	PlayChess.h
	PlayChess.cpp
	Save.h
	Save.cpp
	Settings.h
	Settings.cpp
	main.cpp
    #Please put the new source files and header files here
    )
	
if(MINGW)
	link_libraries(graphics gdi32 imm32 msimg32 ole32 oleaut32 winmm uuid)
	set(CMAKE_EXE_LINKER_FLAGS "-mwindows")
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_executable (Simple-Gobang ${PROJECT_SOURCE_FILE})

if(MINGW)
	file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/res  DESTINATION ${CMAKE_BINARY_DIR}/bin)
elseif(MSVC)
	file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/res  DESTINATION ${CMAKE_BINARY_DIR}/bin/Release)
	file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/res  DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)
endif()

set(TERMINAL_TEST_SOURCE_FILE
		Gobang.h
		Chess.h
		Chess.cpp
		ComputerPlayer.h
		ComputerPlayer.cpp
		TerminalTestMain.cpp)
if(MINGW)
	set(CMAKE_EXE_LINKER_FLAGS "")
endif()
add_executable (Gobang-Terminal-Test ${TERMINAL_TEST_SOURCE_FILE})

