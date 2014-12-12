#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

class C_Table
{
  protected:
    int Dim;
    char **Table;
  public:
    C_Table(int);
    char* Read(bool hasValue);
    void Print();
    virtual int winner(char *) = 0;
    virtual void process(char *);
};

C_Table::C_Table(int dim)
{
  Dim = dim;
  Table = new char*[Dim];
  for (int i = 0; i < Dim; i++)
  {
      Table[i] = new char[Dim];
      for (int j = 0; j < Dim; j++)
        Table[i][j] = '_';
  }
}

char* C_Table::Read(bool readValue)
{
  char *ret = new char[3];
  cin >> ret;
  if (readValue)
    cin >> ret[2]; //made in case of diferent game (sudoku)
  return ret;
}

void C_Table::process(char * input)
{
  Table[input[0]-'A'][input[1]-'1'] = input[2]; //made in case of diferent game (sudoku)
}

void C_Table::Print()
{
  int i, j;
  cout << "\n  ";
  for (i = 1; i <= Dim; i++)
    cout << i << " ";
  cout << endl;
  for (i = 0; i < Dim; i++)
  {
    cout << char ('A' + i) << " ";
    for (j = 0; j < Dim; j++)
      cout << Table[i][j] << " ";
    cout << "\n";
  }
}

class C_X0 : public C_Table
{
    int Turn;
  public:
    C_X0(int);
    int winner(char *); 
    void process(char *);
};

C_X0::C_X0(int Dim):C_Table(Dim)
{
  Turn = 1;
}

void C_X0::process(char * input)
{
  Turn++;
  if (Table[input[0]-'A'][input[1]-'1'] == '_')
    Table[input[0]-'A'][input[1]-'1'] = (Turn % 2  == 0 ? 'X' : '0');
  else 
  {
       cout << "Cell already filled. Try again\n";
       Turn--;
  }
}

int C_X0::winner(char * input)
{
  // 0 not winner, 1 winner player 1, 2 winner player 2, 3 remiza
  int k, col = input[0]-'A', row = input[1] - '1';
  //column
  for (k = 0; k < Dim; k++)
    if (Table[k][row] != Table[col][row])
      break;  
  if (k == Dim)
  { 
    cout << "game won by player" << Turn %2 + 1;
    return Turn % 2 + 1;
  }
  //row
  for (k = 0; k < Dim; k++)
    if (Table[col][k] != Table[col][row])
      break;  
  if (k == Dim)
  { 
    cout << "game won by player" << Turn %2 + 1;
    return Turn % 2 + 1;
  }
    // check diagonals only if cell modified is on diagonal
  if ( col != row && col + row != Dim-1 )
  {
    if (Turn == 10)
    {
      cout << " null";
      return 3;
    }
    return 0;
  }
  // diagonal1
  for (k = 0; k < Dim; k++)
    if (Table[k][k] != Table[col][row])
      break;  
  if (k == Dim)
  { 
    cout << "game won by player" << Turn %2 + 1;
    return Turn %2 + 1;
  }
  //diagonal2
  for (k = 0; k < Dim; k++)
    if (Table[k][Dim-1-k] != Table[col][row])
      break;  
  if (k == Dim)
  { 
    cout << "game won by player" << Turn %2 + 1;
    return Turn %2 + 1;
  }
  if (Turn == 10)
  {
    cout << " null";
    return 3;
  }
  return 0;
}

class C_Rand : public C_Table
{
  int Turn, MinesNr, *Mine, *PlayerHits; // will have mines at mine[i] / Dim, mine[i] %Dim; 
  public:
    C_Rand(int dimension, int mines);
    int winner(char *); 
    void process(char *);
};

C_Rand::C_Rand(int n, int mines):C_Table(n)
{
  if (mines > Dim * Dim)
    cout << "too many mines for game field";
  int i, j;
  Turn = 1;
  MinesNr = mines;
  Mine = new int[MinesNr];
  PlayerHits = new int[2];
  srand( time(NULL) );
  i = 0;
  while ( i < MinesNr)
  {
    Mine[i] = rand() % (Dim * Dim);
    for ( j = 0; j < i; j++)
      if (Mine[i] == Mine[j])
      {
        i--;
        break;
      }
    i++; 
  }
  for (i = 0; i < 2; i++)
    PlayerHits[i] = 0;
}

void C_Rand::process(char * input)
{
  //for (int i = 0; i < MinesNr; i++) /// Comment these lines during realistic
   // cout << Mine[i] << " ";            /// game play
  if (Table[input[0]-'A'][input[1]-'1'] != '_')
  {
     cout << "cell already discovered. Try again";
     Turn--;
     return;
  }
  int i, location = (input[0]-'A') * Dim + (input[1]-'1');
  for ( i = 0; i < MinesNr; i++ )
    if ( location == Mine[i] )
      break;
  Table[input[0]-'A'][input[1]-'1'] = ( i == MinesNr ? 'X' : (char)(Turn +1) % 2+'1');
  PlayerHits[Turn % 2 ] += ( i == MinesNr ? 0 : 1);
}

int C_Rand::winner(char * input) 
{
  // 0 not winner, 1 winner, 2 winner
  if ( PlayerHits[ Turn % 2 ] == (MinesNr+1) / 2 )
  {
    cout << "game won by player " << (Turn +1) % 2 + 1;
    return (Turn +1) % 2 + 1;
  }
  Turn++;
  return 0;  
}

class C_Games
{
  int Score1, Score2, NrOfGames;
  C_Table **Games;
  void PlayGame(C_Table *Game);
  
  public:
    C_Games(int, int*);  
    void StartGames();
};

C_Games::C_Games(int nrOfGames, int *GameType)
{
  NrOfGames = nrOfGames;
  Score1 = 0;
  Score2 = 0;
  //initialize Games
  Games = new C_Table*[NrOfGames];
  for (int i = 0; i < NrOfGames; i++)
    if ( GameType[i] == 1) 
      Games[i] = new C_X0(3);
    else
      Games[i] = new C_Rand(4,3);
}

void C_Games::PlayGame(C_Table * Game)
{
  int win;
  char *input = new char[3];
  Game->Print();
  while (true)
  {
    input = Game->Read(false);
    Game->process(input);
    Game->Print();
    if (win = Game->winner(input))
       break;
  }
  switch (win)
  {
    case 1: 
      Score1++;
      break;
    case 2:
      Score2++;
      break;
    case 3:
      Score1++;
      Score2++;
      break;
  }
  cout << "\nScorul este " << Score1 << " - " << Score2 << ".\n";
}

void C_Games::StartGames()
{
  // start playing
  for ( int i = 0; i < NrOfGames; i++)
    PlayGame(Games[i]);
  cout << "BYE\n";    
}
int main()
{
  int gametypes[2] = {1,2};//1 -X si 0; 2 pt Mine
  C_Games x(sizeof(gametypes)/sizeof(int), gametypes); 
  
  x.StartGames();
  system("PAUSE");
  return 0;
}
