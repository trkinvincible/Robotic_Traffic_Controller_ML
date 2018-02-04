#include "circuit.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>

Circuit::Circuit()
{

}

bool Circuit::LoadPath(std::string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ifstream::in);

    if (!ifs.is_open())
    {
        return false;
    }

    // loop
    std::string line;
    std::string label;

    while (ifs.good())
    {
        std::getline(ifs, line);

        if (line.empty())
        {
            continue;
        }

        std::stringstream sstr(line);
        sstr >> label;

        //Nodes
        if (label == "start"){

            t_vec2f start,stop;

            float val;
            int i=1;
            std::vector<float> vals;
            while (i <= 2){

                sstr >> val;
                if (std::isnan(val)){

                    return false;
                }
                vals.push_back(val);
                i++;
            }
            start.x = vals.at(0);
            start.y = vals.at(1);

            sstr >> label;

            if (label == "stop"){

                float val;
                int i=1;
                std::vector<float> vals;
                while (i <= 2){

                    sstr >> val;
                    if (std::isnan(val)){

                        return false;
                    }
                    vals.push_back(val);
                    i++;
                }
                stop.x = vals.at(0);
                stop.y = vals.at(1);

            }else{

                std::cout << "failed No stop" <<std::endl;
            }
            nodes pos;
            pos.push_back(start);
            pos.push_back(stop);

            mStartStopPositions.push_back(pos);

        }else if (label == "node"){

            std::vector<float> vals;
            float val;
            while (sstr >> val)
            {
                if (std::isnan(val))
                {
                    return false;
                }

                vals.push_back(val);
            }

            if (vals.size() != 2)
            {
                return false;
            }

            t_vec2f p1(vals[0],vals[1]);
            mNodes.push_back(p1);
        }
    }

    ifs.close();

    return true;
}

