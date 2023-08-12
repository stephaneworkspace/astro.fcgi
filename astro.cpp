#include <fcgi_stdio.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    while (FCGI_Accept() >= 0) {
        printf("Content-type: text/html\r\n");
        printf("\r\n");
        printf("<html><head><title>FastCGI Hello!</title></head><body>");
        printf("<h1>Hello, World!</h1>");
        printf("</body></html>");
    }
    return 0;
}
