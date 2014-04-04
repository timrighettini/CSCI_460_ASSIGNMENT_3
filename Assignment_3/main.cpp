#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <string>

// Global consts
const int ROWS = 6;
const int COLS = 3;

struct Point
{
    int r;
    int c;

    Point(){};

    Point(int row, int col)
    {
        this->r = row;
        this->c = col;
    }
};

struct Node
{
    struct Point movedFrom;  // Point Moved From
    struct Point movedTo;    // Point Moved To
    std::vector<Node *> children; // List of Children
    char boardState[ROWS][COLS];   // Board State (rows x cols)
    bool isWhitePlayer;      // Is this player 1?
};

/* Three things I need to check right now:
1.  Check win condition -> Stops recursion - DONE
    A.  Black Team
    B.  White Team
2.  Check that movement expansions work for trees
    A.  Check all cases involving invalid moves
        1.  Forward Move  -> Space ahead must be empty
        2.  Diagonal Move -> Make sure move is in bounds
            a.  Capture piece if piece is there
                1.  Do not capture one of your own pieces
                2.  Do capture one of the enemy pieces
            b.  Otherwise, just move there
    B.  For each piece currently in play, get all possible moves and branch out
        1.  Loop through array
            a.  if (BoardState[r][c] = W &&  isWhitePlayer) -> Expand out this piece's moves
            b.  if (BoardState[r][c] = B && !isWhitePlayer) -> Expand out this piece's moves
        2.  Generate three new states for this piece if they are valid
            a.  After creating new nodes, go down the left branch in DFS and continue expanding
            b.  These new states will have isWhitePlayer = !isWhitePlayer
        3.  How to move a piece into a new board state if valid
            a.  Hold reference to array index, along with the player who owns the piece
            b.  Nuke old position, move piece to new position, save board state into a node
                (1).  Use from and to points to make this work (write to position to player letter, and write from position to empty)
3.  Methods Needed
    A.  Check Win (Node *)
        1.  If either player wins, return true
            a.  if (isWhitePlayer  && (any top square is W)) return +1
            b.  if (!isWhitePlayer && (any bot square is B)) return -1
            c.  if (numberBlackPieces == 0) return +1
            d.  if (numberWhitePieces == 0) return -1
    B.  CreateChildren (Node *current)
        1.  Create three, two, or one valid nodes based upon board states - there is always a valid move
            a.  Loop through the current board state array, find all areas that are 'W' for isWhitePlayer, or 'B' otherwise
                and when each player piece is found, do the following:
                (1)  Try to create three points (refer to 3.E and 3.F) from this position for possible piece moves
                (2)  If any point is NULL, do not create the related child node for this piece
        2.  Link all of these new nodes up as children to the parent (store them in the vector)
    C.  GoAlphaBeta(Node *child)
        1.  Go into the next stage of a-B pruning within the child (refer to Wikipedia)
    D.  CreateChildNode(Point from, Point to, char[6][3] boardState)
        1.  create a new Node* based upon the from and to points
        2.  modify the board state appropriately (refer to 2.B.3)
        3.  set isWhitePlayer to !isWhitePlayer
        4.  set newNode->movedFrom = from, newNode->movedTo = to
        5.  set new board state to newNode->boardState
        6.  return this newNode as a ptr
    E.  CreateForwardPoint(Point p, char[6][3] boardState, bool isWhitePlayer) - DONE
        1.  if (isWhitePlayer)
                if (boardState[p.r - 1][p.c] == 'X')
                    return Point(p.r - 1, p.c)
                else
                    return NULL; // NOT Valid
        2.  else
            if (boardState[p.r + 1][p.c] == 'X')
                    return Point(p.r + 1, p.c)
                else
                    return NULL; // NOT Valid
    F.  Create DiagonalPoint(Point p, char[6][3] boardState, bool isLeftMove, bool isWhitePlayer) - DONE
        1.  if (isWhitePlayer)
                if (isLeftMove)
                    if (p.c - 1 < 0)
                        return NULL
                    else
                        if (boardState[p.r - 1][p.c - 1] == 'W') // Cannot capture yourself
                            return NULL
                        else
                            return new Point(p.r - 1, p.c - 1)
            else
                    if (p.c + 1 > 2)
                        return NULL
                    else
                        if (boardState[p.r - 1][p.c + 1] == 'W') // Cannot capture yourself
                            return NULL
                        else
                            return new Point(p.r - 1, p.c + 1)
        2.  else
                if (isLeftMove)
                    if (p.c - 1 < 0)
                        return NULL
                    else
                        if (boardState[p.r + 1][p.c - 1] == 'B') // Cannot capture yourself
                            return NULL
                        else
                            return new Point(p.r + 1, p.c - 1)
                else
                    if (p.c + 1 > 2)
                        return NULL
                    else
                        if (boardState[p.r + 1][p.c + 1] == 'B') // Cannot capture yourself
                            return NULL
                        else
                            return new Point(p.r + 1, p.c + 1)
    G.  Read in board state - DONE
        1.  Take in initial node, and then set board state of initial node to stuff in txt file
        2.  If no text file, load in the default configuration
    H.  Print Board - DONE
    I.  Check for number of pieces - DONE
*/

/*

-----TEST PLAN-----
A.  Tree Creation Tests - Do this for WHITE AND BLACK Players
    1.  Test Forward movement edge case
    2.  Test Diagonal Movement edge cases
        A.  OB Left
        B.  OB Right
        C.  Do not capture your own piece
B.  Test one level of expansion from white player
C.  Test one level of expansion from black player
D.  Test a 1 v 1 pawn simulation
E.  Test a full 6 v 6 pawn simulation
F.  Do a-B pruning with 1 v 1
G.  Do a-B pruning with 6 v 6
*/

bool LoadFile(Node*); // Load in the initial board state
void PrintList(Node*); // Print the board state of the node in question
int CheckWinCondition(Node *);
int CheckNumberOfPieces(Node *, char); // Check # of pieces for win cond
void LookForPieces(Node *n); // Loop through the array and find all playable pieces for a player
void CreateChildren(Node *n, Point p);
Point CreateForwardPoint(Node *n, Point p);
Point CreateDiagonalPoint(Node *n, Point p, bool isLeftMove);
void CreateChild(Node *n, Point startPt, Point endPt);

int main()
{
    // Create the initial node:
    Node* startNode = new Node();
    startNode->isWhitePlayer = true;
    LoadFile(startNode);
    PrintList(startNode);

    // Some Tests

    std::cout << "All conditions should check out" << std::endl;

    // 1. Test all win conditions

    // All P1 Conditions
    LoadFile(startNode);
    startNode->boardState[0][0] = 'W';
    printf("%d\n", CheckWinCondition(startNode));

    LoadFile(startNode);
    startNode->boardState[0][1] = 'W';
    printf("%d\n", CheckWinCondition(startNode));

    LoadFile(startNode);
    startNode->boardState[0][2] = 'W';
    printf("%d\n", CheckWinCondition(startNode));

    // All P2 Conditions
    startNode->isWhitePlayer = !startNode->isWhitePlayer;
    LoadFile(startNode);
    startNode->boardState[5][0] = 'B';
    printf("%d\n", CheckWinCondition(startNode));

    LoadFile(startNode);
    startNode->boardState[5][1] = 'B';
    printf("%d\n", CheckWinCondition(startNode));

    LoadFile(startNode);
    startNode->boardState[5][2] = 'B';
    printf("%d\n", CheckWinCondition(startNode));

    // Check Capture condition wins

    // White Should Win
    LoadFile(startNode);
    startNode->boardState[0][0] = 'X';
    startNode->boardState[0][1] = 'X';
    startNode->boardState[0][2] = 'X';
    startNode->boardState[1][0] = 'X';
    startNode->boardState[1][1] = 'X';
    startNode->boardState[1][2] = 'X';
    printf("%d\n", CheckWinCondition(startNode));

    // Black Should Win
    LoadFile(startNode);
    startNode->boardState[4][0] = 'X';
    startNode->boardState[4][1] = 'X';
    startNode->boardState[4][2] = 'X';
    startNode->boardState[5][0] = 'X';
    startNode->boardState[5][1] = 'X';
    startNode->boardState[5][2] = 'X';
    printf("%d\n", CheckWinCondition(startNode));

    // There should be NO wins here
    LoadFile(startNode);
    printf("%d\n", CheckWinCondition(startNode));

    return 0;
}

void PrintList(Node *n)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            std::cout << "[" << n->boardState[i][j] << "]";
        }
        std::cout << std::endl;
    }

    return;
}

bool LoadFile(Node *n)
{
    std::ifstream file("input.txt");

    bool fullyLoaded = false;

    if (file.is_open())
    {
        for (int i = 0; i < ROWS; i++)
        {
            std::string line = "";
            getline(file, line);
            for (int j = 0; j < COLS; j++)
            {
                n->boardState[i][j] = line[j];
            }
        }
        fullyLoaded = true;
    }
    else
    {
        std::cout << "NO FILE FOUND, LOADING IN DEFAULT CONFIGURATION!" << std::endl;
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (i == 0 || i == 1)
                {
                    n->boardState[i][j] = 'B';
                }
                else if (i == 4 || i == 5)
                {
                    n->boardState[i][j] = 'W';
                }
                else
                {
                    n->boardState[i][j] = 'X';
                }
            }
        }
        fullyLoaded = true;
    }

    file.close();

    return fullyLoaded;
}

int CheckWinCondition(Node *n)
{
    if (n->isWhitePlayer) // Player 1 win check
    {
        for (int i = 0; i < COLS; i++)
        {
            if (n->boardState[0][i] == 'W')
            {
                return 1;
            }
        }
    }
    else // Player 2 win check
    {
        for (int i = 0; i < COLS; i++)
        {
            if (n->boardState[5][i] == 'B')
            {
                return -1;
            }
        }
    }

    // Capture win checks

    if (CheckNumberOfPieces(n, 'B') == 0) // If all Black Pieces captured, White win
    {
        return 1;
    }

    if (CheckNumberOfPieces(n, 'W') == 0) // If all White Pieces captured, Black win
    {
        return -1;
    }

    return 0;
}

int CheckNumberOfPieces(Node *n, char c)
{
    int numChars = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (n->boardState[i][j] == c)
            {
                numChars++;
            }
        }
    }
    return numChars;
}

void LookForPieces(Node *n)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            Point p(i, j); // Starting point to pass into CreateChildren(...)
            if (n->isWhitePlayer)
            {
                if (n->boardState[i][j] == 'W')
                {
                    CreateChildren(n, p);
                }
            }
            else
            {
                if (n->boardState[i][j] == 'B')
                {
                    CreateChildren(n, p);
                }
            }
        }
    }

    return;
}

void CreateChildren(Node *n, Point p)
{
    // Try to create left diagonal move
    Point p_dl = CreateDiagonalPoint(n, p, true);
    if (!(p_dl.r == -1 && p_dl.c == -1))
    {
        CreateChild(n, p, p_dl);
    }

    // Try to create forward move
    Point p_f = CreateForwardPoint(n, p);
    if (!(p_f.r == -1 && p_f.c == -1))
    {
        CreateChild(n, p, p_f);
    }

    // Try to create right diagonal move
    Point p_dr = CreateDiagonalPoint(n, p, false);
    if (!(p_dr.r == -1 && p_dr.c == -1))
    {
        CreateChild(n, p, p_dr);
    }

    return;
}

void CreateChild(Node *n, Point startPt, Point endPt)
{
    Node *newNode = new Node();

    newNode->movedFrom = startPt;
    newNode->movedTo = endPt;
    newNode->isWhitePlayer = !n->isWhitePlayer;

    // Set the new board state to the old board state
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            newNode->boardState[i][j] = n->boardState[i][j];
        }
    }

    // Now do modify the new board state in newNode

    // Set New Position
    if (n->isWhitePlayer)
    {
        newNode->boardState[endPt.r][endPt.c] = 'W';
    }
    else
    {
        newNode->boardState[endPt.r][endPt.c] = 'B';
    }

    // Erase Old Position
    newNode->boardState[startPt.r][startPt.c] = 'X';

    // Push this child into n
    n->children.push_back(newNode);
    return;
}

Point CreateForwardPoint(Node *n, Point p)
{
    if (n->isWhitePlayer) // White pieces move up
    {
        if (n->boardState[p.r - 1][p.c] == 'X')
        {
            return Point(p.r - 1, p.c);
        }
        else
        {
            return Point(-1, -1);
        }
    }
    else // Black pieces move down
    {
        if (n->boardState[p.r + 1][p.c] == 'X')
        {
            return Point(p.r + 1, p.c);
        }
        else
        {
            return Point(-1, -1);
        }
    }
}

Point CreateDiagonalPoint(Node *n, Point p, bool isLeftMove)
{
    if (n->isWhitePlayer)
    {
        if (isLeftMove)
        {
            if (p.c - 1 < 0)
            {
                return Point(-1, -1);
            }
            else
            {
                if (n->boardState[p.r - 1][p.c - 1] == 'W') // Cannot capture yourself
                {
                    return Point(-1, -1);
                }
                else
                {
                    return Point(p.r - 1, p.c - 1);
                }
            }
        }
        else
        {
            if (p.c + 1 > 2)
            {
                return Point(-1, -1);
            }
            else
            {
                if (n->boardState[p.r - 1][p.c + 1] == 'W') // Cannot capture yourself
                {
                    return Point(-1, -1);
                }
                else
                {
                    return Point(p.r - 1, p.c + 1);
                }
            }
        }
    }
    else
    {
        if (isLeftMove)
        {
            if (p.c - 1 < 0)
            {
                return Point(-1, -1);
            }
            else
            {
                if (n->boardState[p.r + 1][p.c - 1] == 'B') // Cannot capture yourself
                {
                    return Point(-1, -1);
                }
                else
                {
                    return Point(p.r + 1, p.c - 1);
                }
            }
        }
        else
        {
            if (p.c + 1 > 2)
            {
                return Point(-1, -1);
            }
            else
            {
                if (n->boardState[p.r + 1][p.c + 1] == 'B') // Cannot capture yourself
                {
                    return Point(-1, -1);
                }
                else
                {
                    return Point(p.r + 1, p.c + 1);
                }
            }
        }
    }
}
