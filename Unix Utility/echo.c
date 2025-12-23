#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// main_echo
int main(int argc, char *argv[]) {
    // Write your code here
    // Do not write a main() function. Instead, deal with echo_main() as the main function of your program.
        if (argc < 2)
        {
                printf("\n");
                return 0;
        }
        int i;
        for (i=1;i<argc;i++)
        {
        if(write(1,argv[i],strlen(argv[i]))<0)
        {
                printf("write failed\n");
                return 0;
        }
        if (i!=argc-1)
            write(1," ",1);
        }
        write(1,"\n",1);
        return 0;

}
