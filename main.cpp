
#include"utils.h"

using namespace std;

struct instructions{
    string program;
    vector<string> parameters;
    bool bg;
};

typedef struct instructions instruct;

string read_input(){
    //  here getch will be used
    string input;
    getline(cin, input);
    return input;
}

vector<instruct> parse_input(string input){
    
    vector<instruct> vec;
    string T;
    stringstream X(input);

    while(getline(X, T, '|')){
        stringstream Y(T);
        string words;
        instruct temp;
        while(getline(Y, words, ' ')){
            temp.parameters.push_back(words);
        }
        vec.push_back(temp);
    }

    return vec;
}

void execute_instruction(vector<instruct> vec){
    
    instruct temp;
    int count = 0;
    for(int i=0;i<vec.size();i++){
        temp = vec[i];

        vector<string> str;
        str = temp.parameters;

        char *argv[2048];

        int index = 0;
        vector<string>::iterator it;

        for(it = str.begin(); it!=str.end();it++){
            string s = *it;
            argv[count] = (char *)malloc((str.size()+1)*(sizeof(char)));
            strcpy(argv[index++], s.c_str());
            count++;
        }

        argv[index] = (char*)NULL;
        execvp(argv[0], (char* const*)argv);
    }   
}
int main(){


    //init_shell();
    
    while(1){
    //take input a string with getch
    string input = read_input();

    //parse with pipes
    vector<instruct> vec;
    vec = parse_input(input);

    //print the vector<instruct>
    vector<instruct>::iterator it;
    // for(it = vec.begin(); it!=vec.end(); it++){

    //     vector<string> param;
    //     param = it->parameters;

    //     vector<string>::iterator it1;
        
    //     for(it1 = param.begin(); it1!=param.end(); it1++){
    //         cout<<*it1<<endl;;
    //     }
    // }

    // create child process & execute instruction

    int id = fork();
    int status = 0;
        if(id == 0){
            execute_instruction(vec);
            exit(-1);
        }
        else{
            wait(&status);
        }
    }
    return 0;
}