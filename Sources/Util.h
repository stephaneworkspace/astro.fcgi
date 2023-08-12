//
// Created by Stéphane on 12.08.23.
//

#ifndef ASTRO_FCGI_UTIL_H
#define ASTRO_FCGI_UTIL_H


#include <string>
#include <vector>
using namespace std;

class Util {
public:
    static vector<string> tokenize(const std::string& s, char c);
    static float get_znorm(float angle);
    static float get_closest_distance(float angle1, float angle2);
};


#endif //ASTRO_FCGI_UTIL_H
