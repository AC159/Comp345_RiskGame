cmake_minimum_required(VERSION 3.21)
project(Comp345RiskGame)

set(CMAKE_CXX_STANDARD 23)


add_executable(Comp345RiskGame
#        main.cpp
#        Map/Map.cpp Map/Map.h Map/MapDriver.cpp
#        Player/Player.cpp Player/Player.h
#        Player/PlayerDriver.cpp
#        Orders/Orders.cpp Orders/Orders.h Orders/OrdersDriver.cpp
        Cards/Cards.cpp Cards/Cards.h Cards/CardsDriver.cpp
#        GameEngine/GameEngine.cpp GameEngine/GameEngine.h GameEngine/GameEngineDriver.cpp
        )
# part 1: map driver
add_executable(A1Part1-Map
        Map/Map.cpp Map/Map.h Map/MapDriver.cpp
        Player/Player.cpp Player/Player.h
        Orders/Orders.cpp Orders/Orders.h
        Cards/Cards.cpp Cards/Cards.h
        GameEngine/GameEngine.cpp GameEngine/GameEngine.h)

# part 2: player driver
add_executable(A1Part2-Player
        Map/Map.cpp Map/Map.h
        Player/Player.cpp Player/Player.h Player/PlayerDriver.cpp
        Orders/Orders.cpp Orders/Orders.h
        Cards/Cards.cpp Cards/Cards.h
        GameEngine/GameEngine.cpp GameEngine/GameEngine.h)

# part 3: orders list
add_executable(A1Part3-Orders
        main.cpp
        Map/Map.cpp Map/Map.h Map/MapDriver.cpp
        Player/Player.cpp Player/Player.h Player/PlayerDriver.cpp
        Orders/Orders.cpp Orders/Orders.h Orders/OrdersDriver.cpp
        Cards/Cards.cpp Cards/Cards.h Cards/CardsDriver.cpp
        GameEngine/GameEngine.cpp GameEngine/GameEngine.h GameEngine/GameEngineDriver.cpp)

# part 4: cards deck/hand
add_executable(A1Part4-Cards
        main.cpp
        Map/Map.cpp Map/Map.h Map/MapDriver.cpp
        Player/Player.cpp Player/Player.h Player/PlayerDriver.cpp
        Orders/Orders.cpp Orders/Orders.h Orders/OrdersDriver.cpp
        Cards/Cards.cpp Cards/Cards.h Cards/CardsDriver.cpp
        GameEngine/GameEngine.cpp GameEngine/GameEngine.h GameEngine/GameEngineDriver.cpp)

# part 5: game engine
add_executable(A1Part5-Engine
        Map/Map.cpp Map/Map.h
        Player/Player.cpp Player/Player.h
        Orders/Orders.cpp Orders/Orders.h
        Cards/Cards.cpp Cards/Cards.h
        GameEngine/GameEngine.cpp GameEngine/GameEngine.h GameEngine/GameEngineDriver.cpp)

# main.cpp driver (test file)
add_executable(A1Test
        main.cpp
        Map/Map.cpp Map/Map.h
        Player/Player.cpp Player/Player.h
        Orders/Orders.cpp Orders/Orders.h
        Cards/Cards.cpp Cards/Cards.h
        GameEngine/GameEngine.cpp GameEngine/GameEngine.h)
