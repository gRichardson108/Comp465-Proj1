# Makefile for use with Linux for Comp 465/L
# Rename this file from Makefile-linux to Makefile before using.
#
# There are two options:
#    $ make		will make the Target
#    $ make clean	will remove the Target to force rebuilding on next make
#
# Edit the "SRC="  and "TARGET=" lines to set a new source file and target
#
# Notes on Makefile
# http://www.csun.edu/~renzo/programming/makefile.htm
#
# Mike Barnes 9/3/2016

# SRC specifies which files to compile as part of the project
SRC = src/WarbirdSimulation.cpp src/Model.cpp src/BaseEntity.cpp src/CelestialBody.cpp src/StaticCamera.cpp src/DynamicCamera.cpp src/Ship.cpp src/Missile.cpp

# CC specifies which compiler we're using
CC = g++

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w   to supresses warnings
# -v   for verbose output
# -std=c++11  to set c++ version
COMPILER_FLAGS = -w

# INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I/usr/include -I/usr/local/include 

# LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L /usr/lib -L/usr/local/lib

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lGLEW -lglut -lGL 

# TARGET specifies the name of our exectuable
TARGET = manyModelsStatic

$(TARGET) :	$(SRC)
	$(CC)  $(SRC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TARGET) 
clean :
	rm -r $(TARGET)
rebuild :
	rm -r $(TARGET)
	make
