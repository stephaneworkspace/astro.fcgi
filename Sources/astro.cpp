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
                "&gmt=" + string(argv[8]);
        map<string, string> params = Util::parseQueryString(simulatedQueryString);
        string year = params["year"].c_str();
        string month = params["month"].c_str();
        string day = params["day"].c_str();
        string hour = params["hour"].c_str();
        string min = params["min"].c_str();
        string lat = params["lat"].c_str();
        string lng = params["lng"].c_str();
        string gmt = params["gmt"].c_str();
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
        }
#endif
        int i_year = 0;
        try {
            i_year = stoi(year);
        } catch (const invalid_argument& ia) {
            i_year = 0;
        } catch (const out_of_range& oor) {
            i_year = 0;
        }
        int i_month = 0;
        try {
            i_month = stoi(month);
        } catch (const invalid_argument& ia) {
            i_month = 0;
        } catch (const out_of_range& oor) {
            i_month = 0;
        }
        int i_day = 0;
        try {
            i_day = stoi(day);
        } catch (const invalid_argument& ia) {
            i_day = 0;
        } catch (const out_of_range& oor) {
            i_day = 0;
        }
        int i_hour = 0;
        try {
            i_hour = stoi(hour);
        } catch (const invalid_argument& ia) {
            i_hour = 0;
        } catch (const out_of_range& oor) {
            i_hour = 0;
        }
        int i_min = 0;
        try {
            i_min = stoi(min);
        } catch (const invalid_argument& ia) {
            i_min = 0;
        } catch (const out_of_range& oor) {
            i_min = 0;
        }
        double f_lat = 0.0;
        try {
            f_lat = stof(lat);
        } catch (const invalid_argument& ia) {
            f_lat = 0.0;
        } catch (const out_of_range& oor) {
            f_lat = 0.0;
        }
        double f_lng = 0.0;
        try {
            f_lng = stof(lng);
        } catch (const invalid_argument& ia) {
            f_lng = 0.0;
        } catch (const out_of_range& oor) {
            f_lng = 0.0;
        }
        double f_gmt = 0.0;
        try {
            f_gmt = stof(gmt);
        } catch (const invalid_argument& ia) {
            f_gmt = 0.0;
        } catch (const out_of_range& oor) {
            f_gmt = 0.0;
        }
        Swe sweInstance(i_year, i_month, i_day, i_hour, i_min, f_lat, f_lng, f_gmt);

#if SW_DEBUG
        printf("Content-type: text/html\r\n");
        printf("\r\n");
        printf("<html><head><title>FastCGI Hello!</title></head><body>");
        printf("<h1>Params</h1>");
        printf("<p>lat : %.2f</p>", f_lat);
        printf("<p>lng : %.2f</p>", f_lng);
        printf("</body></html>");
        FCGX_Finish_r(&request);
#else
        FCGX_PutS("Content-type: text/html\r\n", request.out);
        FCGX_PutS("\r\n", request.out);
        FCGX_PutS("<html><head><title>FastCGI Hello!</title></head><body>", request.out);
        FCGX_PutS("<h1>Params</h1>", request.out);
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "<p>lat : %.2f</p>", f_lat);
        FCGX_PutS(buffer, request.out);
        //FCGX_PutS("<p>lng : %.2f</p>", f_lng);
        FCGX_PutS("</body></html>", request.out);
        FCGX_Finish_r(&request);
#endif
    }
    return 0;
}
