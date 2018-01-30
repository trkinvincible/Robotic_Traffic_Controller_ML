#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <string>
#include "./utils/t_line.hpp"


class Circuit
{
    typedef std::vector<t_vec2f> points;
private:
    points mNodes;

    t_vec2f mStartPosition;
    float mStartAngle;

    t_vec2f mStopPosition;
    float mStopAngle;

    bool misValid;

public:
    Circuit();
    bool LoadPath(std::string filename);
public:
    // setter/getter
    inline bool	isValid() const { return misValid; }
    inline const points&	getCheckpoints() const { return mNodes; }

    inline const t_vec2f&	getStartingPositon() const { return mStartPosition; }
    inline float			getStartingAngle() const { return mStartAngle; }

    inline const t_vec2f&	getStoppingPositon() const { return mStopPosition; }
    inline float			getStoppingAngle() const { return mStopAngle; }
};

#endif // CIRCUIT_H
