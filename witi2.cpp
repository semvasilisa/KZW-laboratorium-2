#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <algorithm>
using namespace std;

struct Task {
    int id;
    int p, w, d;
    // p - czas produkcji
    // w - waga
    // d - deadline
};

int n;
Task tasks[20];

// słownik par maska ​​- minimalna kara, np. memo[13] = 42
unordered_map<int, int> memo;
// pamięć podręczna: maska ​​- ostatnie identyfikatory zadań (w celu przywrócenia kolejności)
unordered_map<int, int> lastTask;

void readFile(string fileName, Task* tasks) {
    ifstream f(fileName);
    if (!f.is_open()) { cerr << "Error opening the file!" << endl; return; }
    
    string line;
    getline(f, line);
    
    int p, w, d;
    for (int i = 0; i < n; i++) {
        if (f >> p >> w >> d) {
            tasks[i].id = i + 1;
            tasks[i].p = p;
            tasks[i].w = w;
            tasks[i].d = d;
        }
    }
    f.close();
}

// całkowity czas wykonania zadań w masce
// maska - każdy bit reprezentuje czy dane zadanie jest w zbiorze (1) czy nie (0)

// metoda do obliczania całkowitego czasu wykonania zadań w danej masce
int totalTime(int mask) {
    int t = 0;
    for (int i = 0; i < n; i++)
        if (mask & (1 << i)) t += tasks[i].p;
    return t;
}

// metoda do obliczenia kary
int calcKara(int mask) {
    if (mask == 0) return 0; // brak zadań
    if (memo.count(mask)) return memo[mask]; // jeśli taki zestaw już był obliczony, odczytujemy z pamięci

    int time = totalTime(mask); // kiedy skończy się ostatnie zadanie 
    int best = INT_MAX;
    int bestLast = -1;

    // sprawdzamy w pętli każde zadanie, które jest w masce, jako potencjalnie ostatnie do wykonania
    for (int i = 0; i < n; i++) {
        if (!(mask & (1 << i))) continue; // zadanie nie znajduje się w bieżącym zbiorze, więc pomijamy

        int kara = max(0, time - tasks[i].d) * tasks[i].w;
        int result = calcKara(mask ^ (1 << i)) + kara; // obliczamy karę dla pozostałych zadań po usunięciu zadania i

        if (result < best) {
            best = result;
            bestLast = i;
        }
    }

    memo[mask] = best; // zapisujemy wynik w pamięci
    lastTask[mask] = bestLast;
    return best;
}

// przywracamy kolejność po tabeli lastTask
vector<int> getOrder() {
    vector<int> order;
    int mask = (1 << n) - 1;

    while (mask > 0) {
        int i = lastTask[mask];
        order.push_back(tasks[i].id);
        mask ^= (1 << i);
    }

    // kolejność postrzeczona od końca — odwracamy
    reverse(order.begin(), order.end());
    return order;
}

int main() {
    for (int fileNum = 10; fileNum <= 20; fileNum++) {

        string filename = "data." + to_string(fileNum) + ".txt";

        ifstream infile(filename);
        if (!infile.good()) {
            cout << "Error opening the file: " << filename << endl;
            continue;
        }

        string sLine;
        getline(infile, sLine);
        n = stoi(sLine);   
        infile.close();

        memo.clear();
        lastTask.clear();

        readFile(filename, tasks);

        int fullMask = (1 << n) - 1;
        int f = calcKara(fullMask);
        vector<int> order = getOrder();

        cout << "File: " << filename << endl;
        cout << "(order) ";
        for (int id : order) cout << id << " ";
        cout << endl;
        cout << "f = " << f << endl;
        cout << "------------------------" << endl;
    }

    return 0;
}