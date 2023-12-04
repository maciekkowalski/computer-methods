#include <iostream>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <limits>

using namespace std;

int maxN = 5000;	//maksymalna liczba wezlow

double* t;
double* analyticSolution;
double* directEulerSolution, * indirectEulerSolution, * trapesiumSolution;
double* directEulerError, * indirectEulerError, * trapesiumError;

double analyticFunction(double t);
double normMax(double* vector, int n);
void createNodes(double* nodes, double x0, double h, double n);

void directEulerMethod(double* y, double* error, double* nodes, double h, int n);
void indirectEulerMethod(double* y, double* error, double* nodes, double h, int n);
void trapesiumMethod(double* y, double* error, double* nodes, double h, int n);

void chart1(int N);		//PME i analityczna od t
void chart2(int N);		//PMT i analityczna od t
void chart3(int N);		//BME i analityczna od t
void chart4(int N, double tp, double tk, double h);	//log10(blad) od log10(dt), gdzie dt=h

void init(int N, double tp, double tk, double h);
void freeMemory();


int main()
{
	int N = 101;
	double tp = 0.0, tk = 0.5;
	double h = (tk - tp) / (N - 1);  //h=0.005

	init(N, tp, tk, h);

	chart1(N);		//PME
	chart2(N);		//PMT
	chart3(N);		//BME stabilna
	/*BME jest stabilna dla h<0.1 (obliczone w zadaniu nr 1)*/

	N = 41;
	tp = 0.0, tk = 10;
	h = (tk - tp) / (N - 1);  //h=0.25

	freeMemory();
	init(N, tp, tk, h);

	chart3(N);		//BME niestabilna

	N = 11;
	tp = 0.0, tk = 0.5;
	h = (tk - tp) / (N - 1);  //h=0.05 

	chart4(N, tp, tk, h);	//log10(blad) od log10(dt), gdzie dt=h

	freeMemory();

	return 0;
}



double analyticFunction(double t)
{
	return 1 - exp(-10 * (t + atan(t)));
}

double normMax(double* vector, int n)
{
	double max = abs(vector[0]);

	for (int i = 1; i < n; i++)
	{
		if (abs(vector[i]) > max)
			max = abs(vector[i]);
	}

	return max;
}

void createNodes(double* nodes, double x0, double h, double n)
{
	for (int i = 0; i < n; i++)
	{
		nodes[i] = x0 + h * i;
	}
}


void directEulerMethod(double* y, double* error, double* nodes, double h, int n)
{
	y[0] = 0.0;
	error[0] = abs(analyticFunction(nodes[0]) - y[0]);

	for (int i = 1; i < n; i++)
	{
		y[i] = y[i - 1] - h * (10.0 * nodes[i - 1] * nodes[i - 1] + 20.0) / (nodes[i - 1] * nodes[i - 1] + 1) * (y[i - 1] - 1);

		error[i] = abs(analyticFunction(nodes[i]) - y[i]);
	}
}

void indirectEulerMethod(double* y, double* error, double* nodes, double h, int n)
{
	double tmp;

	y[0] = 0.0;
	error[0] = abs(analyticFunction(nodes[0]) - y[0]);

	for (int i = 1; i < n; i++)
	{
		tmp = (10.0 * nodes[i] * nodes[i] + 20.0) / (nodes[i] * nodes[i] + 1.0) * h;
		y[i] = (y[i - 1] + tmp) / (1.0 + tmp);

		error[i] = abs(analyticFunction(nodes[i]) - y[i]);
	}
}

void trapesiumMethod(double* y, double* error, double* nodes, double h, int n)
{
	double tmp1;
	double tmp2;

	y[0] = 0.0;
	error[0] = abs(analyticFunction(nodes[0]) - y[0]);

	for (int i = 1; i < n; i++)
	{
		tmp1 = (10.0 * nodes[i - 1] * nodes[i - 1] + 20.0) / (nodes[i - 1] * nodes[i - 1] + 1.0) * h;
		tmp2 = (10.0 * nodes[i] * nodes[i] + 20.0) / (nodes[i] * nodes[i] + 1.0) * h;
		y[i] = (2.0 * y[i - 1] - tmp1 * (y[i - 1] - 1.0) + tmp2) / (2.0 + tmp2);

		error[i] = abs(analyticFunction(nodes[i]) - y[i]);
	}
}

void chart1(int N)
{
	ofstream chart_1;
	chart_1.open("chart_1.txt", ios::out);
	if (!chart_1.good())
		exit(1);
	chart_1 << scientific << setprecision(12);
	chart_1 << "t " << "analityczna " << "PME " << endl;

	for (int i = 0; i < N; i++)
	{
		chart_1 << t[i] << " " << analyticSolution[i] << " " << indirectEulerSolution[i] << endl;
	}
	chart_1.close();

	system("gnuplot -p -e \"set ylabel 'y'; set xlabel 't'; plot 'chart_1.txt' u 1:2 title columnheader w l, '' u 1:3 title columnheader\"");
}

void chart2(int N)
{
	ofstream chart_2;
	chart_2.open("chart_2.txt", ios::out);
	if (!chart_2.good())
		exit(1);
	chart_2 << scientific << setprecision(12);
	chart_2 << "t " << "analityczna " << "PMT " << endl;

	for (int i = 0; i < N; i++)
	{
		chart_2 << t[i] << " " << analyticSolution[i] << " " << trapesiumSolution[i] << endl;
	}
	chart_2.close();

	system("gnuplot -p -e \"set ylabel 'y'; set xlabel 't'; plot 'chart_2.txt' u 1:2 title columnheader w l, '' u 1:3 title columnheader\"");
}

void chart3(int N)
{
	ofstream chart_3;
	chart_3.open("chart_3.txt", ios::out);
	if (!chart_3.good())
		exit(1);
	chart_3 << scientific << setprecision(12);
	chart_3 << "t " << "analityczna " << "BME " << endl;

	for (int i = 0; i < N; i++)
	{
		chart_3 << t[i] << " " << analyticSolution[i] << " " << directEulerSolution[i] << endl;
	}
	chart_3.close();

	system("gnuplot -p -e \"set ylabel 'y'; set xlabel 't'; plot 'chart_3.txt' u 1:2 title columnheader w l, '' u 1:3 title columnheader\"");
}

void chart4(int N, double tp, double tk, double h)
{
	ofstream chart_4;
	chart_4.open("chart_4.txt", ios::out);
	if (!chart_4.good())
		exit(1);
	chart_4 << scientific << setprecision(8);
	chart_4 << "log10(dt) " << "blad(BME) " << "blad(PME) " << "blad(PMT) " << endl;

	while (h > numeric_limits<double>::epsilon() && N <= maxN)
	{
		h = (tk - tp) / (N - 1);

		freeMemory();
		init(N, tp, tk, h);

		chart_4 << log10(h) << " " << log10(normMax(directEulerError, N)) << " " << log10(normMax(indirectEulerError, N)) << " " << log10(normMax(trapesiumError, N)) << endl;

		N += 100;
	}
	chart_4.close();

	system("gnuplot -p -e \"set ylabel 'blad bezwzgledny'; set xlabel 'log10(dt)'; plot for [col=2:4] 'chart_4.txt' using 1:col title columnheader w l\"");
}

void init(int N, double tp, double tk, double h)
{
	t = new double[N];

	analyticSolution = new double[N];
	directEulerSolution = new double[N];
	indirectEulerSolution = new double[N];
	trapesiumSolution = new double[N];

	directEulerError = new double[N];
	indirectEulerError = new double[N];
	trapesiumError = new double[N];

	createNodes(t, tp, h, N);

	for (int i = 0; i < N; i++)
	{
		analyticSolution[i] = analyticFunction(t[i]);
	}
	directEulerMethod(directEulerSolution, directEulerError, t, h, N);
	indirectEulerMethod(indirectEulerSolution, indirectEulerError, t, h, N);
	trapesiumMethod(trapesiumSolution, trapesiumError, t, h, N);
}

void freeMemory()
{
	delete[]t;
	delete[]analyticSolution;
	delete[]directEulerSolution;
	delete[]indirectEulerSolution;
	delete[]trapesiumSolution;
	delete[]directEulerError;
	delete[]indirectEulerError;
	delete[]trapesiumError;
}