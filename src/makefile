V=0.1

BaseObjects=Player.o Game.o World.o Agent.o Packet.o
ClientObjects=Client_Main.o GameRenderer.o Sprite3D.o TileSet.o
ServerObjects=Server_Main.o


CXX=clang++
WINCXX=i486-msvc-g++
CFLAGS=-g -DVERSION="\"$V\"" -DDEBUG=false


.PHONY: all
all: client server

.PHONY: client
client: ../smallworld

.PHONY: server
client: ../smallworld-server

../smallworld: $(ClientObjects) $(BaseObjects)
	$(CXX) -o $@ $^ -L../../lib/ -lGL -lGLU -lSDL -lIL -Wl,-Bstatic -lenet -ldgeom -ldrenderloop -ldtime -ldengine -Wl,-Bdynamic

../smallworld-server: $(ServerObjects) $(BaseObjects)
	$(CXX) -o $@ $^ -L../../lib/ -llua -Wl,-Bstatic -lenet -ldgeom -ldtime -Wl,-Bdynamic

%.o: %.cpp
	$(CXX) -std=c++0x -c -o $@ $< $(CFLAGS) -I../../include/

-include .depends

.PHONY: depends
depends:
	@rm -f .depends
	@find . -name "*.cpp" -exec g++ -std=c++11 -MM {} \; >> .depends

#Clean up
.PHONY: clean
clean:
	rm $(Objects)
