CXX      = $(shell fltk-config --cxx)
CXXFLAGS = $(shell fltk-config --cxxflags ) -I.
LDSTATIC = $(shell fltk-config --ldstaticflags )
LINK     = $(CXX)

TARGET = minesweeper
OBJS = CellBox.o Score.o GameWindow.o HighscoreWindow.o \
       CustomWindow.o Minesweeper.o Main.o
SRCS = CellBox.cpp Score.cpp GameWindow.cpp HighscoreWindow.cpp \
       CustomWindow.cpp Minesweeper.cpp Main.cpp

.SUFFIXES: .o .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -g $<

all: $(TARGET)
	$(LINK) -o $(TARGET) $(OBJS) $(LDSTATIC)

$(TARGET): $(OBJS)
CellBox.o: CellBox.cpp CellBox.h
Score.o: Score.cpp Score.h
GameWindow.o: GameWindow.cpp GameWindow.h HighscoreWindow.h CustomWindow.h CellBox.h
HighscoreWindow.o: HighscoreWindow.cpp HighscoreWindow.h GameWindow.h
CustomWindow.o: CustomWindow.cpp CustomWindow.h GameWindow.h
Minesweeper.o: Minesweeper.cpp Minesweeper.h GameWindow.h CellBox.h
Main.o: Main.cpp Minesweeper.h GameWindow.h

.PHONY: clean
clean:
	rm -f *.o $(TARGET)

