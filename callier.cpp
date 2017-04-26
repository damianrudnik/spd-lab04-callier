#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstring>
#include <new>
#include <vector>
#include <ctime>
#include <queue>

double t;// czas wykonywania algorytmu 
using namespace std;

struct Zadanie {
    int kolejnosc;//kolejnosc zadania
    int r;//czas przygotowania
    int p;//czas na maszynie
    int q;//czas stygniecia
};

struct PorownajZadaniaR{
    bool operator ()( const Zadanie & Zadanie1, const Zadanie & Zadanie2 ) {
        //kolejność - rosnąco
        if( Zadanie1.r > Zadanie2.r ) return true;
        if( Zadanie1.r < Zadanie2.r ) return false;
        if( Zadanie1.q > Zadanie2.q ) return false;
        if( Zadanie1.q < Zadanie2.q ) return true;
        return false;
    }
};

struct PorownajZadaniaQ{
    bool operator ()( const Zadanie & Zadanie1, const Zadanie & Zadanie2 ) {
        //kolejność - malejaco
        if( Zadanie1.q > Zadanie2.q ) return false;
        if( Zadanie1.q < Zadanie2.q ) return true;
        return false;
    }
};

typedef priority_queue < Zadanie, std::vector < Zadanie >, PorownajZadaniaR > KolejkaZadan;
typedef priority_queue < Zadanie, std::vector < Zadanie >, PorownajZadaniaQ > KolejkaZadanQ;

void WstawZadanie( KolejkaZadan & kp, int kolejnosc, int r, int p, int q){
    Zadanie NoweZadanie;
    NoweZadanie.kolejnosc = kolejnosc;
    NoweZadanie.r = r;
    NoweZadanie.p = p;
    NoweZadanie.q = q;
    kp.push( NoweZadanie );
}
void WstawZadanie( KolejkaZadanQ & kp, int kolejnosc, int r, int p, int q){
    Zadanie NoweZadanie;
    NoweZadanie.kolejnosc = kolejnosc;
    NoweZadanie.r = r;
    NoweZadanie.p = p;
    NoweZadanie.q = q;
    kp.push( NoweZadanie );
}

ostream& operator<< (ostream& wyjscie, const Zadanie& zad){
    cout << zad. kolejnosc << ": " << zad.r << " - " << zad.p << " - " << zad.q << endl; 
    return wyjscie;
}

void WyswietlKolejke(KolejkaZadan kp) {
    while(kp.size() > 0){
        Zadanie odczytane = kp.top();
        kp.pop();
        cout << odczytane.kolejnosc << " "; 
    }
    cout << '\n';
}
void WyswietlKolejke(KolejkaZadanQ kp) {
    while(kp.size() > 0){
        Zadanie odczytane = kp.top();
        kp.pop();
        cout << odczytane.kolejnosc << " "; 
    }
    cout << '\n';
}

unsigned int max(unsigned int x, unsigned int y){ return (x<=y)?y:x; }
unsigned int min(unsigned int x, unsigned int y){ return (x<=y)?x:y; }

/*istream& operator>> (istream& wejscie, Zadanie& zad){
    zad.kolejnosc = ++iIndexZadan;
    wejscie >> zad.r >> zad.p >> zad.q;
    return wejscie;
}*/
int cmax(KolejkaZadan & kp){
    unsigned int t=0,u=0;
    while (!kp.empty()){
        t=max(t,kp.top().r)+kp.top().p;
        u=max(u,t+kp.top().q);
        kp.pop();
    }
    return u;
}
int cmaxtab(Zadanie Tab[], unsigned int rozmiar){
    unsigned int i, t=0,u=0;
    for (i=0;i<rozmiar;i++){
        t=max(t,Tab[i].r)+Tab[i].p;
        u=max(u,t+Tab[i].q);
    }
    return u;
}

int fdr(KolejkaZadan kp){
    int t=0,cmax=0;
    KolejkaZadanQ kg;
    Zadanie e, l;
    while(!kp.empty() || !kg.empty()){
        while((!kp.empty()) && (kp.top().r <= t)){
            e = kp.top();
            kp.pop();
            kg.push(e);
            if (kg.top().q>l.q){
                l.p=t-e.r;
                t=e.r;
                if (l.p>0)
                    kg.push(l);
            }  
        }
        if (kg.empty()){
            t = kp.top().r;
        }else{
            e = kg.top();
            kg.pop();
            l=e;
            t=t+e.p;
            cmax=max(cmax,t+e.q);
        }
    }
    //cout << "\n CZAS OBLICZONY:" << cmax << endl;
    return cmax;
}

void intime(){ 
  t=clock();
}

void showtime(){ 
  t=clock()-t; 
  cout <<"\nWszystkie operacje zajely: " << ((double)t)/CLOCKS_PER_SEC << " s.\n\n";
}

string itos(int a) {
    string sign = a<0?"-":"";
    string result = a>0?string(1,(a%10+'0')):string(1,((a=-a)%10+'0'));
    (a/=10)>0?result=itos(a)+result:result;
    return sign+result;
}

int main(){
    unsigned int LiczbaZadan = 0;
    string filename;
    int tablica[9] = {32,641,1257,1386,3472,3617,6885,6904,72852};

    cout<<"Generuje odpowiedzi dla wszystkich plikow.\n###############################################\n\n";
    intime();

    for (int iNrPliku = 1; iNrPliku<=9;iNrPliku++){ // odczyt, szeregowanie i zapis wyniku do plikow

        KolejkaZadan Kolejka;
        Zadanie Zad;
        filename = itos(iNrPliku);
        ifstream myfile ("SCHRAGE"+filename+".DAT");
        cout << "Odczytuje plik SZCHRAGE"+filename+".DAT\n";

        if (myfile.is_open()){
            myfile>>LiczbaZadan;
            for (unsigned int i = 0; i < LiczbaZadan; i++){
                myfile>>Zad.r;
                myfile>>Zad.p;
                myfile>>Zad.q;
                Zad.kolejnosc = i+1;
                Kolejka.push(Zad);
            }
            myfile.close();
        }else cout << "Nie moge otworzyc pliku";
       
        // obsluga algorytmu, czyli petle zamieniajace kolejnosc dzialan
        cout<<"  Obliczanie kolejnosci... \n";
        int cmtab=fdr(Kolejka);
        cout << "Cmax = "<<cmtab<<endl;
        if (cmtab==tablica[iNrPliku-1]){
            cout << "OBLICZONA WARTOSC ZGODNA.\n";
        }else
            cout << "!!!\t OBLICZONA WARTOSC NIEZGODNA\t !!!\n";
        // wyswietlenie kolejnosci:
        //cout << endl << "Wyliczona kolejnosc: ";
        //WyswietlKolejke(Kolejka);

        // zapis do pliku
        string fname = "out";
        fname.append(filename);
        fname.append(".txt");
        FILE *fp = fopen(fname.c_str(),"w");
        if (fp){
            fprintf(fp,"%d\n", cmtab);
            fclose(fp);
            cout << "Plik zostal zapisany.\n";
        }else {
            cout << "Nie moge otworzyc pliku " << filename << endl;
            return -1;
        }
        cout<<"###############################################\n\n";
    }
    showtime();
    return 0;
}