#include "util.h"
#include <iostream>
using namespace std;
int clampInt(int val, int minVal, int maxVal) {
    if (val < minVal) 
    return minVal;
    if (val > maxVal) 
    return maxVal;
    
    return val;
}
bool stringEquals(const char*a,const char*b){
    int i=0;
    while(a[i]!='\0' && b[i]!='\0'){
        if(a[i]!=b[i]) return false;
        i++;
    }
    return a[i]==b[i];
}
void trimWhitespace(char* str)
{
int start = 0;
int end = 0;
while(str[start] == ' ' || str[start] == '\t' ||
      str[start] == '\n' || str[start] == '\r')
{
    start++;
}
while(str[end] != '\0')
{
    end++;
}
end--;
while(end>=start&&
      (str[end] == ' ' || str[end] == '\t' ||
       str[end] == '\n' || str[end] == '\r'))
{
    end--;
}
int i = 0;
while(start<=end)
{
    str[i]=str[start];
    i++;
    start++;
}
str[i] = '\0';
}
void printHeader(const char* title)
{
cout << "================================" << endl;
cout << "          " << title << endl;
cout << "================================" << endl;
}
void clearScreen()
{
cout << "\033[2J\033[H";
}
    

// Primitive helper utilities, clamping, and screen control
