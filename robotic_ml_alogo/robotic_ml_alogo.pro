TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#this game is fun where Gamer need to challenge the robot which
#move from Right-Left and Gamer Move from Left-Right
#who reach first with the shortest path will win

        ################################################################
        #                               #                       #      #
        #                                         #                    #
        #       #         #                                            #
#Robot  #                                                  #           # #Gamer
        #                                                              #
        #                      #         #                             #
        #              #                                         #     #
        #                                          #                   #
        ################################################################

INCLUDEPATH += ~/Documents/ML/SFML-2.4.2/include

LIBS += -lsfml-graphics
LIBS += -lsfml-graphics
LIBS += -lsfml-window
LIBS += -lsfml-system
LIBS += -lGL
#LIBS += -lGLU
#LIBS += -lGLEW
#LIBS += -lglut

SOURCES += main.cpp \
    circuit.cpp \
    robots.cpp \
    TrafficController.cpp \
    ai/GeneticAlgorithm.cpp \
    ai/NeuralNetwork.cpp \
    ai/NeuralNetworkTopology.cpp \
    Renderer/RendererSFML.cpp \
    utils/collision.cpp \
    utils/randomiser.cpp \
    utils/t_line.cpp \
    utils/t_vec2f.cpp \
    utils/TraceLogger.cpp \
    utils/utils.cpp

HEADERS += \
    circuit.h \
    robots.h \
    TrafficController.h \
    ai/NeuralNetworkTopology.h \
    ai/NeuralNetwork.h \
    ai/GeneticAlgorithm.h \
    Renderer/RendererSFML.h \
    utils/collision.hpp \
    utils/randomiser.hpp \
    utils/t_line.hpp \
    utils/t_vec2f.hpp \
    utils/TraceLogger.hpp \
    utils/utils.hpp
