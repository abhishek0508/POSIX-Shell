#include"utils.h"
using namespace std;

void setShell(string key, string value){
    fstream inFile;;
    inFile.open("myrc.txt");
    if(!inFile){
        cerr << "Unable to open file";
        exit(1);
    }
    string x;
    int flag = 0;
    string new_string;
    while(getline(inFile,x)){
        int pos = x.find("=");
        if(pos>=0){
            string sub1 = x.substr(0,pos);
            if(sub1.compare(key)==0){
                flag = 1;
                new_string += (sub1+"="+value);
            }
            else{
                new_string += x;
            }
            new_string += '\n';   
        }
    }
    if(flag==0){
        new_string += (key+"="+value+"\n");
    }
    inFile.close();

    fstream inFile1;

    inFile1.open("myrc.txt",ios::trunc| ios::out | ios::in);
    if(!inFile1){
        cerr << "Unable to open file";
        exit(1);
    }
    inFile1<<new_string;
    inFile1.close();
}
string get(string key){ 
    ifstream inFile;;
    inFile.open("myrc.txt");
    if(!inFile){
        cerr << "Unable to open file";
        exit(1);
    }
    string x;
    while(getline(inFile,x)){
        int pos = x.find("=");
        if(pos>=0){
            string sub1 = x.substr(0,pos);
            if(sub1.compare(key)==0){
                string sub = x.substr(pos+1);
                inFile.close();
                return sub;
            }
        }
    }
    inFile.close();
    return NULL;
}