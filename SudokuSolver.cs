using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SudokuSolver
{
    class Program
    {
        private static int EMPTY_CELL = 0;

        public static bool SolveBoard(int [,] board)
        {
            return CanSolveFromCell(board, 0, 0);
        }

        private static bool CanSolveFromCell(int [,] board, int row, int col)
        {
            // If at end of row (col == board.GetLength()), then move to next row
            // 0 - Row dimension of array
            // 1 - Column dimension of array
            if (col == board.GetLength(1))
            {
                col = 0;
                row++;

                if (row == board.GetLength(0))
                {
                    return true;
                }
            }

            // Skip cells that already contain value
            if (board[row, col] != EMPTY_CELL)
            {
                return CanSolveFromCell(board, row, col + 1);
            }

            for (int val = 1; val <= board.GetLength(0); val++)
            {
                int valToInsert = val;

                if (CanInsertVal(board, row, col, valToInsert))
                {
                    board[row, col] = valToInsert;
                    if (CanSolveFromCell(board, row, col + 1))
                    {
                        return true;
                    }
                    board[row, col] = EMPTY_CELL;
                }
            }

            return false;
        }

        private static bool CanInsertVal(int [,] board, int row, int col, int valToInsert)
        {
            //Check column of the insertion
            for (int i = 0; i < board.GetLength(0); i++)
            {
                if (valToInsert == board[i, col])
                {
                    return false;
                }
            }

            //Check row of the insertion
            for (int j = 0; j < board.GetLength(1); j++)
            {
                if (valToInsert == board[row, j])
                {
                    return false;
                }
            }

            // Check board constraints
            // Find the size of the sub-box
            int subBoxSize = (int)System.Math.Sqrt(board.GetLength(0));

            // Find index of box
            // Example - For 9x9 traditional sudoku index would be 0, 1, or 2
            int verticalBoxIndex = row / subBoxSize;
            int horizontalBoxIndex = col / subBoxSize;

            // Find top left corner of sub-box
            int topLeftSubBoxRow = subBoxSize * verticalBoxIndex;
            int topLeftSubBoxCol = subBoxSize * horizontalBoxIndex;

            //Check sub-box of the insertion
            for (int m = 0; m < subBoxSize; m++)
            {
                for (int n = 0; n < subBoxSize; n++)
                {
                    if (valToInsert == board[topLeftSubBoxRow + m, topLeftSubBoxCol + n])
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        static void Main()
        {
            int [ , ] sudokuBoard = new int[9, 9] {
                {0, 3, 0, 2, 8, 7, 0, 5, 0},
                {5, 8, 0, 6, 4, 1, 9, 0, 0},
                {1, 0, 6, 9, 0, 0, 0, 2, 4},
                {2, 0, 0, 0, 6, 0, 3, 0, 8},
                {0, 9, 5, 0, 0, 0, 2, 6, 0},
                {8, 0, 4, 0, 3, 0, 0, 0, 9},
                {6, 2, 0, 0, 0, 5, 4, 0, 3},
                {0, 0, 3, 8, 2, 6, 0, 1, 5},
                {0, 5, 0, 3, 1, 4, 0, 9, 0}};

            if (SolveBoard(sudokuBoard))
            {
                for (int i = 0; i < sudokuBoard.GetLength(0); i++)
                {
                    for (int j = 0; j < sudokuBoard.GetLength(1); j++)
                    {
                        Console.Write(string.Format("{0} ", sudokuBoard[i, j]));
                    }

                    Console.WriteLine();
                }
            }
            else
            {
                Console.Write("NO_SOLUTION\n");
            }

            string input = Console.ReadLine();
        }
    }
}
