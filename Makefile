PROG = app
FONTES = Point.cpp Polygon.cpp Detector.cpp

OBJETOS = $(FONTES:.cpp=.o)
CPPFLAGS = -g -O3 -DGL_SILENCE_DEPRECATION -Wno-unused-result -fopenmp

UNAME = `uname`

all: $(TARGET)
	-@make $(UNAME)

Linux: $(OBJETOS)
	gcc $(OBJETOS) -O3 -lGL -lGLU -lglut -lm -lstdc++ -fopenmp -o $(PROG)

clean:
	-@ rm -f $(OBJETOS) $(PROG)