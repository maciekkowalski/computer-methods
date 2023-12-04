#include <iostream>

using namespace std;


//Deklaracje funkcji używanych w kodzie

void gauss(double matrix[4][4], int i, int j);
void swapRows(double matrix[4][4], double vector[4], int i1, int i2);
void Upper(double matrix[4][4], double x[4], double b[4]);
void Lower(double matrix[4][4], double y[4], double b[4]);
void printMatrix(double matrixA[4][4]);
void printVector(double vector[4]);


int main()
{

 // Deklaracja i inicjalizacja macierzy A oraz wektora b
	double A[4][4] = {  { 1,  -20,   30, -4 },
						{ 2,  -40,  -6,   50},
						{ 9,  -180,  11, -12},
						{-16,  15,  -140, 13} };

	double b[4] = { 35, 104, -366, -354 };

	double x[4], y[4];


	//Wyświetlanie macierzy A i wektora b
	printf("Macierz A:\n");
	printMatrix(A);

	printf("Wektor b:\n");
	printVector(b);


	// Pętla iterująca po kolumnach macierzy A w celu przeprowadzenia eliminacji Gaussa
	for (int i = 0; i < 4; i++)
	{

		if (A[i][i] == 0)
		{
			swapRows(A, b, i, i);
		}

		gauss(A, i, i);
	}

	// Wyświetlanie macierzy A po przeprowadzeniu rozkładu LU oraz wektora b
	printf("Macierz A po rozkladzie na LU:\n");
	printMatrix(A);

	printf("Wektor b po rozkladzie:\n");
	printVector(b);


	// Rozwiązywanie równania dla macierzy dolnotrójkątnej L
	Lower(A, y, b);	//rozwiazanie dla y

	printf("wektor y");
	printVector(y);


	// Rozwiązywanie równania dla macierzy górnotrójkątnej U
	Upper(A, x, y);	//rozwiazanie dla x

	printf("wektor x");
	printVector(x);


	return 0;
}


// Funkcja gauss przeprowadza eliminację Gaussa na macierzy, aby uzyskać macierz trójkątną górną. Działa w podanej kolumnie i1 i rzędzie j1
void gauss(double matrix[4][4], int i1, int j1)
{
	for (int i = i1 + 1; i < 4; i++)
	{
		/*	ponizsza linia kodu wyznacza macierz trojkatna dolna - L, wartosci tej macierzy to wspolczynniki,
			przez ktore mnozymy wiersze macierzy A, na przekatnej ma jedynki (ktore nie sa zapisywane w macierzy)*/
		matrix[i][j1] = matrix[i][j1] / matrix[i1][j1];
		for (int j = j1 + 1; j < 4; j++)
		{
			/*macierz trojkatna gorna, powstaje w wyniku eliminacji Gaussa*/
			matrix[i][j] = matrix[i][j] - matrix[i][j1] * matrix[i1][j];
		}
	}
}


// Funkcja swapRows zamienia miejscami wiersze w macierzy A i wektorze b. Wykorzystuje to w przypadku napotkania zera na przekątnej macierzy
void swapRows(double matrix[4][4], double vector[4], int i1, int i2)
{
	double tmp;
	int maxIdx = i1;

	/*znajduje najwieksza liczbe w kolumnie i2, zakladam ze bedzie rozna od 0*/
	for (int w = i1 + 1; w < 4; w++)
	{
		if (abs(matrix[w][i2]) > abs(matrix[maxIdx][i2]))
		{
			maxIdx = w;
		}
	}

	i1 = maxIdx;

	/*zamieniam wiersze macierzy A oraz wektora b*/
	for (int k = 0; k < 4; k++)
	{
		tmp = matrix[i1][k];
		matrix[i1][k] = matrix[i2][k];
		matrix[i2][k] = tmp;
	}

	tmp = vector[i1];
	vector[i1] = vector[i2];
	vector[i2] = tmp;
}


// Funkcja Upper rozwiązuje równanie dla macierzy górnotrójkątnej U i oblicza wektor x
void Upper(double matrix[4][4], double x[4], double y[4])
{
	/*	x[n] = y[n] / A[n][n]
		x[i] = y[i] - suma(A[i][j]*x[j]) dla j od i+1 do n*/
	x[3] = y[3] / matrix[3][3];
	for (int i = 2; i >= 0; i--)
	{
		double sum = 0.0;
		for (int j = i + 1; j < 4; j++)
		{
			sum += matrix[i][j] * x[j];
		}
		x[i] = (y[i] - sum) / matrix[i][i];
	}
}


// Funkcja Lower rozwiązuje równanie dla macierzy dolnotrójkątnej L i oblicza wektor y
void Lower(double matrix[4][4], double y[4], double b[4])
{
	/*	y[0] = b[0] / A[0][0]
		y[i] = b[i] - suma(y[k]*A[i][j]) dla j od 0 do i-1*/

	y[0] = b[0]; // /1
	for (int i = 1; i < 4; i++)
	{
		double suma = 0.0;
		for (int j = 0; j <= i - 1; j++)
		{
			suma += matrix[i][j] * y[j];
		}
		y[i] = (b[i] - suma);	// /1
	}
}



// Funkcja printMatrix wyświetla macierz w formacie tabelarycznym
void printMatrix(double matrix[4][4])
{
	int i = 0, j = 0;

	for (i = 0; i < 4; i++)
	{
		printf("|");
		for (j = 0; j < 4; j++)
		{
			printf("%10.4f ", matrix[i][j]);
		}
		printf("|\n");
	}
	printf("\n\n");
}


// Funkcja printVector wyświetla wektor w formacie [a b c d]
void printVector(double vector[4])
{
	int i = 0;

	printf("[ ");
	for (i = 0; i < 4; i++)
	{
		printf("%f ", vector[i]);
	}
	printf("]\n\n");
}


