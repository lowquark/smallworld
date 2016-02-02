V=0.1

BaseObjects := Agent Player Terrain World Game Packet
ClientObjects := $(BaseObjects) TileSet Sprite3D GameRenderer Client_Main
ServerObjects := $(BaseObjects) Server_Main

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


smallworld: $(patsubst %, build/%.o, $(ClientObjects))
	$(CXX) -o $@ $^ -L../lib/ -lGL -lGLU -lSDL -lIL -Wl,-Bstatic -lenet -ldgeom -ldrenderloop -ldtime -ldengine -Wl,-Bdynamic

smallworld-server: $(patsubst %, build/%.o, $(ServerObjects))
	$(CXX) -o $@ $^ -L../lib/ -llua -Wl,-Bstatic -lenet -ldgeom -ldtime -Wl,-Bdynamic

$(BUILDDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) -std=c++0x -c -o $@ $< $(CFLAGS) -I../include/

smallworld.exe: $(patsubst %, build/%.obj, $(ClientObjects))
	$(WINCXX) -o $@ $^ -L../lib/i486-mingw32/ -static-libgcc -static-libstdc++ -Wl,-Bstatic -lenet -ldgeom -ldrenderloop -ldtime -ldengine -Wl,-Bdynamic -lws2_32 -lwinmm -lmingw32 -lSDLmain -lSDL -lDevIL -lopengl32 -lglu32 

smallworld-server.exe: $(patsubst %, build/%.obj, $(ServerObjects))
	$(WINCXX) -o $@ $^ -L../lib/i486-mingw32/ -static-libgcc -static-libstdc++ -Wl,-Bstatic -lenet -ldgeom -ldtime -Wl,-Bdynamic -lws2_32 -lwinmm -lmingw32 -llua

build/%.obj: $(SRCDIR)%.cpp
	$(WINCXX) -std=c++0x -c -o $@ $< $(CFLAGS) -DWIN32 -I../include/

-include $(SRCDIR).depends

.PHONY: depends
depends:
	@rm -f $(SRCDIR).depends
	@for x in $(sort $(BaseObjects) $(ClientObjects) $(ServerObjects));\
	  do\
	    echo $$x;\
		g++ -std=c++11 -MT $(BUILDDIR)$$x.o -MM $(SRCDIR)$$x.cpp >> $(SRCDIR).depends ;\
	  done

#Clean up
.PHONY: clean
clean:
	rm smallworld smallworld-server smallworld.exe smallworld-server.exe build/*
