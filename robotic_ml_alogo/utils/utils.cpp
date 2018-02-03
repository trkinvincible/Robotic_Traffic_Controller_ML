

#include "utils.hpp"

#include <cmath> // cos/sin
#include <vector>


t_vec2f	rotateVec2(const t_vec2f& point, const t_vec2f& center, float angle)
{
	t_vec2f	newPos;

	float cos_a = cosf(angle);
	float sin_a = sinf(angle);

	newPos.x = center.x + (point.x-center.x) * cos_a + (point.y-center.y)*sin_a;
	newPos.y = center.y + (point.x-center.x) * sin_a - (point.y-center.y)*cos_a;

	return newPos;
}

t_vec2f	lerp(const t_vec2f& p1, const t_vec2f& p2, float ratio)
{
	t_vec2f ret_val;
	ret_val.x = p1.x + (p2.x - p1.x) * ratio;
	ret_val.y = p1.y + (p2.y - p1.y) * ratio;
	return ret_val;
}

std::vector<float> sigmoid (const std::vector <float>& m1) {

    /*  Returns the value of the sigmoid function f(x) = 1/(1 + e^-x).
        Input: m1, a vector.
        Output: 1/(1 + e^-x) for every element of the input matrix m1.
    */

    const unsigned long VECTOR_SIZE = m1.size();
    std::vector <float> output (VECTOR_SIZE);


    for( unsigned i = 0; i != VECTOR_SIZE; ++i ) {
        output[ i ] = log(1 + exp(-m1[ i ]));
    }

    return output;
}
