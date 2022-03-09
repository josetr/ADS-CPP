cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AudioDeviceSwitcher)

add_definitions(
	-DUNICODE
	-DNOMINMAX
	-D_CRT_SECURE_NO_WARNINGS)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(src)
