#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <omp.h>

using namespace std;

struct stuple
{
    int length;
    vector <string> s_tuple;
    stuple()
    {
        length = -1;
    }
};

struct variable
{
    string name;
    string value;
};

struct thread_cmd
{
    string thread_no;
    string cmd;
    vector <string> cmd_tuple;
};

string tuple2string(struct stuple tuple)
{
    string tuple_string = "(";
    for(int j = 0; j < tuple.length; j++)
    {
        if(j == 0){tuple_string += tuple.s_tuple[j];}
        else{tuple_string += ", " + tuple.s_tuple[j];}
    }
    tuple_string += ")";
    return tuple_string;
}

void show_tuple_space(vector <stuple> tuple_space)
{
    printf("-----------------------------------\n");
    for(int i = 0; i < 2000; i++)
    {
        if(tuple_space[i].length != -1)
        {
            string show_tuple = tuple2string(tuple_space[i]);
            printf("%s\n", show_tuple.c_str());
        }
    }
    printf("-----------------------------------\n");
}



int main()
{
    string line;
    printf("> ");
    int thread_num;
    cin >> thread_num;
    printf("Thread num: %d\n", thread_num+1);
    omp_set_num_threads(thread_num+1);

    vector <stuple> tuple_space(2000);
    vector <thread_cmd> wait_cmd;
    vector <string> thread_rw_tuple(thread_num+1);

    while(true) {
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            if(tid == 0)
            {
                show_tuple_space(tuple_space);
                printf("> ");
                getline(cin, line);
                // stringstream class check1 
                stringstream check1(line); 
                string intermediate; 
                // Tokenizing w.r.t. space ' '
                string thread_no;
                check1 >>  thread_no;
                string cmd;
                check1 >> cmd;
                vector <string> thread_tuple;
                while(check1 >> intermediate) 
                { 
                    thread_tuple.push_back(intermediate); 
                }
                // Printing the token vector 
                for(int i = 0; i < thread_tuple.size(); i++) 
                    cout << thread_tuple[i] << '\n';
                
                // struct thread_cmd th_cmd;
                // th_cmd.thread_no = thread_no;
                // th_cmd.cmd_tuple = thread_tuple;
                // th_cmd.cmd = cmd;

                if(cmd == "out")
                {
                    //--Find an empty space to store tuple-------------------
                    for(int i = 0; i < 2000; i++)
                    {
                        if(tuple_space[i].length == -1)
                        {
                            tuple_space[i].length = thread_tuple.size();
                            tuple_space[i].s_tuple = thread_tuple;
                            break;
                        }
                    }
                    //-------------------------------------------------------
                }
                else if(cmd == "read" || cmd == "in")
                {
                    //--Check if there exist target tuple--------------------
                    for(int i = 0 ; i < 2000; i++)
                    {
                        if(thread_tuple.size() == tuple_space[i].length)
                        {
                            int j;
                            for(j = 0; j < thread_tuple.size(); j++)
                            {
                                if(thread_tuple[j] != tuple_space[i].s_tuple[j]){break;}
                            }
                            if(j == thread_tuple.size())
                            {
                                //--Convert thread no from str to int-------------
                                istringstream sno2no(thread_no);
                                int th_no = 0;
                                sno2no >> th_no;
                                //------------------------------------------------
                                
                                string th_tuple = tuple2string(tuple_space[i]);//Convert tuple to string
                                thread_rw_tuple[th_no] = th_tuple;
                                if(cmd == "in")
                                {
                                    tuple_space[i].length = -1;//Erace tuple space
                                }
                            }
                        }
                    }
                    //------------------------------------------------------
                }
                else
                {
                    printf("Unknown command.\n");
                }
            }
        }
    };
    return 0;
}