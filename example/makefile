TARGET = test 

CXX = g++

MYFLAGS = -g -Wall -O2

INC_IOCONCIF = ../
LIB_IOCONCIF = ../lib/libIOConfig.a
SRC_DIR = ./
OBJ_DIR = ./obj/

INC += -I \
	   $(INC_IOCONCIF) \

LIB += \
	   $(LIB_IOCONCIF) \


LIB += -ldl -lrt -lz -lpthread

SVR_OBJS = $(OBJ_DIR)example.o

all: $(TARGET)

$(TARGET): $(SVR_OBJS)
	@echo -e "\033[1;33m\nLinking $< ==> $@\033[0m"
	$(CXX) $(MYFLAGS) $^ -o $@ $(LIB)
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@echo -e "\033[1;33m\nCompiling $< ==> $@\033[0m"
	$(CXX) $(MYFLAGS) $< -c -o $@ $(INC)

clean:
	rm $(TARGET) $(SVR_OBJS)
