#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <limits>

// Global consts
const int ROWS = 6;
const int COLS = 3;

const bool DEBUG_PRINTS = false;

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
    Node *whiteWinNode;      // Will be set the first time a node receives a "1" from a child
    std::vector<Node *> children; // List of Children
    char boardState[ROWS][COLS];   // Board State (rows x cols)
    bool isWhitePlayer;      // Is this player 1?
};

/* Design Stuff:
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
    A.  Check Win (Node *) - DONE
        1.  If either player wins, return true
            a.  if (isWhitePlayer  && (any top square is W)) return +1
            b.  if (!isWhitePlayer && (any bot square is B)) return -1
            c.  if (numberBlackPieces == 0) return +1
            d.  if (numberWhitePieces == 0) return -1
    B.  CreateChildren (Node *current) - DONE
        1.  Create three, two, or one valid nodes based upon board states - there is always a valid move
            a.  Loop through the current board state array, find all areas that are 'W' for isWhitePlayer, or 'B' otherwise
                and when each player piece is found, do the following:
                (1)  Try to create three points (refer to 3.E and 3.F) from this position for possible piece moves
                (2)  If any point is NULL, do not create the related child node for this piece
        2.  Link all of these new nodes up as children to the parent (store them in the vector)
    C.  GoAlphaBeta(Node *child)
        1.  Go into the next stage of a-B pruning within the child (refer to Wikipedia)
    D.  CreateChildNode(Point from, Point to, char[6][3] boardState) - DONE
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
    1.  Test Forward movement edge case - DONE
    2.  Test Diagonal Movement edge cases - DONE
        A.  OB Left  - DONE
        B.  OB Right - DONE
        C.  Do not capture your own piece - DONE
B.  Test one level of expansion from white player - DONE
C.  Test one level of expansion from black player - DONE
D.  Test a 1 v 1 pawn simulation
E.  Do a-B pruning with 1 v 1
*/

bool LoadFile(Node*); // Load in the initial board state
bool LoadFileCustom(Node*, std::string); // Load in the initial board state
void PrintList(Node*); // Print the board state of the node in question
int CheckWinCondition(Node *); // Return 1 if white wins, -1 if black wins
int CheckNumberOfPieces(Node *, char); // Check # of pieces for win cond
void FindAndMovePieces(Node *); // Loop through the array and find all playable pieces for a player
void CreateChildren(Node *, Point); // Create all of the valid children of a current node
Point CreateForwardPoint(Node *, Point); // Will create a forward movement point, if it is valid
Point CreateDiagonalPoint(Node *, Point, bool); // Will create a left or right diagonal move, if it is valid
void CreateChild(Node *, Point, Point); // Create a child of a current board state
void Tests(Node *); // Tests for tree functionality
int AlphaBetaSearch(Node *, int a, int b); // Search Algorithm + Pruning

int main()
{
    // Create the initial node:
    Node* startNode = new Node();
    startNode->isWhitePlayer = true;
    LoadFileCustom(startNode, "input.txt");
    if (DEBUG_PRINTS) PrintList(startNode);
    //Tests(startNode);
    int winInt = AlphaBetaSearch(startNode, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    std::cout << std::endl << "-----Expansions Complete-----" << std::endl;

    std::cout << std::endl << "----------------RESULTS-----------------"<< std::endl;

    printf("Answer:  Player A moves the piece at (%d, %d) to (%d, %d).\n",
           startNode->whiteWinNode->movedFrom.r, startNode->whiteWinNode->movedFrom.c,
           startNode->whiteWinNode->movedTo.r, startNode->whiteWinNode->movedTo.c
    );

    std::cout << "Alpha Beta Search Result: " << winInt << std::endl;

    return 0;
}

int AlphaBetaSearch(Node *n, int a, int b)
{
    // Check the win conditions
    int winInt = CheckWinCondition(n);
    if (winInt != 0)
    {
        if (DEBUG_PRINTS) printf("Win value found, returning %d.\n", winInt);
        return winInt;
    }
    else
    {
        // Create the children first
        // Find and Move Pieces finds all of the pieces
        // of the current player, and then creates children
        // from all of them.  Then, for each of those children
        // we recurse through a-B pruning until a <= B
        FindAndMovePieces(n);

        // Loop through all children and expand through tree
        for (unsigned int i = 0; i < n->children.size(); i++) // For each child
        {
            if (n->isWhitePlayer) // Maximizing Player
            {
                if (b > a)
                {
                    printf("Player A moves the piece at (%d,%d) to (%d,%d).\n", n->children[i]->movedFrom.r, n->children[i]->movedFrom.c, n->children[i]->movedTo.r, n->children[i]->movedTo.c);
                    if (DEBUG_PRINTS) PrintList(n->children[i]);
                    a = std::max(a, AlphaBetaSearch(n->children[i], a, b));

                    if ((a == 1 || i == n->children.size() - 1) && n->whiteWinNode == NULL)
                    {
                        // Get first reference to winning move or just the last move if there is no good one
                        // This is done specifically for White Player, or P1, to satisfy the first move problem
                        // as outlined in the assignment 3 document
                        n->whiteWinNode = n->children[i];
                    }

                    if (b <= a && i != n->children.size() - 1) // Start off the print sequence if true
                    {
                        printf("Skipping Player A's moves: ");
                    }
                }
                else
                {
                    printf("(%d,%d) to (%d,%d)", n->children[i]->movedFrom.r, n->children[i]->movedFrom.c, n->children[i]->movedTo.r, n->children[i]->movedTo.c);
                    if (i == n->children.size() - 1)
                    {
                        printf("; Alpha = %d, Beta = %d.\n", a, b);
                    }
                    else
                    {
                        printf(", ");
                    }
                }
            }
            else // Minimizing Player
            {
                if (b > a)
                {
                    printf("Player B moves the piece at (%d,%d) to (%d,%d).\n", n->children[i]->movedFrom.r, n->children[i]->movedFrom.c, n->children[i]->movedTo.r, n->children[i]->movedTo.c);
                    if (DEBUG_PRINTS) PrintList(n->children[i]);
                    b = std::min(b, AlphaBetaSearch(n->children[i], a, b));

                    if ((b == 1 || i == n->children.size() - 1) && n->whiteWinNode == NULL)
                    {
                        // Get first reference to winning move or just the last move if there is no good one
                        // This is done specifically for White Player, or P1, to satisfy the first move problem
                        // as outlined in the assignment 3 document
                        n->whiteWinNode = n->children[i];
                    }

                    if (b <= a && i != n->children.size() - 1) // Start off the print sequence if true
                    {
                        printf("Skipping Player B's moves: ");
                    }
                }
                else// Start off the print sequence if true
                {
                    printf("(%d,%d) to (%d,%d)", n->children[i]->movedFrom.r, n->children[i]->movedFrom.c, n->children[i]->movedTo.r, n->children[i]->movedTo.c);
                    if (i == n->children.size() - 1)
                    {
                        printf("; Alpha = %d, Beta = %d.\n", a, b);
                    }
                    else
                    {
                        printf(", ");
                    }
                }
            }

            //printf("Alpha: %d, Beta: %d\n", a, b);
        }

        // Return the appropriate value
        if (n->isWhitePlayer)
        {
            return a;
        }
        else
        {
            return b;
        }
    }
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

bool LoadFileCustom(Node *n, std::string fileName)
{
    std::ifstream file(fileName.c_str());

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

    // Capture win checks
    if (CheckNumberOfPieces(n, 'B') == 0) // If all Black Pieces captured, White win
    {
        return 1;
    }

    if (CheckNumberOfPieces(n, 'W') == 0) // If all White Pieces captured, Black win
    {
        return -1;
    }

    // Goal Zone Win Checks
    for (int i = 0; i < COLS; i++)
    {
        if (n->boardState[0][i] == 'W') // If White piece is at top
        {
            return 1;
        }
    }

    for (int i = 0; i < COLS; i++)
    {
        if (n->boardState[5][i] == 'B') // If Black piece is at bottom
        {
            return -1;
        }
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
            if (n->boardState[i][j] == c) // If character found, increment
            {
                numChars++;
            }
        }
    }
    return numChars;
}

void FindAndMovePieces(Node *n) // Look for white or black pieces
{
    if (!n->isWhitePlayer) // Iterate from bottom right to top left
    {
        for (int i = ROWS - 1; i >= 0; i--)
        {
            for (int j = COLS - 1; j >= 0 ; j--)
            {
                Point p(i, j); // Starting point to pass into CreateChildren(...)
                if (n->boardState[i][j] == 'B')
                {
                    CreateChildren(n, p);
                }
            }
        }
    }
    else // Iterate from top left to bottom
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                Point p(i, j); // Starting point to pass into CreateChildren(...)
                if (n->boardState[i][j] == 'W')
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
    newNode->whiteWinNode = NULL;

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

void Tests(Node *startNode)
{
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
    printf("%d\n\n", CheckWinCondition(startNode));

    // Now Lets do some tests with checking tree expansions

    // Checks moves that are all valid (testing tree child creation, all cases)

    std::cout << "Basic Movement Tests" << std::endl;

    // Test Whites moves
    LoadFileCustom(startNode, "InputTestAllValidMoves.txt");
    startNode->isWhitePlayer = true;
    FindAndMovePieces(startNode);

    // Print out the parent board state, and then the children board states
    std::cout << "Board of White Player Parent" << std::endl;
    PrintList(startNode);
    for (unsigned int i = 0; i < startNode->children.size(); i++)
    {
        std::cout << "Board of child " << i << std::endl;
        PrintList(startNode->children[i]);
    }

    // Delete all of the children
    for (unsigned int i = startNode->children.size() - 1; i > 0; i--)
    {
        delete startNode->children[i];
    }
    startNode->children.clear();

    // Test Blacks moves
    LoadFileCustom(startNode, "InputTestAllValidMoves.txt");
    startNode->isWhitePlayer = false;
    FindAndMovePieces(startNode);

    // Print out the parent board state, and then the children board states
    std::cout << "Board of Black Player Parent" << std::endl;
    PrintList(startNode);
    for (unsigned int i = 0; i < startNode->children.size(); i++)
    {
        std::cout << "Board of child " << i << std::endl;
        PrintList(startNode->children[i]);
    }

    // Delete all of the children
    for (unsigned int i = startNode->children.size() - 1; i > 0; i--)
    {
        delete startNode->children[i];
    }
    startNode->children.clear();

    std::cout << std::endl;

    std::cout << "Illegal Movement Tests (bad moves will not be created)" << std::endl;

    std::cout << "Illegal Forward Movement Tests" << std::endl;
    // Test Whites moves
    LoadFileCustom(startNode, "blank.txt");
    startNode->isWhitePlayer = true;

    startNode->boardState[5][1] = 'W';
    startNode->boardState[4][1] = 'W';

    FindAndMovePieces(startNode);

    // Print out the parent board state, and then the children board states
    std::cout << "Board of White Player Parent: W Case" << std::endl;
    PrintList(startNode);
    for (unsigned int i = 0; i < startNode->children.size(); i++)
    {
        std::cout << "Board of child " << i << std::endl;
        PrintList(startNode->children[i]);
    }

    // Delete all of the children
    for (unsigned int i = startNode->children.size() - 1; i > 0; i--)
    {
        delete startNode->children[i];
    }
    startNode->children.clear();

    startNode->boardState[4][1] = 'B';

    FindAndMovePieces(startNode);

    // Print out the parent board state, and then the children board states
    std::cout << "Board of White Player Parent: B Case" << std::endl;
    PrintList(startNode);
    for (unsigned int i = 0; i < startNode->children.size(); i++)
    {
        std::cout << "Board of child " << i << std::endl;
        PrintList(startNode->children[i]);
    }

    // Delete all of the children
    for (unsigned int i = startNode->children.size() - 1; i > 0; i--)
    {
        delete startNode->children[i];
    }
    startNode->children.clear();

    // Test Blacks moves
    LoadFileCustom(startNode, "blank.txt");
    startNode->isWhitePlayer = false;

    startNode->boardState[0][1] = 'B';
    startNode->boardState[1][1] = 'B';
    FindAndMovePieces(startNode);

    // Print out the parent board state, and then the children board states
    std::cout << "Board of Black Player Parent: B Case" << std::endl;
    PrintList(startNode);
    for (unsigned int i = 0; i < startNode->children.size(); i++)
    {
        std::cout << "Board of child " << i << std::endl;
        PrintList(startNode->children[i]);
    }

    // Delete all of the children
    for (unsigned int i = startNode->children.size() - 1; i > 0; i--)
    {
        delete startNode->children[i];
    }
    startNode->children.clear();

    startNode->boardState[1][1] = 'W';
    FindAndMovePieces(startNode);

     // Print out the parent board state, and then the children board states
    std::cout << "Board of Black Player Parent: W Case" << std::endl;
    PrintList(startNode);
    for (unsigned int i = 0; i < startNode->children.size(); i++)
    {
        std::cout << "Board of child " << i << std::endl;
        PrintList(startNode->children[i]);
    }

    // Delete all of the children
    for (unsigned int i = startNode->children.size() - 1; i > 0; i--)
    {
        delete startNode->children[i];
    }
    startNode->children.clear();

    std::cout << "Illegal Diagonal Movement Tests" << std::endl;
    // Test Whites moves
    LoadFileCustom(startNode, "blank.txt");
    startNode->isWhitePlayer = true;

    startNode->boardState[5][0] = 'W';
    startNode->boardState[5][2] = 'W';

    FindAndMovePieces(startNode);

    // Print out the parent board state, and then the children board states
    std::cout << "Board of White Player Parent" << std::endl;
    PrintList(startNode);
    for (unsigned int i = 0; i < startNode->children.size(); i++)
    {
        std::cout << "Board of child " << i << std::endl;
        PrintList(startNode->children[i]);
    }

    // Delete all of the children
    for (unsigned int i = startNode->children.size() - 1; i > 0; i--)
    {
        delete startNode->children[i];
    }
    startNode->children.clear();


    // Test Blacks moves
    LoadFileCustom(startNode, "blank.txt");
    startNode->isWhitePlayer = false;

    startNode->boardState[0][0] = 'B';
    startNode->boardState[0][2] = 'B';

    FindAndMovePieces(startNode);

    // Print out the parent board state, and then the children board states
    std::cout << "Board of Black Player Parent" << std::endl;
    PrintList(startNode);
    for (unsigned int i = 0; i < startNode->children.size(); i++)
    {
        std::cout << "Board of child " << i << std::endl;
        PrintList(startNode->children[i]);
    }

    // Delete all of the children
    for (unsigned int i = startNode->children.size() - 1; i > 0; i--)
    {
        delete startNode->children[i];
    }
    startNode->children.clear();

    std::cout << "Legal Capture Movement Tests" << std::endl;
    // Test Whites moves
    LoadFileCustom(startNode, "blank.txt");
    startNode->isWhitePlayer = true;

    startNode->boardState[5][0] = 'W';
    startNode->boardState[5][2] = 'W';
    startNode->boardState[4][1] = 'B';

    FindAndMovePieces(startNode);

    // Print out the parent board state, and then the children board states
    std::cout << "Board of White Player Parent: Capture Black" << std::endl;
    PrintList(startNode);
    for (unsigned int i = 0; i < startNode->children.size(); i++)
    {
        std::cout << "Board of child " << i << std::endl;
        PrintList(startNode->children[i]);
    }

    // Delete all of the children
    for (unsigned int i = startNode->children.size() - 1; i > 0; i--)
    {
        delete startNode->children[i];
    }
    startNode->children.clear();


    // Test Blacks moves
    LoadFileCustom(startNode, "blank.txt");
    startNode->isWhitePlayer = false;

    startNode->boardState[0][0] = 'B';
    startNode->boardState[0][2] = 'B';
    startNode->boardState[1][1] = 'W';

    FindAndMovePieces(startNode);

    // Print out the parent board state, and then the children board states
    std::cout << "Board of Black Player Parent: Capture White" << std::endl;
    PrintList(startNode);
    for (unsigned int i = 0; i < startNode->children.size(); i++)
    {
        std::cout << "Board of child " << i << std::endl;
        PrintList(startNode->children[i]);
    }

    // Delete all of the children
    for (unsigned int i = startNode->children.size() - 1; i > 0; i--)
    {
        delete startNode->children[i];
    }
    startNode->children.clear();

    std::cout << "Illegal Capture Movement Tests" << std::endl;
    // Test Whites moves
    LoadFileCustom(startNode, "blank.txt");
    startNode->isWhitePlayer = true;

    startNode->boardState[5][0] = 'W';
    startNode->boardState[5][2] = 'W';
    startNode->boardState[4][1] = 'W';

    FindAndMovePieces(startNode);

    // Print out the parent board state, and then the children board states
    std::cout << "Board of White Player Parent: Capture Nothing" << std::endl;
    PrintList(startNode);
    for (unsigned int i = 0; i < startNode->children.size(); i++)
    {
        std::cout << "Board of child " << i << std::endl;
        PrintList(startNode->children[i]);
    }

    // Delete all of the children
    for (unsigned int i = startNode->children.size() - 1; i > 0; i--)
    {
        delete startNode->children[i];
    }
    startNode->children.clear();

    // Test Blacks moves
    LoadFileCustom(startNode, "blank.txt");
    startNode->isWhitePlayer = false;

    startNode->boardState[0][0] = 'B';
    startNode->boardState[0][2] = 'B';
    startNode->boardState[1][1] = 'B';

    FindAndMovePieces(startNode);

    // Print out the parent board state, and then the children board states
    std::cout << "Board of Black Player Parent: Capture Nothing" << std::endl;
    PrintList(startNode);
    for (unsigned int i = 0; i < startNode->children.size(); i++)
    {
        std::cout << "Board of child " << i << std::endl;
        PrintList(startNode->children[i]);
    }

    // Delete all of the children
    for (unsigned int i = startNode->children.size() - 1; i > 0; i--)
    {
        delete startNode->children[i];
    }
    startNode->children.clear();
}
