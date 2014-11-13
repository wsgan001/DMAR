#ifndef COMMON_H
#define COMMON_H

#include <vector>

using namespace std;

#define	unknown			-10000000
#define infinity		10000000

#define bin				5
#define HAAR			5

/*
	double		temp;
	double		Ix, Iy, Iz;
	double		IIx, IIy, IIz;
	double		Gx, Gy, Gz;
	double		Mx, My, Mz;
*/

struct RawDataType {
	//struct SensorType	S[3];
	double	sensor[3][13];
	int		heart;
	int		minute;
	int		second;
	int		label;
};

struct FeatureType {
	double		Xavg, Yavg, Zavg;
	double		Xvar, Yvar, Zvar;
	double		Xaad, Yaad, Zaad;
	double		Xtp, Ytp, Ztp;
	double		Xdst[bin], Ydst[bin], Zdst[bin];
	double		Xhaar[HAAR], Yhaar[HAAR], Zhaar[HAAR];
	double		ara;
};

struct DataType {
	int		label;
	double	HR_avg, HR_var, HR_maxmin, HR_headtail;

	double	T_avg[3], T_var[3], T_maxmin[3], T_tp[3], T_headtail[3];
	struct FeatureType	I[3], II[3], G[3], M[3];
};


double convert(char *);

double get_avg(vector<double> &);
double get_var(vector<double> &, double);
double get_aad(vector<double> &, double);
double get_tp(vector<double> &, vector<double> &);
double get_maxmin(vector<double> &);
double get_headtail(vector<double> &);
double get_ara(vector<double> &, vector<double> &, vector<double> &);
void get_dst(vector<double> &, double *);
void get_harr(const vector<double> &, double *);
struct FeatureType XYZ(vector<double> &, vector<double> &, vector<double> &, vector<double> &);
void Feature(const vector<struct RawDataType> &, vector<struct DataType> &, int, int);
struct RawDataType	Compressing(const vector<struct RawDataType> &, int, int);

void print(int &, const struct FeatureType &);
void FinalPrint(const vector<struct DataType> &);

#endif
