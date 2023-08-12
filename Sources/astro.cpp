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
                "&color=" + string(argv[9]);
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
        }
#endif
        Swe sweInstance(year, month, day, hour, min, lat, lng, gmt, color, "0");

#if SW_DEBUG
        printf("Content-type: text/html\r\n");
        printf("\r\n");
        printf("<html><head><title>FastCGI Hello!</title></head><body>");
        printf("<h1>Params</h1>");
        //printf("<p>lat : %.2f</p>", f_lat);
        //printf("<p>lng : %.2f</p>", f_lng);
        printf("</body></html>");
        cout << sweInstance.getAo() << endl;
        FCGX_Finish_r(&request);
#else
        FCGX_PutS("Content-type: text/html\r\n", request.out);
        FCGX_PutS("\r\n", request.out);
        FCGX_PutS("<html><head><title>FastCGI Hello!</title></head><body>", request.out);
        FCGX_PutS("<h1>Params</h1>", request.out);
        //char buffer[256];
        //snprintf(buffer, sizeof(buffer), "<p>lat : %.2f</p>", f_lat);
        //FCGX_PutS(buffer, request.out);
        //FCGX_PutS("<p>lng : %.2f</p>", f_lng);
        FCGX_PutS("</body></html>", request.out);
        FCGX_Finish_r(&request);
#endif
    }
    return 0;
}
