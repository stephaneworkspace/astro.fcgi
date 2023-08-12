//
// Created by Stéphane on 12.08.23.
//

#include "Util.h"

vector<string> Util::tokenize(const std::string& s, char c) {
    auto end = s.cend();
    auto start = end;

    vector<string> v;
    for( auto it = s.cbegin(); it != end; ++it ) {
        if( *it != c ) {
            if( start == end )
                start = it;
            continue;
        }
        if( start != end ) {
            v.emplace_back(start, it);
            start = end;
        }
    }
    if( start != end )
        v.emplace_back(start, end);
    return v;
}

float Util::getZnorm(float angle) {
    int ang = static_cast<int>(angle) % 360;
    if (ang <= 360 / 2) {
        return static_cast<float>(ang);
    } else {
        return angle - 360.0f;
    }
}

float Util::getClosestDistance(float angle1, float angle2)  {
    return getZnorm(angle2 - angle1);
}

map<string, string> Util::parseQueryString(std::string &query) {
    map<string, string> data;
    size_t pos = 0;
    while (pos < query.length()) {
        size_t ampPos = query.find('&', pos);
        if (ampPos == std::string::npos) ampPos = query.length();

        size_t eqPos = query.find('=', pos);
        if (eqPos == std::string::npos || eqPos > ampPos) {
            pos = ampPos + 1;
            continue;
        }

        string key = query.substr(pos, eqPos - pos);
        string value = query.substr(eqPos + 1, ampPos - eqPos - 1);

        data[key] = value;

        pos = ampPos + 1;
    }
    return data;
}
