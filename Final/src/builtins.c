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

char* find_command_in_path(const char* command, char** env);

int command_which(char** args, char** env)
{
    if (args[1] == NULL) {
        printf("which: expected argument\n");
        return 1;
    }

    // List of the built-ins
    const char* built_in_commands[] = {"cd", "pwd", "echo", "env", "setenv", "unsetenv", "which", "exit", NULL};
    for (size_t i = 0; built_in_commands[i]; i++) {
        if (my_strcmp(args[1], built_in_commands[i]) == 0) {
            printf("%s: shell built-in command\n", args[1]);
            return 0;
        }
    }

    // Check external commands
    char* full_path = find_command_in_path(args[1], env);
    if (full_path != NULL) {
        printf("%s\n", full_path);
        free(full_path);
        return 0;
    } else {
        printf("which: %s command not found\n", args[1]);
        return 1;
    }
}

// Function to search for the command in PATH
char* find_command_in_path(const char* command, char** env)
{
    char* path_env = NULL; // Store the PATH value
    char* path = NULL; // Duplicate of PATH
    char* token = NULL; // Tokenize dirs from the PATH
    char full_path[1024]; // BUffer to construct full paths

    // Locate the PATH
    for (size_t i = 0; env[i]; i++) {
        if (my_strncmp(env[i], "PATH=", 5) == 0) {
            path_env = env[i] + 5; // Skip PATH= prefix
            break;
        }
    }    
    if (path_env == NULL) {
        return NULL; // No path
    } 

    // Dupliacte the PATH avoiding modifying the original
    path = my_strdup(path_env);
    if(path == NULL) {
        perror("my_strdup");
        return NULL;
    }

    // Split PATH into individual dirs using ':' delimiter
    token = my_strtok(path, ":");
    while (token != NULL) {
        // Constract full path, check for '/'
        size_t len = my_strlen(token);
        if (token[len - 1] != '/') {
            snprintf(full_path, sizeof(full_path), "%s%s%s", token, "/", command);
        } else {
            snprintf(full_path, sizeof(full_path), "%s%s", token, command);
        }

        // Check if the command exists as executable
        if (access(full_path, X_OK) == 0) {
            free(path);
            return my_strdup(full_path); // found commands path
        }

        token = my_strtok(NULL, ":"); // move to the next dir
    }

    free(path);
    return NULL;
}

// Helper function to count env vars
int count_env_vars(char** env) {
    int count = 0;
    while (env[count])
    {
        count++;
    }
    return count;
}

// Function to set an environement variable
char** command_setenv(char** args, char** env)
{
    if (args[1] == NULL) {
        printf("Usage:  setenv VAR=value\nor\tsetenv <variable> <value>\n");
        return env;
    }
    
    int env_count = count_env_vars(env);
    char** new_env = malloc((env_count + 2) * sizeof(char*));
    if(!new_env) {
        perror("malloc");
        return env;
    }

    // Copy existing environment variables
    for (int i = 0; i < env_count; i++)
    {
        new_env[i] = my_strdup(env[i]);
        if (!new_env[i]) {
            perror("strdup");
            for (int j = 0; j < i; j++) {
                free(new_env[j]);
            }
            free(new_env);
            return env;            
        }
    }

    // Determine the format of the uinput and create the new variable
    char* new_var = NULL;
    if(args[2] == NULL) {  // Format Var=value
        new_var = my_strdup(args[1]);
    } else {
        new_var = malloc(my_strlen(args[1]) + my_strlen(args[2]) + 2);
        if (new_var) {
            sprintf(new_var, "%s=%s", args[1], args[2]);
        }
    }

    if(!new_var) {
        perror("malloc");
        for (int i = 0; i < env_count; i++) {
            free(new_env[i]);
        }
        free(new_env);
        return env;
    }

    new_env[env_count] = new_var;
    new_env[env_count  + 1] = NULL;

    // Free the old env array
    // for (size_t i = 0; env[i]; i++) {
    //     free(env[i]);
    // }
    // free(env);  

    return new_env;
}

// Function to unset environment variables
char** command_unsetenv(char** args, char** env){
    if (!args[1]) {
        printf("Usage: unsetenv <variable>\n");
        return env;
    }

    int env_count = count_env_vars(env);
    char** new_env = malloc(env_count * sizeof(char*));
    if(!new_env) {
        perror("malloc");
        return env;
    }

    int j = 0, found = 0;
    for (int i = 0; i < env_count; i++) { // var=123
        if (my_strncmp(env[i], args[1], my_strlen(args[1])) == 0 && env[i][my_strlen(args[1])] == '=') {
            found = 1;
            free(env[i]); // Free the matching variable
        } else {
            new_env[j++] = env[i];
        }
    }

    if (!found) {
        printf("Variable %s not found in environment\n", args[1]);
        free(new_env);
        return env;
    }

    new_env[j] = NULL;
    // free(env);
    return new_env;
}