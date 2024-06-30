#ifndef HULL_BRUTE_FORCE_H
#define HULL_BRUTE_FORCE_H
#include <set>
#include <vector>
#include <fstream>

struct Point {
    //no ctors are defined so that initialization lists are allowed
	float x;
	float y;
    bool operator==( Point const& arg2 ) const;
};

std::ostream& operator<< (std::ostream& os, Point const& p);
std::istream& operator>> (std::istream& os, Point & p);

//returns a set of indices of points that form convex hull
std::set<int> hullBruteForce ( std::vector< Point > const& points );

//returns a vector of indices of points that form convex hull in 
//a counter-clockwise order
std::vector<int> hullBruteForce2 ( std::vector< Point > const& points );

#endif
