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
        cout<<"\n1) Deal";
        cout<<"\n2) Rules";
        cout<<"\n3) View Stats";
        cout<<"\n4) Test Math";
        cout<<"\n7) Quit";
        cout<<"\nPick: ";  
        cin>>opt;
        cin.ignore(10000,'\n');

        if(opt==1){
            cout<<"Deal selected\n";
        }
        else if(opt==2){
            cout<<"Rules selected\n";
        }
        else if(opt == 3){
            cout << "\n--- Player Stats ---\n";
            cout << "Name: " << nam << "\n";
            cout << "Bank: $" << fixed << setprecision(2) << bank << "\n";

            int cnt = 0;
            for(int i = 0; i < 5; i++){
                cnt++;
               }
               cout << "Rounds played: " << cnt << "\n";
         }

         else if(opt == 4){
             cout << "\n--- Math Test ---\n";

             float test = bank;
             for(int i = 0; i < 5; i++){
                 test = test * 1.05f;
                 cout << "After step " << i+1 << ": $" 
                      << fixed << setprecision(2) << test << "\n";
             }

               if(test > bank){
                   cout << "Growth confirmed.\n";
               } else {
                   cout << "No growth.\n";
               }
           }

        else if(opt==7){
            run=false;
        }
    }

    cout<<"Bye\n";
    return 0;
}

void mkDk(int val[],int sut[],int siz){e
    int k=0;
    for(int s=0;s<4;s++){
        for(int v=1;v<=13;v++){
            val[k]=v;
            sut[k]=s;
            k++;
        }
    }
}

int ptsH(int hand[][12],int idx,int cnt){
    int sum=0;
    for(int i=0;i<cnt;i++){
        sum+=hand[idx][i];
    }
    return sum;
}

char gtAct(){
    cout<<"(H)it or (S)tand: ";
    string ln;
    getline(cin,ln);
    return (ln.size()>0)?toupper(ln[0]):'S';
}

int plyr(int hand[][12],int &pcnt){
    int pt=ptsH(hand,0,pcnt);
    char act='H';
    while(act!='S' && pt<21){
        act=gtAct();
        if(act=='H'){
            hand[0][pcnt]=(rand()%13)+1;
            pcnt++;
            pt=ptsH(hand,0,pcnt);
        }
    }
    return pt;
}

int dler(int hand[][12],int &dcnt){
    int dt=ptsH(hand,1,dcnt);
    while(dt<17){
        hand[1][dcnt]=(rand()%13)+1;
        dcnt++;
        dt=ptsH(hand,1,dcnt);
    }
    return dt;
}

float outc(int pt,int dt,float bet,bool nat){
    float winm=0.0f;
    if(pt>21) winm=-bet;
    else if(dt>21 || pt>dt) winm=bet;
    else if(pt<dt) winm=-bet;
    if(nat && winm>0) winm*=1.5f;
    return winm;
}

int linSr(string nm[],int siz,string key){
    for(int i=0;i<siz;i++){
        if(nm[i]==key) return i;
    }
    return -1;
}

int binSr(string nm[],float bnk[],int win[],int los[],
          int siz,string key){
    int lo=0, hi=siz-1;
    while(lo<=hi){
        int mid=(lo+hi)/2;
        if(nm[mid]==key) return mid;
        else if(nm[mid]<key) lo=mid+1;
        else hi=mid-1;
    }
    return -1;
}

void selSt(string nm[],float bnk[],int win[],int los[],int siz){
    for(int i=0;i<siz-1;i++){
        int mx=i;
        for(int j=i+1;j<siz;j++){
            if(bnk[j]>bnk[mx]) mx=j;
        }
        if(mx!=i) swp2(nm,bnk,win,los,i,mx);
    }
}

void bubSt(float bnk[],int gid[],int siz){
    bool swp=true;
    while(swp){
        swp=false;
        for(int i=0;i<siz-1;i++){
            if(gid[i]>gid[i+1]){
                swpF(bnk[i],bnk[i+1]);
                swpI(gid[i],gid[i+1]);
                swp=true;
            }
        }
    }
}

void swpS(string &a,string &b){ string t=a; a=b; b=t; }
void swpF(float &a,float &b){ float t=a; a=b; b=t; }
void swpI(int &a,int &b){ int t=a; a=b; b=t; }

void swp2(string nm[],float bnk[],int win[],int los[],int i,int j){
    swpS(nm[i],nm[j]);
    swpF(bnk[i],bnk[j]);
    swpI(win[i],win[j]);
    swpI(los[i],los[j]);
}





