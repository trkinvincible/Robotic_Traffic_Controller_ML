#ifndef D_RENDERERSFML_H
#define D_RENDERERSFML_H


#include "./TrafficController.h"
#include <functional>


class RendererSFML
{
private: // attributs
    TrafficController &	mController;

public: // ctor/dtor
    RendererSFML(TrafficController& rk_controller);

public: // method(s)
	void	run(std::function<void()> simulation_callback);
};



#endif // D_RENDERERSFML_H
