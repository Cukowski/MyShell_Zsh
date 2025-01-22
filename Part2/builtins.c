#include "my_shell.h"

// cd, cd [path], cd - (previous dir), cd ~ (home dir), cd .., handle non exsiting dirs, permission issues
int command_cd(char** args, char* init_dir)
{
    (void)init_dir;
    if (args[1] == NULL) {
        printf("cd: expected argument \"cd [path]\"\n");
    } else if (chdir(args[1]) == 0) {
        // printf("CD worked!\n");
    } else {
        perror("CD");
    }
    return 0;
}

int command_pwd()
{
    char* cwd = NULL;

    // Use dynamic allocation
    cwd = getcwd(NULL, 0);
    if (cwd != NULL) {
        printf("%s\n", cwd);
        free(cwd);
    } else {
        perror("getcwd");
    }
    return 0;
}

// echo Hello World, echo -n Hello, echo $PATH
int command_echo(char** args, char** env)
{
    int new_line = 1; // Default echo ends with newline
    size_t i = 1; // Skipping the -n

    if (args[1] != NULL && my_strcmp(args[1], "-n") == 0) {
        new_line = 0;
        i++;
    }

    // Proces remaining args
    for (; args[i]; i++)
    {
        if (args[i][0] == '$') { // Handle env vars
            char* value = my_getenv(args[i] + 1, env); // Skip $ and get the variable
            if (value) {
                printf("%s", value);
            } else {
                printf(" ");
            }
        } else {
            printf("%s", args[i]);
        }

        if (args[i + 1] != NULL) {
            new_line = 0;
            printf(" "); 
        }
        
        if (new_line) {
            printf("\n");
        }
        new_line = 1;
    }
    return 0;
}

int command_env(char** env)
{
    size_t index = 0;
    while (env[index])
    {
        printf("%s\n", env[index]);
        index++;
    }
    return 0;    
}

int command_which(char** args, char** env);
