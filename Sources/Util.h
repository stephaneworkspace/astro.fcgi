//
// Created by Stéphane on 12.08.23.
//

#ifndef ASTRO_FCGI_UTIL_H
#define ASTRO_FCGI_UTIL_H


#include <string>
#include <vector>
#include <iostream>
#include <map>
using namespace std;

class Util {
public:
    static vector<string> tokenize(const string& s, char c);
    static float getZnorm(float angle);
    static float getClosestDistance(float angle1, float angle2);
    static map<string, string> parseQueryString(string& query);
};


#endif //ASTRO_FCGI_UTIL_H
