#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

struct Task {
    int id;
    int p, w, d;
    // p - czas produkcji
    // w - waga
    // d - deadline
};

void readFile(string fileName, int size, Task* tasks){
   ifstream f(fileName);

    if (!f.is_open()) {
        cerr << "Error opening the file!" << endl;
        return;
    }
    
    string line;
    getline(f, line); 
    
    int p, w, d;
    for (int i = 0; i < size; i++) {
        if (f >> p >> w >> d) {
            tasks[i].id = i + 1;
            tasks[i].p = p;
            tasks[i].w = w;
            tasks[i].d = d;
        }
    }
    f.close();
};

void printAllTasks(Task* tasks, int size){
  for (int i = 0; i < size; ++i) {
    cout << "Task " << i + 1 << ": "
          << "id = " << tasks[i].id << ", "
          << "p = " << tasks[i].p << ", "
          << "w = " << tasks[i].w << ", "
          << "d = " << tasks[i].d << endl;
  }
}

int calcKara(Task* tasks, int size, vector<int>& bestOrder){
  if(size == 0) return 0;

  int currentTime = 0;
  int kara = 0;
  vector<Task> order; 
  int F = INT_MAX; // minimalna kara

  // całkowity czas wykonania wszystkich zadań
  for(int i = 0; i < size; i++){
    currentTime += tasks[i].p;
  }

  for(int i=0; i<size;i++){
    
    // kara dla ostatniego zadania
    kara = max(0, currentTime - tasks[i].d) * tasks[i].w;

    // tworzymy tablicę bez i-tego zadania
    Task* newTasks = new Task[size - 1];   
    int index = 0;
    for(int j=0; j<size; j++){
      if(j != i){
        newTasks[index++] = tasks[j];
      }
    }

    vector<int> subOrder; // kolejność zadań dla podproblemu
    // --- rekurencyjne wywolanie metody calckara dla pozostalych zadan ---
    int minKara = calcKara(newTasks, size - 1, subOrder) + kara;

    if(minKara < F){
      F = minKara;
      bestOrder = subOrder;
      bestOrder.push_back(tasks[i].id);
    }

    delete[] newTasks;
  }

  return F;
};

int main() {
  ifstream infile("data.10.txt");
  int rozmiar = 0;

  if (infile.good())
  {
    string sLine;
    getline(infile, sLine);
    rozmiar = stoi(sLine);
  }
  infile.close();

  Task tasks[rozmiar]; 
  readFile("data.10.txt", rozmiar, tasks);

  vector<int> order;
  int f = calcKara(tasks, rozmiar, order);

  cout << "(kolejnosc) ";
  for (int id : order) cout << id << " ";
  cout << endl;
  cout << "f = " << f << endl;

  return 0;

}