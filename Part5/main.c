#include "my_shell.h"

// Shell loop
// Input Parsing
// Command execution 
// Handle Built-in commands exp. cd, pwd, echo, env, setenv, unsetenv, which, exit
// Execute external commands
// Manage environment variables
// Manage Path
// Error Handling

// Built-ins: cd, pwd, echo, env, setenv, unsetenv, which, exit
// Binary: ls, cat.. we'll use executor
int shell_builts(char** args, char** env, char* initial_directory)
{
    // printf("Arg[0]: %s", args[0]);
    // printf("\n");
    if (my_strcmp(args[0], "cd") == 0) {
        return command_cd(args, initial_directory);
    } else if (my_strcmp(args[0], "pwd") == 0) {
        return command_pwd();
    } else if (my_strcmp(args[0], "echo") == 0) {
        return command_echo(args, env);
    } else if (my_strcmp(args[0], "env") == 0) {
        return command_env(env);
    } else if (my_strcmp(args[0], "which") == 0) {
        // return command_which(args, env);
    } else if (my_strcmp(args[0], "exit") == 0 || my_strcmp(args[0], "quit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        // Not a builtin command, execute as external command
        return executor(args, env);
    }
    return 0;
}


void shell_loop(char** env)
{
    char* input = NULL;
    size_t input_size = 0;

    char** args;

    char* initial_directory = getcwd(NULL, 0);

    while (1)
    {
        printf("[my_shell]> ");
        if (getline(&input, &input_size, stdin) == -1) // End of the file (EOF), ctrl + D
        {
            perror("getline");
            break;
        }    

        // printf("Input: %s", input);

        args = parse_input(input);

        // for (size_t i = 0; args[i]; i++)
        // {
        //     printf("Args: %s", args[i]);
        //     printf("\n");
        // }

        if (!args[0]) {
            return;
        } else if (my_strcmp(args[0], "setenv") == 0) {
            env = command_setenv(args, env);
        } else if (my_strcmp(args[0], "unsetenv") == 0) {
            env = command_unsetenv(args, env);
        } else {
            shell_builts(args, env, initial_directory);
        }
    }
    
    free_tokens(args);
    free(env);
} 

int main (int argc, char** argv, char** env)
{
    (void)argc;
    (void)argv;

    shell_loop(env);


    return 0;
}