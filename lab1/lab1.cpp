#include <iostream>

using namespace std;

int main()
{

    float f_epsilon = 0.5;
    float f_epsilon1; // przechowywanie wyniku epsilona maszynowego
    double d_epsilon = 0.5;
    double d_epsilon1; // przechowywanie wyniku epsilona maszynowego
    int bity_f = 0; // liczba bitów mantysy
    int bity_d = 0; // liczba bitów mantysy

    f_epsilon1 = f_epsilon;
    d_epsilon1 = d_epsilon;

    while (1.0f + f_epsilon > 1.0f)
    {
        f_epsilon1 = f_epsilon;
        f_epsilon /= 2.0f;
        bity_f++;
        // zmienną f_epsilon dzielimy przez 2 tak długo, dopóki warunek nie będzie spełniony
        // po zakończeniu epsilon maszynowy otrzymujemy w zmiennej f_epsilon1 a liczbę bitów mantysy w bity_f
    }
    cout << "Epsilon maszynowy dla float: " << f_epsilon1 << endl;
    cout << " Liczba bitow mantysy: " << bity_f << endl;

    while (1.0 + d_epsilon > 1.0)
    {
        d_epsilon1 = d_epsilon;
        d_epsilon /= 2.0;
        bity_d++;
    }
    cout << "Epsilon maszynowy dla double: " << d_epsilon1 << endl;
    cout << " Liczba bitow mantysy: " << bity_d << endl;
}
