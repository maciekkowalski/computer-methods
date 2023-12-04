#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;


#define _USE_MATH_DEFINES
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.78539816339744830962


template <class typeT> typeT fx(typeT x)
{
	return sin(x);
}

// Definijemy szablony funkcji przybliżających pochodne 
// Każda z tych funkcji przyjmuje odpowiednie argumenty i oblicza przybliżoną wartość pochodnej na podstawie różnicowych wzorów dwupunktowych i trzypunktowych.
template <class typeT> typeT derivative(typeT x)
{
	return cos(x);
}

template <class typeT> typeT roznica_progresywna2p(typeT x1, typeT x2, typeT h)
{
	return (fx(x2) - fx(x1)) / h;
}

template <class typeT> typeT roznica_progresywna3p(typeT x0, typeT x1, typeT x2, typeT h)
{
	return (-fx(x2) + static_cast <typeT>(4.0) * fx(x1) - static_cast <typeT>(3.0) * fx(x0)) / static_cast<typeT>(2.0) / h;
}

template <class typeT> typeT roznica_wsteczna2p(typeT x0, typeT x1, typeT h)
{
	return (fx(x1) - fx(x0)) / h;
}

template <class typeT> typeT roznica_wsteczna3p(typeT x0, typeT x1, typeT x2, typeT h)
{
	return (static_cast <typeT>(3.0) * fx(x2) - static_cast <typeT>(4.0) * fx(x1) + fx(x0)) / static_cast<typeT>(2.0) / h;
}

template <class typeT> typeT roznica_centralna2p(typeT x0, typeT x2, typeT h)
{
	return (fx(x2) - fx(x0)) / static_cast<typeT>(2.0) / h;
}


// szablon funkcji calcEps, która oblicza najmniejszą wartość epislon która może być reprezentowana na danym typie zmiennoprzecinkowym
template <class typeT>typeT calcEps()
{
	typeT eps = static_cast <typeT> (0.5);
	typeT eps2 = eps;
	typeT one = static_cast <typeT> (1.0);
	typeT two = static_cast <typeT> (2.0);

	while (1 + eps2 > 1)
	{
		eps = eps2;
		eps2 /= 2;
	}

	return eps;
}

// funkcja writeData wykonuje główną część obliczeń i zapisuje wyniki do pliku tekstowego
template <class typeT> int writeData(ofstream& f_out)
{
	typeT h = static_cast <typeT> (0.1);
	typeT epsilon = calcEps<typeT>();
	typeT ten = static_cast <typeT> (1.1);
	typeT data[9];
	typeT a = static_cast <typeT>(0);
	typeT b = static_cast <typeT>(M_PI_2);
	typeT mid = static_cast <typeT>(M_PI_4);

	f_out.open("out.txt", ios::out);
	if (!f_out.good())
		return 1;
	f_out << scientific << setprecision(6);

	f_out << "log10(h) " << "roznica_progresywna2p " << "roznica_progresywna3p " <<
		"roznica_progresywna_centr " << "roznica_progresywna_centr2 " << "roznica_centralna2p " << "roznica_wsteczna2p " << "roznica_wsteczna3p " <<
	
	"roznica_wsteczna2ps " << "roznica_wsteczna3ps" << endl;

	// funkcja wykonuje pętlę, obliczając pochodne numeryczne dla różnych wartości h

	// Obliczamy logarytm dziesiętny z błędu bezwzględnego dla każdej pochodnej numerycznej i zapisujemy wyniki do pliku

	// Zmniejszamy wartość h i kontynuujemy pętlę, aż wartość h będzie mniejsza niż epsilon

	while (h > epsilon)
	{
		data[0] = log10(abs(derivative(a) - roznica_progresywna2p(a, a + h, h)));
		data[1] = log10(abs(derivative(a) - roznica_progresywna3p(a, a + h, a + h + h, h)));

		data[2] = log10(abs(derivative(mid) - roznica_progresywna2p(mid, mid + h, h)));
		data[3] = log10(abs(derivative(mid) - roznica_progresywna3p(mid - h, mid, mid + h, h)));
		data[4] = log10(abs(derivative(mid) - roznica_centralna2p(mid - h, mid + h, h)));
		data[5] = log10(abs(derivative(mid) - roznica_wsteczna2p(mid - h, mid, h)));
		data[6] = log10(abs(derivative(mid) - roznica_wsteczna3p(mid - h, mid, mid + h, h)));

		data[7] = log10(abs(derivative(b) - roznica_wsteczna2p(b - h, b, h)));
		data[8] = log10(abs(derivative(b) - roznica_wsteczna3p(b - h - h, b - h, b, h)));

		f_out << log10(h) << " ";
		for (int i = 0; i < 9; i++)
		{
			f_out << data[i] << " ";
		}
		f_out << endl;

		h /= ten;
	}
}


int main()
{
	ofstream f_out;

	writeData<double>(f_out);

	// Uruchamiamy program gnuplot za pomocą polecenia systemowego, aby wygenerować wykres na podstawie danych z pliku "out.txt"

	system("gnuplot -p -e \"set ylabel 'blad bezwzgledny'; set xlabel 'log10(h)'; plot for [col=2:10] 'out.txt' using 1:col title columnheader w l\"");

	return 0;
}

