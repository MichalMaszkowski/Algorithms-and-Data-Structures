#include<iostream>
#include<vector>
#include<algorithm>
#include<map>

//najpierw posortowac po czasie
//potem kolejno przejsc po liscie i dla elementu x najpierw usunac wszystkie elementy o czasie mniejszym niz
//x.time - k, potem zliczyc kolizje tego przedzialu z obecnymi w drzewach, 
//a nastepnie dodac do drzew przedzialowych odpowiednie konce przedzialu

//kazda para zliczona dokladnie raz przez pozniej dodany przedzial z pary

//drzewa przedzialowe w tablicy bo jest max 200 000 zapytan wiec max 400 000 roznych wartosci koncow przedzialu
//zatem mozna je zmapowac na liczby z mniejszego zakresu zachowujac porzadek

//zlozonosc to chyba nlogn - sortowanie n elementow i n zapytan do drzewa przedzialowego

struct trojka {
    int a;
    int b;
    int t;
};

// czy x.t < y.t
bool porownaj_czas(trojka x, trojka y) { //todo: moze cos z przedzialami?
    return x.t < y.t;
}

void dodaj(int* drzewo_przedzialowe, int gdzie, int ile) { //na miejscu "gdzie" i na sciezce tam zwiekszenie o "ile"
    while (gdzie >= 1) {
        drzewo_przedzialowe[gdzie] = (drzewo_przedzialowe[gdzie] + ile);
        gdzie /= 2;
    }
    return;
}

int query(int* drzewo_przedzialowe, int odkad, int dokad, int podkorzen, int lewy, int prawy) { 
    //przedzial domknięty [$odkad$, $dokad$] probuje odczytac z poddrzewa o korzeniu w $podkorzen$
    //odpowiadajacym za przedzial [$lewy$, $prawy$]

    if((dokad < lewy) || (prawy < odkad)){ //przedzialu nie ma w poddrzewie
        return 0;
    }

    if((odkad <= lewy) && (prawy <= dokad)){ //przedzial za ktory odpowiada poddrzewo zawiera sie w szukanym
        return drzewo_przedzialowe[podkorzen];
    }

    int mid = (lewy + prawy) / 2;
    int wynik = query(drzewo_przedzialowe, odkad, dokad, (podkorzen * 2), lewy, mid);
    wynik += query(drzewo_przedzialowe, odkad, dokad, ((podkorzen * 2) + 1), (mid + 1), prawy);
    return wynik;
}

int main() {
    //magiczne komendy:
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int n, k;
    std::cin >> n  >> k;

    std::vector<trojka> dane(n);
    std::vector<int> koncowki(n*2);

    for(int i = 0; i < n; ++i) {
        trojka x;
        std::cin >> x.a >> x.b >> x.t;
        dane[i] = x;
        koncowki[2 * i] = x.a;
        koncowki[2 * i + 1] = x.b;
    }


    std::sort(dane.begin(), dane.end(), porownaj_czas); //sortuje niemalejaco po czasie
    std::sort(koncowki.begin(), koncowki.end());
    

    //licze ile jest roznych koncow przedzialow:
    int ile_roznych_koncowek = 1;
    for(int i = 1; i < 2 * n; ++i) {
        if(koncowki[i] != koncowki[i - 1]) {
            ++ile_roznych_koncowek;
        }
    }


    //tworze dwa drzewa przedzialowe - do przechowywania odpowiednio lewych i prawych koncow przedzialow
    int ile_lisci = 2;
    while(ile_lisci < ile_roznych_koncowek) {
        ile_lisci *= 2;
    }
    int rozmiar_drzewa = 2 * ile_lisci;
    int* lewe_konce = new int [rozmiar_drzewa];
    int* prawe_konce = new int [rozmiar_drzewa];
    for(int i = 0; i < rozmiar_drzewa; ++i) {
        lewe_konce[i] = 0;
        prawe_konce[i] = 0;
    }


    //przenumerowanie - mapuje liczby z zakresu [1, 1000 000 000] na liczby 
    //z zakresu [0, ile_roznych_koncowek - 1] zachowujac porzadek
    std::map<int, int> mapa;
    int indeks = 0;
    mapa[koncowki[0]] = indeks;

    for(int i = 1; i < 2 * n; ++i) {
        if(koncowki[i] != koncowki[i - 1]) {
            ++indeks;
            mapa[koncowki[i]] = indeks;
        }
    }



    //praca na drzewie:
    long long wynik = 0;
    int indeks_ostatniego_obecnego = 0;
    int ile_przedzialow_obecnych = 0;

    for(int i = 0; i < n; ++i) {
        int nowy_czas = dane[i].t;

        //usuwam przestarzale przedzialy:
        while(dane[indeks_ostatniego_obecnego].t < nowy_czas - k) {
            dodaj(lewe_konce, (ile_lisci + mapa[dane[indeks_ostatniego_obecnego].a]), -1);
            dodaj(prawe_konce, (ile_lisci + mapa[dane[indeks_ostatniego_obecnego].b]), -1);
            ++indeks_ostatniego_obecnego;
            --ile_przedzialow_obecnych;
        }


        //zliczam kolizje z nowym przedziałem tych już obecnych:
        int a = ile_lisci + mapa[dane[i].a];
        int b = ile_lisci + mapa[dane[i].b];

        //liczba kolizji to liczba wszystkich przedzialow obecnych pomniejszona o te przedzialy ktore sie nie przecinaja
        int ile_kolizji = ile_przedzialow_obecnych;
        //nie przecinaja sie z rozpatrywanym wszystkie ktore maja lewy koniec na prawo od jego prawego konca
        ile_kolizji -= query(lewe_konce, (b + 1), (rozmiar_drzewa - 1), 1, ile_lisci, (rozmiar_drzewa - 1));
        //nie przecinaja sie z rozpatrywanym wszystkie ktore maja prawy koniec na lewo od jego lewego konca
        ile_kolizji -= query(prawe_konce, ile_lisci, (a - 1), 1, ile_lisci, (rozmiar_drzewa - 1));

        //wynik zwiekszam o wykryte kolizje
        wynik += (long long) ile_kolizji;

        //dodaję nowy przedzial:
        ++ile_przedzialow_obecnych;
        dodaj(lewe_konce, a, 1);
        dodaj(prawe_konce, b, 1);
    }

    std::cout << wynik;

    delete[] lewe_konce;
    delete[] prawe_konce;
    return 0;
}