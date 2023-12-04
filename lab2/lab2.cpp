#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>
#include <cmath>

using namespace std;

#pragma warning (disable : 4996)

/*  Implementumey wzór Taylora, który pozwoli nam na obliczenie
    przybliżonej wartości funkcji matematycznej w określonym punkcie. 
*/

double taylor(double x, int n)
{
	int i;
	double w = 1.0;
	double wynik = 1.0;

	/*
	  Funkcja taylor oblicza przybliżoną wartość funkcji dla danego punktu x, używając
	  szeregu Taylora z liczbą wyrazów określoną przez parametr n. W każdej iteracji
	  pętli for, funkcja oblicza kolejny wyraz szeregu i dodaje go do dotychczasowego
	  wyniku. Wyraz szeregu jest obliczany na podstawie poprzedniego wyrazu, mnożąc
	  go przez -x/(i+1)
	*/

	for (i = 1; i < n; i++)
	{
		w *= -x / ((double) i + 1.0);
		wynik += w;
	}
	return wynik;
}

double funkcja(double x) {
	return (1.0 - exp(-x)) / x;
}

int main()
{
	double x, logx, wart_dokladna, blad_bezwzgledny, blad_wzgledny, fx, logbw;
	ifstream f_in;
	ofstream f_out;
	// deklarUJEMY zmiennE f_in i f_out jako strumieniE plikowe w celu otwarcia plików wejściowych i wyjściowych

	f_in.open("dane_do_laboratorium_2.txt");
	if (!f_in.good())
		return 1;

	f_out.open("out.txt", ios::out);
	if (!f_out.good())
		return 1;
	/* 
	Każda linia w pliku wejściowym zawiera trzy wartości:
	logarytm dziesiętny argumentu funkcji, wartość argumentu funkcji
	oraz dokładną wartość funkcji w tym punkcie. Dla każdej linii program najpierw oblicza
	wartość funkcji numerycznie, a następnie analizuje błędy względne i bezwzględne.
	*/

	f_in >> scientific >> setprecision(20);
	f_out << scientific << setprecision(20);

	// Ustawiamy formatowanie wyjścia strumienia plikowego i określamy dokładność zapisu liczb.

	while (f_in >> logx >> x >> wart_dokladna)


		/* 
		Instrukcja while odczytuje trzy liczby zmiennoprzecinkowe z każdej linii pliku wejściowego
		i przypisuje je kolejno do zmiennych. Odczytywanie zakończy się, gdy nie będzie już dostępnych
		danych w pliku wejściowym lub gdy pojawi się błąd odczytu.
		*/
	{

		fx = funkcja(x);
		blad_bezwzgledny = fabs(wart_dokladna - fx);

		/* 
		W przypadku kiedy błąd bezwzględny jest duży, świadczy to o tym, że wartość funkcji znacznie odbiega od rzeczywistej.
		W naszym przypadku taka sytuacja występuje z racji odejmowania liczb o mocno zbliżonych wartościach. W celu uniknięcia 
		takiej sytuacji wykorzystujemy wzór taylora do obliczenia przybliżonej wartości funkcji.
		*/

//============================================================================================================
      
		if (blad_bezwzgledny > numeric_limits<double>::epsilon())
		{
			fx = taylor(x, 40);
			blad_bezwzgledny = fabs(wart_dokladna - fx);

		} 
		// Sprawdzamy, czy wartość błędu bezwzględnego jest większa niż minimalna wartość dla typu double

		

//=============================================================================================================

		blad_wzgledny = fabs(blad_bezwzgledny / wart_dokladna);
		logbw = log10(blad_wzgledny); // Obliczamy błąd względny i logarytm dziesiętny błędu względnego dla każdej linii danych

		if (logbw == INFINITY)			
			logbw = log(0.0000001);

		f_out << logx << " " << logbw << endl;
	}

	/* 
	   Sprawdzamy, czy wartość logarytmu dziesiętnego błędu względnego jest równa nieskończoności,
	   co może się zdarzyć, gdy błąd względny jest równy zero. Jeśli wartość logarytmu dziesiętnego
	   błędu względnego jest równa nieskończoności, przypisujemy wartość logarytmu dziesiętnego
	   bardzo małej wartości (0.0000001) do zmiennej logbw
	*/

	f_in.close();
	f_out.close(); 

	// Zamykamy pliki wejściowy i wyjściowy oraz uruchamiamy program gnuplot w celu wygenerowania wykresu.

	//system("gnuplot -p -e \"set ylabel 'blad wzgledny'; set xlabel 'log(x)'; plot 'dane_do_laboratorium_2.txt'\"");
	system("gnuplot -p -e \"set ylabel 'blad wzgledny'; set xlabel 'log(x)'; plot 'out.txt'\"");

	
	/*
	Na podstawie wygenerowanych wykresów możemy zauważyć, że dla mniejszych wartości błąd względny
	jest dosyć duży, jednak maleje wraz ze wzrostem wartości "x". Jest to spowodowane faktem, iż dla 
	małych wartosci "x" exp(-x) jest bliskie 1, a co za tym idzie 1-exp(-x) daje relatywnie duży błąd
	względny.
	*/
}
