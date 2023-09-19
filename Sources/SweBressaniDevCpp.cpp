//
// Created by Stéphane on 12.08.23.
//

#include "SweBressaniDevCpp.h"


SweBressaniDevCpp::SweBressaniDevCpp(int y, int m, int d, int h, int mn, float la, float ln, int gm, int c, const string& a)
        : year(y), month(m), day(d), hour(h), min(mn), lat(la), lng(ln), gmt(gm), color(c), aspect_option(std::move(a)) {

}

SweBressaniDevCpp::SweBressaniDevCpp(const string& y, const string& m, const string& d,
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

map<string, string> SweBressaniDevCpp::parseQueryString(std::string &query) {
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

const string SweBressaniDevCpp::Svg() {
    const char *path = "./";
    const char *a_o = aspect_option.c_str();
    string svg = sweinterfacelib::theme_astral_svg(year, month, day, hour, min, lat, lng, gmt, path, color, a_o);
    static std::string decode;
    if (!Base64::Decode(svg, &decode)) {
#if SW_DEBUG
        std::cout << "Failed to decode" << std::endl;
#endif
        return "";
    } else {
        return decode;
    }
}

const string SweBressaniDevCpp::Json() {
    Swe02::set_ephe_path("./");
    // TimeZone
    TimeZone time_zone = {year, month, day, hour, min, 0};
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
    Json::Value js;
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
        js["bodie"][i]["sign"]["asset"] = asset_sign(calcul_ut.split.sign + 1, color);
        CalcUt calcul_ut_t= Swe03::calc_ut(utc_to_jd_t.julian_day_ut, astres[i], OPTION_FLAG_SPEED);
        js["bodie"][i]["deg_min_sec_transit"] = calcul_ut_t.split.print;
        js["bodie"][i]["deg_transit"] = calcul_ut_t.split.deg;
        js["bodie"][i]["min_transit"] = calcul_ut_t.split.min;
        js["bodie"][i]["sec_transit"] = calcul_ut_t.split.sec;
        js["bodie"][i]["sign_transit"]["id"] = calcul_ut_t.split.sign;
        string sign_transit = Sign::nom(calcul_ut_t.split.sign + 1);
        js["bodie"][i]["sign_transit"]["nom"] = sign_transit;
        js["bodie"][i]["sign_transit"]["asset"] = asset_sign(calcul_ut_t.split.sign + 1, color);
    }
    // Bodies angle
    //int* astresAngle = new int[MAX_ASTRES + 2];
    auto astresAngle = std::make_unique<int[]>(MAX_ASTRES + 2);
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
                float separation = SweBressaniDevCpp::getClosestDistance(lon1, lon2);
                float abs_separation = abs(separation);
                for (int k = 0; k < ASPECTS_SEMISEXTILE ; ++k) {
                    int* angle = Aspect::angle(k);
                    int asp = angle[0];
                    int orb = angle[1];
                    if (abs(abs_separation - asp) <= orb) {
                        aspect = k;
                    }
                }
                /*
                js["aspect"][i]["id"] = astresAngle[i];
                if (astresAngle[i] == 98) {
                    js["aspect"][i]["nom"] = "Asc";
                    const char* res = asset_angle(1, color);
                    if (res != nullptr) {
                        string res_angle(res);
                        js["aspect"][i]["asset"] = res_angle;
                    }
                } else if (astresAngle[i] == 99) {
                    js["aspect"][i]["nom"] = "Mc";
                    const char* res = asset_angle(4, color);
                    if (res != nullptr) {
                        string res_angle(res);
                        js["aspect"][i]["asset"] = res_angle;
                    }
                } else {
                    const char* res = Astre::name(astresAngle[i]);
                    if (res != nullptr) {
                        string astre(res);
                        js["aspect"][i]["nom"] = astre;
                    } else {
                        js["aspect"][i]["nom"] = "";
                    }
                    const char* res2 = asset_bodie(astresAngle[i]);
                    if (res2 != nullptr) {
                        string a_bodie(res2);
                        js["aspect"][i]["asset"] = a_bodie;
                    } else {
                        js["aspect"][i]["asset"] = "";
                    }
                }
                js["aspect"][i]["liens"][j]["id"] = astresAngle[j];
                if (astresAngle[j] == 98) {
                    js["aspect"][i]["liens"][j]["nom"] = "Asc";
                } else if (astresAngle[j] == 99) {
                    js["aspect"][i]["liens"][j]["nom"] = "Mc";
                } else {
                    const char* res = Astre::name(astresAngle[j]);
                    if (res != nullptr) {
                        string astre(res);
                        js["aspect"][i]["liens"][j]["nom"] = astre;
                    } else {
                        js["aspect"][i]["liens"][j]["nom"] = "";
                    }
                }
                if (aspect == 100) {
                    js["aspect"][i]["liens"][j]["aspect_id"] = Json::Value::null;
                    js["aspect"][i]["liens"][j]["aspect_name"] = Json::Value::null;
                    js["aspect"][i]["liens"][j]["asset"] = Json::Value::null;
                } else {
                    js["aspect"][i]["liens"][j]["aspect_id"] = aspect;
                    const char* res = text_aspect(aspect);
                    if (res != nullptr) {
                        string t_aspect(res);
                        js["aspect"][i]["liens"][j]["aspect_name"] = t_aspect;
                    } else {
                        js["aspect"][i]["liens"][j]["aspect_name"] = "";
                    }
                    const char* res2 = asset_aspect(aspect);
                    if (res2 != nullptr) {
                        string a_aspect(res2);
                        js["aspect"][i]["liens"][j]["asset"] = a_aspect;
                    } else {
                        js["aspect"][i]["liens"][j]["asset"] = "";
                    }
                }
                 */
            } else {
                /*
                js["aspect"][i]["id"] = astresAngle[i];
                if (astresAngle[i] == 98) {
                    js["aspect"][i]["nom"] = "Asc";
                    const char* res = asset_angle(1, color);
                    if (res != nullptr) {
                        string res_angle(res);
                        js["aspect"][i]["asset"] = res_angle;
                    }
                } else if (astresAngle[i] == 99) {
                    js["aspect"][i]["nom"] = "Mc";
                    const char* res = asset_angle(4, color);
                    if (res != nullptr) {
                        string res_angle(res);
                        js["aspect"][i]["asset"] = res_angle;
                    }
                } else {
                    const char* res = Astre::name(astresAngle[i]);
                    if (res != nullptr) {
                        string astre(res);
                        js["aspect"][i]["nom"] = astre;
                    } else {
                        js["aspect"][i]["nom"] = "";
                    }
                    const char* res2 = asset_bodie(astresAngle[i]);
                    if (res2 != nullptr) {
                        string a_bodie(res2);
                        js["aspect"][i]["asset"] = a_bodie;
                    } else {
                        js["aspect"][i]["asset"] = "";
                    }
                }
                js["aspect"][i]["liens"][j]["id"] = astresAngle[j];
                if (astresAngle[j] == 98) {
                    js["aspect"][i]["liens"][j]["nom"] = "Asc";
                } else if (astresAngle[j] == 99) {
                    js["aspect"][i]["liens"][j]["nom"] = "Mc";
                } else {
                    const char* res = Astre::name(astresAngle[j]);
                    if (res != nullptr) {
                        string astre(res);
                        js["aspect"][i]["liens"][j]["nom"] = astre;
                    } else {
                        js["aspect"][i]["liens"][j]["nom"] = "";
                    }
                }
                js["aspect"][i]["liens"][j]["aspect_id"] = Json::Value::null;
                js["aspect"][i]["liens"][j]["aspect_name"] = Json::Value::null;
                js["aspect"][i]["liens"][j]["asset"] = Json::Value::null;
                */
            }
        }
    }
    Json::StreamWriterBuilder writer;
    std::string output = Json::writeString(writer, js);
    delete[] house;
    house = nullptr;
    delete[] astres;
    astres = nullptr;
    astresAngle.reset();
    return output;
}

/*
 * Documentation de la méthode `JsonApiV2`
 *
 * Description:
 * La méthode `JsonApiV2` est utilisée pour générer un JSON basées sur différentes options fournies.
 *
 * Signature de la méthode:
 *    const string SweBressaniDevCpp::JsonApiV2(JsonApiV2Option option)
 *
 * Paramètres:
 *    - `option` (Type: `JsonApiV2Option`): Une énumération qui détermine quel type de données JSON retourner.
 *    - `i_color` (Type: `ìnt`): Couleur 0 ou 1
 *
 * Renvoi:
 * La méthode renvoie une chaîne de caractères représentant un objet JSON.
 *
 * Comportement de la méthode:
 * La méthode effectue des actions différentes en fonction de la valeur de l'option fournie.
 *
 * 1. Option `JsonGrid`:
 *    - Retourne un SVG représentant une grille.
 *
 * 2. Option `JsonAspect`:
 *      Soleil
 *      [ ] Lune
 *      [ ] [ ] Mercure
 *      [ ] [ ] [ ] Venus
 *      [ ] [ ] [ ] [ ] Mars
 *      [ ] [ ] [ ] [ ] [ ] Jupiter
 *      [ ] [ ] [ ] [ ] [ ] [ ] Saturne
 *      [ ] [ ] [ ] [ ] [ ] [ ] [ ] Uranus
 *      [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] Neptune
 *      [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] Pluton
 *      [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] Noeud lunaire
 *      [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] Chiron
 *      [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] Ceres
 *      [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] Noeud lunaire sur
 *      [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] Ascendant
 *      [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] Milieu du ciel
 *    - Calcule les angles et les aspects entre différents astres et points de référence
 *      comme l'Ascendant (Asc) et le Milieu du Ciel (Mc).
 *    - Produit un JSON contenant des informations sur ces astres, leurs aspects entre eux
 *      et leurs noms.
 *
 * 3. Option `JsonAspectsAsset`:
 *    - Produit un JSON contenant des ressources associées à différents aspects.
 *
 * 4. Option `JsonBodiesAsset`:
 *    - Produit un JSON contenant des ressources associées à différents corps ou astres,
 *      comme le Soleil, la Lune, etc.
 *
 * Exemple d'utilisation:
 *    SweBressaniDevCpp instance;
 *    string output = instance.JsonApiV2(JsonApiV2Option::JsonAspect, 1);
 *
 */
const string SweBressaniDevCpp::JsonApiV2(JsonApiV2Option option) {
    Json::Value js;
    switch (option) {
        case JsonApiV2Option::JsonChartGrid:
        {
            const char *path = "./";
            const char *a_o = aspect_option.c_str();
            string svg0 = sweinterfacelib::theme_astral_empty_svg(year, month, day, hour, min, lat, lng, gmt, path, 0, a_o);
            string svg1 = sweinterfacelib::theme_astral_empty_svg(year, month, day, hour, min, lat, lng, gmt, path, 1, a_o);
            js["chart"][0] = svg0;
            js["chart"][1] = svg1;
            const sweinterfacelib::PosAsset* pam = sweinterfacelib::theme_astral_maison_pos(year, month, day, hour, min, lat, lng, gmt, path);
            int j = 0;
            string cr = "";
            for (int i = 1; i < 13; ++i) {
                switch (i) {
                    case 1:
                        cr = "I";
                        break;
                    case 2:
                        cr = "II";
                        break;
                    case 3:
                        cr = "III";
                        break;
                    case 4:
                        cr = "IV";
                        break;
                    case 5:
                        cr = "V";
                        break;
                    case 6:
                        cr = "VI";
                        break;
                    case 7:
                        cr = "VII";
                        break;
                    case 8:
                        cr = "VIII";
                        break;
                    case 9:
                        cr = "IX";
                        break;
                    case 10:
                        cr = "X";
                        break;
                    case 11:
                        cr = "XI";
                        break;
                    case 12:
                        cr = "XII";
                        break;
                }
                string lien = "/la-maison-" + cr;
                string ancre = "#DEFINITION";
                js["house"][j]["lien"] = lien;
                js["house"][j]["ancre"] = ancre;
                js["house"][j]["width"] = pam[i].width;
                js["house"][j]["height"] = pam[i].height;
                js["house"][j]["x"] = pam[i].x;
                js["house"][j]["y"] = pam[i].y;
                string svgh0 = sweinterfacelib::asset_house(i, 0);
                string svgh1 = sweinterfacelib::asset_house(i, 1);
                js["house"][j]["asset"][0] = svgh0;
                js["house"][j]["asset"][1] = svgh1;
                j++;
            }
            const sweinterfacelib::PosAsset* pas = sweinterfacelib::theme_astral_signe_pos(year, month, day, hour, min, lat, lng, gmt, path);
            j = 0;
            for (int i = 1; i < 13; ++i) {
                string lien = "/todo";
                string ancre = "#DEFINITION";
                js["signe"][j]["lien"] = lien;
                js["signe"][j]["ancre"] = ancre;
                js["signe"][j]["width"] = pas[i].width;
                js["signe"][j]["height"] = pas[i].height;
                js["signe"][j]["x"] = pas[i].x;
                js["signe"][j]["y"] = pas[i].y;
                string svgh0 = sweinterfacelib::asset_sign(i, color);
                string svgh1 = sweinterfacelib::asset_sign(i, color);
                js["signe"][j]["asset"][0] = svgh0;
                js["signe"][j]["asset"][1] = svgh1;
                string nom = "TODO"; // TODO
                js["signe"][j]["nom"] = nom;
                j++;
            }
            const sweinterfacelib::PosAstreAsset* paa = sweinterfacelib::theme_astral_astre_pos(year, month, day, hour, min, lat, lng, gmt, path, a_o);
            j = 0;
            for (int i = 0; i < MAX_ASTRES; ++i) {
                if (paa[i].id >= 0) {
                    string lien = "/todo";
                    string ancre = "#DEFINITION";
                    js["astre"][j]["lien"] = lien;
                    js["astre"][j]["ancre"] = ancre;
                    js["astre"][j]["id"] = paa[i].id;
                    js["astre"][j]["nom"] = paa[i].retrograde ? string(paa[i].nom) + " Retrograde" : paa[i].nom;
                    js["astre"][j]["astre"]["width"] = paa[i].astre.width;
                    js["astre"][j]["astre"]["height"] = paa[i].astre.height;
                    js["astre"][j]["astre"]["x"] = paa[i].astre.x;
                    js["astre"][j]["astre"]["y"] = paa[i].astre.y;
                    const char* svg0 = sweinterfacelib::asset_bodie(paa[i].id);
                    const char* svg1 = sweinterfacelib::asset_bodie(paa[i].id);
                    js["astre"][j]["astre"]["asset"][0] = svg0;
                    js["astre"][j]["astre"]["asset"][1] = svg1;
                    js["astre"][j]["astre_r"]["width"] = paa[i].astre_r.width;
                    js["astre"][j]["astre_r"]["height"] = paa[i].astre_r.height;
                    js["astre"][j]["astre_r"]["x"] = paa[i].astre_r.x;
                    js["astre"][j]["astre_r"]["y"] = paa[i].astre_r.y;
                    const char* svgr0 = sweinterfacelib::asset_bodie_r(paa[i].id);
                    const char* svgr1 = sweinterfacelib::asset_bodie_r(paa[i].id);
                    js["astre"][j]["astre_r"]["asset"][0] = svgr0;
                    js["astre"][j]["astre_r"]["asset"][1] = svgr1;
                    j++;
                }
            }
            break;
        }
        case JsonApiV2Option::JsonGrid:
        {
            string color_0 = sweinterfacelib::grille_aspect_svg(0);
            string color_1 = sweinterfacelib::grille_aspect_svg(1);
            js["grille"][0] = color_0;
            js["grille"][1] = color_1;
            break;
        }
        case JsonApiV2Option::JsonAspect:
        {
            Swe02::set_ephe_path("./");
            // TimeZone
            TimeZone time_zone = {year, month, day, hour, min, 0};
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

            auto astresAngle = std::make_unique<int[]>(MAX_ASTRES + 2);
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

            double lon_asc = house[1].longitude;
            double lon_mc = house[10].longitude;

            map<pair<int, int>, AspectApiV2> m;
            for (int i = 0; i < MAX_ASTRES + 2; ++i) {
                for (int j = 0; j < MAX_ASTRES + 2; ++j) {
                    if (i != j) {
                        double lon1 = 0.0;
                        double lon2 = 0.0;
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
                                    //lon2 = house[1].angle; // Asc
                                    lon2 = lon_asc;
                                } else {
                                    //lon2 = house[10].angle; // Mc
                                    lon2 = lon_mc;
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
                                    //lon2 = house[1].angle; // Asc
                                    lon2 = lon_asc;
                                } else {
                                    //lon2 = house[10].angle; // Mc
                                    lon2 = lon_mc;
                                }
                            } else {
                                // Astre
                                CalcUt calcul_ut = Swe03::calc_ut(utc_to_jd.julian_day_ut, astresAngle[j], OPTION_FLAG_SPEED);
                                lon2 = calcul_ut.longitude;
                            }
                        }
                        float separation = SweBressaniDevCpp::getClosestDistance(lon1, lon2);
                        float abs_separation = abs(separation);
                        for (int k = 0; k < ASPECTS_SEMISEXTILE; ++k) {
                            int *angle = Aspect::angle(k);
                            int asp = angle[0];
                            int orb = angle[1];
                            if (abs(abs_separation - asp) <= orb) {
                                m[make_pair(i,j)] = AspectApiV2(abs(abs_separation - asp), orb, k);
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < MAX_ASTRES; i++) {
                js["aspect"][i]["id"] = astresAngle[i];
                if (astresAngle[i] == 98) {
                    js["aspect"][i]["nom"] = "Asc";
                } else if (astresAngle[i] == 99) {
                    js["aspect"][i]["nom"] = "Mc";
                } else {
                    const char *res = Astre::name(astresAngle[i]);
                    if (res != nullptr) {
                        string astre(res);
                        js["aspect"][i]["nom"] = astre;
                    } else {
                        js["aspect"][i]["nom"] = "";
                    }
                }
                double loni = 0;
                if (astresAngle[i] == 98) {
                    loni = lon_asc;
                } else if (astresAngle[i] == 99) {
                    loni = lon_mc;
                } else {
                    CalcUt calcul_ut = Swe03::calc_ut(utc_to_jd.julian_day_ut, astresAngle[i], OPTION_FLAG_SPEED);
                    loni = calcul_ut.longitude;
                }
                js["aspect"][i]["lon"] = loni;
                int k = 0;
                for (int j = i + 1; j < MAX_ASTRES + 2; j++) {
                    js["aspect"][i]["liens"][k]["id"] = astresAngle[j];
                    /*if (astresAngle[j] == 98) {
                        js["aspect"][i]["liens"][k]["nom"] = "Asc";
                    } else if (astresAngle[j] == 99) {
                        js["aspect"][i]["liens"][k]["nom"] = "Mc";
                    } else {
                        const char* res = Astre::name(astresAngle[j]);
                        if (res != nullptr) {
                            string astre(res);
                            js["aspect"][i]["liens"][k]["nom"] = astre;
                        } else {
                            js["aspect"][i]["liens"][k]["nom"] = "";
                        }
                    }*/
                    pair<int, int> key(i, j);
                    auto it = m.find(key);
                    if (it != m.end()) { // la clé existe
                        AspectApiV2 value = it->second;
                        js["aspect"][i]["liens"][k]["aspect_id"] = value.aspect;
                        /*
                        const char* res = text_aspect(value.aspect);
                        if (res != nullptr) {
                            string t_aspect(res);
                            js["aspect"][i]["liens"][k]["aspect_name"] = t_aspect;
                        } else {
                            js["aspect"][i]["liens"][k]["aspect_name"] = "";
                        }*/
                        //js["aspect"][i]["liens"][k]["asp"] = value.asp;
                        //js["aspect"][i]["liens"][k]["orb"] = value.orb;
                        double lonj = 0;
                        if (astresAngle[j] == 98) {
                            lonj = lon_asc;
                        } else if (astresAngle[j] == 99) {
                            lonj = lon_mc;
                        } else {
                            CalcUt calcul_ut = Swe03::calc_ut(utc_to_jd.julian_day_ut, astresAngle[j], OPTION_FLAG_SPEED);
                            lonj = calcul_ut.longitude;
                        }
                        js["aspect"][i]["liens"][k]["lon"] = lonj;
                    } else {
                        js["aspect"][i]["liens"][k]["aspect_id"] = Json::Value::null;
                        //js["aspect"][i]["liens"][k]["aspect_name"] = Json::Value::null;
                        //js["aspect"][i]["liens"][k]["asp"] = Json::Value::null;
                        //js["aspect"][i]["liens"][k]["orb"] = Json::Value::null;
                        js["aspect"][i]["liens"][k]["lon"] = Json::Value::null;
                    }
                    k++;
                }
            }
            break;
        }
        case JsonApiV2Option::JsonAspectsAsset:
        {
            for (int i = 0; i < ASPECTS_SEMISEXTILE ; ++i) {
                const char* res1 = asset_aspect2(i, color);
                if (res1 != nullptr) {
                    string a_aspect(res1);
                    js["aspect"][i]["asset"] = a_aspect;
                } else {
                    js["aspect"][i]["asset"] = Json::Value::null;
                }
                const char* res2 = text_aspect(i);
                if (res2 != nullptr) {
                    string t_aspect(res2);
                    js["aspect"][i]["name"] = t_aspect;
                } else {
                    js["aspect"][i]["name"] = Json::Value::null;
                }
                const char* r_ca = color_aspect2(i, color);
                if (r_ca != nullptr) {
                    string ca(r_ca);
                    js["aspect"][i]["color"] = ca;
                } else {
                    if (color == 1) {
                        js["aspect"][i]["color"] = "#ffffff";
                    } else {
                        js["aspect"][i]["color"] = "#000000";
                    }
                }
                int point = type_aspect(i);
                js["aspect"][i]["point"] = point;
            }
            break;
        }
        case JsonApiV2Option::JsonBodiesAsset:
        {
            auto astres = std::make_unique<int[]>(MAX_ASTRES + 2);
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
            astres[NOEUD_LUNAIRE_SUD + 1] = 98; // Asc
            astres[NOEUD_LUNAIRE_SUD + 2] = 99; // Mc

            int* astres_aspect = new int[MAX_ASTRES + 2];
            astres_aspect[SOLEIL] = ASTRE_SOLEIL;
            astres_aspect[LUNE] = ASTRE_LUNE;
            astres_aspect[MERCURE] = ASTRE_MERCURE;
            astres_aspect[VENUS] = ASTRE_VENUS;
            astres_aspect[MARS] = ASTRE_MARS;
            astres_aspect[JUPITER] = ASTRE_JUPITER;
            astres_aspect[SATURN] = ASTRE_SATURN;
            astres_aspect[URANUS] = ASTRE_URANUS;
            astres_aspect[NEPTUNE] = ASTRE_NEPTUNE;
            astres_aspect[PLUTON] = ASTRE_PLUTON;
            astres_aspect[NOEUD_LUNAIRE] = ASTRE_NOEUD_LUNAIRE;
            astres_aspect[CHIRON] = ASTRE_CHIRON;
            astres_aspect[CERES] = ASTRE_CERES;
            astres_aspect[NOEUD_LUNAIRE_SUD] = ASTRE_NOEUD_LUNAIRE_SUD;
            astres_aspect[NOEUD_LUNAIRE_SUD + 1] = 98;
            astres_aspect[NOEUD_LUNAIRE_SUD + 2] = 99;

            for (int i = 0; i < MAX_ASTRES + 2; ++i) {
                js["bodie"][i]["id"] = astres[i];
                const char* res1 = text_bodie(astres[i]);
                if (res1 != nullptr) {
                    string t_bodie(res1);
                    js["bodie"][i]["name"] = t_bodie;
                } else {
                    js["bodie"][i]["name"] = "";
                }
                const char* res2 = asset_bodie(astres[i]);
                if (res2 != nullptr) {
                    string a_bodie(res2);
                    js["bodie"][i]["asset"] = a_bodie;
                } else {
                    js["bodie"][i]["asset"] = Json::Value::null;
                }
                js["bodie"][i]["top"] = i * 25;
                if (i == 99) {
                    js["bodie"][i]["left"] = ((i - 1) * 25) + 1; // Mc sous Asc dans la présentation
                } else {
                    js["bodie"][i]["left"] = (i * 25) + 1; // 1 pixel de décalage pour que la bordure ne soit pas a l'extérieur du svg
                }
            }
            break;
        }
    }
    Json::StreamWriterBuilder writer;
    std::string output = Json::writeString(writer, js);
    return output;
}

const string SweBressaniDevCpp::decodeComma(const string& str) {
    std::string decoded = "";
    for (size_t i = 0; i < str.length(); ++i) {
        if (i <= str.length() - 3 && str.substr(i, 3) == "%2C") {
            decoded += ',';
            i += 2; // Avance de 2 caractères supplémentaires
        } else {
            decoded += str[i];
        }
    }
    return decoded;
}

float SweBressaniDevCpp::getZnorm(float angle) {
    int ang = static_cast<int>(angle) % 360;
    if (ang <= 360 / 2) {
        return static_cast<float>(ang);
    } else {
        return angle - 360.0f;
    }
}

float SweBressaniDevCpp::getClosestDistance(float angle1, float angle2)  {
    return getZnorm(angle2 - angle1);
}