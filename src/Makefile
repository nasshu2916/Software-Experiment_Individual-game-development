CC = gcc

CFLAGS = -Wall $(COPTIONS)

LDFLAGS = 
LOADLIBES =
LDLIBS = -lSDL -lglut -lm -lGLU -lSDL_ttf -lGL -lSDL_image -L/usr/lib -lusb 

TARGET = game
OBJS = $(TARGET).o system.o window.o character.o map.o mob.o voxel.o vr920.o libglpng.a vr920lib.a
HEADS = system.h window.h character.h map.h mob.h voxel.h vr920.h ../glpng.h

# 課題に取り組むにはこのターゲットで
#   -g3 はデバッグ情報を埋め込むオプション
#
debug: CPPFLAGS = 
debug: COPTIONS = -g3
debug: $(TARGET)

# デバッグ情報を埋め込まないターゲット
#   -O3 は最適化オプション
#
release: CPPFLAGS = -DNDEBUG
release: COPTIONS = -O3 
release: $(TARGET)

#
$(TARGET): $(OBJS) 
$(OBJS): $(HEADS) 


.PHONY: clean
clean:
	@rm -f $(TARGET) $(OBJS) core *~
