#include "robots.h"
#include "./utils/utils.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Robots::Robots()
{

}

void	Robots::collideCheckpoints(const t_lines& checkpoints)
{
//    if (m_current_checkpoint < checkpoints.size())
//    {
//        auto& checkpoint = checkpoints[m_current_checkpoint];
//        if (CollisionSegmentCercle(checkpoint.p1, checkpoint.p2, m_position, 5.0f))
//        {
//            m_min_updates = 50;
//            ++m_fitness;
//            ++m_current_checkpoint;
//        }
//    }

//    if (m_current_checkpoint >= checkpoints.size())
//    {
//        // this line reward a faster car once the circuit is completed
//        m_fitness += (1000.0f / m_total_updates);

//        m_alive = false;
//    }
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

    //
    // min update

    if (m_min_updates > 0)
        --m_min_updates;
    if (m_min_updates == 0)
    {
        // m_min_updates = 50;
        m_alive = false;
    }

    // min update
    //
    //

    // m_fitness += 0.01f;

    this->updateSensors();
//    this->collideCheckpoints( circuit.getCheckpoints() );

    // return;

    //


    std::vector<float>	input;
    std::vector<float>	output;

    for (t_sensor& sensor : m_sensors)
        input.push_back( sensor.m_value );

    // if (input.size() != 5)
    //     throw std::invalid_argument( "about to use an invalid number of inputs" );

    in_NN.process(input, output);

    // if (output.size() != 2)
    //     throw std::invalid_argument( "received invalid number of outputs" );

    float leftTheta		= output[0];
    float rightTheta	= output[1];

    if (std::isnan(leftTheta))	leftTheta = 0.0f;
    if (std::isnan(rightTheta))	rightTheta = 0.0f;



    float steer_range = M_PI/32.0f;
    leftTheta = std::min(steer_range, std::max(-steer_range, leftTheta));
    m_angle += leftTheta * step;

    float speed = rightTheta * 3.0f;
    speed = std::min(15.0f, std::max(-10.0f, speed));





    t_vec2f	prev_pos = m_position;

    m_position.x += (speed * cosf(m_angle)) * step;
    m_position.y += (speed * sinf(m_angle)) * step;


    m_trail.push_back( t_line(prev_pos, m_position) );

    ++m_total_updates;
}

void Robots::reset(const Circuit& circuit)
{
    m_position = circuit.getStartingPositon();
    m_angle = circuit.getStartingAngle();

    m_alive = true;
    m_fitness = 0;
    m_total_updates = 0;
    m_trail.clear();
    m_min_updates = 50;
    m_current_checkpoint = 0;
}
