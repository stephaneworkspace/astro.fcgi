//
// Created by Stéphane on 12.08.23.
//

#ifndef ASTRO_FCGI_SWE_H
#define ASTRO_FCGI_SWE_H

#include <utility>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "swelib.h"
#include "Util.h"
#include "sweinterfacelib.h"

using namespace std;
using json = nlohmann::json;
using namespace sweinterfacelib;

class Swe {
public:
    Swe(int year = 1984, int month = 4, int day = 1, int hour = 0, int min = 0,
        float lat = 46.12, float lng = 6.09, int gmt = 2, int color=0, const string& aspect_option="0");
    Swe(const string& y, const string& m, const string& d,
        const string& h, const string& mn, const string& la,
        const string& ln, const string& gm, const string& c, const string& a);
    const string Svg();
    const string Json();
private:
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
