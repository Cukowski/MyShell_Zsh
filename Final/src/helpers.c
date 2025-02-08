#include "my_shell.h"

// 0: The strings are equal
// < 0: str1 < str2
// > 0: str1 > str2
// Compare the given strings.
int my_strcmp(const char* str1, const char* str2)
{
    if (str1 == NULL || str2 == NULL) return 1;

    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }

    return *(unsigned char*)str1 - *(unsigned char*)str2;    
}

// Length of the given strting.
int my_strlen(const char* str)
{
    if (str == NULL) return -1;

    size_t len = 0;
    while (*str) {
        len++;
        str++;
    }
    return len;
}

// 0: if strin are equal to n characters
// <0: str1 < str2 in the first n characters
// >0: str1 > str2 in the first n characters
// Compare the n length of given strings.
int my_strncmp(const char* str1, const char* str2, size_t n)
{
    if (str1 == NULL || str2 == NULL) return 1;

    size_t i = 0;

    while (i < n && str1[i] && str2[i]) {
        if (str1[i] != str2[i]) {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }
        i++;
    }

    if (i == n) {
        return 0;
    }
    
    return (unsigned char)str1[i] - (unsigned char)str2[i];
}

// Searches the environment variables for the specified name and returns its value.
char* my_getenv(const char *name, char**env)
{
    if (name == NULL || env == NULL) {
        return NULL;
    }

    size_t name_len = my_strlen(name);

    for (size_t i = 0; env[i]; i++)
    {
        // Check if the curr env var starts with 'name='
        if (my_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=') {
            return &env[i][name_len + 1];
        }
    }
    return NULL;
}

// Creates a duplicate of the input string by allocating new memory.
char* my_strdup(const char* str)
{
    if (str == NULL) return NULL;

    size_t len = my_strlen(str);

    char* duplicated = (char*)malloc((len+1) * sizeof(char));

    if (duplicated == NULL) return NULL;

    my_strcpy(duplicated, str);

    return duplicated;
}

// Copies the content of the source string into the destination string.
char* my_strcpy(char* dest, const char* src)
{
    if (src == NULL) return NULL;
    char* ret = dest;
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return ret;    
}

// Locates the first occurrence of a character in a string.
char* my_strchr(const char* str, char c)
{
    while(*str) {
        if (*str == c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

// Tokenizes a string by splitting it based on a set of delimiter characters.
char* my_strtok(char* input_string, const char* delimiter)
{
    static char* next_token = NULL;

    // If input is NULL
    if (input_string == NULL) {
        input_string = next_token;
    }
    if (input_string == NULL) {
        return NULL;
    }

    while (*input_string && my_strchr(delimiter, *input_string)) {
        input_string++;
    }

    if (*input_string == '\0') {
        next_token = NULL;
        return NULL;
    }

    char* token = input_string;

    while (*input_string && !my_strchr(delimiter, *input_string)) {
        input_string++;
    }

    if (*input_string) {
        *input_string = '\0';
        next_token = input_string + 1;
    } else {
        next_token = NULL;
    }

    return token; 
}

// Copies up to n characters from the source string to the destination string, ensuring null-termination.
char* my_strncpy(char* dest, const char* src, size_t n) {
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    for (; i < n; i++) {
        dest[i] = '\0';
    }

    return dest;
}