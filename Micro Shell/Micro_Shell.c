#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>


#define BUF_SIZE 4096
#define CMD_SIZE 256
#define MAXARGS 128

extern char **environ;

// Structure for local shell variables
typedef struct {
    char *name;
    char *value;
} Var;

Var *vars = NULL;
int var_count = 0;

// Find local variable by name
const char* find_local(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0)
            return vars[i].value;
    }
    return NULL;
}

// Add or update local variable
void add_local(const char *name, const char *value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            free(vars[i].value);
            vars[i].value = strdup(value);
            return;
        }
    }
    vars = (Var*)realloc(vars, (var_count + 1) * sizeof(Var));
    vars[var_count].name = strdup(name);
    vars[var_count].value = strdup(value);
    var_count++;
}

// Process variable substitution in a token
char *process_var(const char *token) {
    char buf[BUF_SIZE];
    int bi = 0;

    for (int i = 0; token[i]; i++) {
        if (token[i] == '$') {
            i++;
            char varname[CMD_SIZE];
            int vi = 0;

            while (token[i] && (isalnum(token[i]) || token[i]=='_'))
                varname[vi++] = token[i++];
            varname[vi] = 0;
            i--; // adjust for loop

            const char *val = find_local(varname);
            if (!val) val = "";
            for (int j = 0; val[j]; j++)
                buf[bi++] = val[j];
        } else {
            buf[bi++] = token[i];
        }
    }
    buf[bi] = 0;
    return strdup(buf);
}

// microshell_main
int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    char buf[BUF_SIZE];
    char command[CMD_SIZE];
    int last_status = 0;

    while (1) {
        printf("Nano Shell Prompt > ");
        fflush(stdout);

        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;

        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n')
            buf[len - 1] = 0;

        if (strlen(buf) == 0)
            continue;

        // Extract command word
        int i = 0;
        while (buf[i] != 0 && buf[i] != ' ' && i < CMD_SIZE - 1) {
            command[i] = buf[i];
            i++;
        }
        command[i] = 0;

        // EXIT
        if (strcmp(command, "exit") == 0) {
            printf("Good Bye\n");
            break;
        }

        // LOCAL VARIABLE ASSIGNMENT
        else if (strchr(buf, '=') && buf[0] != ' ') {
            char *eq = strchr(buf, '=');
            if (*(eq - 1) == ' ' || *(eq + 1) == ' ') {
                printf("Invalid command\n");
                last_status = 1;
                continue;
            }
            *eq = 0;
            char *name = buf;
            char *value = eq + 1;
            if (!*name || !*value) {
                printf("Invalid command\n");
                last_status = 1;
                continue;
            }
            add_local(name, value);
            last_status = 0;
        }

        // EXPORT
        else if (strcmp(command, "export") == 0) {
            int j = i + 1;
            while (buf[j] == ' ') j++;
            if (!buf[j]) {
                printf("Invalid command\n");
                last_status = 1;
                continue;
            }
            char *arg = &buf[j];
            char *eq = strchr(arg, '=');
            if (eq) { // export x=5
                *eq = 0;
                char *name = arg;
                char *value = eq + 1;
                if (!*name || !*value) {
                    printf("Invalid command\n");
                    last_status = 1;
                    continue;
                }
                setenv(name, value, 1);
            } else { // export x
                const char *val = find_local(arg);
                if (!val) val = "";
                setenv(arg, val, 1);
            }
            last_status = 0;
        }

        // PWD
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

        // CD
        else if (strcmp(command, "cd") == 0) {
            int j = i + 1;
            while (buf[j] == ' ') j++;
            char *raw_target = buf[j] ? &buf[j] : getenv("HOME");
            char *expanded_target = process_var(raw_target);

            if (expanded_target[0] == '~') {
                free(expanded_target);
                expanded_target = strdup(getenv("HOME"));
            }

            if (chdir(expanded_target) == 0)
                last_status = 0;
            else {
                printf("cd: %s: No such file or directory\n", expanded_target);
                last_status = 1;
            }

            free(expanded_target);
        }

        // ECHO
        else if (strcmp(command, "echo") == 0) {
            char *outfile=NULL, *errfile=NULL;
            char *args[MAXARGS]; int argi=0;
            char *tok = strtok(&buf[i+1]," ");
            while (tok) {
                if (!strcmp(tok,">")) { tok=strtok(NULL," "); if(!tok){printf("syntax error\n"); break;} outfile=process_var(tok); }
                else if (!strcmp(tok,"2>")) { tok=strtok(NULL," "); if(!tok){printf("syntax error\n"); break;} errfile=process_var(tok); }
                else args[argi++] = process_var(tok);
                tok=strtok(NULL," ");
            }
            args[argi]=NULL;

            int saved_out=-1,saved_err=-1;
            if (outfile) {
                saved_out=dup(1);
                int fd=open(outfile,O_WRONLY|O_CREAT|O_TRUNC,0644);
                if(fd<0){
                    printf("%s: Permission denied\n",outfile);
                    last_status=1;
                    goto echo_cleanup;
                }
                dup2(fd,1);
                close(fd);
            }
            if (errfile) {
                saved_err=dup(2);
                int fd=open(errfile,O_WRONLY|O_CREAT|O_TRUNC,0644);
                if(fd<0){
                    printf("%s: Permission denied\n",errfile);
                    last_status=1;
                    goto echo_cleanup;

                }
                dup2(fd,2);
                close(fd);

            }

            for (int j=0;j<argi;j++){ if(j) printf(" "); printf("%s",args[j]); } printf("\n");
            last_status=0;

            echo_cleanup:
            if (saved_out>=0){dup2(saved_out,1); close(saved_out);}
            if (saved_err>=0){dup2(saved_err,2); close(saved_err);}
            for(int j=0;j<argi;j++) free(args[j]);
            if(outfile) free(outfile); if(errfile) free(errfile);
        }

        // EXTERNAL COMMANDS
        else {
            pid_t pid = fork();
            if (pid > 0) {
                int wstatus;
                waitpid(pid, &wstatus, 0);
                last_status = WIFEXITED(wstatus) ? WEXITSTATUS(wstatus) : 1;
            } else if (pid == 0) {
                char buf_copy[BUF_SIZE];
                strcpy(buf_copy, buf);

                char *infile = NULL;
                char *outfile = NULL;
                char *errfile = NULL;


                char *args[MAXARGS];
                int argi = 0;
                char *token = strtok(buf_copy, " ");
                while (token && argi < MAXARGS - 1) {
                    if(strcmp(token, "<") == 0)
                    {
                        infile = strtok(NULL, " ");
                        infile = process_var(infile);
                        if(!infile)
                        {
                            printf("syntax error\n");
                            exit(1);
                        }

                        if (infile)
                            {
                                int fd = open(infile,O_RDONLY);
                                if (fd<0)
                                {
                                    //perror("cannot access %s: No such file or directory\n",infile);
                                    fprintf(stderr, "cannot access %s: No such file or directory\n", infile);
                                    exit(1) ;
                                }

                                dup2(fd,0);
                                close(fd);
                            }

                        token = strtok(NULL, " ");
                        continue;
                    }
                    else if (strcmp(token,">")==0)
                    {
                        outfile = strtok(NULL, " ");
                        outfile = process_var(outfile);
                        if(!outfile)
                        {
                            printf("syntax error\n");
                            exit(1);
                        }

                        if (outfile)
                            {
                                int fd = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, 0644);
                                if (fd<0)
                                {
                                //printf("cannot access %s: No such file or directory\n",outfile);
                                exit(1);
                                }
                                dup2(fd,1);
                                close(fd);
                            }

                        token = strtok(NULL," ");
                        continue;
                    }
                    else if (strcmp(token,"2>")==0)
                    {
                        errfile = strtok(NULL," ");
                        errfile = process_var(errfile);
                        if(!errfile)
                        {
                            printf("syntax error\n");
                            exit(1);
                        }

                        if (errfile)
                            {
                                int fd = open(errfile, O_WRONLY|O_CREAT|O_TRUNC, 0644);
                                if (fd<0)
                                {
                                //printf("cannot access %s: No such file or directory\n",errfile);
                                exit(1);
                                }
                                dup2(fd,2);
                                close(fd);
                            }

                        token = strtok(NULL," ");
                        continue;
                    }

                    args[argi++] = process_var(token);
                    token = strtok(NULL, " ");
                }
                args[argi] = NULL;


                execvp(args[0], args);

                printf("%s: command not found\n", args[0]);
                fflush(stdout);
                for (int k = 0; k < argi; k++) free(args[k]);
                exit(1);
            } else {
                perror("fork");
                last_status = 1;
            }
        }
    }

    // Free all local variables
    for (int i = 0; i < var_count; i++) {
        free(vars[i].name);
        free(vars[i].value);
    }
    free(vars);

    return last_status;
}
