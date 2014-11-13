#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include "common.h"

using namespace std;


int Label;

vector<struct RawDataType>		Raw;
vector<struct DataType>		Data;

int		interval;

void DataProcessing();

void Read(char *feature_file, char *label_file)
{
	FILE *f= fopen(feature_file, "r");
	if (!f)
	{
		fprintf(stderr, "[Error!] Input Error: Feature file!\n");
		exit(-1);
	}
	FILE *l= fopen(label_file, "r");
	if (!l)
	{
		fprintf(stderr, "[Error!] Input Error: Label file!\n");
		exit(-1);
	}

	char	temp[41][20]; // 0 --> time, 40 --> heart_rate, 1~39 --> sensors

	struct RawDataType		K;

	bool	part = false;

	int		last = -1;

	while (fscanf(f, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", temp[0], temp[40], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7], temp[8], temp[9], temp[10], temp[11], temp[12], temp[13], temp[14], temp[15], temp[16], temp[17], temp[18], temp[19], temp[20], temp[21], temp[22], temp[23], temp[24], temp[25], temp[26], temp[27], temp[28], temp[29], temp[30], temp[31], temp[32], temp[33], temp[34], temp[35], temp[36], temp[37], temp[38], temp[39]) != EOF && (!part || Raw.size() < 1000))
	{
		fscanf(l, "%d", &K.label);
		if (K.label != last)
		{
			if (Raw.size() != 0)
			{
				DataProcessing();
				Raw.clear();
			}
			last = K.label;
		}
		if (K.label != Label) continue;

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

	fclose(l);
	fclose(f);
}


vector<struct RawDataType>	Compress;


void DataProcessing()
{
	// Phase I
	Compress.clear();

	int N = Raw.size();

	for (int i = 0; i < N; i += 100)
	{
		if (i + 100 >= N)
		{
			if (N - i >= 20)
				Compress.push_back(Compressing(Raw, i, N - 1));
			break;
		}
		Compress.push_back(Compressing(Raw, i, i + 99));
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
		Feature(Compress, Data, i, i + interval - 1);
	}
}

int main(int argc, char **argv)	// feature_file + label_file + Label + interval
{
	if (argc != 5)
	{
		fprintf(stderr, "[Error!] Input Error!\n");
		exit(-1);
	}

	sscanf(argv[3], "%d", &Label);

	sscanf(argv[4], "%d", &interval);

	Data.clear();

	Read(argv[1], argv[2]);
	
	FinalPrint(Data);
	
	return 0;
}
