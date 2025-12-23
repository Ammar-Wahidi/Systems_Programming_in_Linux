#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>



#define count 100

// main_mv
int main(int argc, char *argv[]) {
    // Write your code here
    // Do not write a main() function. Instead, deal with cp_main() as the main function of your program
        int fd1;
        int fd2;
        char buf [count];
        int num_read;

        if(argc != 3)
        {
                write(2, "usage: prog src dst\n", 20);
                return 1;
        }

        fd1 = open(argv[1],O_RDONLY);
        if (fd1<0)
            return 1;
        fd2 = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644);

        if (fd2<0)
        {
                close(fd1);
                return 1;
        }
        while((num_read = read(fd1,buf,count))>0)
        {
               if (write(fd2, buf, num_read) <0)
               {
                       printf("Couldn't write");
                       return 0;
               }
        }
        if (num_read < 0)
        {
                printf("Error in Reading");
        }
        if(unlink(argv[1])<0)
        {
            printf("Couldn't remove");
            return 0;
        }
        close(fd1);
        close(fd2);
        return 0;


}
