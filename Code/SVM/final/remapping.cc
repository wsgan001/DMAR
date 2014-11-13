#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;

vector<int> range;

vector<int>		vote;

vector<int>		result;

int N;

void Read_Range(char *range_file)
{
	FILE *f = fopen(range_file, "r");
	if (!f)
	{
		fprintf(stderr, "Range File Error!\n");
		exit(-1);
	}

	int k;
	range.clear();

	while (fscanf(f, "%d", &k) != EOF)
		range.push_back(k);	

	fclose(f);
}

void Read_Vote(char *vote_file)
{
	FILE	*f = fopen(vote_file, "r");

	if (!f)
	{
		fprintf(stderr, "Vote File Error!\n");
		exit(-1);
	}
	int k;

	while (fscanf(f, "%d", &k) != EOF)
		vote.push_back(k);

	fclose(f);

}

void Remapping(char *file)
{
	FILE	*f = fopen(file, "w");

	int left, right;
	left = 0;
	for (int i = 0; i < N; ++ i)
	{
		right = range[i];

		for (int j = left; j < right; ++ j)
			fprintf(f, "%d\n", vote[i]);

		left = right;
	}

	/*
	for (int i = left; i < range[N]; ++ i)
		fprintf(f, "%d\n", vote[N - 1]);
	*/

	fclose(f);
}

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		fprintf(stderr, "Input Error!\n");
		exit(-1);
	}

	Read_Range(argv[1]);

	vote.clear();

	Read_Vote(argv[2]);

	if (range.size() != vote.size())
	{
		fprintf(stderr, "Range Size != Vote Size\n");
		exit(-1);
	}

	N = vote.size();

	Remapping(argv[3]);

	return 0;
}

