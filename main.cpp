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
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>
#include <cctype>

using namespace std;

//Prototypes (names <= 7 chars)
bool  rdFil(string fnm,float &bank);
void  wrFil(string fnm,float bank);
void  prnMn(float bank,string nam);
int   gtInt(int lo,int hi);
float gtBet(float bank,float minb=1.0f);
void  clrIn();

void  mkDk(int val[],int siz);
void  shuf(int val[],int siz);
int   draw(int val[],int siz,int &top);

int   ptsH(int hnd[][12],int idx,int cnt);
void  prnH(int hnd[][12],int idx,int cnt,bool hid);

int   plyr(int hnd[][12],int &pcnt,int val[],int siz,int &top);
int   dler(int hnd[][12],int &dcnt,int val[],int siz,int &top);
float outc(int pt,int dt,float bet,bool nat);

void  logRd(ofstream &f,string nam,float bet,int pt,int dt,float win,float bank);

//======================== MAIN ========================
int main(){
    srand((unsigned)time(0));

    const float MINB=1.0f;
    const int   DK=52;

    float bank=0.0f;
    string nam="Player";
    int opt=0;
    bool run=true;

    rdFil("bank.txt",bank);
    if(bank<MINB) bank=100.0f;

    cout<<"Blackjack - Project 2\n";
    cout<<"Enter name: ";
    getline(cin,nam);
    if(nam.size()==0) nam="Player";

    ofstream lg("log.txt",ios::app);

    while(run){
        prnMn(bank,nam);
        opt=gtInt(1,3);

        if(opt==1){
            if(bank<MINB){
                cout<<"Bank too low to bet.\n";
                continue;
            }

            float bet=gtBet(bank,MINB);

            //Deck (1D array)
            int val[DK];
            mkDk(val,DK);
            shuf(val,DK);
            int top=0;

            //Hands (2D array)
            int hnd[2][12];
            for(int r=0;r<2;r++){
                for(int c=0;c<12;c++){
                    hnd[r][c]=0;
                }
            }

            int pcnt=0, dcnt=0;

            //Deal 2 each
            hnd[0][pcnt++]=draw(val,DK,top);
            hnd[1][dcnt++]=draw(val,DK,top);
            hnd[0][pcnt++]=draw(val,DK,top);
            hnd[1][dcnt++]=draw(val,DK,top);

            int pt=ptsH(hnd,0,pcnt);
            int dt=ptsH(hnd,1,dcnt);

            bool nat=(pcnt==2 && pt==21);

            cout<<"\n"<<nam<<" ";
            prnH(hnd,0,pcnt,false);
            cout<<" tot="<<pt<<"\n";

            cout<<"Dealer ";
            prnH(hnd,1,dcnt,true);
            cout<<"\n";

            //Player turn
            pt=plyr(hnd,pcnt,val,DK,top);

            //Dealer turn only if player not bust
            if(pt<=21){
                cout<<"\nDealer ";
                prnH(hnd,1,dcnt,false);
                dt=ptsH(hnd,1,dcnt);
                cout<<" tot="<<dt<<"\n";

                dt=dler(hnd,dcnt,val,DK,top);
            }

            //Final show
            dt=ptsH(hnd,1,dcnt);

            cout<<"\nFINAL\n";
            cout<<nam<<" ";
            prnH(hnd,0,pcnt,false);
            pt=ptsH(hnd,0,pcnt);
            cout<<" tot="<<pt<<"\n";

            cout<<"Dealer ";
            prnH(hnd,1,dcnt,false);
            cout<<" tot="<<dt<<"\n";

            float win=outc(pt,dt,bet,nat);
            bank+=win;

            if(win>0) cout<<"You win $"<<fixed<<setprecision(2)<<win<<"\n";
            else if(win<0) cout<<"You lose $"<<fixed<<setprecision(2)<<-win<<"\n";
            else cout<<"Push.\n";

            cout<<"Bank now: $"<<fixed<<setprecision(2)<<bank<<"\n";

            logRd(lg,nam,bet,pt,dt,win,bank);
        }
        else if(opt==2){
            cout<<"\nRULES:\n";
            cout<<"- Try to reach 21 without going over.\n";
            cout<<"- Face cards count as 10.\n";
            cout<<"- Ace counts as 11 then drops to 1 if needed.\n";
            cout<<"- Dealer hits until 17+.\n\n";
        }
        else if(opt==3){
            run=false;
        }
    }

    wrFil("bank.txt",bank);
    lg.close();
    cout<<"Saved. Bye!\n";
    return 0;
}

//======================== I/O ========================
bool rdFil(string fnm,float &bank){
    ifstream fin(fnm.c_str());
    if(!fin){
        bank=100.0f;
        return false;
    }
    fin>>bank;
    if(!fin) bank=100.0f;
    fin.close();
    return true;
}

void wrFil(string fnm,float bank){
    ofstream fout(fnm.c_str());
    fout<<fixed<<setprecision(2)<<bank<<"\n";
    fout.close();
}

void prnMn(float bank,string nam){
    cout<<"\n=====================\n";
    cout<<"Player: "<<nam<<"\n";
    cout<<"Bank: $"<<fixed<<setprecision(2)<<bank<<"\n";
    cout<<"1) Deal\n";
    cout<<"2) Rules\n";
    cout<<"3) Save & Quit\n";
    cout<<"Pick: ";
}

int gtInt(int lo,int hi){
    int x;
    cin>>x;
    while(!cin || x<lo || x>hi){
        clrIn();
        cout<<"Enter "<<lo<<"-"<<hi<<": ";
        cin>>x;
    }
    clrIn();
    return x;
}

float gtBet(float bank,float minb){
    float b;
    cout<<"Bet (min "<<fixed<<setprecision(2)<<minb<<"): ";
    cin>>b;
    while(!cin || b<minb || b>bank){
        clrIn();
        cout<<"Bet must be "<<minb<<" to "<<bank<<": ";
        cin>>b;
    }
    clrIn();
    return b;
}

void clrIn(){
    cin.clear();
    cin.ignore(10000,'\n');
}

//======================== DECK ========================
void mkDk(int val[],int siz){
    int k=0;
    for(int s=0;s<4;s++){
        for(int v=1;v<=13;v++){
            int c=v;
            if(c==1) c=11;       //Ace
            else if(c>=10) c=10; //10,J,Q,K
            val[k]=c;
            k++;
        }
    }
}

void shuf(int val[],int siz){
    for(int i=siz-1;i>0;i--){
        int j=rand()%(i+1);
        int t=val[i];
        val[i]=val[j];
        val[j]=t;
    }
}

int draw(int val[],int siz,int &top){
    if(top>=siz) top=0;
    int c=val[top];
    top++;
    return c;
}

//======================== HANDS ========================
int ptsH(int hnd[][12],int idx,int cnt){
    int sum=0, ac=0;
    for(int i=0;i<cnt;i++){
        int v=hnd[idx][i];
        sum+=v;
        if(v==11) ac++;
    }
    while(sum>21 && ac>0){
        sum-=10;
        ac--;
    }
    return sum;
}

void prnH(int hnd[][12],int idx,int cnt,bool hid){
    cout<<"[";
    for(int i=0;i<cnt;i++){
        if(hid && idx==1 && i==1) cout<<"?";
        else cout<<hnd[idx][i];
        if(i<cnt-1) cout<<",";
    }
    cout<<"]";
}

//======================== TURNS ========================
int plyr(int hnd[][12],int &pcnt,int val[],int siz,int &top){
    int pt=ptsH(hnd,0,pcnt);

    while(pt<21){
        cout<<"(H)it or (S)tand: ";
        string ln;
        getline(cin,ln);
        char a=(ln.size()>0)?toupper(ln[0]):'S';

        if(a=='H'){
            if(pcnt<12){
                hnd[0][pcnt++]=draw(val,siz,top);
                pt=ptsH(hnd,0,pcnt);
                cout<<"You ";
                prnH(hnd,0,pcnt,false);
                cout<<" tot="<<pt<<"\n";
            }else{
                break;
            }
        }else{
            break;
        }
    }
    return pt;
}

int dler(int hnd[][12],int &dcnt,int val[],int siz,int &top){
    int dt=ptsH(hnd,1,dcnt);
    while(dt<17 && dcnt<12){
        hnd[1][dcnt++]=draw(val,siz,top);
        dt=ptsH(hnd,1,dcnt);
        cout<<"Dealer ";
        prnH(hnd,1,dcnt,false);
        cout<<" tot="<<dt<<"\n";
    }
    return dt;
}

//======================== RESULT ========================
float outc(int pt,int dt,float bet,bool nat){
    float win=0.0f;

    if(pt>21) win=-bet;
    else if(dt>21) win=bet;
    else if(pt>dt) win=bet;
    else if(pt<dt) win=-bet;
    else win=0.0f;

    if(nat && win>0.0f) win=win*1.5f;
    return win;
}

void logRd(ofstream &f,string nam,float bet,int pt,int dt,float win,float bank){
    f<<nam<<" bet="<<fixed<<setprecision(2)<<bet
     <<" pt="<<pt<<" dt="<<dt
     <<" win="<<fixed<<setprecision(2)<<win
     <<" bank="<<fixed<<setprecision(2)<<bank<<"\n";
}
