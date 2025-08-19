#include<iostream>
#include<fstream>
using namespace std;

bool isValidDateFormat(const string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '/' || date[7] != '/') return false;
    for (int i = 0; i < date.length(); ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }
 
    return true;
}
void input(){
    string date;
    cout<<" Enter Unit Price Date (YYYY/MM/DD) :";
    getline(cin,date);
    if(!isValidDateFormat(date)){
            cout<<"invalid date";
            return;
    }
}
void getdata(){
    ofstream files1("sales.csv");
   if(files1.is_open()){
     input();
   }
   else{
    ofstream createFile("sales.csv");
   }
    
}
int main(){
    getdata();
}