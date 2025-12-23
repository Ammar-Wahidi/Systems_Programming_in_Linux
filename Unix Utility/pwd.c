#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define count 1024

// main_pwd
int main() {
    // Write your code here
    // Do not write a main() function. Instead, deal with pwd_main() as the main function of your program.
        char buf [count];

        if (getcwd(buf,count) == NULL)
        {
                printf("Error: Finding the directory path");
        }
        else
        {
                if(write(1,buf,strlen(buf))<0)
                        printf("Error in Writing\n");
                write(1, "\n", 1);
        }
        return 0;

}
