#include <iostream>
#include <cstring>
#include <string.h>
using namespace std;

#define TOTAL_STATES            5

#define INPUT_SYMBOLS           3
#define AUTOMATON_SYMBOL        4
#define INPUT_ALPHABET          2
#define BRANCH_MAX_NUMBER       2
#define START_MAGAZINE_NUMBER   1
#define START_STRING_SIZE       0

#define EMPTY_SET              -2
#define UNKNOWN_SYMBOL_ERR      0

enum PDA_STATES { q0, q1, q2, q3, q4 };   // The set Q
enum end_of_line { _a, _b, _l, _Z };  // Конечный магазинный алфавит
enum branches { FIRST, SECOND };

char g_alphabet[INPUT_ALPHABET] = { 'a', 'b' };  // The set Sigma
int g_Transition_Table[TOTAL_STATES][INPUT_SYMBOLS][AUTOMATON_SYMBOL][BRANCH_MAX_NUMBER] = { }; // Transition function

void SetNFA_Transitions()
{
  g_Transition_Table[q0][_a][_Z][FIRST] = q1;
  g_Transition_Table[q0][_a][_Z][SECOND] = EMPTY_SET;
  g_Transition_Table[q0][_b][_Z][FIRST] = EMPTY_SET;
  g_Transition_Table[q0][_b][_Z][SECOND] = EMPTY_SET;
  g_Transition_Table[q0][_l][_Z][FIRST] = q4;
  g_Transition_Table[q0][_l][_Z][SECOND] = EMPTY_SET;

  g_Transition_Table[q1][_a][_a][FIRST] = q1;
  g_Transition_Table[q1][_a][_a][SECOND] = q3;
  g_Transition_Table[q1][_b][_a][FIRST] = q2;
  g_Transition_Table[q1][_b][_a][SECOND] = EMPTY_SET;
  g_Transition_Table[q1][_l][_a][FIRST] = EMPTY_SET;
  g_Transition_Table[q1][_l][_a][SECOND] = EMPTY_SET;

  g_Transition_Table[q2][_a][_a][FIRST] = q3;
  g_Transition_Table[q2][_a][_a][SECOND] = EMPTY_SET;
  g_Transition_Table[q2][_b][_a][FIRST] = q2;
  g_Transition_Table[q2][_b][_a][SECOND] = EMPTY_SET;
  g_Transition_Table[q2][_l][_a][FIRST] = EMPTY_SET;
  g_Transition_Table[q2][_l][_a][SECOND] = EMPTY_SET;


  g_Transition_Table[q3][_a][_a][FIRST] = q3;
  g_Transition_Table[q3][_a][_a][SECOND] = EMPTY_SET;
  g_Transition_Table[q3][_b][_a][FIRST] = EMPTY_SET;
  g_Transition_Table[q3][_b][_a][SECOND] = EMPTY_SET;
  g_Transition_Table[q3][_b][_Z][FIRST] = EMPTY_SET;
  g_Transition_Table[q3][_b][_Z][SECOND] = EMPTY_SET;
  g_Transition_Table[q3][_a][_Z][FIRST] = EMPTY_SET;
  g_Transition_Table[q3][_a][_Z][SECOND] = EMPTY_SET;
  g_Transition_Table[q3][_l][_Z][FIRST] = q4;
  g_Transition_Table[q3][_l][_Z][SECOND] = EMPTY_SET;
  g_Transition_Table[q3][_l][_a][FIRST] = EMPTY_SET;
  g_Transition_Table[q3][_l][_a][SECOND] = EMPTY_SET;
}

char* read_string(int &string_size)
{
  char* string = new char[1];
  char current_symbol;
  bool check = true;
  int pos;
  string_size = 0;
  while ((current_symbol = getchar()) != '\n' && current_symbol != EOF)
  {
    string_size++;
    char* temp = new char[string_size];
    for(int i = 0; i < string_size - 1; ++i)
      temp[i] = string[i];
    for (pos = 0; pos < INPUT_ALPHABET; ++pos)
    {
      if (current_symbol == g_alphabet[pos])
        break;     // stops if any character other than 0 or 1
    }
    if (pos == INPUT_ALPHABET)
      return nullptr;
    temp[string_size - 1] = current_symbol;
    string = temp;
  }
  if (string_size == 0)
  {
	  string[0] = 'l';
  }
  return string;
}

int char_to_int(char symbol)
{
  if (symbol == 'b')
    return _b;
  else if (symbol == 'a')
    return _a;
  else
    return _l;
}

// Рекурсивная функция, которая будет выполнять функции переходов
bool PDA(int current_state, char* string, int* magazine, int string_size, int magazine_size)
{
  int result[2];
  for (int i = 0; i < 2; i++)
  {
    /*cout << "magazine{";
    for (int z = 0; z < magazine_size; z++)
      cout << magazine[z];*/
    result[i] = g_Transition_Table[current_state][char_to_int(string[0])][magazine[magazine_size - 1]][i];
    /*cout << "}, (q" << current_state << ", " << string[0] << ", " 
	       << magazine[magazine_size - 1] << ") -> q"<< result[i] << endl;*/

    if (result[i] != EMPTY_SET)
    {
      int new_current_state = result[i];

      int* new_magazine;
      int new_magazine_size = magazine_size;
      if (new_current_state == q1)
      {
        new_magazine_size++;
        new_magazine = new int[new_magazine_size]();
        for(int j = 0; j < new_magazine_size - 1; ++j)
          new_magazine[j] = magazine[j];
        new_magazine[new_magazine_size - 1] = char_to_int(string[0]);
      }
      else if (new_current_state == q3)
      {
        new_magazine_size--;
        new_magazine = new int[new_magazine_size]();
        for(int j = 0; j < new_magazine_size; ++j)
          new_magazine[j] = magazine[j];
      }
      else if (new_current_state == q4)
      {
        return true;
      }
      else
        new_magazine = magazine;

      int new_string_size = string_size;
      new_string_size--;
      char* new_string;
      if (new_string_size == 0)
      {
        new_string_size = 1;
        new_string = new char[new_string_size]{'l'};
      }
      else
      {
        new_string = new char[new_string_size]();
        for(int j = 1; j <= new_string_size; j++)
          new_string[j - 1] = string[j];
      }

      bool pda = PDA(new_current_state, new_string, new_magazine, new_string_size, new_magazine_size);
      if (pda)
        return true;
      else
        continue;
    }
  }
  return false;
}


int main(void)
{
  SetNFA_Transitions();     // Fill transition table

  cout << "Enter a string with 'a' s and 'b's:"
       << endl << "Press Enter Key to stop" << endl;

  int string_size = START_STRING_SIZE;
  char* string = read_string(string_size);  // Reading and checking string
  if (string == nullptr)
  {
    cout << "The string contains unknown symbols or string is null. Allow only {a, b}";
    return 0;
  }

  int* g_Magazine = new int[START_MAGAZINE_NUMBER]{_Z};  // Magazine and its start value Z
  int magazine_size = START_MAGAZINE_NUMBER;
  int g_Current_state = q0;  // Start state of PDA

  if(PDA(g_Current_state, string, g_Magazine, string_size, magazine_size))
    cout << "Success";
  else
    cout << "Fail";
  return 0;
}
