add_library(AudioLib STATIC
	src/AudioDevice.cpp
	src/Audio.cpp
	src/ComException.cpp
	src/ComInstance.cpp)

target_include_directories(AudioLib PUBLIC include)
