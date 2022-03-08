cmake_minimum_required(VERSION 3.21)
project(Comp345RiskGame)

set(CMAKE_CXX_STANDARD 23)

add_executable(
        Comp345RiskGame
        GameEngine/GameEngine.cpp GameEngine/GameEngine.h GameEngine/GameEngineDriver.cpp
        Map/Map.cpp Map/Map.h Map/MapDriver.cpp
        Player/Player.cpp Player/Player.h
        Orders/Orders.cpp Orders/Orders.h Orders/OrdersDriver.cpp
        Cards/Cards.cpp Cards/Cards.h Cards/CardsDriver.cpp
        main.cpp
        CommandProcessor/CommandProcessing.cpp CommandProcessor/CommandProcessing.h CommandProcessor/CommandProcessingDriver.cpp
        Logging/LoggingObserver.cpp Logging/LoggingObserver.h
        Logging/LoggingDriver.cpp)
