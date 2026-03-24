BUILD_DIR = out
SRC_DIR = printf


CC = clang
CFLAGS = -std=c23 -g 
 
LD_FLAGS = ""


OBJECTS_TO_BUILD = $(BUILD_DIR)/main.elf 
 
ALL: CLEAN INIT  ${OBJECTS_TO_BUILD} 

INIT:
	mkdir -p ${BUILD_DIR}

CLEAN:
	rm -rf ${BUILD_DIR}


$(BUILD_DIR)/main.o: main.c
	$(CC)  $(CFLAGS) -c  $^ -o $@		



$(BUILD_DIR)/asm.o: asm.asm
	nasm  -f elf64 -o $@ $^ 


$(BUILD_DIR)/main.elf: $(BUILD_DIR)/main.o $(BUILD_DIR)/asm.o
							
	
	gcc  -o $@ $^ -lglfw -lvulkan -lm
# 	ld -o $@ $^ 


	