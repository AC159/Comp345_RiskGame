cmake_minimum_required(VERSION 3.21)
project(Comp345RiskGame)

set(CMAKE_CXX_STANDARD 23)

add_executable(
        Comp345RiskGame
        GameEngine/GameEngine.cpp GameEngine/GameEngine.h GameEngine/GameEngineDriver.cpp
        Map/Map.cpp Map/Map.h
        Player/Player.cpp Player/Player.h
        Orders/Orders.cpp Orders/Orders.h
        Cards/Cards.cpp Cards/Cards.h
        main.cpp
        CommandProcessor/CommandProcessing.cpp CommandProcessor/CommandProcessing.h
        Logging/LoggingObserver.cpp Logging/LoggingObserver.h
        PlayerStrategies/PlayerStrategies.cpp PlayerStrategies/PlayerStrategies.h PlayerStrategies/PlayerStrategiesDriver.cpp)
