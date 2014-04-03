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
            b.  Otherwise, just move there
    B.  For each piece currently in play, get all possible moves and branch out
        1.  Loop through array
            a.  if (BoardState[x][y] = W && isWhitePlayer)  -> Expand out this piece's moves
            b.  if (BoardState[x][y] = B && !isWhitePlayer) -> Expand out this piece's moves
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
            a.  if (isWhitePlayer  && (any top square is W)) return true
            b.  if (!isWhitePlayer && (any bot square is B)) return true
    B.  CreateChildren (Node *current)
        1.  Create three, two, or one valid nodes based upon board states - there is always a valid move
        2.  Link these new nodes up as children to the parent (store them in the vector)
    C.  GoAlphaBeta(Node *child)
        1.  Go into the next stage of a-B pruning within the child (refer to Wikipedia)

*/

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
