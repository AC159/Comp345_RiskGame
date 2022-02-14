cmake_minimum_required(VERSION 3.21)
project(Comp345RiskGame)

set(CMAKE_CXX_STANDARD 23)


#add_executable(
#    Cards
#    Map/Map.cpp Map/Map.h
#    Orders/Orders.cpp Orders/Orders.h
#    Player/Player.cpp Player/Player.h
#    Cards/Cards.cpp Cards/Cards.h
#    Cards/CardsDriver.cpp
#)

#add_executable(
#        Player
#        Map/Map.cpp Map/Map.h
#        Orders/Orders.cpp Orders/Orders.h
#        Player/Player.cpp Player/Player.h
#        Player/PlayerDriver.cpp
#        Cards/Cards.cpp Cards/Cards.h
#)

#add_executable(
#        Orders
#        Orders/Orders.cpp Orders/Orders.h Orders/OrdersDriver.cpp
#)

#add_executable(
#    Map
#    Map/Map.cpp Map/Map.h Map/MapDriver.cpp
#    Player/Player.cpp Player/Player.h
#    Orders/Orders.cpp Orders/Orders.h
#    Cards/Cards.cpp Cards/Cards.h
#)

add_executable(
        GameEngine
        GameEngine/GameEngine.cpp GameEngine/GameEngine.h GameEngine/GameEngineDriver.cpp
        Map/Map.cpp Map/Map.h
        Player/Player.cpp Player/Player.h
        Orders/Orders.cpp Orders/Orders.h
        Cards/Cards.cpp Cards/Cards.h
)
