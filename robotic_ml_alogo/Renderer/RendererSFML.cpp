#include "RendererSFML.h"

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

void drawRobot(sf::RenderWindow& rwindow, const Robots& vehical, const sf::Color& color,bool render_sensors,sf::Font &font)
{
    const t_vec2f& position = vehical.getPosition();
    float radius = vehical.getRadius();

    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setOutlineColor(sf::Color::Red);
    circle.setOutlineThickness(3);
    circle.setFillColor(color);
    circle.setPosition(position.x-(radius), position.y-(radius));

    rwindow.draw(circle);

    if (!vehical.isAlive() || !render_sensors){
#if 0
        sf::Font font;
        font.loadFromFile("./arial.ttf");
        sf::Text text;
        text.setFont(font);
        text.setColor(sf::Color::Red);
        text.setPosition(position.x-(radius), position.y-(radius));
        std::string f;
        std::stringstream ss(f);
        ss<<vehical.getFitness();
        text.setString(ss.str());
        rwindow.draw(text);
#endif
        return;
    }

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
    sf::Text text;
    text.setFont(font);
    text.setColor(sf::Color::White);
    text.setPosition(position.x, position.y);
    std::string f;
    std::stringstream ss(f);
    ss<<vehical.getdistance();
    text.setString(ss.str());
    rwindow.draw(text);
}

void drawSquare(sf::RenderWindow& rwindow,int side,int x,int y)
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(side,side));
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(4);
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


} // namespace


RendererSFML::RendererSFML(TrafficController& rk_controller)
    : mController(rk_controller)
{
    mFont.loadFromFile("./arial.ttf");
}

void  RendererSFML::run(std::function<void()> callback)
{
    const std::vector<t_vec2f>&	checkpoints = mController.getCircuit().getCheckpoints();

    sf::Text Textview;
    Textview.setFont(mFont);
    Textview.setColor(sf::Color::White);

    // Create the main window
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(sf::VideoMode(desktopMode.width,
                            desktopMode.height,
                            desktopMode.bitsPerPixel), "SFML window");
    window.setFramerateLimit(60);
    int	index_target_robot = -1;

    while (window.isOpen())
    {
        Textview.setPosition(1100,300);

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
            for (int i = 0; i < 1; ++i)
                callback();
        }
        // Clear screen
        window.clear();

        // render circuit
        std::pair<int,int> grid_degree = std::make_pair<int,int>(5,5);
        std::vector<int> skip_index = {1,2,4,5,6,10,16,20,21,22,24,25};
        drawGrap(window,grid_degree,150,skip_index);

        std::vector<int> index_fittest_robots;

        //Get the fittest Robot
        {
            auto vec_vec_robots = mController.getTragetVechicals();

            //competeing robots
            for(vec_robots &vec_robots : vec_vec_robots){

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
                index_fittest_robots.push_back(index_target_robot);
            }
        }
        //render robots
        {
             auto vec_vec_robots = mController.getTragetVechicals();
             std::vector<sf::Color> some_colors={sf::Color::Yellow,sf::Color::Blue,sf::Color::Cyan,sf::Color::Green};

             int vehicle_count = 0;
             int color_index=0;

             //competeing robots
             for(vec_robots &vec_robots : vec_vec_robots){

                 sf::CircleShape circle1;
                 circle1.setRadius(25);
                 circle1.setFillColor(some_colors.at(color_index));

                 index_target_robot = index_fittest_robots.at(vehicle_count);

                 // render robots (except targetted robot)
                 for (int index = 0; index < static_cast<int>(vec_robots.size()); ++index)
                     if (index != index_fittest_robots.at(vehicle_count)){

                         drawRobot(window, vec_robots[index], some_colors.at(color_index), false,mFont);
                     }

                 color_index++;
                 sf::CircleShape circle2;
                 circle2.setRadius(25);
                 circle2.setFillColor(some_colors.at(color_index));

                 // render targetted robot
                 if (index_fittest_robots.at(vehicle_count) != -1){

                     drawRobot(window, vec_robots[index_fittest_robots.at(vehicle_count)],some_colors.at(color_index) , true,mFont);
                 }

                 color_index++;

                 std::string s;
                 std::stringstream ss(s);

                 //ss << std::string("NN output[0]: ") << vec_robots[index_target_robot].getNNOutPut().at(0) << std::endl;
                 //ss << std::string("NN output[1]: ") << vec_robots[index_target_robot].getNNOutPut().at(1) << std::endl;

                 //ss << std::string("Sensor-back: ") << vec_robots[index_target_robot].getSensors().at(0).m_value << std::endl;
                 //ss << std::string("Sensor-down: ") << vec_robots[index_target_robot].getSensors().at(1).m_value << std::endl;
                 //ss << std::string("Sensor-right: ") << vec_robots[index_target_robot].getSensors().at(2).m_value << std::endl;
                 //ss << std::string("Sensor-top: ") << vec_robots[index_target_robot].getSensors().at(3).m_value << std::endl;

                 ss << std::string("Robot ") << index_target_robot << std::string("  is Alive") << std::endl;

                 //ss << std::string("Robot X: ") << vec_robots[index_target_robot].getPosition().x << std::endl;
                 //ss << std::string("Robot Y: ") << vec_robots[index_target_robot].getPosition().y << std::endl;

                 ss << std::string("Generation: ") << mController.getCurrentGeneration(vehicle_count) << std::endl;

                 ss << std::string("Robot Fitness: ") << vec_robots[index_target_robot].getFitness() << std::endl;

                 //ss << std::string("Robot total update: ") << vec_robots[index_target_robot].getTotalUpdates() << std::endl;
                 ss << std::string("Robot trapped count: ") << vec_robots[index_target_robot].gettrappedcount() << std::endl;

                 ss << std::string("Robot Status: ") << vec_robots[index_target_robot].getLog() << std::endl;

                 ss << std::string("Success Rate: ") << mController.getSuccessRate() << std::endl;

                 //ss << std::string("Max fitness ") << Robots::max_fitness_robo << std::endl;

                 Textview.setString(ss.str());

                 Textview.setPosition(1100,Textview.getPosition().y + (vehicle_count*300));

                 //display who are competing

                 circle1.setPosition(Textview.getPosition().x, Textview.getPosition().y - 50);
                 circle2.setPosition(circle1.getPosition().x + 50, circle1.getPosition().y);

                 window.draw(circle1);window.draw(circle2);

                 window.draw(Textview);

                 vehicle_count++;
             }
        }
        // End the current frame and display its contents on screen
        window.display();
    }
}


