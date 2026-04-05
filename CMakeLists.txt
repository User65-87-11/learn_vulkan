cmake_minimum_required(VERSION 3.23)

project(LearningVulkan C)


add_executable(Main)

set(CMAKE_PREFIX_PATH
    "C:/libs/glfw-ucrt"
    "C:/VulkanSDK/1.4.341.1"
)



find_package(Vulkan REQUIRED)
find_package(glfw3  REQUIRED)




target_link_libraries(Main 
	PRIVATE 
		Vulkan::Vulkan
		glfw
		m
)



target_sources(
	Main PRIVATE main.c
)