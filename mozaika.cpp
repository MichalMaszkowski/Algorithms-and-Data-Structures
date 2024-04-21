//Autor: Michał Maszkowski
//pierwsze podejscie zlozonosc O(n*k) i pamiec O(n+k)

#include <iostream>

#define MOD 1000000000

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);

    int n, k;
    std::cin >> n >> k;

    int tab_d[n];
    for(int i = 0; i < n; ++i) {
        std::cin >> tab_d[i];
    }


    int l_ciagow[k + 1][2]; //po d-tej iteracji: #l[i][0] = #ciagow o d-elementowych poprawnych zaczynajacych sie elementem i (i jest od 0 do k wlacznie; d jest od 1 do n wlacznie //l[..][0] jest smieciowe)
    for(int i = 0; i <= k; ++i) {//inicjalizacja: iteracja d = 1
        l_ciagow[i][0] = 1; //dla kazdego i jest 1 "poprawny" ciag o dlugosci 1 zaczynajacy sie na i
    }


    int wynik[n + 1];
    wynik[1] = k + 1;

    for(int d = 2; d <= n; ++d) { //nastepne iteracje - pod koniec spisuje wynik
        for(int i = 0; i <= k; ++i) {
            int wynik = l_ciagow[i][0];
            wynik = (wynik + (l_ciagow[i - 1][0] * (i > 0))) % MOD;
            wynik = (wynik + (l_ciagow[i + 1][0] * (i < k))) % MOD;
            l_ciagow[i][1] = wynik;
        }


        //policzylem caly poziom wyzszy wiec ten nizszy juz nie jest potrzebny - przeisuje tam rzeczy z wyzszego (w przygotowaniu do nastepnej iteracji):
        for(int i = 0; i <= k; ++i) {
            l_ciagow[i][0] = l_ciagow[i][1];
        }


        //podliczam sume z danego poziomu i spamietuje ten wynik:
        int suma = 0;
        for(int i = 0; i <= k; ++i) {
            //std::cout << l_ciagow[i][d] << " ";
            suma = (suma + l_ciagow[i][0]) % MOD;
        }
        wynik[d] = suma;
        //std::cout << "\n";
    }

    for(int i = 0; i < n; ++i) {
        std::cout << wynik[tab_d[i]] << " "; //wypisuje w odpowiedniej kolejnosci tzn odpowiada na danej pozycji na pytanie o odpowiadajace d z inputu
    }


    return 0;
}