#include <iostream>
using namespace std;

#define TOTAL_STATES            6
#define FINAL_STATES            1
#define ALPHABET_CHARCTERS      2
#define BRANCH_MAX_NUMBER       2
#define START_BRANCH_NUMBER     1

#define EMPTY_SET              -2
#define UNKNOWN_SYMBOL_ERR      0
#define NOT_REACHED_FINAL_STATE 1
#define REACHED_FINAL_STATE     2

/* 
Вариант 3
Построить НКА, допускающий цепочки в алфавите Z = {0, 1}, в которых
содержится два нуля, разделенных позициями в количестве, кратном 4.
Примечание: нуль тоже кратен 4.
*/

enum NFA_STATES {q0, q1, q2, q3, q4, q5};   // The set Q
enum input {_0, _1};
enum branches {FIRST, SECOND};

int  g_Accepted_state = q5;     // The set F
char g_alphabet[ALPHABET_CHARCTERS] = {'0', '1'};  // The set Sigma
int  branches_number = START_BRANCH_NUMBER;
int  g_Transition_Table[TOTAL_STATES][ALPHABET_CHARCTERS][BRANCH_MAX_NUMBER] = {}; // Transition function
int* g_Current_state = new int[START_BRANCH_NUMBER];  // Start state of NFA

void SetNFA_Transitions()
{
  g_Transition_Table[q0][_0][FIRST] = q0;
  g_Transition_Table[q0][_0][SECOND] = q1;
  g_Transition_Table[q0][_1][FIRST] = q0;
  g_Transition_Table[q0][_1][SECOND] = EMPTY_SET;

  g_Transition_Table[q1][_0][FIRST] = q2;
  g_Transition_Table[q1][_0][SECOND] = q5;
  g_Transition_Table[q1][_1][FIRST] = q2;
  g_Transition_Table[q1][_1][SECOND] = EMPTY_SET;

  g_Transition_Table[q2][_0][FIRST] = q3;
  g_Transition_Table[q2][_0][SECOND] = EMPTY_SET;
  g_Transition_Table[q2][_1][FIRST] = q3;
  g_Transition_Table[q2][_1][SECOND] = EMPTY_SET;

  g_Transition_Table[q3][_0][FIRST] = q4;
  g_Transition_Table[q3][_0][SECOND] = EMPTY_SET;
  g_Transition_Table[q3][_1][FIRST] = q4;
  g_Transition_Table[q3][_1][SECOND] = EMPTY_SET;

  g_Transition_Table[q4][_0][FIRST] = q1;
  g_Transition_Table[q4][_0][SECOND] = EMPTY_SET;
  g_Transition_Table[q4][_1][FIRST] = q1;
  g_Transition_Table[q4][_1][SECOND] = EMPTY_SET;

  g_Transition_Table[q5][_0][FIRST] = q5;
  g_Transition_Table[q5][_0][SECOND] = EMPTY_SET;
  g_Transition_Table[q5][_1][FIRST] = q5;
  g_Transition_Table[q5][_1][SECOND] = EMPTY_SET;
}

int** NFA(const char current_symbol)
{
  int i, j, pos;
  for (pos = 0; pos < ALPHABET_CHARCTERS; ++pos)
  {
    if (current_symbol == g_alphabet[pos])
      break;     // stops if any character other than 0 or 1
  }
  if (ALPHABET_CHARCTERS == pos)
    return nullptr;  // Unknown symbol

  int** result = new int*[branches_number];
  for (i = 0; i < branches_number; ++i)
    result[i] = new int[BRANCH_MAX_NUMBER];
  for (i = 0; i < branches_number; ++i)
  {
    for (j = 0; j < BRANCH_MAX_NUMBER; ++j)
      result[i][j] = g_Transition_Table[g_Current_state[i]][pos][j];
  }
  return result;
}

int main(void)
{
  g_Current_state[FIRST] = q0;  // Initial State
  char current_symbol;
  int** result;             // For a getting result from NFA function
  int counter;              // For a counting current states after NFA function

  SetNFA_Transitions();     // Fill transition table

  cout << "Enter a string with '0' s and '1's:"
       << endl << "Press Enter Key to stop" << endl;

  bool check = true;        // Variable for exit from loop
  while ((current_symbol = getchar()) != '\n' && current_symbol != EOF)
  {
    result = NFA(current_symbol);
    if (result == nullptr)  // Transfering unknown symbol
    {
      check = false;
      break;
    }
    counter = 0;
    for (int i = 0; i < branches_number; ++i)  // Counting non-empty states in the result
    {
      for (int j = 0; j < BRANCH_MAX_NUMBER; ++j)
      {
        if (result[i][j] != EMPTY_SET)
          ++counter;
      }
    }
    if (!check)
      break;

    delete[] g_Current_state;
    g_Current_state = new int[counter];
    counter = 0;
    for (int i = 0; i < branches_number; ++i)  // Forming new g_Current_state
    {
      for (int j = 0; j < BRANCH_MAX_NUMBER; ++j)
      {
        if (result[i][j] != EMPTY_SET)
        {
          g_Current_state[counter] = result[i][j];
          ++counter;
        }
      }
    }
    for (int i = 0; i < branches_number; ++i)
      delete[] result[i];
    delete[] result;
    branches_number = counter;
  }

  cout << "Final states: { ";  // Printing final states
  for (int i = 0; i < branches_number; ++i)
    if (i + 1 == branches_number)
      cout << g_Current_state[i] << " ";
    else
      cout << g_Current_state[i] << ", ";
  cout << "}" << endl;

  bool accepted = false;  // Checking existence of accepted state
  for (int i = 0; i < branches_number; ++i)
  {
    if ((g_Current_state[i] == g_Accepted_state) && check)
    {
      cout << "Accepted";
      accepted = true;
      break;
    }
  }
  if (!accepted)
    cout << "Rejected";
  delete[] g_Current_state;

  return 0;
}
