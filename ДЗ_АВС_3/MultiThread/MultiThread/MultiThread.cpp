#include <iostream>
#include <thread>
#include <math.h>

using namespace std;

const double eps = 0.01;//константа эпсилон - диаметр разбиения данного интеграла

double Func(double x) {//функция для описания математической функции от которой считаем интеграл
    return exp(-x*x);
}

void Integral(double& res, double a, double b) {//функция для подсчета интеграла с помощью рекурсии, многопоточности и принципа дихотомии
    if (abs(a - b) <= eps) {
        res = Func((a + b) / 2) * abs((a - b));//если разбиение меньше эпсилон, находим площадь
    }else{
        double a1;//если больше эпсилон, то делим отрезок на 2 и рекурсивно применяем функцию к обоим отрезкам
        thread At(Integral, ref(a1), a, (a + b) / 2);
        double b1;
        thread Bt(Integral, ref(b1), (a + b) / 2, b);
        At.join();
        Bt.join();
        res = a1 + b1;//сумируем площадти под кривой на 2 отрезках
    }
}

int main()
{
    double res = 0;
    Integral(ref(res), -1, 1);//считаем интегралл от функции и выводим значение
    cout << res;
}
