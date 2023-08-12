#define SW_DEBUG 1
#include <fcgi_stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "fcgio.h"
#include "Util.h"
#include "Swe.h"

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
    string simulatedQueryString = "lat=" + string(argv[1]) + "&lng=" + string(argv[2]);
    map<string, string> params = Util::parseQueryString(simulatedQueryString);
    string lat = params["lat"].c_str();
    string lng = params["lng"].c_str();
#else
    char* queryString = FCGX_GetParam("QUERY_STRING", request.envp);
    string lat;
    string lng;
    if (queryString) {
        std::map<std::string, std::string> params = parseQueryString(queryString);
        std::string lat = params["lat"];
        std::string lng = params["lng"];
    }
#endif

#if SW_DEBUG
    while (FCGI_Accept() >= 0) {
#else
    while (FCGX_Accept_r(&request) == 0) {
#endif
        Swe sweInstance(2000, 5, 15, 12, 30, 45.67, 5.78, 1);

        printf("Content-type: text/html\r\n");
        printf("\r\n");
        printf("<html><head><title>FastCGI Hello!</title></head><body>");
        printf("<h1>Params</h1>");
        printf("<p>lat : %s</p>", lat.c_str());
        printf("<p>lng : %s</p>", lng.c_str());
        printf("</body></html>");
        FCGX_Finish_r(&request);
    }
    return 0;
}
