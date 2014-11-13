#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include "common.h"

using namespace std;


vector<struct RawDataType>		Raw;
vector<struct DataType>		Data;

FILE	*range_file;

int		interval;

int		alpha;

void DataProcessing();

void Read(char *feature_file)
{
	FILE *f= fopen(feature_file, "r");
	if (!f)
	{
		fprintf(stderr, "[Error!] Input Error: Feature file!\n");
		exit(-1);
	}
	char	temp[41][20]; // 0 --> time, 40 --> heart_rate, 1~39 --> sensors

	struct RawDataType		K;

	bool	part = false;

	int		last = -1;

	while (fscanf(f, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", temp[0], temp[40], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7], temp[8], temp[9], temp[10], temp[11], temp[12], temp[13], temp[14], temp[15], temp[16], temp[17], temp[18], temp[19], temp[20], temp[21], temp[22], temp[23], temp[24], temp[25], temp[26], temp[27], temp[28], temp[29], temp[30], temp[31], temp[32], temp[33], temp[34], temp[35], temp[36], temp[37], temp[38], temp[39]) != EOF && (!part || Raw.size() < 1000))
	{
		K.label = 1;

		sscanf(temp[0], "%d.%d", &K.minute, &K.second);

		if (temp[40][0] == '?')
			K.heart = unknown;
		else
			sscanf(temp[40], "%d", &K.heart);

		for (int s = 0; s < 3; ++ s)
			for (int item = 0; item < 13; ++ item)
				K.sensor[s][item] = convert(temp[s * 13 + item + 1]);
		Raw.push_back(K);
	}

	DataProcessing();
	Raw.clear();

	fclose(f);
}


vector<struct RawDataType>	Compress;

void DataProcessing()
{
	// Phase I
	Compress.clear();

	int N = Raw.size();

	for (int i = 0; i < N; i += alpha)
	{
		if (i + alpha >= N)
		{
			Compress.push_back(Compressing(Raw, i, N - 1));
			break;
		}
		Compress.push_back(Compressing(Raw, i, i + alpha - 1));
	}

	// Phase II
	int M = Compress.size();
	for (int i = 0; i < M; i += interval)
	{
		if (i + interval >= M)
		{
			Feature(Compress, Data, i, M - 1);
			break;
		}
		fprintf(range_file, "%d\n", (i + interval) * alpha);
		Feature(Compress, Data, i, i + interval - 1);
	}

	fprintf(range_file, "%d\n", N); // NNNNN
}

int main(int argc, char **argv)	// feature + interval + range_file + alpha
{
	if (argc != 5)
	{
		fprintf(stderr, "[Error!] Input Error!\n");
		exit(-1);
	}

	sscanf(argv[2], "%d", &interval);
	sscanf(argv[4], "%d", &alpha);

	Data.clear();

	range_file = fopen(argv[3], "w");	

	Read(argv[1]);

	FinalPrint(Data);

	return 0;
}
