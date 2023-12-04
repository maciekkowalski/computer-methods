#include <iostream>
#include <cmath>


double fun1(double x) // funkcja 1 
{
    return pow(sin(x / 4), 2) - x;
}

double fun2(double x) // funkcja 2
{
    return tan(2 * x) - x - 1;
}

double p1(double x) // pochodna 1
{
    return 1 / 2 * sin(x / 4) * cos(x / 4) - 1;
}

double p2(double x) // pochodna 2
{
    return 2 / (pow(cos(2 * x), 2)) - 1;
}

double metoda_piccarda(double(*funkcja)(double), double x0, int iteracje, double tolerancja_bledu, double tolerancja_residuum)

// W metodzie kolejne wartości funkcji wyznaczają nowe punkty, które zbliżają nas do miejsca zerowego
{
    printf(" \n Metoda Piccarda: \n"); 
    int i = 1;
    double xn, En;

    do
    {
        xn = funkcja(x0);
        En = abs(xn - x0);
        x0 = xn;
        printf("i: %3d  |  xn: %20.12e  |  En: %20.12e\n", i, xn, En);
    } while (++i <= iteracje && abs(En) > tolerancja_bledu && abs(xn) > tolerancja_residuum);

    /*
    Pętla "do...while" będzie wykonywać iteracje, dopóki nie zostaną spełnione
    jednocześnie wszystkie trzy warunki: nie przekroczono liczby iteracji, długość przedziału
    między argumentami "xn" a "xn+1" (czyli wartość błędu "En") jest mniejsza od zadanej
    tolerancji, a wartość funkcji (residuum) znajduje się w zakresie tolerancji.
    */

    if (abs(xn) <= tolerancja_residuum)
        //Jeśli residuum będzie mieścić się w zakresie tolerancji oznacza to, że mamy miejsce zerowe
    {
        printf("\nMiejsce zerowe: xn = %.12e\n", xn);
    }
    else
        printf("\nResiduum jest wieksze od tolerancji, xn = %.12e\n", xn);

    return xn;
}

double metoda_bisekcji(double(*fun)(double), double a, double b, int iteracje, double tolerancja_bledu, double tolerancja_residuum)
{
    printf(" \n Metoda Bisekcji: \n");

    double fa, fb;
    fa = fun(a);
    fb = fun(b);
    if (fa * fb >= 0)
    {
        printf(" Błąd - wartosci funkcji w punktach a i b  musza byc przeciwnych znakow. ");
        return -1;
    }
    /*
    Jeśli wartości funkcji w punktach "a" i "b" mają przeciwne znaki, metoda bisekcji będzie wykonywać
    iteracje, w których przedział [a,b] będzie dzielony na pół i wybierana będzie połowa przedziału
    zawierająca miejsce zerowe. Pętla "do...while" będzie wykonywać iteracje, dopóki nie zostaną spełnione
    jednocześnie wszystkie trzy warunki: nie przekroczono liczby iteracji, długość przedziału
    między "a" i "b" (czyli wartość błędu "En") jest mniejsza od zadanej tolerancji, a wartość
    funkcji (residuum) znajduje się w zakresie tolerancji.
    */

    int i = 1;
    double c, fc, En;
    do {
        c = (a + b) / 2; // Wyznaczamy wartość punktu środkowego przedziału 
        fc = fun(c);
        En = abs((b - a) / 2); // Wartość błędu "En" jest wyznaczana jako połowa długości przedziału [a,b]
        if (fa * fc < 0)
        {
            b = c;
            fb = fc;
        }
        else
        {
            a = c;
            fa = fc;
        }

        /*
        Dokomujemy wyboru połowy przedziału, w której znajduje się miejsce zerowe. Jeśli wartość funkcji
        w punkcie "c" ma przeciwny znak niż wartość funkcji w punkcie "a", to miejsce zerowe znajduje się między "a" i "c".
        W przeciwnym przypadku miejsce zerowe znajduje się między "c" i "b"
        */
        printf("i: %3d  |  f(c): %20.12e  |  En: %20.12e\n", i, fc, En);
    } while (++i <= iteracje && abs(En) > tolerancja_bledu && abs(fc) > tolerancja_residuum);

    if (abs(fc) <= tolerancja_residuum)
    {
        printf("\nMiejsce zerowe: c = %.12e, f(c) = %.12e\n", c, fc);
    }
    else
        printf("\nResiduum jest wieksze od tolerancji, f(c) = %.12e\n", fc);

    return fc;
}

double metoda_newtona(double(*fun)(double), double(*pochodna)(double), double x0, int iteracje, double tolerancja_bledu, double tolerancja_residuum)
{
    printf(" \n Metoda Newtona: \n");

    double xn, En, xn_poprz, fxn;
    int i = 1;

    xn = x0;
    do
    {
        xn_poprz = xn; // poprzednia wartość przybliżenia pierwiastka
        xn = xn_poprz - fun(xn_poprz) / pochodna(xn_poprz); /* Obliczamy nową wartość przybliżenia pierwiastka xn,
        korzystając z wartości poprzedniego przybliżenia */
        fxn = fun(xn);
        En = abs(xn_poprz - xn);
        /*
        Obliczamy wartość błędu pomiędzy obecnym przybliżeniem pierwiastka xn a poprzednim przybliżeniem.
        Wartość błędu En służy do sprawdzenia warunku stopu - jeśli En jest mniejsze niż zadana tolerancja błędu tolerancja_bledu,
        to algorytm kończy iteracje i zwraca obecne przybliżenie pierwiastka xn.
        */
        printf("i: %3d  |  f(xn): %20.12e  |  En: %20.12e\n", i, fxn, En);

    } while (++i <= iteracje && abs(En) > tolerancja_bledu && abs(fxn) > tolerancja_residuum);

    if (abs(fxn) <= tolerancja_residuum)
    {
        printf("\n Miejsce zerowe:  xn = %.12e, f(xn) = %.12e\n", xn, fxn);
    }
    else
        printf("\n Residuum jest wieksze od tolerancji, f(xn) = %.12e\n", fxn);

    return fxn;
}

double metoda_siecznych(double(*fun)(double), double x1, double x2, int iteracje, double tolerancja_bledu, double tolerancja_residuum)
{
    printf(" \n Metoda Siecznych: \n");

    double xn, En, fxn;
    int i = 1;

    do
    {
        xn = x2 - fun(x2) * (x2 - x1) / (fun(x2) - fun(x1));
        x1 = x2;
        x2 = xn;
        fxn = fun(xn);
        En = abs(x1 - x2);
        printf("i: %3d  |  f(xn): %20.12e  |  En: %20.12e\n", i, fxn, En);

    }
    /*
     W pętlI obliczane są kolejne przybliżenia miejsca zerowego, zaczynając od punktów x1 i x2.
     Następnie aktualizowane są wartości zmiennych x1, x2 oraz En, które określają odległość między
     poprzednim, a aktualnym przybliżeniem.
    */
    while (++i <= iteracje && abs(En) > tolerancja_bledu && abs(fxn) > tolerancja_residuum);

    if (abs(fxn) <= tolerancja_residuum)
    {
        printf("\n Miejsce zerowe: xn = %.12e, f(xn) = %.12e\n", xn, fxn);
    }
    else
        printf("\n Residuum jest wieksze od tolerancji, f(xn) = %.12e\n", fxn);

    return fxn;
}

int main()
{
    double fx;

    fx = metoda_piccarda(fun1, 2, 100, 1.0e-15, 1.0e-15); // (fun1, x0:, iteracje:, tolerancja_bledu:, tolerancja_residuum:)
    fx = metoda_bisekcji(fun1, -5, 10, 100, 1.0e-15, 1.0e-15); // (fun1, a:, b:, iteracje:, tolerancja_bledu:, tolerancja_residuum:)
    fx = metoda_newtona(fun1, p1, 2, 100, 1.0e-15, 1.0e-15); // (fun1, p1, x0:, iteracje, tolerancja_bledu:, tolerancja_residuum:)
    fx = metoda_siecznych(fun1, 10, 2, 100, 1.0e-15, 1.0e-15); // (fun1, x1:, x2:, iteracje, tolerancja_bledu:, tolerancja_residuum:)

    // Wywołujemy kolenjo wsztstkie 4 funkcje znajdujące miejsca zerowe 

    /*
    fx = metoda_piccarda(fun2, 2, 100, 1.0e-15, 1.0e-15);
    fx = metoda_bisekcji(fun2, 0, 0.5, 100, 1.0e-15, 1.0e-10);
    fx = metoda_newtona(fun2, p2, 2, 100, 1.0e-15, 1.0e-15);
    fx = metoda_siecznych(fun2, 10, 2, 100, 1.0e-15, 1.0e-15);
*/
}



