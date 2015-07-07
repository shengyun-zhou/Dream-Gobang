cmake_minimum_required (VERSION 2.6)

project (Simple-Gobang)

set(PROJECT_SOURCE_FILE
    GameMain.cpp
    Gobang.h
    Chess.h
    Chess.cpp
    #Please put the new source files and header files here
    )

if(MINGW)
	link_libraries(graphics gdi32 imm32 msimg32 ole32 oleaut32 winmm uuid)
	set(CMAKE_EXE_LINKER_FLAGS "-mwindows")
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_executable (Simple-Gobang ${PROJECT_SOURCE_FILE})

if(MINGW)
	set(CMAKE_EXE_LINKER_FLAGS "")
endif()
set(TERMINAL_TEST_SOURCE_FILE
		Chess.h
		Chess.cpp
		TerminalTestMain.cpp)
add_executable (Gobang-Terminal-Test ${TERMINAL_TEST_SOURCE_FILE})

