#include "circuit.h"
#include <fstream>
#include <sstream>
#include <cmath>

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

            std::vector<float> vals;
            float val;
            while (sstr >> val){

                if (std::isnan(val)){

                    return false;
                }
                vals.push_back(val);
            }
            mStartPosition.x = vals[0];
            mStartPosition.y = vals[1];

        }if (label == "stop"){

            std::vector<float> vals;
            float val;
            while (sstr >> val){

                if (std::isnan(val)){

                    return false;
                }
                vals.push_back(val);
            }
            mStopPosition.x = vals[0];
            mStopPosition.y = vals[1];

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

