
GAME_OBJECTS := build/game/server.o build/game/resource.o build/game/Zone.o build/game/Player.o
CLIENT_OBJECTS := build/client/main.o $(GAME_OBJECTS)

smallworld: $(CLIENT_OBJECTS)
	clang++ -std=c++11 -o $@ $^ -lenet

build/%.o: src/%.cpp
	@mkdir --parents $(@D)
	clang++ -std=c++11 -Isrc/ -c -o $@ $<

