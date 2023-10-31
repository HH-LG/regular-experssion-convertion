SRC_PATH ?= src
INC_PATH += include
BUILD_PATH ?= build
TEST_PATH ?= test
OBJ_PATH ?= $(BUILD_PATH)/obj
BINARY = $(BUILD_PATH)/nfa_to_dfa
TEST = $(TEST_PATH)/regex.txt
GEN = $(SRC_PATH)/example_gen.py

INC = $(addprefix -I, $(INC_PATH))
SRC = $(shell find $(SRC_PATH)  -name "*.cpp")
OBJ = $(SRC:$(SRC_PATH)/%.cpp=$(OBJ_PATH)/%.o) # 找出所有匹配模式的文件，将其替换为目标文件
CFLAGS = -O0 -g -Wall -Werror $(INC)  # -O2 优化等级2 -g 生成调试信息 -Wall 显示所有警告 -Werror 将警告当做错误
YYAC = $(SRC_PATH)/regular.y
YYACER = $(addsuffix .cpp, $(basename $(YYAC)))

.phony: all app gdb run test gen clean

all: app

$(YYACER):$(YYAC)
	yacc -o $@ $<

$(OBJ_PATH)/%.o:$(SRC_PATH)/%.cpp
	mkdir -p $(OBJ_PATH)
	g++ $(CFLAGS) -c -o $@ $<
	
$(BINARY):$(OBJ)
	g++ -O0 -g -o $@ $^

app: $(YYACER) $(BINARY)

gdb: $(BINARY)
	gdb $(BINARY)

run:app
	$(BINARY) -o out.dot

test:app
	$(BINARY) -t 

gen:
	python3 $(GEN) 

clean:
	rm -rf $(BUILD_PATH) 

