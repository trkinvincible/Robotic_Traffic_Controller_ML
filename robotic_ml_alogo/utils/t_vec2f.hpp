

#ifndef	D_T_VEC2F_HPP
#define	D_T_VEC2F_HPP

#include <vector>
#include <sstream>

struct t_vec2f
{
	float x, y;

	t_vec2f();
	t_vec2f(float in_x, float in_y);
    std::string toString(){

        std::string info;
        std::stringstream ss(info);

        ss << "X: " << x;
        ss << "\tY: " << y;

        return ss.str();
    }
};

typedef std::vector<t_vec2f> t_vec2f_s;
#endif // D_T_VEC2F_HPP

