

#ifndef	D_UTILS_HPP
#define	D_UTILS_HPP


#include "t_vec2f.hpp"


t_vec2f	rotateVec2(const t_vec2f& point, const t_vec2f& center, float angle);

t_vec2f	lerp(const t_vec2f& p1, const t_vec2f& p2, float ratio);

std::vector<float> sigmoid (const std::vector <float>& m1);


#endif // D_UTILS_HPP

