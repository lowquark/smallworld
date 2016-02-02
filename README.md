# smallworld
A small netcode driven game I wrote in 2012, recently dug up to test new core engine stuff.

# New Stuff
Lately I've been working on a more flexible "GameState/View" based approach to a game model.

Ideally, game code ( a Lua script ) will modify a GameState object (via Zone) which will then publish changes to views.
Views lend themselves nicely to network synchronization, and even client side prediction. Player objects contain a view
(whether spectator or not) and send their input to the game code ( Lua script ) via Commands.
