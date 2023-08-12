//
// Created by Stéphane on 12.08.23.
//

#include "Swe.h"


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

const string Swe::Svg() {
    const char *path = "./";
    const char *a_o = aspect_option.c_str();
    string svg = sweinterfacelib::theme_astral_svg(year, month, day, hour, min, lat, lng, gmt, path, color, a_o);
    static std::string decode;
    if (!Base64::Decode(svg, &decode)) {
#if SW_DEBUG
        std::cout << "Failed to decode" << std::endl;
        return "";
#endif
    } else {
        return decode;
    }
}

const string Swe::Json() {
    Swe02::set_ephe_path("./");
    // TimeZone
    TimeZone time_zone;
    time_zone.year = year;
    time_zone.month = month;
    time_zone.day = day;
    time_zone.hour = hour;
    time_zone.min = min;
    time_zone.sec = 0;
    TimeZone utc_time_zone = TZ::utc_time_zone(time_zone, gmt);
    UtcToJd utc_to_jd = Swe08::utc_to_jd(utc_time_zone, CALANDAR_GREGORIAN);
    time_t t = time(0);
    tm* now = localtime(&t);
    TimeZone time_zone_t;
    time_zone_t.year = now->tm_year + 1900;
    time_zone_t.month = now->tm_mon + 1;
    time_zone_t.day = now->tm_mday;
    time_zone_t.hour = now->tm_hour;
    time_zone_t.min = now->tm_min;
    time_zone_t.sec = now->tm_sec;
    double gmt_t = gmt;
    TimeZone utc_time_zone_t = TZ::utc_time_zone(time_zone_t, gmt_t);
    UtcToJd utc_to_jd_t = Swe08::utc_to_jd(utc_time_zone_t, CALANDAR_GREGORIAN);

    H* house = new H[12];
    for (int i = 0; i < 12; ++i) {
        house[i] = Swe14::house(utc_to_jd.julian_day_ut, lat, lng, 'P', i + 1);
    }

    int* astres = new int[MAX_ASTRES];
    astres[SOLEIL] = ASTRE_SOLEIL;
    astres[LUNE] = ASTRE_LUNE;
    astres[MERCURE] = ASTRE_MERCURE;
    astres[VENUS] = ASTRE_VENUS;
    astres[MARS] = ASTRE_MARS;
    astres[JUPITER] = ASTRE_JUPITER;
    astres[SATURN] = ASTRE_SATURN;
    astres[URANUS] = ASTRE_URANUS;
    astres[NEPTUNE] = ASTRE_NEPTUNE;
    astres[PLUTON] = ASTRE_PLUTON;
    astres[NOEUD_LUNAIRE] = ASTRE_NOEUD_LUNAIRE;
    astres[CHIRON] = ASTRE_CHIRON;
    astres[CERES] = ASTRE_CERES;
    astres[NOEUD_LUNAIRE_SUD] = ASTRE_NOEUD_LUNAIRE_SUD;

    json js;
    for (int i = 0; i < MAX_ASTRES; ++i) {
        string astre = Astre::name(astres[i]);
        CalcUt calcul_ut = Swe03::calc_ut(utc_to_jd.julian_day_ut, astres[i], OPTION_FLAG_SPEED);
        js["bodie"][i]["nom"] = astre;
        js["bodie"][i]["asset"] = asset_bodie(astres[i]);
        js["bodie"][i]["deg_min_sec"] = calcul_ut.split.print;
        js["bodie"][i]["deg"] = calcul_ut.split.deg;
        js["bodie"][i]["min"] = calcul_ut.split.min;
        js["bodie"][i]["sec"] = calcul_ut.split.sec;
        js["bodie"][i]["sign"]["id"] = calcul_ut.split.sign;
        string sign = Sign::nom(calcul_ut.split.sign + 1);
        js["bodie"][i]["sign"]["nom"] = sign;
        js["bodie"][i]["sign"]["asset"] = asset_sign(calcul_ut.split.sign + 1);
        CalcUt calcul_ut_t= Swe03::calc_ut(utc_to_jd_t.julian_day_ut, astres[i], OPTION_FLAG_SPEED);
        js["bodie"][i]["deg_min_sec_transit"] = calcul_ut_t.split.print;
        js["bodie"][i]["deg_transit"] = calcul_ut_t.split.deg;
        js["bodie"][i]["min_transit"] = calcul_ut_t.split.min;
        js["bodie"][i]["sec_transit"] = calcul_ut_t.split.sec;
        js["bodie"][i]["sign_transit"]["id"] = calcul_ut_t.split.sign;
        string sign_transit = Sign::nom(calcul_ut_t.split.sign + 1);
        js["bodie"][i]["sign_transit"]["nom"] = sign_transit;
        js["bodie"][i]["sign_transit"]["asset"] = asset_sign(calcul_ut_t.split.sign + 1);
    }
    // Bodies angle
    int* astresAngle = new int[MAX_ASTRES + 2];
    astresAngle[SOLEIL] = ASTRE_SOLEIL;
    astresAngle[LUNE] = ASTRE_LUNE;
    astresAngle[MERCURE] = ASTRE_MERCURE;
    astresAngle[VENUS] = ASTRE_VENUS;
    astresAngle[MARS] = ASTRE_MARS;
    astresAngle[JUPITER] = ASTRE_JUPITER;
    astresAngle[SATURN] = ASTRE_SATURN;
    astresAngle[URANUS] = ASTRE_URANUS;
    astresAngle[NEPTUNE] = ASTRE_NEPTUNE;
    astresAngle[PLUTON] = ASTRE_PLUTON;
    astresAngle[NOEUD_LUNAIRE] = ASTRE_NOEUD_LUNAIRE;
    astresAngle[CHIRON] = ASTRE_CHIRON;
    astresAngle[CERES] = ASTRE_CERES;
    astresAngle[NOEUD_LUNAIRE_SUD] = ASTRE_NOEUD_LUNAIRE_SUD;
    astresAngle[NOEUD_LUNAIRE_SUD + 1] = 98; // Asc
    astresAngle[NOEUD_LUNAIRE_SUD + 2] = 99; // Mc
    for (int i = 0; i < MAX_ASTRES + 2; ++i) {
        for (int j = 0; j < MAX_ASTRES + 2; ++j) {
            if (i != j) {
                int lon1 = 0;
                int lon2 = 0;
                if (astresAngle[i] == 98 || astresAngle[i] == 99) {
                    // Angle
                    if (astresAngle[i] == 98) {
                        lon1 = house[1].angle; // Asc
                    } else {
                        lon1 = house[10].angle; // Mc
                    }
                    if (astresAngle[j] == 98 || astresAngle[j] == 99) {
                        // Angle
                        if (astresAngle[j] == 98) {
                            lon2 = house[1].angle; // Asc
                        } else {
                            lon2 = house[10].angle; // Mc
                        }
                    } else {
                        // Astre
                        CalcUt calcul_ut = Swe03::calc_ut(utc_to_jd.julian_day_ut, astresAngle[j], OPTION_FLAG_SPEED);
                        lon2 = calcul_ut.longitude;
                    }
                } else {
                    // Astre
                    CalcUt calcul_ut = Swe03::calc_ut(utc_to_jd.julian_day_ut, astresAngle[i], OPTION_FLAG_SPEED);
                    lon1 = calcul_ut.longitude;
                    if (astresAngle[j] == 98 || astresAngle[j] == 99) {
                        // Angle
                        if (astresAngle[j] == 98) {
                            lon2 = house[1].angle; // Asc
                        } else {
                            lon2 = house[10].angle; // Mc
                        }
                    } else {
                        // Astre
                        CalcUt calcul_ut = Swe03::calc_ut(utc_to_jd.julian_day_ut, astresAngle[j], OPTION_FLAG_SPEED);
                        lon2 = calcul_ut.longitude;
                    }
                }
                int aspect = 100; // Nothing
                float separation = Util::getClosestDistance(lon1, lon2);
                float abs_separation = abs(separation);
                for (int k = 0; k < ASPECTS_SEMISEXTILE ; ++k) {
                    int* angle = Aspect::angle(k);
                    int asp = angle[0];
                    int orb = angle[1];
                    if (abs(abs_separation - asp) <= orb) {
                        aspect = k;
                    }
                }
                js["aspect"][i]["id"] = astresAngle[i];
                if (astresAngle[i] == 98) {
                    js["aspect"][i]["nom"] = "Asc";
                    js["aspect"][i]["asset"] = asset_angle(1, color);
                } else if (astresAngle[i] == 99) {
                    js["aspect"][i]["nom"] = "Mc";
                    js["aspect"][i]["asset"] = asset_angle(4, color);
                } else {
                    string astre = Astre::name(astresAngle[i]);
                    js["aspect"][i]["nom"] = astre;
                    js["aspect"][i]["asset"] = asset_bodie(astresAngle[i]);
                }
                js["aspect"][i]["liens"][j]["id"] = astresAngle[j];
                if (astresAngle[j] == 98) {
                    js["aspect"][i]["liens"][j]["nom"] = "Asc";
                } else if (astresAngle[j] == 99) {
                    js["aspect"][i]["liens"][j]["nom"] = "Mc";
                } else {
                    string astre = Astre::name(astresAngle[j]);
                    js["aspect"][i]["liens"][j]["nom"] = astre;
                }
                if (aspect == 100) {
                    js["aspect"][i]["liens"][j]["aspect_id"] = nullptr;
                    js["aspect"][i]["liens"][j]["aspect_name"] = nullptr;
                    js["aspect"][i]["liens"][j]["asset"] = nullptr;
                } else {
                    js["aspect"][i]["liens"][j]["aspect_id"] = aspect;
                    js["aspect"][i]["liens"][j]["aspect_name"] = text_aspect(aspect);
                    js["aspect"][i]["liens"][j]["asset"] = asset_aspect(aspect);
                }
            } else {
                js["aspect"][i]["id"] = astresAngle[i];
                if (astresAngle[i] == 98) {
                    js["aspect"][i]["nom"] = "Asc";
                    js["aspect"][i]["asset"] = asset_angle(1, color);
                } else if (astresAngle[i] == 99) {
                    js["aspect"][i]["nom"] = "Mc";
                    js["aspect"][i]["asset"] = asset_angle(4, color);
                } else {
                    string astre = Astre::name(astresAngle[i]);
                    js["aspect"][i]["nom"] = astre;
                    js["aspect"][i]["asset"] = asset_bodie(astresAngle[i]);
                }
                js["aspect"][i]["liens"][j]["id"] = astresAngle[j];
                if (astresAngle[j] == 98) {
                    js["aspect"][i]["liens"][j]["nom"] = "Asc";
                } else if (astresAngle[j] == 99) {
                    js["aspect"][i]["liens"][j]["nom"] = "Mc";
                } else {
                    string astre = Astre::name(astresAngle[j]);
                    js["aspect"][i]["liens"][j]["nom"] = astre;
                }
                js["aspect"][i]["liens"][j]["aspect_id"] = nullptr;
                js["aspect"][i]["liens"][j]["aspect_name"] = nullptr;
                js["aspect"][i]["liens"][j]["asset"] = nullptr;
            }
        }
    }
    return js.dump();
}