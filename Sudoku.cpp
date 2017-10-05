#include <iostream>
#include <vector>
#include <queue>
#include <thread>

using namespace std::chrono_literals;

struct SudokuBoard
{
	typedef std::vector<std::vector<int>> Board;

	SudokuBoard()
	{
		board.resize(9, std::vector<int>(9, 0));
	}

	SudokuBoard(Board b)
	{
		board = b;
	}

	int get_cell_value(int i, int j)
	{
		return board[i][j];
	}

	void set_cell_value(int i, int j, int value)
	{
		board[i][j] = value;
	}

	bool is_consistent()
	{
		Board repeating_numbers((9), std::vector<int>(9, 0));

		Board rows_num(10, std::vector<int>(9, 0));
		Board cols_num(10, std::vector<int>(9, 0));

		
		for (int i = 0; i < board.size(); ++i)
		{
			for (int j = 0; j < board[i].size(); ++j)
			{
				if (board[i][j] != 0)
				{
					++rows_num[i][board[i][j] - 1];
					++cols_num[j][board[i][j] - 1];
				}
				int index = j / 3 + (i / 3 * 3);
				if (check_repeat(repeating_numbers[index], board[i][j]))
					return false;
				repeating_numbers[index].push_back(board[i][j]);
			}
		}
		for (int i = 0; i < rows_num.size(); ++i)
		{
			for (int j = 0; j < rows_num[i].size(); ++j)
			{
				if (rows_num[i][j] > 1 || cols_num[i][j] > 1)
					return false;
			}
		}

		return true;
	}

private:

	bool check_repeat(std::vector<int> const& v, int n)
	{
		for (int i = 0; i < v.size(); ++i)
			if (v[i] != 0 && v[i] == n)
				return true;
		return false;
	}

public:
	Board board;
};

std::istream& operator >> (std::istream& ist, SudokuBoard& sb)
{
	for (int i = 0; i < sb.board.size(); ++i)
	{
		for (int j = 0; j < sb.board[i].size(); ++j)
		{
			ist >> sb.board[i][j];
		}
	}
	return ist;
}

std::ostream& operator << (std::ostream& ost, SudokuBoard sb)
{
	for (int i = 0; i < sb.board.size(); ++i)
	{
		for (int j = 0; j < sb.board[i].size(); ++j)
		{
			ost << sb.board[i][j] << ' ';
		}
		ost << '\n';
	}
	return ost;
}

struct SudokuSolver
{
	SudokuSolver(SudokuBoard& board)
		:sb(board)
	{}

	bool Solve()
	{
		if (!sb.is_consistent())
			return false;
		for (int i = 0; i < sb.board.size(); ++i)
		{
			for (int j = 0; j < sb.board[i].size(); ++j)
			{
				if (sb.board[i][j] == 0)
				{
					for (int k = 1; k < 10; ++k)
					{
						sb.board[i][j] = k;
						//
						system("cls");
						std::cout << sb << std::endl;
						std::this_thread::sleep_for(50ms);
						//
						if (Solve())
							return true;
					}
					sb.board[i][j] = 0;
					return false;
				}
			}
		}
	}
	
public:
	SudokuBoard& sb;
};

int main()
{
	SudokuBoard sb;
	std::cin >> sb;
	SudokuSolver ss(sb);
	if (ss.Solve())
	{
		std::cout << "Solved!" << std::endl << sb << std::endl;
		return 0;
	}
	std::cout << "Unsolvable!" << std::endl;
}

/*

CTRL + Z, Enter

0 0 0 2 6 0 7 0 1
6 8 0 0 7 0 0 9 0
1 9 0 0 0 4 5 0 0
8 2 0 1 0 0 0 4 0
0 0 4 6 0 2 9 0 0
0 5 0 0 0 3 0 2 8
0 0 9 3 0 0 0 7 4
0 4 0 0 5 0 0 3 6
7 0 3 0 1 8 0 0 0

Wrong
1 2 3 1 2 3 1 2 3
4 5 6 4 5 6 4 5 6
7 8 9 7 8 9 7 8 9
1 2 3 1 2 3 1 2 3
4 5 6 4 5 6 4 5 6
7 8 9 7 8 9 7 8 9
1 2 3 1 2 3 1 2 3
4 5 6 4 5 6 4 5 6
7 8 9 7 8 9 7 8 9

Solved
4 3 5 2 6 9 7 8 1
6 8 2 5 7 1 4 9 3
1 9 7 8 3 4 5 6 2
8 2 6 1 9 5 3 4 7
3 7 4 6 8 2 9 1 5
9 5 1 7 4 3 6 2 8
5 1 9 3 2 6 8 7 4
2 4 8 9 5 7 1 3 6
7 6 3 4 1 8 2 5 9

1 7 2 5 4 9 6 8 3
6 4 5 8 7 3 2 1 9
3 8 9 2 6 1 7 4 5
4 9 6 3 2 7 8 5 1
8 1 3 4 5 6 9 7 2
2 5 7 1 9 8 4 3 6
9 6 4 7 1 5 3 2 8
7 3 1 6 8 2 5 9 4
5 2 8 9 3 4 1 6 7
*/