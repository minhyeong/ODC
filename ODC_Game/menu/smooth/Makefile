CC = gcc
DEFINE=
INCLUDE=
CFLAGS=-g -Wall
LDFLAGS=-lglut -lGLU -lSDL

OBJS=smooth.o glm.o tb.o trackball.o
TARGET=smooth

all: $(TARGET)

smooth: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o *~ $(TARGET)