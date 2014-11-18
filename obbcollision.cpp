#include "obbcollision.h"

float dot(const sf::Vector2f& a,const sf::Vector2f& b)
{
    return a.x*b.x + a.y*b.y;
}
float len_squared(const sf::Vector2f& a)
{
    return dot(a,a);
}

//Taken from http://cs.williams.edu/~morgan/code/C++/2DOBB.h
class OBB2D
{
private:
    /** Corners of the box, where 0 is the lower left. */
    sf::Vector2f         corner[4];

    /** Two edges of the box extended away from corner[0]. */
    sf::Vector2f         axis[2];

    /** origin[a] = corner[0].dot(axis[a]); */
    float          origin[2];

    /** Returns true if other overlaps one dimension of this. */
    bool overlaps1Way(const OBB2D& other) const
    {
        for (int a = 0; a < 2; ++a)
        {
            float t = dot(other.corner[0],axis[a]);
            // Find the extent of box 2 on axis a
            float tMin = t;
            float tMax = t;
            for (int c = 1; c < 4; ++c)
            {
                t = dot(other.corner[c],axis[a]);
                if (t < tMin)
                {
                    tMin = t;
                }
                else if (t > tMax)
                {
                    tMax = t;
                }
            }
            // We have to subtract off the origin
            // See if [tMin, tMax] intersects [0, 1]
            if ((tMin > 1 + origin[a]) || (tMax < origin[a]))
            {
                // There was no intersection along this dimension;
                // the boxes cannot possibly overlap.
                return false;
            }
        }
        // There was no dimension along which there is no intersection.
        // Therefore the boxes overlap.
        return true;
    }


    /** Updates the axes after the corners move.  Assumes the
        corners actually form a rectangle. */
    void computeAxes()
    {
        axis[0] = corner[1] - corner[0];
        axis[1] = corner[3] - corner[0];
        // Make the length of each axis 1/edge length so we know any
        // dot product must be less than 1 to fall within the edge.
        for (int a = 0; a < 2; ++a)
        {
            axis[a] /= len_squared(axis[a]);
            origin[a] = dot(corner[0],axis[a]);
        }
    }

public:

    OBB2D(const sf::Vector2f& center, const sf::Vector2f& size, float angle)
    {
        sf::Vector2f X( cos(angle), sin(angle));
        sf::Vector2f Y(-sin(angle), cos(angle));
        X *= size.x / 2;
        Y *= size.y / 2;
        corner[0] = center - X - Y;
        corner[1] = center + X - Y;
        corner[2] = center + X + Y;
        corner[3] = center - X + Y;
        computeAxes();
    }

    /** Returns true if the intersection of the boxes is non-empty. */
    bool overlaps(const OBB2D& other) const
    {
        return overlaps1Way(other) && other.overlaps1Way(*this);
    }
};

bool iscolliding(const sf::Vector2f& a_pos,const sf::Vector2f& a_size, float angle_a,const sf::Vector2f& b_pos,const sf::Vector2f& b_size, float angle_b)
{
    OBB2D _a(a_pos,a_size,angle_a);
    OBB2D _b(b_pos,b_size,angle_b);
    return _a.overlaps(_b);
}
