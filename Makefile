BUILD_DIR = out
SRC_DIR = printf


CC = gcc
CFLAGS = -std=c23 -g 
 
LD_FLAGS = ""


OBJECTS_TO_BUILD = $(BUILD_DIR)/main.exe 
 
ALL: CLEAN INIT  ${OBJECTS_TO_BUILD} 

INIT:
	mkdir -p ${BUILD_DIR}

CLEAN:
	rm -rf ${BUILD_DIR}


$(BUILD_DIR)/main.o: main.c
	$(CC) -I"C:/LIBS/glfw-ucrt/include"  $(CFLAGS) -c  $^ -o $@		



$(BUILD_DIR)/main.exe: $(BUILD_DIR)/main.o 
	$(CC)  -o $@ $^ -L"C:/LIBS/glfw-ucrt/lib"  -lglfw3 -lvulkan-1 -lm -luser32 -lgdi32 -lshell32 -lwinmm

	
	