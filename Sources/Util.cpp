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

float Util::get_znorm(float angle) {
    int ang = static_cast<int>(angle) % 360;
    if (ang <= 360 / 2) {
        return static_cast<float>(ang);
    } else {
        return angle - 360.0f;
    }
}

float Util::get_closest_distance(float angle1, float angle2)  {
    return get_znorm(angle2 - angle1);
}
