cmake_minimum_required(VERSION 3.23)

project(LearningVulkan)


add_executable(Main)


find_package(Vulkan REQUIRED)
find_package(glfw3  REQUIRED)

target_link_libraries(Main 
	PRIVATE 
		Vulkan::Vulkan
		glfw
		m
)


# FetchContent_Declare(
#     glfw
#     GIT_REPOSITORY https://github.com/glfw/glfw.git
#     GIT_TAG 3.3.9
# )

target_sources(
	Main PRIVATE main.c
)