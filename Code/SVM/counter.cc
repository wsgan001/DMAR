#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;

const int	N = 25;

int		tp[N], fp[N], fn[N], tn[N];

bool	flag[N];

int		FLAG;

int main(int argc, char **argv)		// predicted, standard
{
	if (argc != 4)
	{
		fprintf(stderr, "Input Error!\n");
		exit(-1);
	}

	sscanf(argv[3], "%d", &FLAG);

	FILE	*A = fopen(argv[1], "r");
	FILE	*B = fopen(argv[2], "r");

	memset(tp, 0, sizeof(tp));
	memset(fp, 0, sizeof(fp));
	memset(tn, 0, sizeof(tn));
	memset(fn, 0, sizeof(fn));
	memset(flag, false, sizeof(flag));

	int total = 0;
	int same = 0;
	int a, b;
	while (fscanf(A, "%d", &a) != EOF && fscanf(B, "%d", &b) != EOF)
	{
		++ total;
		if (a == b) ++ same;

		flag[a] = flag[b] = true;

		if (a == b)
		{
			if (a != 0)
			{
				++ tp[a];
			}
			else
			{
				++ tn[a];
			}
		}
		else
		{
			if (a == 0)
			{
				++ fn[b];
			}
			else
			{
				++ fp[a];
				if (b != 0) ++ fn[b];
			}
		}
	}

	double totalF = 0;

	for (int i = 1; i < N; ++ i)
		if (flag[i] && (FLAG == -1 || i == FLAG))
		{
			double P = (double)tp[i] / (tp[i] + fp[i]);
			double R = (double)tp[i] / (tp[i] + fn[i]);
			double F = 2 * P * R / (P + R);
			totalF += F;
			printf("%d: Precision = %d/%d = %.3lf%%, Recall = %d/%d = %.3lf%%, F = %.3lf\n", i, tp[i], tp[i] + fp[i], P * 100, tp[i], tp[i] + fn[i], R * 100, F);
		}

	printf("Accuracy = %d/%d, %.3lf%%\n", same, total, 100.00 * double(same) / double(total));
	printf("Sum F = %.3lf\n", totalF);

	fclose(A); fclose(B);

	return 0;
}
