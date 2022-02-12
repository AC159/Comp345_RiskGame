cmake_minimum_required(VERSION 3.21)
project(Comp345RiskGame)

set(CMAKE_CXX_STANDARD 23)


add_executable(Comp345RiskGame
#        main.cpp
        Player/Player.cpp Player/Player.h
        Player/PlayerDriver.cpp
        Map/Map.cpp Map/Map.h
#        Map/MapDriver.cpp
        Orders/Orders.cpp Orders/Orders.h
#        Orders/OrdersDriver.cpp
        Cards/Cards.cpp Cards/Cards.h
        # Cards/CardsDriver.cpp
#        GameEngine/GameEngine.cpp GameEngine/GameEngine.h GameEngine/GameEngineDriver.cpp
        )
