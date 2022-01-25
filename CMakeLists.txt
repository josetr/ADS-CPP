cmake_minimum_required(VERSION 3.9.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AudioDeviceSwitcher)

add_definitions(
	-DUNICODE
	-DNOMINMAX
	-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(src)
