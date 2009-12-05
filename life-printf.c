#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define DIM 25

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

bool board_0[DIM][DIM], board_1[DIM][DIM];
bool b = 0;

// get the value at (x, y) on the live board.
bool board(int i, int j)
{
	return (b ? board_1[i][j] : board_0[i][j]);
}

// set (x, y) on the buffer board (i.e. the _not_ being shown) to n.
int set_board(int i, int j, bool n)
{
	b ? (board_0[i][j] = n) : (board_1[i][j] = n);
}

// loop through a 3x3 square around (x, y) and count the live neighbours (that
// is, ignoring the point (x, y) itself).
int get_live_nbs(int x, int y)
{
	int i, j, count = 0;

	for (i = max(x-1, 0); i <= min(x+1, DIM-1); i++)
	{
		for (j = max(y-1, 0); j <= min(y+1, DIM-1); j++)
		{
			if ((i == x) && (j == y))
				continue;

			count += board(i, j);
		}
	}
	return count;
}

int main(int argc, char **argv)
{
	int i, j, live_nbs;

	for (i = 0; i < DIM; i++)
		for (j = 0; j < DIM; j++)
			board_0[i][j] = board_1[i][j] = 0; // zero the boards

	if (argc % 2 == 0)
	{
		printf("Need an even number of arguments, for (x, y) pairs.\n");
		return 1;
	}

	b = !b;
	for (i = 1; i < argc; i += 2)
		set_board(atoi(argv[i]), atoi(argv[i + 1]), 1);
	b = !b;

	while (1)
	{
		printf("\n");
		for (i = 0; i < DIM; i++)
		{
			printf("\n|");
			for (j = 0; j < DIM; j++)
			{
				live_nbs = get_live_nbs(i, j);

				if (live_nbs == 3)
					set_board(i, j, 1);

				else if (live_nbs != 2)
					set_board(i, j, 0);

				else
					set_board(i, j, board(i, j));

				printf("%c", ((board(i, j) == 1) ? '@' : ' '));
			}
			printf("| %d", i);
			fflush(stdout);
		}
		b = !b;

		usleep(100000);
	}
}
