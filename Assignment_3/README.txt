Name:  Tim Righettini
USC ID: 9445525333
E-mail Address: trighett@usc.edu

-- CSCI 460 Assignment #3 README --

Hello,

This document will go over four key points: The design/structure behind the program, instructions on compilation, any issues that might come up when running the program, and the answers to the review questions.

----- 1. Program Structure & Design -----

The program is designed in such a way so that all you have to do is edit the input.txt file, and 
Alpha-Beta pruning will run on the board read in from that file. Here are the key things you should
know about the structure and design of the program:


---Design Paradigms---

A.  The game tree builds itself as the program executes.  Thus, the parts of the tree that are pruned 
    should not even be present in the tree.  The board state is represented as a 2D array -> BoardState[rows][columns].

B.  This program implements DFS, so the left most node is always selected for expansion in the tree.  
    Nodes for each piece's moves are ordered as follows:

    1.  Diagonal Left Moves come up first
    2.  Straight ahead moves come up in the middle
    3.  Diagonal Left Moves come up last    

    Which assumes each move is valid.  One, two, or all of these moves may be missing for a piece if 
    the current board state of tree node n does not allow these moves to occur.

C.  The best initial move is stored for Player A when a child of the root returns a +1.  This specific child
    is stored as a reference in the root, and this node contains which piece in question moved from and to within 
    in the initial board state to get the best possible advantage.  No more best moves are stored after 
    that.  If no scenarios return a +1 for Player A, the last child to be checked is set as the reference 
    in the root, because something needs to be set.

D.  Board State:  It is a 6 x 3 array that directly maps to the input.txt structure outlined in the assignment 3 specifications.
    If input.txt does not open, then the initial board state from Figure 1 in the spec will be loaded in.

E.  Player A always goes first, no matter what is loaded from input.txt.
    

---Program Structure---

A.  Here is a general program flow:

    1.  In main(), the first node loads in the board state from input.txt, and then the AlphaBetaSearch() begins.
    2.  AlphaBetaSearch() is run recursively, and works similarly to the lecture slides seen in class.
    3.  For the node examined in AlphaBetaSearch(), these steps are taken to find the pieces of the current player, 
        create child nodes, and then expand from there.

        a.  First, check the board state for a win condition as outlined in the doc.  If it exists, exit out of the recursion
            going up a level.  If b <= a, prune everything else and move up another level and continue if there are still nodes
            to check.
        b.  Given the proper player, loop through the current board state, find all valid moves for the proper pieces, create 
            new states that result from those moves, and then add them as children to the current node.
        c.  Expand down the first node as seen in DFS, and repeat with the new node in question.

    4.  When the program is complete, the optimal first move for Player A is outputted to the screen, along with the final value
        that the game tree root returns (-1, or +1)

B.  If you want to view the board state along with other print values, set DEBUG_PRINTS to true.

---Final Comments

If you want to look more into my design specifically, there are numerous comments within main.cpp that elaborate upon the concepts
aforementioned.


----- 2. Instructions for Compilation/Execution -----

Go on aludra (after copying main.cpp to it) and then run the following commands:

g++ -g -o assign3 main.cpp

./assign3

This will compile the file and run it as you deem fit with the parameters you place in input.txt.

-------------------------------------------

And that's a wrap!  Thanks for viewing the README.

Tim
Bloomthegame.com