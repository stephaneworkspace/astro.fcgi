//
// Created by Stéphane on 12.08.23.
//

#include "Swe.h"

#include <utility>

Swe::Swe(int y, int m, int d, int h, int mn, float la, float ln, int gm, int c, const string& a)
        : year(y), month(m), day(d), hour(h), min(mn), lat(la), lng(ln), gmt(gm), color(c), aspect_option(std::move(a)) {

}

Swe::Swe(const string& y, const string& m, const string& d,
    const string& h, const string& mn, const string& la,
    const string& ln, const string& gm, const string& c, const string& ao)  {
    try {
        year = stoi(y);
    } catch (const invalid_argument& ia) {
        year = 0;
    } catch (const out_of_range& oor) {
        year = 0;
    }
    try {
        month = stoi(m);
    } catch (const invalid_argument& ia) {
        month = 0;
    } catch (const out_of_range& oor) {
        month = 0;
    }
    try {
        day = stoi(d);
    } catch (const invalid_argument& ia) {
        day = 0;
    } catch (const out_of_range& oor) {
        day = 0;
    }
    try {
        hour = stoi(h);
    } catch (const invalid_argument& ia) {
        hour = 0;
    } catch (const out_of_range& oor) {
        hour = 0;
    }
    try {
        min = stoi(mn);
    } catch (const invalid_argument& ia) {
        min = 0;
    } catch (const out_of_range& oor) {
        min = 0;
    }
    try {
        lat = stof(la);
    } catch (const invalid_argument& ia) {
        lat = 0.0;
    } catch (const out_of_range& oor) {
        lat = 0.0;
    }
    try {
        lng = stof(ln);
    } catch (const invalid_argument& ia) {
        lng = 0.0;
    } catch (const out_of_range& oor) {
        lng = 0.0;
    }
    try {
        gmt = stof(gm);
    } catch (const invalid_argument& ia) {
        gmt = 0.0;
    } catch (const out_of_range& oor) {
        gmt = 0.0;
    }
    try {
        color = stoi(c);
    } catch (const invalid_argument& ia) {
        color = 0;
    } catch (const out_of_range& oor) {
        color = 0;
    }
    aspect_option = std::move(ao);
}

string Swe::getAo() {
    return aspect_option;
}