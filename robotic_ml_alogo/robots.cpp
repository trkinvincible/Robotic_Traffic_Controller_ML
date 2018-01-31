#include "robots.h"
#include "./utils/utils.hpp"

#include "./utils/collision.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Robots::Robots()
{
    m_current_checkpoint = 1 ;
}

void Robots::updateFitness()
{
    if (m_current_checkpoint < 5/*no. of columsn*/)
    {
        if (isDistantNode(m_position,m_previous_position))
        {
            ++m_fitness;
            ++m_current_checkpoint;
        }
    }

    if (isSameNode(m_position,mDestination))
    {
        // this line reward a faster car once the circuit is completed
        m_fitness += (1000.0f / m_total_updates);
        m_alive = false;
    }
}

void	Robots::collideNodes(const Circuit& circuit)
{
    for (t_sensor& sensor : m_sensors)
    {
        sensor.m_value = 1.0f;

        t_vec2f_s checkpoints = circuit.getCheckpoints();
        for (const t_vec2f& node : checkpoints)
        {
            t_vec2f src = sensor.m_line.p1;
            t_vec2f dst = node;
            if(src.x == dst.x && src.y == dst.y)
                continue;
            bool v = isPointOntheLine(sensor.m_line.p1, sensor.m_line.p2,node);

            if (v){
                sensor.m_value = 1;
                break;
            }else{
                sensor.m_value = 0;
            }
        }
    }
}

void Robots::updateSensors()
{
    // initialise

    static const float angles[5] = {
        -M_PI,
        -M_PI/2.0f,
        0.0f,
        M_PI/2.0f,
        M_PI
    };

    for (unsigned int i = 0; i < m_sensors.size(); ++i)
    {
        m_sensors[i].m_line.p1 = t_vec2f();
        m_sensors[i].m_line.p2.x = 200.0f * cosf(angles[i]);
        m_sensors[i].m_line.p2.y = 200.0f * sinf(angles[i]);

        m_sensors[i].m_value = 1.0f;
    }

    // translate/rotate

    for (t_sensor& sensor : m_sensors)
    {
        sensor.m_line.p1.x += m_position.x;
        sensor.m_line.p2.x += m_position.x;
        sensor.m_line.p1.y += m_position.y;
        sensor.m_line.p2.y += m_position.y;

        sensor.m_line.p1 = rotateVec2(sensor.m_line.p1, m_position, m_angle);
        sensor.m_line.p2 = rotateVec2(sensor.m_line.p2, m_position, m_angle);
    }
}

void Robots::update(float step, const Circuit& circuit, const NeuralNetwork& in_NN)
{
    if (std::isnan(m_position.x) ||
        std::isnan(m_position.y) ||
        std::isnan(m_angle))
        m_alive = false;

    if (!m_alive)
        return;

    this->updateSensors();
    this->collideNodes(circuit);
    this->updateFitness();

    //Core Logic is here
    std::vector<float>	input;
    std::vector<float>	output;

    for (t_sensor& sensor : m_sensors)
        input.push_back( sensor.m_value );

    in_NN.process(input, output);

    mDisplay_Output.clear();
    mDisplay_Output.push_back(output.at(0));
    mDisplay_Output.push_back(output.at(1));



    if(output.at(0)-output.at(1)  > 3)
        m_position.x += 0.100f;
    else
        m_position.y += 0.100f;

    ++m_total_updates;
}

void Robots::reset(const Circuit& circuit)
{
    m_angle = circuit.getStartingAngle();
    m_alive = true;
    m_fitness = 0;
    m_total_updates = 0;
    m_trail.clear();
    m_min_updates = 50;
    m_current_checkpoint = 0;
}
