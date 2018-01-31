

#ifndef	D_T_VEC2F_HPP
#define	D_T_VEC2F_HPP

#include <vector>

struct t_vec2f
{
	float x, y;

	t_vec2f();
	t_vec2f(float in_x, float in_y);
};

typedef std::vector<t_vec2f> t_vec2f_s;
#endif // D_T_VEC2F_HPP

