#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <cstdlib>

using namespace std;

const int activity[] = {1, 2, 3, 4, 5, 6, 7, 12, 13, 16, 17, 24};

const int N = 12;

const int infinity = 10000000;

vector<int>		voteA[N], voteB[N], voteC[N];

vector<double>	Pr;

vector<int>		result;

double	Magi[N];

int		M;


void Read_Vote()
{
	char	fileA[20], fileB[20], fileC[20];
	for (int i = 0; i < N; ++ i)
	{
		sprintf(fileA, "%d.vote", activity[i]);
		/*
		sprintf(fileB, "%d.voteB", activity[i]);
		sprintf(fileC, "%d.voteC", activity[i]);
		*/

		FILE	*fa = fopen(fileA, "r");
		/*
		FILE	*fb = fopen(fileB, "r");
		FILE	*fc = fopen(fileC, "r");
		*/

		if (!fa) // || !fb || !fc)
		{
			fprintf(stderr, "Vote File Error!\n");
			exit(-1);
		}
		int k;

		while (fscanf(fa, "%d", &k) != EOF)
			voteA[i].push_back(k);

		/*
		while (fscanf(fb, "%d", &k) != EOF)
			voteB[i].push_back(k);

		while (fscanf(fc, "%d", &k) != EOF)
			voteC[i].push_back(k);
		*/

		fclose(fa);// fclose(fb); fclose(fc);
	}
	
}

void Arbiter()
{
	int		confirmed, confused, unknown;

	M = voteA[0].size();

	for (int i = 0; i < N; ++ i)
		if (voteA[i].size() != M)
		{
			fprintf(stderr, "Amount of Votes Error!\n");
			exit(-1);
		}

	confirmed = confused = unknown = 0;

	vector <int>	candidate;

	result.clear();

	double pa = 100;// 120; //80;
	double pb = 0; // 20; //25;
	double pc = 0; // -80; //30;

	for (int i = 0; i < M; ++ i)
	{
		int winner = -1;

		for (int voter = 0; voter < N; ++ voter)
		{
			//Magi[voter] = pa * voteA[voter][i] + pb * voteB[voter][i] + pc * voteC[voter][i];
			Magi[voter] = (voteA[voter][i] == 1) ? voteA[voter][i] * Pr[voter] : -1;
			Magi[voter] = pa * Magi[voter];		// Bug!
			if (Magi[voter] > 0)
			{
				if (winner == -1 || Magi[voter] > Magi[winner])
				{
					candidate.clear();
					candidate.push_back(voter);
					winner = voter;
				}
				else if (winner != -1 && Magi[voter] == Magi[winner])
					candidate.push_back(voter);
			}
		}

		if (winner == -1)
		{
			++ unknown;
			result.push_back(0);
		}
		else if (candidate.size() == 1)
		{
			++ confirmed;
			result.push_back(activity[candidate[0]]);
		}
		else
		{
			++ confused;
			result.push_back(activity[candidate[rand() % candidate.size()]]);
		}
	}
	if (result.size() != M)
	{
		fprintf(stderr, "Result Size Error!\n");
		exit(-1);
	}

	printf("\n");
	printf("Arbiter says:\n");
	printf("\tConfirmed = %d, Confused = %d, Unknown = %d\n", confirmed, confused, unknown);
}

void Print(char *file)
{
	FILE	*f = fopen(file, "w");

	for (int i = 0; i < M; ++ i)
		fprintf(f, "%d\n", result[i]);

	fclose(f);
}

void Read_Pr(char *file)
{
	FILE	*f = fopen(file, "r");
	if (!f)
	{
		fprintf(stderr, "Pr File Error!\n");
		exit(-1);
	}

	Pr.clear();
	double k;
	for (int i = 0; i < N; ++ i)
	{
		fscanf(f, "%lf", &k);
		Pr.push_back(k);
	}
	fclose(f);
}


int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Input Error!\n");
		exit(-1);
	}

	srand(time(NULL));

	for (int i = 0; i < N; ++ i)
	{
		voteA[i].clear();
		voteB[i].clear();
		voteC[i].clear();
	}

	Read_Vote();

	Read_Pr(argv[1]);

	Arbiter();

	Print(argv[2]);

	return 0;
}
		/*
			if (Magi[voter] > UP)
			{
				candidate.clear();
				UP = Magi[voter];
				candidate.push_back(voter);
			}
			else if (Magi[voter] == UP)
			{
				candidate.push_back(voter);
			}
		}

		if (winner <= 0)
		{
			++ unknown;
			result.push_back(0);
		}
		else
		{
			if (candidate.size() == 1)
			{
				++ confirmed;
				result.push_back(activity[candidate[0]]);
			}
			else
			{
				++ confused;
				if (candidate.size() >= 3)
				{
					result.push_back(0);
				}
				else
				{
					result.push_back(activity[candidate[rand() & 1]]);
				}
			}
		}
		*/
