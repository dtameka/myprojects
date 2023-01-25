#include <fcgi_stdio.h>
#include <stdlib.h>

int main(void) {
while (FCGI_Accept() >= 0) {
    printf("Content-type: text/html;\n\n");
    printf("<title>Prime FastCGI</title>\n"
           "<h2>Prime FastCGI</h2>\n");
    printf("<h1>Hello, World! I am dtameka</h1>\n");
    }
return 0;
}
