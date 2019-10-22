#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#define READBUF_SIZE 512
#define TOKENBUF_SIZE 128
#define DELI " "

char **parse_line(char *line, char * delimeters)
{
    int bufsize = TOKENBUF_SIZE, position = 0;
    char **tokens = (char **)malloc(sizeof(char*) * bufsize);
    char *token;
    if(!tokens)
    {
        printf("Allocation Fail.\n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line, delimeters);
    while(token != NULL)
    {
        tokens[position] = token;
        position++;
        if(position >= bufsize){
            bufsize += TOKENBUF_SIZE;
            tokens = (char **)realloc(tokens, sizeof(char*) * bufsize);
            if(!tokens)
            {
                printf("Allocation Fail.\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, delimeters);
    }
    tokens[position] = NULL;
    return tokens;
}

char *read_line()
{
    int bufsize = READBUF_SIZE;
    int line_position = 0;
    int c;
    char *buffer = malloc(sizeof(char)*bufsize);
    if(!buffer)
    {
        printf("Allocation Fail.\n");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        c = getchar();
        if(c == '\n')
        {
            buffer[line_position] = '\0';
            return buffer;
        }
        else if(c == EOF)
        {
            printf("\n");
            exit(0);
        }
        else
        {
            buffer[line_position] = c;
        }
        line_position++;
        if(line_position >= bufsize)
        {
            bufsize += READBUF_SIZE;
            buffer = realloc(buffer, bufsize);
            if(!buffer)
            {
                printf("Allocation Fail.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

struct tuple
{
    int length;
    char** tuple;
};


int main()
{
    char* line;
    char** args;
    do
    {
        printf("> ");
        line = read_line();
        args = parse_line(line, DELI);
        int i = 0;
        while(args[i])
        {
            printf("Tokens: %s\n", args[i]);
            i++;
        }
    } while (strcmp(line, "exit") != 0);
    
    return 0;
}