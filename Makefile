CXX = g++
RM = rm

CFLAG = `pkg-config --cflags opencv`

LIBS = `pkg-config --libs opencv`

SRC = corp_image.cpp

OBJ = $(SRC:.cpp=.o) 

TARGET = corpImage

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LIBS)

%.o:%.cpp
	$(CXX) -c -o $@ $^

clean:
	$(RM) -rf $(TARGET) $(OBJ)

