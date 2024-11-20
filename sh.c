#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"
#include <glob.h>

int pid;

int sh(int argc, char **argv, char **envp) {
    char *prompt = calloc(PROMPTMAX, sizeof(char));
    char *commandline = calloc(MAX_CANON, sizeof(char));
    char *command, *pwd, *owd;
    char **args = calloc(MAXARGS, sizeof(char*));
    int go = 1, status;
    struct passwd *password_entry;
    char *homedir;
    struct pathelement *pathlist;

    int uid = getuid();
    password_entry = getpwuid(uid);  /* Get passwd info */
    homedir = password_entry->pw_dir; /* Home directory */

    /* Get the current working directory */
    if ((pwd = getcwd(NULL, PATH_MAX+1)) == NULL) {
        perror("getcwd");
        exit(2);
    }
    owd = strdup(pwd);
    prompt[0] = ' '; prompt[1] = '\0';

    /* Put PATH into a linked list */
    pathlist = get_path();

    while (go) {
        /* Display the prompt */
        if (getcwd(pwd, PATH_MAX) == NULL) {
            perror("getcwd");
            continue;
        }
        printf("%s [%s]> ", prompt, pwd);
        fflush(stdout);

        /* Get command line */
        if (fgets(commandline, MAX_CANON, stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        /* Remove newline from command line */
        commandline[strcspn(commandline, "\n")] = '\0';

        /* Parse command and arguments */
        command = strtok(commandline, " ");
        int i = 0;
        while (command != NULL && i < MAXARGS - 1) {
            args[i++] = command;
            command = strtok(NULL, " ");
        }
        args[i] = NULL;

        /* Handle empty command */
        if (args[0] == NULL) continue;

        /* Built-in command: exit */
        if (strcmp(args[0], "exit") == 0) {
            go = 0;
            continue;
        }

        /* Built-in command: cd */
        else if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                chdir(homedir);
            } else {
                if (chdir(args[1]) != 0) {
                    perror("chdir");
                }
            }
            continue;
        }

        /* Built-in command: pwd */
        else if (strcmp(args[0], "pwd") == 0) {
            if (getcwd(pwd, PATH_MAX) == NULL) {
                perror("getcwd");
            } else {
                printf("%s\n", pwd);
            }
            continue;
        }

        /* Built-in command: list */
        else if (strcmp(args[0], "list") == 0) {
            if (args[1] == NULL) {
                list(".");
            } else {
                for (int j = 1; args[j] != NULL; j++) {
                    printf("\n%s:\n", args[j]);
                    list(args[j]);
                }
            }
            continue;
        }

        /* Built-in command: pid */
        else if (strcmp(args[0], "pid") == 0) {
            printf("Process ID: %d\n", getpid());
            continue;
        }

        /* Built-in command: prompt */
        else if (strcmp(args[0], "prompt") == 0) {
            if (args[1] == NULL) {
                printf("Error: No prompt string provided\n");
            } else {
                strncpy(prompt, args[1], PROMPTMAX - 1);
                prompt[PROMPTMAX - 1] = '\0';
            }
            continue;
        }

        /* Built-in command: printenv */
        else if (strcmp(args[0], "printenv") == 0) {
            char **env = envp;
            while (*env) {
                printf("%s\n", *env);
                env++;
            }
            continue;
        }

        /* Built-in command: setenv */
        else if (strcmp(args[0], "setenv") == 0) {
            if (args[1] != NULL && args[2] != NULL) {
                setenv(args[1], args[2], 1);
            } else {
                printf("Usage: setenv VAR VALUE\n");
            }
            continue;
        }

        /* External command execution */
        pid_t pid = fork();
        if (pid == 0) {
            /* In child process */
            execvp(args[0], args);
            perror("execvp");  /* Exec failed */
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            /* In parent process */
            waitpid(pid, &status, 0);  /* Wait for child to finish */
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                printf("Command exited with status %d\n", WEXITSTATUS(status));
            }
        } else {
            perror("fork");
        }
    }

    /* Clean up */
    free(prompt);
    free(commandline);
    free(args);
    free(pwd);
    free(owd);
    return 0;
}

char *which(char *command, struct pathelement *pathlist) {
    /* Search for the command in the pathlist */
    struct pathelement *current = pathlist;
    while (current != NULL) {
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", current->element, command);
        if (access(path, X_OK) == 0) {
            return strdup(path);
        }
        current = current->next;
    }
    return NULL;  /* Command not found */
}

void list(char *dir) {
    /* List files in the directory using opendir() and readdir() */
    DIR *dp;
    struct dirent *entry;

    if ((dp = opendir(dir)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dp);
}
