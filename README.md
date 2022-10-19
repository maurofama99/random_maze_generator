# Random maze generator implemented with Disjoint Sets data structures.
Cells are represented by their positions in the matrix (not coordinates). 
I used an array of integers called parent[]. If we are dealing with N cells, the i’th element of the parent[] array is the parent of the i’th cell, which is the i’th element of the array. These relationships create one or more virtual trees.
At the end of the execution of the algorithm, we’ll have the parent[] array representing all the disjoint sets created, thus the maze.

Link to Google Colab: https://colab.research.google.com/drive/1HPZXrkAzTAFyDMcEWm0z45nWdSOVHUWr?usp=sharing

Check out a little presentation of the project here[]
