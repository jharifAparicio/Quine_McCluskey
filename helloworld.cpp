#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>

using namespace std;

// Main class
class QM {
public:
  int VARIABLES;
  string dontcares;

  QM(int a) {
    VARIABLES = a;
    dontcares.append(a, '-');
  }

  // función para obtener las letras del término booleano
  vector<string> getVars(){
    vector<string> v;
    string letters[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    for (int i = 0; i < this->VARIABLES; i++)
      v.push_back(letters[i]);
    return v;
  }

  /*función para convertir decimal a binario
   * Ej: 14 = 1110
   */
  string decToBin(int n)
  {
    if (n == 0)
      return n + "";

    if (n % 2 == 0)
      return decToBin(n / 2) + "0";
    else
      return decToBin(n / 2) + "1";
  }

  // función para rellenar ceros con el equivalente binario de dígitos.
  // Ej: Si hay 4 variables, 2, o sea 10 en binario se representa como 0010
  string pad(string bin)
  {
    int max = VARIABLES - bin.length();
    for (int i = 0; i < max; i++)
      bin = "0" + bin;
    return bin;
  }

  //función para comprobar si dos términos difieren solo en un bit
  bool isGreyCode(string a, string b)
  {
    int flag = 0;
    for (int i = 0; i < a.length(); i++)
    {
      if (a[i] != b[i])
        flag++;
    }
    return (flag == 1);
  }

  // función para reemplazar términos complementarios con no me importa
  // Por ejemplo: 0110 y 0111 se convierten en 011-
  string replace_complements(string a, string b)
  {
    string temp = "";
    for (int i = 0; i < a.length(); i++)
      if (a[i] != b[i])
        temp = temp + "-";
      else
        temp = temp + a[i];

    return temp;
  }

  // función para comprobar si la cadena b existe en el vector a
  bool in_vector(vector<string> a, string b)
  {
    for (int i = 0; i < a.size(); i++)
      if (a[i].compare(b) == 0)
        return true;
    return false;
  }

  // función para reducir minterms
  vector<string> reduce(vector<string> minterms)
  {
    /*para (int i=0; i<minterms.size(); i++)
           cout << minterms[i]<<endl;
           cout<<endl;*/

    vector<string> newminterms;

    int max = minterms.size();
    int *checked = new int[max];
    for (int i = 0; i < max; i++)
    {
      for (int j = i; j < max; j++)
      {
        // Si se encuentra un par de códigos grises, reemplace los bits diferentes con "no me importa".
        if (isGreyCode(minterms[i], minterms[j]))
        {
          checked[i] = 1;
          checked[j] = 1;
          if (!in_vector(newminterms, replace_complements(minterms[i], minterms[j])))
            newminterms.push_back(replace_complements(minterms[i], minterms[j]));
        }
      }
    }

    // añadiendo todos los términos reducidos a un nuevo vector
    for (int i = 0; i < max; i++)
    {
      // cout<<checked[i]<<endl;
      if (checked[i] != 1 && !in_vector(newminterms, minterms[i]))
        newminterms.push_back(minterms[i]);
    }

    delete[] checked;

    return newminterms;
  }

  // Convirtiendo el minterm booleano en variables
  // Por ejemplo: 011- se convierte en a'bc
  string getValue(string a)
  {
    string temp = "";
    vector<string> vars = this->getVars();
    if (a == dontcares)
      return "1";

    for (int i = 0; i < a.length(); i++)
    {
      if (a[i] != '-')
      {
        if (a[i] == '0')
          temp = temp + vars[i] + "\'";
        else
          temp = temp + vars[i];
      }
    }
    return temp;
  }

  // función para comprobar si 2 vectores son iguales
  bool VectorsEqual(vector<string> a, vector<string> b)
  {
    if (a.size() != b.size())
      return false;

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    for (int i = 0; i < a.size(); i++)
    {
      if (a[i] != b[i])
        return false;
    }
    return true;
  }
};

// Función principal
int main()
{
  char check = 'y';
  while (check == 'y')
  {
    int no;
    cout << endl
         << "Introduzca el número de variables:" << endl;
    cin >> no;
    if (no > 8 || no < 1)
    {
      cout << "Número de variables no válido (1-8)" << endl;
      continue;
    }
    QM q(no);

    string temp = "";
    cout << "Ingrese los términos mínimos (RANGO = 0-" << pow(2, no) - 1 << ") Separados por comas:" << endl;
    cin >> temp;

    // splitting the input
    vector<string> minterms;
    istringstream f(temp);
    string s;
    while (getline(f, s, ','))
    {
      // cout << s << endl;
      int t = atoi(s.data());
      minterms.push_back(q.pad(q.decToBin(t)));
    }

    sort(minterms.begin(), minterms.end());

    do
    {
      minterms = q.reduce(minterms);
      sort(minterms.begin(), minterms.end());
    } while (!q.VectorsEqual(minterms, q.reduce(minterms)));

    int i;
    cout << "La expresión booleana reducida en forma SOP:" << endl;
    for (i = 0; i < minterms.size() - 1; i++)
      cout << q.getValue(minterms[i]) << "+";
    cout << q.getValue(minterms[i]) << endl;

    cout << "<¿Le gustaría ingresar otra expresión? (Y/n)" << endl;
    cin >> check;
  }
  cout << endl
       << "---END--" << endl;
}