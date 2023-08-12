//
// Created by Stéphane on 12.08.23.
//

#ifndef ASTRO_FCGI_SWE_H
#define ASTRO_FCGI_SWE_H

#include <string>
#include <vector>
using namespace std;

class Swe {
public:
    Swe(int year = 1984, int month = 4, int day = 1, int hour = 0, int min = 0,
        float lat = 46.12, float lng = 6.09, int gmt = 2);
    Swe(const string& y, const string& m, const std::string& d,
        const string& h, const string& mn, const std::string& la,
        const string& ln, const string& gm);
private:
    int year;
    int month;
    int day;
    int hour;
    int min;
    float lat;
    float lng;
    int gmt;
};




#endif //ASTRO_FCGI_SWE_H
