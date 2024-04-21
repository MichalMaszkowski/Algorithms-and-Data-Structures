#include<iostream>
#include<vector>
#include<algorithm>
#include<map>

struct trojka {
    int p;
    int k;
    int i;
};


bool porownaj_poczatki(trojka x, trojka y) { //todo: moze cos z przedzialami?
    return (x.p < y.p);
}

bool porownaj_konce(trojka x, trojka y) { //todo: moze cos z przedzialami?
    return (x.k < y.k);
}

// bool porownaj_prezesa(trojka x, trojka y) { //todo: moze cos z przedzialami?
//     return ((x.i < y.i) || ((x.i == y.i) && (x.p < y.p)) || ((x.i == y.i) && (x.p == y.p) && (x.k < y.k)));
// }

int main() {
    //magiczne komendy:
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<trojka> poczatki(n);
    std::vector<trojka> konce(n);

    for(int i = 0; i < n; ++i) {
        trojka x;
        std::cin >> x.p >> x.k >> x.i;
        poczatki[i] = x;
        konce[i] = x;
    }


    std::sort(poczatki.begin(), poczatki.end(), porownaj_poczatki); //sortuje niemalejaco
    std::sort(konce.begin(), konce.end(), porownaj_konce); //sortuje niemalejaco
    // std::cout << '\n' << '\n';
    // for(int i = 0; i < n; ++i) {
    //     trojka x = poczatki[i];
    //     std::cout << x.p << ' ' << x.k << ' ' << x.i << '\n';
    // }


    std::map<int,int> mymap;
    int poczatek;
    int indeks_ost = 0;
    int koniec = (konce[indeks_ost]).k;

    int max = 0;
    int arg_max = 0;
    

    for(int i = 0; i < n; ++i) {
        trojka pocz = poczatki[i];
        poczatek = pocz.p;
        while(koniec <= poczatek) {
            //zwiekszaj koniec (indeks) i usuwaj
            int ktory = (konce[indeks_ost]).i;
            int ile = mymap[ktory] - 1;
            if(ile == 0) {
                mymap.erase(ktory);
            } else {
                mymap[ktory] = ile;
            }
            indeks_ost++;
            koniec = (konce[indeks_ost]).k;
        }
        mymap.emplace(pocz.i, 0);
        mymap[pocz.i] += 1;

        if(mymap.size() > max) {
            max = mymap.size();
            arg_max = poczatek;
        }
    }
    
    std::cout << arg_max << ' ' << max;


    // std::cout << '\n' << '\n';
    // for (std::map<int,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    // std::cout << it->first << " => " << it->second << '\n';

    return 0;
}
/*
6
1 10 1
2 5 2
6 10 2
4 7 3
5 6 3
5 6 4
*/