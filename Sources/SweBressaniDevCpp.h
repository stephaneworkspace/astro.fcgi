//
// Created by Stéphane on 12.08.23.
//

#ifndef ASTRO_FCGI_SWE_H
#define ASTRO_FCGI_SWE_H

#include <map>
#include <utility>
#include <string>
#include <vector>
#include <json/json.h>
#include "swelib.h"
#include "sweinterfacelib.h"

using namespace std;
using namespace sweinterfacelib;

class SweBressaniDevCpp {
public:
    SweBressaniDevCpp(int year = 1984, int month = 4, int day = 1, int hour = 0, int min = 0,
        float lat = 46.12, float lng = 6.09, int gmt = 2, int color=0, const string& aspect_option="0");
    SweBressaniDevCpp(const string& y, const string& m, const string& d,
        const string& h, const string& mn, const string& la,
        const string& ln, const string& gm, const string& c, const string& a);
    static map<string, string> parseQueryString(string& query);
    const string Svg();
    const string Json();
private:
    static vector<string> tokenize(const string& s, char c);
    static float getZnorm(float angle);
    static float getClosestDistance(float angle1, float angle2);

    int year;
    int month;
    int day;
    int hour;
    int min;
    float lat;
    float lng;
    int gmt;
    int color;
    string aspect_option;
};

#endif //ASTRO_FCGI_SWE_H