SRC_PATH ?= src
INC_PATH += include
BUILD_PATH ?= build
OBJ_PATH ?= $(BUILD_PATH)/obj
BINARY = $(BUILD_PATH)/nfa_to_dfa

INC = $(addprefix -I, $(INC_PATH))
SRC = $(shell find $(SRC_PATH)  -name "*.cpp")
OBJ = $(SRC:$(SRC_PATH)/%.cpp=$(OBJ_PATH)/%.o) # 找出所有匹配模式的文件，将其替换为目标文件
CFLAGS = -O2 -g -Wall -Werror $(INC)
YYAC = $(SRC_PATH)/regular.y
YYACER = $(addsuffix .cpp, $(basename $(YYAC)))

.phony: all app

all: app

$(YYACER):$(YYAC)
	yacc -o $@ $<

$(OBJ_PATH)/%.o:$(SRC_PATH)/%.cpp
	mkdir -p $(OBJ_PATH)
	g++ $(CFLAGS) -c -o $@ $<
	
$(BINARY):$(OBJ)
	g++ -O0 -g -o $@ $^

app: $(YYACER) $(BINARY)

run:app
	$(BINARY) -o out.dot

clean:
	rm -rf $(BUILD_PATH) 

