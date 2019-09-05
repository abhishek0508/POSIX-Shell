
#include "utils.h"
#include "setmyrc.h"
#include "initshell.h"
#include "addhistory.h"

using namespace std;

struct instructions{
    string program;
    vector<string> parameters;
    bool bg;
};

typedef struct instructions instruct;
static int countbuf=0;
string read_input(){
    
    char c[100000];
    int i = 0;
    int add_history_flag = 0;
    int history_count = countbuf;
    while((c[i] = getchar())){
        if((int)c[i] == 10){
            add_history_flag = 1;
            countbuf++;
            break;
        }
        else if((int)c[i] == 27){
            history_count--;
            if(history_count<countbuf-1){
                string string1 = print_history(history_count+1);
                for(int i=1;i<=string1.length()+2;i++){
                    cout<<"\b";
                    cout<<" ";
                    cout<<"\b";
                }
            }
            if(history_count>=0)
            cout<<print_history(history_count);
            else
            history_count = countbuf;
        }
        else if(c[i] == 0x7f && i>0){
            cout<<"\b";
            cout<<" ";
            cout<<"\b";
            i--;
        }
        else{
            if((int)c[i]!=27){
                cout<<c[i];
                i++;
            }
        }
    }
    cout<<endl;
    c[i] = '\0';
    string input(c);

    if(add_history_flag == 1){
        adhistory(input);
    }
    return input;

    // char c[100000];
    // read(STDIN_FILENO, c , 100);
    // // while(read(STDIN_FILENO, &c[i], 1) == 1){
    // //     i++;
    // // }
    // //c[i] = '\0';
    // string input(c);
    // return input;
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
    
    for(int i=0;i<vec.size()-1;i++){
        instruct temp;
        temp = vec[i];

        //
        int pd[2];
        
        if(pipe(pd)<0){
            printf("error creating pipe");
            exit(EXIT_FAILURE);
        }

        if(!fork()){


            dup2(pd[1],1);
    
            vector<string> str;
            str = temp.parameters;

            char *argv[2048];

            int index = 0;
            vector<string>::iterator it;

            for(it = str.begin(); it!=str.end();it++){
                string s = *it;
                argv[index] = (char *)malloc((str.size()+1)*(sizeof(char)));
                strcpy(argv[index++], s.c_str());
            }
            if(strcmp(argv[0],"cd")==0)
            {
                chdir(argv[1]);
            }
            
            argv[index] = (char*)NULL;
            execvp(argv[0], (char* const*)argv);
            
            for(int i=0;i<index;i++){
                free(argv[i]);
            }   
            perror("exec");
            abort();
        }
        dup2(pd[0], 0);
        close(pd[1]);    
    }

    instruct temp;
    temp = vec[vec.size()-1];
    vector<string> str;
    str = temp.parameters;

    char *argv[2048];

    int index = 0;
    vector<string>::iterator it;

    for(it = str.begin(); it!=str.end();it++){
        string s = *it;
        argv[index] = (char *)malloc((str.size()+1)*(sizeof(char)));
        strcpy(argv[index++], s.c_str());
    }
    if(strcmp(argv[0],"cd")==0)
    {
        chdir(argv[1]);
    }  
    // prompt handle in case here    
    else if(strcmp("PS1",argv[0])==0){
        setShell("PS1", argv[1]);
    }
    else if(strcmp("echo", argv[0]) == 0)
    {
        cout<<get(argv[1])<<endl;
    }
    else{
        argv[index] = (char*)NULL;
        execvp(argv[0], (char* const*)argv);
        for(int i=0;i<index;i++){
            free(argv[i]);
        }    
        perror("exec");
        abort();
    }
}
int main(){

    // initialize shell
    init_shell();

    while(1){
    // display prompt
    cout<<get("PS1");

    //take input a string with getch
    string input = read_input();


    // exit from shell, on exit keyword
    string input2 = "exit";
    if(input.compare(input2) == 0)
    break;
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