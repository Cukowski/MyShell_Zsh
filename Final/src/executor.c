#include "my_shell.h"

// Executes a command by forking and running it in a child process
int executor(char** args, char** env)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) // child process
    { 
        if (child_process(args, env)) {
            perror("execve");
            return 1;
        }
    } 
    else // Parent process
    { 
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return 1;
        }
        if (WIFSIGNALED(status)) {
            printf("Process terminated by signal: %d\n", WTERMSIG(status));
        }
        // if (WTERMSIG(status) == SIGSEGV) {
        //     perror("segmentation fault");
        // }
    }
    return 1;
}

// Attempts to execute the command by searching paths and the current directory
int child_process(char** args, char** env) 
{
    char* path_string = get_path(env);
    int num_paths;
    char** path_list = split_paths(path_string, &num_paths);

    // access() execve()
    for (int i = 0; i < num_paths; i++) {
        char full_path[MAX_INPUT];
        snprintf(full_path, sizeof(full_path), "%s/%s", path_list[i], args[0]);

        if (access(full_path, X_OK) == 0) {
            execve(full_path, args, env);
        }
    }

    for (int i = 0; path_list[i]; i++)
        free(path_list[i]);
    free(path_string);
    free(path_list);

    // Try executing the command in the current working directory
    char* cwd = NULL;
    cwd = getcwd(NULL, 0);
    if(cwd == NULL) {
        perror("getcwd");
        return 1;
    }

    char full_cwd_path[MAX_INPUT];
    snprintf(full_cwd_path, sizeof(full_cwd_path), "%s/%s", cwd, args[0]);
    execve(full_cwd_path, args, env);

    perror("execve");
    
    return 1;
}

// Fetches the PATH environment variable
char* get_path(char** env) {
    for (int i = 0; env[i]; i++) {
        if(my_strncmp(env[i], "PATH=", 5) == 0) {
            return my_strdup(env[i] + 5);
        }
    }
    return NULL;
}

// Split the PATH string into individual paths
char** split_paths(char* paths, int* count) {
    char** result = NULL;
    char* token;
    size_t size_of_path = my_strlen(paths);
    char paths_copy[size_of_path];

    my_strncpy(paths_copy, paths, sizeof(paths_copy));
    paths_copy[sizeof(paths_copy) - 1] = '\0';

    token = my_strtok(paths_copy, ":");
    *count = 0;

    while (token) {
        result = realloc(result, ((*count + 1) * sizeof(char*)));
        if (!result) {
            perror("realloc");
            return NULL;
        }
        result[*count] = my_strdup(token);
        if (!result[*count]) {
            perror("my_strdup");
            return NULL;
        }

        (*count)++;
        token = my_strtok(NULL, ":");
    }

    return result;
}