LFLAG1 = -lSDL -L/usr/lib -lGL -lSDL_image -lSDL_ttf -lSDL_gfx -lGLU -O2 -Wall -lglut -lm -lSDL_mixer

FUNCS1 = main.c window.c init.c joystick.c object.c Loop_Title.c Finalize.c Loop_Game.c Cellect.c setup.c texture_sample.c
FUNCS2 = ./smooth/tb.c ./smooth/trackball.c ./smooth/glm.c ./smooth/gltx.c ./smooth/smooth.c

OBJCS1 = main.o window.o init.o joystick.o object.o Loop_Title.o Finalize.o  Loop_Game.o Cellect.o setup.o texture_sample.o
OBJCS2 = tb.o trackball.o glm.o gltx.o smooth.o

.PHONY: clean

odc	: main.c
	gcc -g -c $(FUNCS1) $(FUNCS2)
	gcc -g -o odc $(OBJCS1) $(OBJCS2) $(LFLAG1)
clean:
	rm -rf *.o *~ odc core
