
#include "RendererSFML.h"
#include "./utils/utils.hpp"
#include "./utils/TraceLogger.hpp"


#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>


namespace
{

void drawPoint(sf::RenderWindow& rwindow, const t_vec2f& point, const sf::Color& color)
{
    float size = 10.0f;
    sf::Vertex vertices[] = {
        sf::Vertex(sf::Vector2f(point.x - size, point.y - size), color),
        sf::Vertex(sf::Vector2f(point.x + size, point.y + size), color),
        sf::Vertex(sf::Vector2f(point.x + size, point.y - size), color),
        sf::Vertex(sf::Vector2f(point.x - size, point.y + size), color)
    };

    rwindow.draw(vertices, 4, sf::Lines);
}

void drawLine(sf::RenderWindow& rwindow, const t_line& line, const sf::Color& color, float thickness = 1.0f)
{
    float length = sqrtf( (line.p1.x-line.p2.x)*(line.p1.x-line.p2.x) + (line.p1.y-line.p2.y)*(line.p1.y-line.p2.y) );

    sf::RectangleShape recline(sf::Vector2f(length, thickness));

    recline.setOrigin(0, thickness/2);
    recline.setPosition(line.p1.x, line.p1.y);

    recline.setFillColor(color);

    float angle = atan2f(line.p2.y - line.p1.y, line.p2.x - line.p1.x);

    recline.rotate(angle * 180.0f / 3.14f);
    rwindow.draw(recline);
}

void drawRobot(sf::RenderWindow& rwindow, const Robots& vehical, const sf::Color& color,bool render_sensors)
{
    const t_vec2f& position = vehical.getPosition();
    float radius = 20.0f;

    sf::Color col = (vehical.isAlive() ? sf::Color::Green : sf::Color::Yellow);

    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setOutlineColor(sf::Color::Red);
    circle.setOutlineThickness(5);
    circle.setPosition(position.x, position.y);
    rwindow.draw(circle);

    if (!vehical.isAlive() || !render_sensors)
        return;

    const Robots::t_sensors&	sensors = vehical.getSensors();

    for (Robots::t_sensor sensor : sensors)
    {
        sf::Color	LightBlue(128,128,255);
        // drawLine(rwindow, sensor.m_line, LightBlue, LightBlue);

        t_vec2f	pos;
        pos.x = sensor.m_line.p1.x + (sensor.m_line.p2.x - sensor.m_line.p1.x) * sensor.m_value;
        pos.y = sensor.m_line.p1.y + (sensor.m_line.p2.y - sensor.m_line.p1.y) * sensor.m_value;

        t_line	tmp_line(sensor.m_line.p1, pos);
        t_line	tmp_line2(pos, sensor.m_line.p2);

        // drawLine(rwindow, tmp_line, LightBlue, LightBlue);
        drawLine(rwindow, tmp_line, LightBlue, 5.0f);
        drawLine(rwindow, tmp_line2, sf::Color::Red, 1.0f);

        drawPoint(rwindow, pos, sf::Color::Yellow);
    }
}

void drawSquare(sf::RenderWindow& rwindow,int side,int x,int y)
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(side,side));
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(5);
    rectangle.setPosition(x, y);
    rectangle.setFillColor(sf::Color());
    rwindow.draw(rectangle);
}

void drawGrap(sf::RenderWindow& rwindow,const std::pair<int,int> &grid_degree,int side_lenght,const std::vector<int> &skip_index)
{
    int row    = grid_degree.first;
    int column = grid_degree.second;
    int y=200;
    int xoffest=50;
    int count = 1;

    for(int j=1;j<=row;j++){

        for(int i=1;i <= column;i++){

            if(std::find(skip_index.begin(),skip_index.end(),count) != skip_index.end()){
                count++;
                continue;
            }
            drawSquare(rwindow,side_lenght,(i*side_lenght)+xoffest,y);
            count++;
        }
      y+=side_lenght;
    }
}

}; // namespace


RendererSFML::RendererSFML(TrafficController& rk_controller)
    : mController(rk_controller)
{
}

void  RendererSFML::run(std::function<void()> callback)
{
    const std::vector<t_vec2f>&	checkpoints = mController.getCircuit().getCheckpoints();

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML window");
    window.setFramerateLimit(60);
    int	index_target_robot = -1;

    while (window.isOpen())
    {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Request for closing the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Activate the window for OpenGL rendering
        window.setActive();
        //Update Cordinates for drawning
        {
            for (int i = 0; i < 50; ++i)
                callback();
        }
        // Clear screen
        window.clear();

        // render circuit
        std::pair<int,int> grid_degree = std::make_pair<int,int>(2,5);
        std::vector<int> skip_index = {2,3,4,8};
        drawGrap(window,grid_degree,150,skip_index);

        //Get the fittest Robot
        {
            auto vec_robots = mController.getTragetVechicals();
            float	curr_fitness = -1;
            for (unsigned int ii = 0; ii < vec_robots.size(); ++ii)
            {
                auto& vehical = vec_robots[ii];
                if (!vehical.isAlive())
                    continue;
                if (curr_fitness > vehical.getFitness())
                    continue;
                curr_fitness = vehical.getFitness();
                index_target_robot = ii;
            }
        }
        //render robots
        {
            // render cars (except targetted car)
            auto vec_robots = mController.getTragetVechicals();
            for (int index = 0; index < static_cast<int>(vec_robots.size()); ++index)
                if (index != index_target_robot)
                    drawRobot(window, vec_robots[index], sf::Color::Green, false);

            // render targetted car
            if (index_target_robot != -1)
                drawRobot(window, vec_robots[index_target_robot], sf::Color::Blue, true);
        }
        // End the current frame and display its contents on screen
        window.display();
    }
}


