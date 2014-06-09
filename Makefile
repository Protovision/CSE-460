EXE=os
OBJ_DIR=obj
CXX=c++
CCFLAGS=-I. -g -std=c++0x

SRC_ISA=$(shell find ./ISA -name '*.cpp' -exec basename {} \;)
SRC_AS=$(shell find ./Assembler -name '*.cpp' -exec basename {} \;)
SRC_VM=$(shell find ./VirtualMachine -name '*.cpp' -exec basename {} \;)
SRC_OS=$(shell find ./OperatingSystem -name '*.cpp' -exec basename {} \;)

OBJ_ISA=$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_ISA))
OBJ_AS=$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_AS))
OBJ_VM=$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_VM))
OBJ_OS=$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_OS))

OBJ=$(OBJ_DIR)/main.o $(OBJ_ISA) $(OBJ_AS) $(OBJ_VM) $(OBJ_OS)

all: makedirs $(EXE)

$(OBJ_DIR)/%.o : ISA/%.cpp
	$(CXX) $(CCFLAGS) -c -o $@ $< 
$(OBJ_DIR)/%.o : Assembler/%.cpp
	$(CXX) $(CCFLAGS) -c -o $@ $< 
$(OBJ_DIR)/%.o : VirtualMachine/%.cpp
	$(CXX) $(CCFLAGS) -c -o $@ $< 
$(OBJ_DIR)/%.o : OperatingSystem/%.cpp
	$(CXX) $(CCFLAGS) -c -o $@ $< 
$(OBJ_DIR)/main.o : main.cpp
	$(CXX) $(CCFLAGS) -c -o $@ $< 

$(EXE) : $(OBJ_DIR)/main.o $(OBJ)
	$(CXX) $(CCFLAGS) -o $@ $^ 

clean:
	rm -rf $(OBJ)

makedirs:
	if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	
