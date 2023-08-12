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
        string simulatedQueryString = "lat=" + string(argv[1]) + "&lng=" + string(argv[2]);
        map<string, string> params = Util::parseQueryString(simulatedQueryString);
        string lat = params["lat"].c_str();
        string lng = params["lng"].c_str();
#else
    while (FCGX_Accept_r(&request) == 0) {
        std::cerr << "Information de log ici 0." << endl;
        const char* queryString = FCGX_GetParam("QUERY_STRING", request.envp);
        std::cerr << "Information de log ici 00." << endl;
        string qS = queryString ? string(queryString) : "";
        std::cerr << "Information de log ici 000." << endl;
        string lat;
        string lng;
        std::cerr << "Information de log ici 1." << endl;
        if (queryString) {
            map<string, string> params = Util::parseQueryString(qS);
            string lat = params["lat"];
            string lng = params["lng"];
        }
#endif
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

        std::cerr << "Information de log ici 2." << endl;

        std::cerr << "Information de log ici 3." << endl;
        Swe sweInstance(2000, 5, 15, 12, 30, f_lat, f_lng, 1);

        printf("Content-type: text/html\r\n");
        printf("\r\n");
        printf("<html><head><title>FastCGI Hello!</title></head><body>");
        printf("<h1>Params</h1>");
        printf("<p>lat : %.2f</p>", f_lat);
        printf("<p>lng : %.2f</p>", f_lng);
        printf("</body></html>");
        std::cerr << "Information de log ici 4." << endl;
        FCGX_Finish_r(&request);
        std::cerr << "Information de log ici 5." << endl;
    }
    return 0;
}
