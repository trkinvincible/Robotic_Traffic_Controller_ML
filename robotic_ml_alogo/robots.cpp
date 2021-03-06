#include "robots.h"
#include "./utils/utils.hpp"

#include "./utils/collision.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <bitset>
#include <unistd.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::map<int,int> direction_angles={{0,270},{1,0},{2,90},{3,180}};

int Robots::max_fitness_robo=-1;

Robots::Robots()
{
    success_rate = 0;
    m_current_checkpointX = 0 ;
    m_current_checkpointY = 0 ;
}

void Robots::updateFitness()
{
    int x_pos = isDistantNodeX(m_position,mSource);
    int y_pos = isDistantNodeY(m_position,mSource);

    if(x_pos && x_pos > m_current_checkpointX)
    {
        trapped_count = 0;
        m_current_checkpointX = x_pos;
    }else{

        if(trapped_count > 50){

            m_alive = false;
        }
    }
    if(y_pos && y_pos > m_current_checkpointY)
    {
        trapped_count = 0;
        m_current_checkpointY = y_pos;
    }else{

        if(trapped_count > 50){

            m_alive = false;
        }
    }

    if(!m_alive) return;

    //credit the robo which getting close to its destination
    int distancex = (mDestination.x - m_position.x) * (mDestination.x - m_position.x);
    int distancey = (mDestination.y - m_position.y) * (mDestination.y - m_position.y);

    double distance = std::sqrt(distancex + distancey);
    if(distance < mDistancetoTarget && distance < mBestDistance){

        mBestDistance = distance;
        ++m_fitness;
        trapped_count=0;
        if(max_fitness_robo < m_fitness)
        {
            max_fitness_robo = m_fitness;
        }
    }else if(mBestDistance > 150){

        //when they move away reduce fitness
        float temp = m_fitness-1;
        m_fitness == std::min(0.0f,temp);
    }
    //Destination reached?
    if (isSameNode(m_position,mDestination))
    {
        // this line reward a faster robo once the mission is completed
        m_fitness += 1000.0f / m_total_updates;
        m_alive = false;
//        sleep(2);
        success_rate++;
        has_reached = true;
    }
}

void	Robots::collideNodes(const Circuit& circuit)
{
    int void_sensor_count = 0;

    for (t_sensor& sensor : m_sensors)
    {
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
                //Don check for all possible nodes just check which is near you.
                break;
            }else{
                sensor.m_value = 0;
            }
        }
        if(sensor.m_value == 0){

            void_sensor_count++;
        }
    }
    //3 sides have NO nodes to go
    if(void_sensor_count >= 3){

        m_alive = false;
        float diff = std::min(0.0f,max_fitness_robo - m_fitness);
        if(diff > 0.0f && diff < 50.0f){
            max_fitness_robo = -1;

        }
//        sleep(2);
    }
}

#if 1
void Robots::collideAnotherRobot(const std::vector<t_vec2f> opponent_pos)
{
    int index=0;
    while(index < opponent_pos.size()){

        t_vec2f opp_pos = opponent_pos.at(index);

        if (isSameNode(m_position,opp_pos))
        {
            m_alive = false;
        }
        index++;
    }
}
#endif

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

        sensor.m_line.p1 = rotateVec2(sensor.m_line.p1, m_position, 0);
        sensor.m_line.p2 = rotateVec2(sensor.m_line.p2, m_position, 0);
    }
}

void Robots::update(float step, const Circuit& circuit, const NeuralNetwork& in_NN,std::vector<t_vec2f> oponent_pos)
{
    if (std::isnan(m_position.x) ||std::isnan(m_position.y))
        m_alive = false;

    this->updateSensors();
    this->collideNodes(circuit);
    collideAnotherRobot(oponent_pos);
    this->updateFitness();

    if(has_reached){

        std::string info;
        std::stringstream ss(info);

        std::string output_file("../weights.txt");
        std::ofstream myfile(output_file,std::fstream::app);
        ss << "source: " << mSource.toString() << "\ndestination: " << mDestination.toString() << std::endl;
        std::vector<float> log_weights;
        in_NN.getWeights(log_weights);
        ss << "weights:\n";
        for(auto item : log_weights){

            ss << "  " << item;
        }
        ss << std::endl;
        myfile << ss.str();
        myfile.close();
    }


    if (!m_alive)
        return;

    //Core Logic is here
    std::vector<float>	input;
    std::vector<float>	output;

    std::string info;
    std::stringstream ss(info);

    std::bitset<4> no_of_side_unlocked;
    int i = 3;
    for (t_sensor& sensor : m_sensors){

        if(sensor.m_value == 1){

            no_of_side_unlocked.set(i);
        }
        i--;
        input.push_back( sensor.m_value );
    }

    in_NN.process(input, output);

    mDisplay_Output.clear();
    mDisplay_Output.push_back(output.at(0)); //Direction (Can be used to calculate Angle too later)
    mDisplay_Output.push_back(output.at(1)); //Speed

    float direction	=0;
    int unlocked_count = no_of_side_unlocked.count();
    if(no_of_side_unlocked.any()){

        direction	= static_cast<int>(std::abs(output[0]*10)) % unlocked_count/*possible sides*/;
    }

    float speed;
    float dummy_speed;
    if(unlocked_count >= 3 && m_fitness > 1){
        speed	    = std::min(50.0f,output[1]*10.0f) * 1.5f;
        dummy_speed = std::min(50.0f,output[1]*10.0f) * 1.5f;
    }else{
        speed = std::max(50.0f,output[1]*10.0f) * 1.5f;
        dummy_speed = std::max(50.0f,output[1]*10.0f) * 1.5f;
    }

#if 1
    //std::lock_guard<std::mutex> lock(mtx);
    //std::string output_file("../../output.txt");
    //std::ofstream myfile(output_file,std::fstream::app);
    //ss << std::abs(output.at(0)-output.at(1)) << std::endl;
    ss << "available directions: " << no_of_side_unlocked.to_string();
    ss << "\tdirection: " << direction << std::endl;
    mLog = ss.str();
    //myfile << ss.str();
    //myfile.close();
#endif

    ++m_total_updates;
    trapped_count++;

    if (no_of_side_unlocked.test(direction) == false){

//        std::cout << "No luck!!!" << std::endl;
        if(mCurrentAngle == 0 || mCurrentAngle == 180){
            m_position.x += (dummy_speed * cosf(m_angle));
        }else if(mCurrentAngle == 90 || mCurrentAngle == 270){
            m_position.y += (dummy_speed * sinf(m_angle));
        }
        return;
    }
    std::map<int,int>::iterator itr = direction_angles.find(direction);
    if(itr != direction_angles.end())
        m_angle = itr->second;

    mCurrentAngle = m_angle;

    if(m_angle == 0 || m_angle == 180){
        m_position.x += (speed * cosf(m_angle));
    }else if(m_angle == 90 || m_angle == 270){
        m_position.y += (speed * sinf(m_angle));
    }
}

void Robots::reset(const t_vec2f start,const t_vec2f stop)
{
    setSource(start);
    setDestination(stop);
    m_alive = true;
    m_fitness = 0;
    m_total_updates = 0;
    m_trail.clear();
    m_min_updates = 50;
    m_current_checkpointX = 0;
    m_current_checkpointY = 0;
    success_rate = 0;
    trapped_count = 0;
    max_fitness_robo = -1;
}
