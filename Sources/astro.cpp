#include <fcgi_stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "fcgio.h"
#include "Util.h"
#include "Swe.h"

using namespace std;

#if SW_DEBUG
    int main(int argc, char* argv[])
#else
    int main(void)
#endif
{
    // Initialisation FastCGI
    FCGX_Init();
    FCGX_Request request;
    FCGX_InitRequest(&request, 0, 0);

#if SW_DEBUG
    while (FCGI_Accept() >= 0) {
        string simulatedQueryString =
                "year=" + string(argv[1]) +
                "&month=" + string(argv[2]) +
                "&day=" + string(argv[3]) +
                "&hour=" + string(argv[4]) +
                "&min=" + string(argv[5]) +
                "&lat=" + string(argv[6]) +
                "&lng=" + string(argv[7]) +
                "&gmt=" + string(argv[8]) +
                "&color=" + string(argv[9]) +
                "&aspect_option=" + string(argv[10]);
        map<string, string> params = Util::parseQueryString(simulatedQueryString);
        string year = params["year"].c_str();
        string month = params["month"].c_str();
        string day = params["day"].c_str();
        string hour = params["hour"].c_str();
        string min = params["min"].c_str();
        string lat = params["lat"].c_str();
        string lng = params["lng"].c_str();
        string gmt = params["gmt"].c_str();
        string color = params["color"].c_str();
        string aspect_option = params["aspect_option"].c_str();
        bool sw_chart = true;
        bool sw_json = false;
#else
    while (FCGX_Accept_r(&request) == 0) {
        const char* queryString = FCGX_GetParam("QUERY_STRING", request.envp);
        string qS = queryString ? string(queryString) : "";
        string year;
        string month;
        string day;
        string hour;
        string min;
        string lat;
        string lng;
        string gmt;
        string color;
        string aspect_option;
        bool sw_chart;
        bool sw_json;
        if (queryString) {
            map<string, string> params = Util::parseQueryString(qS);
            year = params["year"];
            month = params["month"];
            day = params["day"];
            hour = params["hour"];
            min = params["min"];
            lat = params["lat"];
            lng = params["lng"];
            gmt = params["gmt"];
            color = params["color"];
            aspect_option = params["aspect_option"];
            sw_chart = params["sw_chart"] == "true" ? true : false;
            sw_json = params["sw_json"] == "true" ? true : false;
        }
#endif
        Swe sweInstance(year, month, day, hour, min, lat, lng, gmt, color, aspect_option);

        if (sw_chart) {
            const string svgOutput = sweInstance.Svg();
#if SW_DEBUG
            // cout << svgOutput << endl;
#else
            FCGX_PutS("Content-type:image/svg+xml\r\n\r\n", request.out);
            if (svgOutput.length() > static_cast<std::string::size_type>(std::numeric_limits<int>::max())) {
                cerr << "La chaîne est trop longue pour être traitée par FCGX_PutStr." << endl;
            } else {
                FCGX_PutStr(svgOutput.c_str(), static_cast<int>(svgOutput.length()), request.out);
            }
#endif
        } else if (sw_json) {

        } else {
            FCGX_PutS("Content-type: text/html\r\n", request.out);
            FCGX_PutS("\r\n", request.out);
            FCGX_PutS("<html><head><title>FastCGI Hello!</title></head><body>", request.out);
            FCGX_PutS("<h1>Params</h1>", request.out);
            //char buffer[256];
            //snprintf(buffer, sizeof(buffer), "<p>lat : %.2f</p>", f_lat);
            //FCGX_PutS(buffer, request.out);
            //FCGX_PutS("<p>lng : %.2f</p>", f_lng);
            FCGX_PutS("</body></html>", request.out);
        }
        FCGX_Finish_r(&request);
    }
    return 0;
}
