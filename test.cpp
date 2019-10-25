#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct variable
{
    string name;
    string value;
};

vector <variable> var_table;

vector <int> parse_variable(vector <string> thread_tuple)
{
    vector <int> var_pos;
    for(int i = 0; i < thread_tuple.size(); i++)
    {
        if(thread_tuple[i].substr(0, 1) != "\"")
        {
            double if_var;
            try
            {
                if_var = stod(thread_tuple[i]);
            }
            catch(exception& e)
            {
                if_var = -1;
                var_pos.push_back(i);
            }
        }
        else{continue;}
    }
    return var_pos;
}

vector <string> replace_var(vector <string> thread_tuple)
{
    vector <int> var_pos = parse_variable(thread_tuple);
    for(int i = 0; i < var_pos.size(); i++)
    {
        for(int j = 0; j < var_table.size(); j++)
        {
            if(thread_tuple[var_pos[i]] == var_table[j].name)
            {
                thread_tuple[var_pos[i]] = var_table[j].value;
            }
        }
    }
    return thread_tuple;
}

int main()
{
    struct variable var1;
    var1.name = "i";
    var1.value = "0";
    var_table.push_back(var1);
    vector <string> thread_tuple;
    string str1 = "1";
    string str2 = "i";
    string str3 = "3";
    thread_tuple.push_back(str1);
    thread_tuple.push_back(str2);
    thread_tuple.push_back(str3);

    for(int i = 0; i < thread_tuple.size(); i++)
    {
        printf("Idx: %d, Value: %s\n", i, thread_tuple[i].c_str());
    }
    thread_tuple = replace_var(thread_tuple);
    for(int i = 0; i < thread_tuple.size(); i++)
    {
        printf("Idx: %d, Value: %s\n", i, thread_tuple[i].c_str());
    }

    return 0;
}