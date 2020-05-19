#include "utils.h"
using namespace std;
struct trie{
    bool isEndofWord;
    struct trie *next[26];
};

typedef struct trie Trie;

Trie *getNode(){
    
    Trie *temp = new Trie;
    temp->isEndofWord = false;
    
    for(int i=0;i<26;i++){
        temp->next[i] = NULL;
    }

    return temp;
}

Trie *root = getNode();

bool isLastNode(Trie *root){

    for(int i=0;i<26;i++){
        if(root->next[i] != NULL)
            return false;
    }
    return true;
}

void insert_trie(string key, Trie *root){

    Trie *crawl = root;
    for(int i=0;i<key.length();i++){
        int index = key.at(i)-97;
        if(crawl->next[index] == NULL){
            crawl->next[index] = getNode();    
        }
        crawl = crawl->next[index];
    }
    crawl->isEndofWord=true;
}

void printRecursive(Trie *temp, vector<string> &vec, string prefix){

    // cout<<prefix<<endl;

    if(temp->isEndofWord){
        vec.push_back(prefix);
    }

    if(isLastNode(temp)){
        return;
    }

    for(int i=0;i<26;i++){
        if(temp->next[i]!=NULL){
            printRecursive(temp->next[i], vec, prefix+char(i+97));
        }
    }   
}


vector<string> suggestions(string query){
    Trie *temp = root;

    int len = query.length();
    vector<string> vec;

    for(int i=0;i<len;i++){
        int index = query.at(i) - 97;
        if(temp->next[index]==NULL){
            return vec;
        }
        temp=temp->next[index];
    }

    bool lastword;
    if(temp->isEndofWord == true){
        lastword=true;
        vec.push_back(query);
    }
    else
        lastword=false;

    bool last = isLastNode(temp);

    if(last==true && lastword==true){
        vec.push_back(query);
        return vec;
    }

    if(isLastNode(temp) == false){
        printRecursive(temp, vec, query);
        return vec;
    }
}   

bool isContainsOtherThanCharacter(char *str){
    for(int i=0;str[i]!='\0';i++){
        if( isalpha(str[i]))
            continue;
        else
            return true;
    }
}


void init_trie(){

    DIR *dir;
    struct dirent *ent;

    // bin directory;
    if((dir=opendir("/bin")) != NULL){
        while((ent = readdir(dir)) != NULL){
            if(isContainsOtherThanCharacter(ent->d_name))
                continue;
            insert_trie(ent->d_name, root);
        }
        closedir(dir);
    }
    else{
        exit(-1);
    }

    // usr/bin
    if((dir=opendir("/usr/bin")) != NULL){
        while((ent=readdir(dir)) != NULL){
            if(isContainsOtherThanCharacter(ent->d_name))
                continue;
            insert_trie(ent->d_name, root);
        }
        closedir(dir);
    }
    else
    {
        exit(-1);
    }

    // user/local/bin
    if((dir=opendir("/usr/local/bin")) != NULL){
        while((ent=readdir(dir)) != NULL){
            if(isContainsOtherThanCharacter(ent->d_name))
                continue;
            insert_trie(ent->d_name, root);
        }
        closedir(dir);
    }
    else
    {
        exit(-1);
    }

    // sbin
    if((dir=opendir("/sbin")) != NULL){
        while((ent=readdir(dir)) != NULL){
            if(isContainsOtherThanCharacter(ent->d_name))
                continue;
            insert_trie(ent->d_name, root);
        }
        closedir(dir);
    }
    else
    {
        exit(-1);
    }
}
