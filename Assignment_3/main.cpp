#include <iostream>
#include <vector>

using namespace std;

struct Point
{
    int x;
    int y;
};

struct Node
{
    struct Point movedFrom;  // Point Moved From
    struct Point movedTo;    // Point Moved To
    vector<Node *> children; // List of Children
    char boardState[3][6];   // Board State
    bool isWhitePlayer;      // Is this player 1?
};

/* Three things I need to check right now:
1.  Check win condition -> Stops recursion
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
            a.  Try to create three points (refer to 3.E and 3.F)
            b.  If any point is NULL, do not create the related child node
        2.  Link these new nodes up as children to the parent (store them in the vector)
    C.  GoAlphaBeta(Node *child)
        1.  Go into the next stage of a-B pruning within the child (refer to Wikipedia)
    D.  CreateChildNode(Point from, Point to, char[6][3] boardState)
        1.  create a new Node* based upon the from and to points
        2.  modify the board state appropriately (refer to 2.B.3)
        3.  set isWhitePlayer to !isWhitePlayer
        4.  set newNode->movedFrom = from, newNode->movedTo = to
        5.  set new board state to newNode->boardState
        6.  return this newNode as a ptr
    E.  CreateForwardPoint(Point p, char[6][3] boardState, bool isWhitePlayer)
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
    F.  Create DiagonalPoint(Point p, char[6][3] boardState, bool isLeftMove, bool isWhitePlayer)
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
*/

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
