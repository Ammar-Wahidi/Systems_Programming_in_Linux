#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// femtoshell_main
int main(int argc, char *argv[]) {
    int status = 0;
    char line[20000];
    char command[20000];

    while (1) {
        write(1, "Femto shell prompt > ", 21);

        if (fgets(line, sizeof(line), stdin) == NULL) break;

        size_t end_of_line = strcspn(line, "\n") ;
        line[end_of_line] = '\0';

        if (end_of_line == 0)
        {
            write(1,"\n",1);
            continue;
        }

        int i = 0;
        while (i < end_of_line && line[i] != ' ' && line[i] != '\0')
        {
            command[i] = line[i];
            i++;
        }

        command[i] = '\0';

        if(strcmp(command,"exit")==0)
        {
            write(1,"\nGood Bye\n",1);
            break ;
        }
        else if (strcmp(command,"echo")==0)
        {
            write(1,"\n",1);
            int j ;
            for(j=i+1;j<end_of_line;j=j+1)
            {
                write(1,&line[j],1);
            }
            write(1,"\n",1);
        }
        else
        {

            write(1,"\nInvalid command\n",17);
            status = 1 ;
        }

    }
    write(1,"\n",1);
    return status;
}
