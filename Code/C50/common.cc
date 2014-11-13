#include "common.h"
#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>

using namespace std;

const int		W = 3;
const double	H = 1.5;

vector<double> data;

vector<double> S;

vector<int> peak;

vector<int> candidate;

double convert(char *x)
{
	if (x[0] == '?') return unknown;
	double key;
	sscanf(x, "%lf", &key);
	return key;
}

double get_avg(vector<double> &x)
{
	double ret = 0;
	int total = 0;
	for (int i = 0; i < x.size(); ++ i)
		if (x[i] != unknown)
		{
			++ total;
			ret += x[i];
		}
	return ret / total;
}

double get_var(vector<double> &x, double avg)
{
	double ret = 0;
	int total = 0;
	for (int i = 0; i < x.size(); ++ i)
		if (x[i] != unknown)
		{
			++ total;
			ret += (x[i] - avg) * (x[i] - avg);
		}
	return sqrt(ret / total);		// standard deviation
}

double get_aad(vector<double> &x, double avg)
{
	double ret = 0;
	int total;
	for (int i = 0; i < x.size(); ++ i)
		if (x[i] != unknown)
		{
			++ total;
			ret += abs(x[i] - avg);
		}
	return ret / total;
}

double get_tp(vector<double> &x, vector<double> &time)
{
	data.clear();
	for (int i = 0; i < x.size(); ++ i)
		if (x[i] != unknown)
			data.push_back(i);
	if (data.empty()) return 0.00;

	S.clear();
	int N = data.size();
	for (int i = 0; i < N; ++ i)
	{
		int left = max(i - W, 0);
		int right = min(i + W, N - 1);
		double k = 0;
		for (int j = left; j < i; ++ j)
			k += x[data[j]];
		for (int j = i + 1; j <= right; ++ j)
			k += x[data[j]];
		S.push_back(x[data[i]] - k / (right - left));
	}
	double avg = get_avg(S);	
	double var = get_var(S, avg);

	candidate.clear();
	for (int i = 0; i < N; ++ i)
		if (S[i] > 0 && (S[i] - avg) > (H * var))
		{
			if (candidate.empty() || i - candidate[candidate.size() - 1] > W)
				candidate.push_back(i);
			else
			{
				if (x[data[i]] > x[data[candidate.size() - 1]])
				{
					candidate[candidate.size() - 1] = i;
				}
			}
		}

	if (candidate.size() == 1) return 0;

	double ret = 0;
	for (int i = 1; i < candidate.size(); ++ i)
		ret += time[data[candidate[i]]] - time[data[candidate[i - 1]]];
	/*
	for (int i = 0; i < data.size(); ++ i)
		fprintf(stderr, "%.3lf ", x[data[i]]);
	fprintf(stderr, "\n");
	*/
	fprintf(stderr, "TP: %d, %d, %.3lf\n", data.size(), candidate.size(), ret / (candidate.size() - 1));
	return ret / (candidate.size() - 1);
}

double get_maxmin(vector<double> &x)
{
	double u = -infinity;
	double l = infinity;
	for (int i = 0; i < x.size(); ++ i)
	{
		if (x[i] == unknown) continue;
		if (x[i] > u) u = x[i];
		if (x[i] < l) l = x[i];
	}
	return u - l;
}

double get_headtail(vector<double> &x)
{
	int head, tail;
	for (head = 0; head < x.size() && x[head] == unknown; ++ head);
	if (head == x.size()) return 0.00;
	for (tail = x.size() - 1; tail >= 0 && x[tail] == unknown; -- tail);
	return x[tail] - x[head];
}

double get_ara(vector<double> &x, vector<double> &y, vector<double> &z)
{
	double ret = 0;
	int total = 0;
	for (int i = 0; i < x.size(); ++ i)
		if (x[i] != unknown && y[i] != unknown && z[i] != unknown)
		{
			ret += sqrt(x[i] * x[i] + y[i] * y[i] + z[i] * z[i]);
			++ total;
		}
	return ret / total;
}

void get_dst(vector<double> &x, double *dst)
{
	double u = -infinity;
	double l = infinity;

	int Bins[bin];

	int total = 0;

	for (int i = 0; i < x.size(); ++ i)
	{
		if (x[i] == unknown) continue;
		if (x[i] > u) u = x[i];
		if (x[i] < l) l = x[i];
		++ total;
	}

	memset(Bins, 0, sizeof(Bins));

	double interval = (u - l) / bin;

	for (int i = 0; i < x.size(); ++ i)
	{
		if (x[i] == unknown) continue;
		if (x[i] == u)
			++ Bins[bin - 1];
		else
			++ Bins[int((x[i] - l) / interval)];
	}

	for (int i = 0; i < bin; ++ i)
		dst[i] = (double)Bins[i] / total;
}

struct FeatureType XYZ(vector<double> &x, vector<double> &y, vector<double> &z, vector<double> &time)
{
	struct FeatureType ret;

	ret.Xavg = get_avg(x);
	ret.Yavg = get_avg(y);
	ret.Zavg = get_avg(z);

	ret.Xvar = get_var(x, ret.Xavg);
	ret.Yvar = get_var(y, ret.Yavg);
	ret.Zvar = get_var(z, ret.Zavg);

	ret.Xaad = get_aad(x, ret.Xavg);
	ret.Yaad = get_aad(y, ret.Yavg);
	ret.Zaad = get_aad(z, ret.Zavg);

	ret.Xtp = get_tp(x, time);
	ret.Ytp = get_tp(y, time);
	ret.Ztp = get_tp(z, time);

	get_dst(x, ret.Xdst);
	get_dst(y, ret.Ydst);
	get_dst(z, ret.Zdst);

	ret.ara = get_ara(x, y, z);

	return ret;
}

void Feature(const vector<struct RawDataType> &Raw, vector<struct DataType> &Data, int left, int right)
{
	struct DataType		K;

	int counter, sum_int;
	double sum;

	// Label
	K.label = Raw[left].label;

	// Heart Rate
	//vector<double>		HR;
	//HR.clear();

	//for (int i = left; i <= right; ++ i)
	//	HR.push_back((double)Raw[i].heart);
	
	//K.HR_avg = get_avg(HR);
	//K.HR_var = get_var(HR, K.HR_avg);
	//K.HR_maxmin = get_maxmin(HR);
	//K.HR_headtail = get_headtail(HR);

	vector<double>		X, Y, Z, Time;

	Time.clear();
	for (int i = left; i <= right; ++ i)
		Time.push_back(Raw[i].minute * 100 + Raw[i].second);

	for (int sensor = 0; sensor < 3; ++ sensor)
	{
		// Temperature
		//X.clear();
		//for (int i = left; i <= right; ++ i)
		//	X.push_back(Raw[i].sensor[sensor][0]);

		//K.T_avg[sensor] = get_avg(X);
		//K.T_var[sensor] = get_var(X, K.T_avg[sensor]);
		//K.T_maxmin[sensor] = get_maxmin(X);
		//K.T_tp[sensor] = get_tp(X, Time);
		//K.T_headtail[sensor] = get_headtail(X);

		// I
		X.clear(); Y.clear(); Z.clear();
		for (int i = left; i <= right; ++ i)
		{
			X.push_back(Raw[i].sensor[sensor][1]);
			Y.push_back(Raw[i].sensor[sensor][2]);
			Z.push_back(Raw[i].sensor[sensor][3]);
		}
		K.I[sensor] = XYZ(X, Y, Z, Time);

		// II
		X.clear(); Y.clear(); Z.clear();
		for (int i = left; i <= right; ++ i)
		{
			X.push_back(Raw[i].sensor[sensor][4]);
			Y.push_back(Raw[i].sensor[sensor][5]);
			Z.push_back(Raw[i].sensor[sensor][6]);
		}
		K.II[sensor] = XYZ(X, Y, Z, Time);

		// G
		X.clear(); Y.clear(); Z.clear();
		for (int i = left; i <= right; ++ i)
		{
			X.push_back(Raw[i].sensor[sensor][7]);
			Y.push_back(Raw[i].sensor[sensor][8]);
			Z.push_back(Raw[i].sensor[sensor][9]);
		}
		K.G[sensor] = XYZ(X, Y, Z, Time);

		// M
		X.clear(); Y.clear(); Z.clear();
		for (int i = left; i <= right; ++ i)
		{
			X.push_back(Raw[i].sensor[sensor][10]);
			Y.push_back(Raw[i].sensor[sensor][11]);
			Z.push_back(Raw[i].sensor[sensor][12]);
		}
		K.M[sensor] = XYZ(X, Y, Z, Time);
	}
					
	Data.push_back(K);
}


void print(int &index, const struct FeatureType &x)
{
	printf(" %d:%.4lf", ++ index, x.Xavg);
	printf(" %d:%.4lf", ++ index, x.Yavg);
	printf(" %d:%.4lf", ++ index, x.Zavg);

	printf(" %d:%.4lf", ++ index, x.Xvar);
	printf(" %d:%.4lf", ++ index, x.Yvar);
	printf(" %d:%.4lf", ++ index, x.Zvar);

	printf(" %d:%.4lf", ++ index, x.Xaad);
	printf(" %d:%.4lf", ++ index, x.Yaad);
	printf(" %d:%.4lf", ++ index, x.Zaad);

	for (int i = 0; i < bin; ++ i)
		printf(" %d:%.4lf", ++ index, x.Xdst[i]);

	for (int i = 0; i < bin; ++ i)
		printf(" %d:%.4lf", ++ index, x.Ydst[i]);

	for (int i = 0; i < bin; ++ i)
		printf(" %d:%.4lf", ++ index, x.Zdst[i]);

	printf(" %d:%.4lf", ++ index, x.Xtp);
	printf(" %d:%.4lf", ++ index, x.Ytp);
	printf(" %d:%.4lf", ++ index, x.Ztp);

	printf(" %d:%.4lf", ++ index, x.ara);
}

void FinalPrint(const vector<struct DataType> &Data)
{
	for (int i = 0; i < Data.size(); ++ i)
	{
		int index = 0;
		printf("%d", Data[i].label);
		//printf(" %d:%.4lf", ++ index, Data[i].HR_avg);
		printf(" %d:%.4lf", ++ index, Data[i].HR_headtail);
		//printf(" %d:%.4lf", ++ index, Data[i].HR_var);
		//printf(" %d:%.4lf", ++ index, Data[i].HR_maxmin);
		for (int s = 0; s < 3; ++ s)
		{
			//printf(" %d:%.4lf", ++ index, Data[i].T_avg[s]);
			printf(" %d:%.4lf", ++ index, Data[i].T_headtail[s]);
			//printf(" %d:%.4lf", ++ index, Data[i].T_var[s]);
			//printf(" %d:%.4lf", ++ index, Data[i].T_maxmin[s]);
			//printf(" %d:%.4lf", ++ index, Data[i].T_tp[s]);

			print(index, Data[i].I[s]);
			print(index, Data[i].II[s]);
			print(index, Data[i].G[s]);
			print(index, Data[i].M[s]);
		}
		printf("\n");
	}
}

struct RawDataType	Compressing(const vector<struct RawDataType> &Raw, int left, int right)
{
	struct RawDataType	ret;

	ret.label = Raw[left].label;

	double total;
	int k;

	int Cheart, Csensor[3][13];
	double Sheart, Ssensor[3][13];

	Sheart = Cheart = 0;
	memset(Csensor, 0, sizeof(Csensor));
	memset(Ssensor, 0, sizeof(Ssensor));

	for (int i = left; i <= right; ++ i)
	{
		if (Raw[i].heart != unknown)
		{
			++ Cheart;
			Sheart += Raw[i].heart;
		}
		for (int j = 0; j < 3; ++ j)
			for (int k = 0; k < 13; ++ k)
				if (Raw[i].sensor[j][k] != unknown)
				{
					++ Csensor[j][k];
					Ssensor[j][k] += Raw[i].sensor[j][k];
				}
	}

	ret.minute = Raw[(left + right) >> 1].minute;
	ret.second = Raw[(left + right) >> 1].second;

	if (Cheart == 0)
	{
		ret.heart = unknown;
		//fprintf(stderr, "Amazing! Heart (%d, %d)\n", left, right);
	}
	else
		ret.heart = Sheart / (double)Cheart;

	for (int i = 0; i < 3; ++ i)
		for (int j = 0 ; j < 13; ++ j)
		{
			if (Csensor[i][j] == 0)
			{
				//fprintf(stderr, "Amazing! Sensors (%d, %d)\n", left, right);
				ret.sensor[i][j] = unknown;
			}
			else
				ret.sensor[i][j] = Ssensor[i][j] / (double)Csensor[i][j];
		}

	return ret;
}
