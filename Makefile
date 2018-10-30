#Tim Schisler
#University of Missouri - St. Louis
#CS 5420 Fall 2018
#Assignment 3: Image Enhancement in Spatial Domain
#10/24/2018
CC	= g++
CFLAGS = -g `pkg-config --cflags opencv`

TARGET1	= Q1
OBJ1	= q1.o

TARGET2	= Q2
OBJ2	= q2.o

TARGET3	= Q3
OBJ3	= q3.o

TARGET4	= Q4
OBJ4	= q4.o

TARGET5	= Q5
OBJ5	= q5.o

TARGET6	= Q6
OBJ6	= q6.o

OBJC = HW3.o

OUTS = Q2out.jpg

LIBS	= `pkg-config --libs opencv`
STD	= -std=c++11

all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6)

$(TARGET1): $(OBJ1) $(OBJC)
	$(CC) -o $@ $(STD) $(OBJ1) $(OBJC) $(LIBS)

$(TARGET2): $(OBJ2) $(OBJC)
	$(CC) -o $@ $(STD) $(OBJ2) $(OBJC) $(LIBS)

$(TARGET3): $(OBJ3) $(OBJC)
	$(CC) -o $@ $(STD) $(OBJ3) $(OBJC) $(LIBS)

$(TARGET4): $(OBJ4) $(OBJC)
	$(CC) -o $@ $(STD) $(OBJ4) $(OBJC) $(LIBS)

$(TARGET5): $(OBJ5) $(OBJC)
	$(CC) -o $@ $(STD) $(OBJ5) $(OBJC) $(LIBS)

$(TARGET6): $(OBJ6) $(OBJC)
	$(CC) -o $@ $(STD) $(OBJ6) $(OBJC) $(LIBS)

.SUFFIXES: .cpp .o
.cpp.o:
	$(CC) $(CFLAGS) $(STD)  -c $<

.PHONY: clean
clean:
	/bin/rm -f *.o $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(OUTS)
