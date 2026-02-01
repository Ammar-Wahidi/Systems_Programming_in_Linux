#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 4096
#define CMD_SIZE 256
#define MAXARGS 128

// picoshell_main
int main(int argc, char *argv[])
{
    char buf[BUF_SIZE];
    char command[CMD_SIZE];

    int status = 0;
    int last_status = 0;

    while (1) {
        printf("Pico shell prompt > ");
        fflush(stdout);

        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;

        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n')
            buf[len - 1] = 0;

        if (strlen(buf) == 0)
            continue;

        int i = 0;
        while (buf[i] != 0 && buf[i] != ' ' && i < CMD_SIZE - 1) {
            command[i] = buf[i];
            i++;
        }
        command[i] = 0;

        /* exit */
        if (strcmp(command, "exit") == 0) {
            printf("Good Bye\n");
            break;
        }

        /* echo */
        else if (strcmp(command, "echo") == 0) {
            int j = i + 1;
            int first = 1;

            while (buf[j]) {
                while (buf[j] == ' ')
                    j++;

                if (!buf[j])
                    break;

                if (!first)
                    printf(" ");

                while (buf[j] && buf[j] != ' ')
                    putchar(buf[j++]);

                first = 0;
            }

            printf("\n");
            last_status = 0;
        }

        /* pwd */
        else if (strcmp(command, "pwd") == 0) {

            char cwd[BUF_SIZE];
            if (getcwd(cwd, sizeof(cwd))) {
                printf("%s\n", cwd);
                last_status = 0;
            } else {
                perror("pwd");
                last_status = 1;
            }
        }

        /* cd */
        else if (strcmp(command, "cd") == 0) {

            int j = i + 1;
            while (buf[j] == ' ')
                j++;

            if (!buf[j]) {
                if (chdir(getenv("HOME")) == 0)
                    last_status = 0;
                else
                    last_status = 1;
                continue;
            }

            char path[BUF_SIZE];
            strcpy(path, &buf[j]);

            if (path[0] == '~') {
                chdir(getenv("HOME"));
                last_status = 0;
                continue;
            }

            if (chdir(path) == 0)
                last_status = 0;
            else {
                printf("cd: %s: No such file or directory\n", path);
                last_status = 1;
            }
        }


        else {

            pid_t pid = fork();

            if (pid > 0) {
                int wstatus;
                waitpid(pid, &wstatus, 0);

                if (WIFEXITED(wstatus))
                    last_status = WEXITSTATUS(wstatus);
                else
                    last_status = 1;
            }

            else if (pid == 0) {

                char buf_copy[BUF_SIZE];
                strcpy(buf_copy, buf);

                char *args[MAXARGS];
                int argi = 0;

                char *token = strtok(buf_copy, " ");
                while (token && argi < MAXARGS - 1) {
                    args[argi++] = token;
                    token = strtok(NULL, " ");
                }
                args[argi] = NULL;

                execvp(args[0], args);

                printf("%s: command not found\n", args[0]);
                fflush(stdout);
                exit(1);
            }

            else {
                perror("fork");
                last_status = 1;
            }
        }
    }

    return last_status;
}



