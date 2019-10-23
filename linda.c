#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#define READBUF_SIZE 1024
#define TOKENBUF_SIZE 256
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
    char* cmd_type;
    char** tuple;
};

struct variable
{
    char* var;
    char* value;
};

// int check_var(char* token)
// {
//     int i = 0;
//     while(token[i]){i++;}
//     if(i >= 3)
//     {
//         if(token[0]!="\"")
//         {
//             return 1;
//         }
//         return 0;
//     }
//     else{return 0;}
// }


int main()
{
    char* line;
    char** args;

    struct tuple tuple_space[2000];
    int tuple_pos = 0;

    int thread_num;
    printf("Input Number of Thread:");
    line = read_line();
    thread_num = atoi(line);
    printf("Thread num: %d\n", thread_num);
    omp_set_dynamic(0);
    omp_set_num_threads(thread_num);

    //--Initialize thread tuple array------------------
    struct tuple thread_tuple_array[thread_num];//Record every thread's cmd
    for(int i = 0; i < thread_num; i++)
    {
        thread_tuple_array[i].length = -1;
        thread_tuple_array[i].cmd_type = NULL;
        thread_tuple_array[i].tuple = NULL;
    }
    //-------------------------------------------------

    #pragma omp parallel
    {
        do
        {
            int tid = omp_get_thread_num();
            printf("Thread number: %d\n", tid);

            if(tid == 0)//Server thread
            {
                //--Parse line--------------------------------
                printf("> ");
                line = read_line();
                args = parse_line(line, DELI);
                //--------------------------------------------

                //--Check cmd length--------------------------
                int cmd_length = 0;
                while(args[cmd_length]){cmd_length++;}
                //--------------------------------------------

                //--Record tid&cmd----------------------------
                int thread_id = atoi(args[0]);
                char* thread_cmd = args[1];
                printf("Tuple thread id: %d\n", thread_id);
                printf("Tuple cmd: %s\n", thread_cmd);
                //--------------------------------------------

                //--Record cmd tuple--------------------------
                int tuple_length = cmd_length-2;
                char* cmd_tuple[tuple_length];//Get rid of tid and cmd
                for(int i = 2; i < cmd_length; i++)
                {
                    cmd_tuple[i-2] = args[i];
                    printf("Tuple parameter: %s\n", cmd_tuple[i-2]);
                }
                //--------------------------------------------

                //--Decide action-----------------------------
                if(thread_cmd == "out")//Server thread change tuple space directly
                {
                    tuple_space[tuple_pos].length = tuple_length;
                    tuple_space[tuple_pos].cmd_type = thread_cmd;
                    tuple_space[tuple_pos].tuple = cmd_tuple;
                    tuple_pos ++;
                    tuple_pos /= 2000;
                }
                else if(thread_cmd == "in")//Server thread store cmd to thread tuple array
                {
                    if(thread_tuple_array[thread_id].length == -1)
                    {
                        thread_tuple_array[thread_id].length = tuple_length;
                        thread_tuple_array[thread_id].cmd_type = "in";
                        thread_tuple_array[thread_id].tuple = cmd_tuple;
                    }
                }
                else if(thread_cmd == "read")
                {
                    thread_tuple_array[thread_id].length = tuple_length;
                    thread_tuple_array[thread_id].cmd_type = "read";
                    thread_tuple_array[thread_id].tuple = cmd_tuple;
                }
                else
                {
                    printf("Unknown command.\n");
                }
                
                //--------------------------------------------
            }
            else
            {
                
            }
            
        } while (strcmp(line, "exit") != 0);
    }
    return 0;
}