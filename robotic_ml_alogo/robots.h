#ifndef ROBOTS_H
#define ROBOTS_H

#include <circuit.h>
#include <array>
#include "./ai/NeuralNetwork.h"

class Robots
{
public: // external structures
    struct t_sensor
    {
        t_line	m_line;
        float	m_value;
    };
    typedef std::array<t_sensor, 4>	t_sensors;

    std::vector<float>	mDisplay_Output;

private: // attributs
    //destination values
    t_vec2f mDestination;

    t_vec2f	m_position;
    t_vec2f	m_previous_position;
    float	m_angle;
    float	m_fitness;
    bool	m_alive;
    unsigned int	m_min_updates;
    unsigned int	m_total_updates;

    // TODO : use enumeration
    t_sensors	m_sensors;

    unsigned int	m_current_checkpoint;

    std::vector<t_line>	m_trail;

public: // ctor/dtor
    Robots();

public: // methods
    std::vector<float> getNNOutPut(){return mDisplay_Output;}
    void    setDestination(t_vec2f dest){mDestination = dest;}
    void    setSource(t_vec2f src){m_position = src;}
    void	update(float step, const Circuit& circuit, const NeuralNetwork& nn);
    void	reset(const Circuit& circuit);

private: // methods
    void updateFitness();
    void updateSensors();
    void collideNodes(const Circuit& circuit);

public: // setter/getter

    inline const int	getRadius() const { return 20; }
    inline const t_vec2f&	getPosition() const { return m_position; }
    inline float			getAngle() const { return m_angle; }

    inline const t_sensors&	getSensors() const { return m_sensors; }


    inline float	getFitness() const { return m_fitness; }
    // inline float	getFitness() const { return (m_fitness + (1000.0f / m_total_updates)); }
    // inline float	getFitness() const { return (m_fitness + (m_total_updates / 1000.0f)); }
    inline bool		isAlive() const { return m_alive; }

    // inline bool		getLife() const { return (m_min_updates / 50.0f); } // TODO: this is hardcoded :(
    float			getLife() const;

    inline unsigned int	getTotalUpdates() const { return m_total_updates; }
};

#endif // ROBOTS_H
