#ifndef D_RENDERERSFML_H
#define D_RENDERERSFML_H


#include "./TrafficController.h"
#include <functional>
#include "./utils/utils.hpp"
#include "./utils/TraceLogger.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class RendererSFML
{
private: // attributs
    TrafficController &	mController;
    sf::Font mFont;
    typedef std::vector<Robots> vec_robots;
    typedef std::vector<std::vector<Robots>> vec_robots_l;

public: // ctor/dtor
    RendererSFML(TrafficController& rk_controller);

public: // method(s)
	void	run(std::function<void()> simulation_callback);
};



#endif // D_RENDERERSFML_H
