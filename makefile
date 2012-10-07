V=0.1

BaseObjects := Agent.o Player.o Terrain.o World.o Game.o Packet.o
ClientObjects := $(BaseObjects) TileSet.o Sprite3D.o GameRenderer.o Client_Main.o
ServerObjects := $(BaseObjects) Server_Main.o

ClientObjects := $(patsubst %.o, build/%.o, $(ClientObjects))
ServerObjects := $(patsubst %.o, build/%.o, $(ServerObjects))

SRCDIR=src/
BUILDDIR=build/

CXX=clang++
WINCXX=i486-mingw32-g++
CFLAGS=-g -DVERSION="\"$V\"" -DDEBUG=false


.PHONY: all
all: linux

.PHONY: linux
linux: smallworld smallworld-server

.PHONY: windows
windows: smallworld.exe smallworld-server.exe

.PHONY: client
client: smallworld smallworld.exe

.PHONY: server
server: smallworld-server smallworld-server.exe


smallworld: $(ClientObjects)
	$(CXX) -o $@ $^ -L../lib/ -lGL -lGLU -lSDL -lIL -Wl,-Bstatic -lenet -ldgeom -ldrenderloop -ldtime -ldengine -Wl,-Bdynamic

smallworld-server: $(ServerObjects)
	$(CXX) -o $@ $^ -L../lib/ -llua -Wl,-Bstatic -lenet -ldgeom -ldtime -Wl,-Bdynamic

$(BUILDDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) -std=c++0x -c -o $@ $< $(CFLAGS) -I../include/

smallworld.exe: $(ClientObjects:.o=.obj)
	$(WINCXX) -o $@ $^ -L../lib/i486-mingw32/ -static-libgcc -static-libstdc++ -Wl,-Bstatic -lenet -ldgeom -ldrenderloop -ldtime -ldengine -Wl,-Bdynamic -lws2_32 -lwinmm -lmingw32 -lSDLmain -lSDL -lDevIL -lopengl32 -lglu32 

smallworld-server.exe: $(ServerObjects:.o=.obj)
	$(WINCXX) -o $@ $^ -L../lib/i486-mingw32/ -static-libgcc -static-libstdc++ -Wl,-Bstatic -lenet -ldgeom -ldtime -Wl,-Bdynamic -lws2_32 -lwinmm -lmingw32 -llua

build/%.obj: src/%.cpp
	$(WINCXX) -std=c++0x -c -o $@ $< $(CFLAGS) -DWIN32 -I../include/

-include .depends

.PHONY: depends
depends:
	@rm -f .depends
	@find . -name "*.cpp" -exec g++ -std=c++11 -MM {} \; >> .depends

#Clean up
.PHONY: clean
clean:
	rm smallworld smallworld-server smallworld.exe smallworld-server.exe build/*
