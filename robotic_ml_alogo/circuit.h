#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <string>
#include "./utils/t_line.hpp"
#include <list>

class Circuit
{
    typedef std::vector<t_vec2f> nodes;
    typedef std::vector<std::vector<t_vec2f>> l_start_stop;
private:
    nodes mNodes;
    l_start_stop mStartStopPositions;
    bool misValid;
public:
    Circuit();
    bool LoadPath(std::string filename);
public:
    // setter/getter
    inline bool	isValid() const { return misValid; }
    inline const nodes&	getCheckpoints() const { return mNodes; }
    inline const l_start_stop&	getStartStopPositions() const { return mStartStopPositions; }
};

#endif // CIRCUIT_H
