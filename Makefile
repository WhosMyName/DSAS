#OBJS specifies which files to compile as part of the project 
OBJS = dsas.cpp 
#CC specifies which compiler we're using 
CC = g++ 
#COMPILER_FLAGS specifies the additional compilation options we're using 
# -w suppresses all warnings 
COMPILER_FLAGS = -w -std=c++11
#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -lboost_system -lboost_regex -lboost_filesystem -lpthread -lboost_date_time -lboost_thread
#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = dsas
#This is the target that compiles our executable 
all: $(OBJS) 
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(OBJS) -o $(OBJ_NAME) 
