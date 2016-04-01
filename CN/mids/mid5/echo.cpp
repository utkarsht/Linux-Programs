#include<iostream>
#include<unistd.h>
using namespace std;

int main(){

   char buff[] = "sushil paneru\n";
   int x = 0;
   while(1){
    cout<<"Echo Server..\n";
    x++;
    sleep(2);
    if(x == 10)
        break;
   }
}
