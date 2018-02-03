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
    sf::Text mEvidenceTextView;

public: // ctor/dtor
    RendererSFML(TrafficController& rk_controller);

public: // method(s)
	void	run(std::function<void()> simulation_callback);
};



#endif // D_RENDERERSFML_H
