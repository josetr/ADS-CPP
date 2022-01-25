add_executable(AudioDeviceSwitcher WIN32
	main.cpp
	system/Console.cpp)

target_link_libraries(AudioDeviceSwitcher PRIVATE AudioLib)
