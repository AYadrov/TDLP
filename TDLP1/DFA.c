#include <stdio.h> 

#define TOTAL_STATES            3
#define FINAL_STATES            2
#define ALPHABET_CHARCTERS      2

#define EMPTY_SET              -2 
#define REJECTED               -1
#define UNKNOWN_SYMBOL_ERR      0
#define NOT_REACHED_FINAL_STATE 1
#define REACHED_FINAL_STATE     2

/*
Вариант 3
Построить ДКА, допускающий в алфавите {0, 1} все цепочки
нулей и единиц, в которых символ 1 следует непосредственно за парой 00.
Пример 1: Цепочки 101, 0010, 0010011001 принадлежат языку. Пример 2:
цепочки 0001 и 00100 — не принадлежат.
*/

enum DFA_STATES {q0, q1, q2};   // The set Q
enum input {_0, _1};

int  g_Accepted_states[FINAL_STATES] = { q0, q1 };     // The set F
char g_alphabet[ALPHABET_CHARCTERS] = {'0', '1'};  // The set Sigma
int  g_Transition_Table[TOTAL_STATES][ALPHABET_CHARCTERS] = {}; // Transition function
int  g_Current_state = q0; // Start state of DFA

void SetDFA_Transitions()
{
  g_Transition_Table[q0][_0] = q1;
  g_Transition_Table[q0][_1] = q0;
  g_Transition_Table[q1][_0] = q2;
  g_Transition_Table[q1][_1] = q0;
  g_Transition_Table[q2][_0] = EMPTY_SET;
  g_Transition_Table[q2][_1] = q0;
}

int DFA(const char current_symbol)
{
  int i, pos;
  for (pos = 0; pos < ALPHABET_CHARCTERS; ++pos)
    if (current_symbol == g_alphabet[pos])   
      break;     // stops if any character other than 0 or 1
  if (ALPHABET_CHARCTERS == pos)
    return UNKNOWN_SYMBOL_ERR;

  g_Current_state = g_Transition_Table[g_Current_state][pos];
  for (i = 0; i < FINAL_STATES; ++i)
    if (g_Current_state == g_Accepted_states[i])
      return REACHED_FINAL_STATE;
  if (g_Current_state == EMPTY_SET)
    return REJECTED;
  else
    return NOT_REACHED_FINAL_STATE;
}

int main(void)
{
  char current_symbol;
  int  result;

  SetDFA_Transitions();    // Fill transition table

  printf("Enter a string with '0' s and '1's:\nPress Enter Key to stop\n");

  while ((current_symbol = getchar()) != '\n' && current_symbol != EOF)
  {
    result = DFA(current_symbol);
    if ((REJECTED == result) || (UNKNOWN_SYMBOL_ERR == result))
    {
      break;
    }
  }

  if (REACHED_FINAL_STATE == result)  
  {
    printf("Accepted");
  }
  else
  {
    printf("Rejected");
  }
  printf("\n\n");
  return 0;
}