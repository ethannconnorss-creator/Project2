/* 
 * File: main.cpp
 * Author: Ethan Connors
 * Created on December 11, 2025
 * Purpose: Project 2 - Blackjack game demonstrating functions, arrays,
 *          searching/sorting, formatted output, and file I/O.
 */

//System Libraries
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>
#include <cctype>

using namespace std;

//Function Prototypes
bool  rdFil(string fnm,float &bank,int &gid);
void  wrFil(string fnm,float bank,int gid);
void  prnMn(float bank);
int   gtInt(int minv,int maxv);
float gtBet(float bank,float minb=1.0f);
int   gId();

void  mkDk(int val[],int sut[],int siz);
void  shuf(int val[],int sut[],int siz);
void  deal(int val[],int sut[],int siz,int &top,int hand[][12],
           int idx,int &cnt);
int   ptsH(int hand[][12],int idx,int cnt);
void  prnH(int hand[][12],int idx,int cnt,bool hid=true);
char  gtAct();
int   plyr(int hand[][12],int &pcnt);
int   dler(int hand[][12],int &dcnt);
float outc(int pt,int dt,float bet,bool nat);

void  addLg(ofstream &lg,string nam,float bet,int pt,int dt,
            float win,float bank);

int   linSr(string nm[],int siz,string key);
int   binSr(string nm[],float bnk[],int win[],int los[],
            int siz,string key);
void  selSt(string nm[],float bnk[],int win[],int los[],int siz);
void  bubSt(float bnk[],int gid[],int siz);

void  swpS(string &a,string &b);
void  swpF(float &a,float &b);
void  swpI(int &a,int &b);
void  swp2(string nm[],float bnk[],int win[],int los[],int i,int j);

void  prnLb(string nm[],float bnk[],int win[],int los[],int siz);
int   fndPl(string nm[],int siz,string key);
void  clrIn();

int main() {
    const float MINB = 1.0f;

    srand(static_cast<unsigned int>(time(0)));

    float bank=0.0f;
    int opt=0, gid=0;
    bool run=true;
    string nam="Player";

    cout<<"Blackjack - Project 2\n";
    cout<<"Enter name: ";
    getline(cin,nam);

    while(run){
        cout<<"\n1) Deal\n2) Rules\n7) Quit\nPick: ";
        cin>>opt;
        cin.ignore(10000,'\n');

        if(opt==1){
            cout<<"Deal selected\n";
        }
        else if(opt==2){
            cout<<"Rules selected\n";
        }
        else if(opt==7){
            run=false;
        }
    }

    cout<<"Bye\n";
    return 0;
}

