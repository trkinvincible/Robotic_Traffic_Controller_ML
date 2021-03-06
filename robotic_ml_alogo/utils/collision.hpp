

#ifndef	D_COLLISION_HPP
#define	D_COLLISION_HPP


#include "t_vec2f.hpp"
float    isDistantNodeX(t_vec2f A, t_vec2f B);
float    isDistantNodeY(t_vec2f A, t_vec2f B);
bool    isSameNode(t_vec2f A, t_vec2f B);
bool    isPointOntheLine(t_vec2f A, t_vec2f B, t_vec2f C);
bool	CollisionLineSeg(const t_vec2f& A, const t_vec2f& B, const t_vec2f& O, const t_vec2f& P);
float	CollisionSegmentSegment_partial(const t_vec2f& A, const t_vec2f& B, const t_vec2f& O, const t_vec2f& P);
bool	CollisionSegmentSegment(const t_vec2f& A, const t_vec2f& B, const t_vec2f& O, const t_vec2f& P);

// cercle

bool	CollisionPointCercle(const t_vec2f& P, const t_vec2f& C, float radius);
bool	CollisionLineCercle(const t_vec2f& A, const t_vec2f&  B, const t_vec2f& C, float radius);
bool	CollisionSegmentCercle(const t_vec2f& A, const t_vec2f&  B, const t_vec2f& C, float radius);


#endif	// D_COLLISION_HPP

