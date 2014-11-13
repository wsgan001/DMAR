#include "common.h"
#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>

using namespace std;

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
	for (int i = 0; i < x.size(); ++ i)
		ret += x[i];
	return ret / x.size();
}

double get_var(vector<double> &x, double avg)
{
	double ret = 0;
	for (int i = 0; i < x.size(); ++ i)
		ret += (x[i] - avg) * (x[i] - avg);
	return sqrt(ret / x.size());		// standard deviation
}

double get_aad(vector<double> &x, double avg)
{
	double ret = 0;
	for (int i = 0; i < x.size(); ++ i)
		ret += abs(x[i] - avg);
	return ret / x.size();
}

double get_tp(vector<double> &x, vector<double> &time)
{

	/*
	int maxptr, minptr;
	maxptr = minptr = -1;
	for (int i = 0; i < x.size(); ++ i)
	{
		if (x[i] == unknown) continue;
		if (maxptr == -1 || x[i] > x[maxptr])
			maxptr = i;
		if (minptr == -1 || x[i] < x[minptr])
			minptr = i;
	}
	if (maxptr ==minptr)
	{
		fprintf(stderr, "+ %d %d\n", maxptr, minptr);
	for (int i = 0; i < x.size(); ++ i)
		fprintf(stderr, "%.3lf ", x[i]);
	fprintf(stderr, "\n");
	}
	return abs(time[maxptr] - time[minptr]) / 100.00;
	*/
}

double get_maxmin(vector<double> &x)
{
	double u = -infinity;
	double l = infinity;
	for (int i = 0; i < x.size(); ++ i)
	{
		if (x[i] > u) u = x[i];
		if (x[i] < l) l = x[i];
	}
	return u - l;
}

double get_headtail(vector<double> &x)
{
	return x[x.size() - 1] - x[0];
}

double get_ara(vector<double> &x, vector<double> &y, vector<double> &z)
{
	double ret = 0;
	for (int i = 0; i < x.size(); ++ i)
		ret += sqrt(x[i] * x[i] + y[i] * y[i] + z[i] * z[i]);
	return ret / x.size();
}

void get_dst(vector<double> &x, double *dst)
{
	double u = -infinity;
	double l = infinity;

	int Bins[bin];

	for (int i = 0; i < x.size(); ++ i)
	{
		if (x[i] > u) u = x[i];
		if (x[i] < l) l = x[i];
	}

	memset(Bins, 0, sizeof(Bins));

	double interval = (u - l) / bin;

	for (int i = 0; i < x.size(); ++ i)
	{
		if (x[i] == u)
			++ Bins[bin - 1];
		else
			++ Bins[int((x[i] - l) / interval)];
	}

	for (int i = 0; i < bin; ++ i)
		dst[i] = (double)Bins[i] / x.size();
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

	//ret.Xtp = get_tp(x, time);
	//ret.Ytp = get_tp(y, time);
	//ret.Ztp = get_tp(z, time);

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
	vector<double>		HR;
	HR.clear();

	for (int i = left; i <= right; ++ i)
		HR.push_back((double)Raw[i].heart);
	
	K.HR_avg = get_avg(HR);
	//K.HR_var = get_var(HR, K.HR_avg);
	K.HR_maxmin = get_maxmin(HR);
	K.HR_headtail = get_headtail(HR);

	vector<double>		X, Y, Z, Time;

	Time.clear();
	for (int i = left; i <= right; ++ i)
		Time.push_back(Raw[i].minute * 100 + Raw[i].second);

	for (int sensor = 0; sensor < 3; ++ sensor)
	{
		// Temperature
		X.clear();
		for (int i = left; i <= right; ++ i)
			X.push_back(Raw[i].sensor[sensor][0]);

		K.T_avg[sensor] = get_avg(X);
		//K.T_var[sensor] = get_var(X, K.T_avg[sensor]);
		K.T_maxmin[sensor] = get_maxmin(X);
		//K.T_tp[sensor] = get_tp(X, Time);
		K.T_headtail[sensor] = get_headtail(X);

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

	//printf(" %d:%.4lf", ++ index, x.Xtp);
	//printf(" %d:%.4lf", ++ index, x.Ytp);
	//printf(" %d:%.4lf", ++ index, x.Ztp);

	printf(" %d:%.4lf", ++ index, x.ara);
}

void FinalPrint(const vector<struct DataType> &Data)
{
	for (int i = 0; i < Data.size(); ++ i)
	{
		int index = 0;
		printf("1");
		printf(" %d:%.4lf", ++ index, Data[i].HR_avg);
		printf(" %d:%.4lf", ++ index, Data[i].HR_headtail);
		//printf(" %d:%.4lf", ++ index, Data[i].HR_var);
		printf(" %d:%.4lf", ++ index, Data[i].HR_maxmin);
		for (int s = 0; s < 3; ++ s)
		{
			printf(" %d:%.4lf", ++ index, Data[i].T_avg[s]);
			printf(" %d:%.4lf", ++ index, Data[i].T_headtail[s]);
			//printf(" %d:%.4lf", ++ index, Data[i].T_var[s]);
			printf(" %d:%.4lf", ++ index, Data[i].T_maxmin[s]);
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

	ret.heart = Sheart / (double)Cheart;

	for (int i = 0; i < 3; ++ i)
		for (int j = 0 ; j < 13; ++ j)
			ret.sensor[i][j] = Ssensor[i][j] / (double)Csensor[i][j];

	ret.minute = ret.second = 0;

	return ret;
}
