#include<iostream>
#include<unistd.h>
using namespace std;

int main(){

    int x = 0;
    while(x != 10){
        x++;
        cout<<"Message from prog: "<<getpid()<<endl;
        int a = 0;
        sleep(2);
    }

}
