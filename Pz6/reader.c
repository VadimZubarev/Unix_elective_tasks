#include <stdio.h>
  
int main(void)
{
    char * filename = "hello.c";
    char buffer[256];
    FILE *fp = fopen(filename, "r");
    if(fp)
    {
        while((fgets(buffer, 256, fp))!=NULL)
        {
            printf("%s", buffer);
        }
        fclose(fp);
    }
    int a;
    a = 1/0;
    printf("%i", a); 
}
