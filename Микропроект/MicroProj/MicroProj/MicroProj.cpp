#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

class WarField {
public:
    vector<vector<int>> fieldForAnch;//задаем матрицы, имитирующие поле войны
    vector<vector<int>> fieldForTar;
    vector<vector<int>> fieldForAnchProv;
    vector<vector<int>> fieldForTarProv;
    int width;//парметры поля для войны
    int height;
    int sumAllTar;
    int sumAllAnch;
    bool flagWar = true;

    void ChangeFlag() {//изменяем флаг перехода хода
        flagWar = !flagWar;
    }

    int minCountAnch = 0;

    int minCountTar = 0;

    bool winAnch = false;
    bool winTar = false;

    WarField() { ; }

    //конструктор для создания поля
    WarField(int width, int height) {
        this->width = width;
        this->height = height;
        fieldForAnch = vector<vector<int>>(height);
        for (int i = 0; i < height; i++) {
            fieldForAnch[i] = vector<int>(width);
        }
        fieldForTar = vector<vector<int>>(height);
        for (int i = 0; i < height; i++) {
            fieldForTar[i] = vector<int>(width);
        }
        fieldForAnchProv = vector<vector<int>>(height);
        for (int i = 0; i < height; i++) {
            fieldForAnchProv[i] = vector<int>(width);
        }
        fieldForTarProv = vector<vector<int>>(height);
        for (int i = 0; i < height; i++) {
            fieldForTarProv[i] = vector<int>(width);
        }
        //генерация полей с определенной важностью с подходящими коэфициентами
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                int variaty = rand() % 100 + 1;
                if (variaty <= 60)
                    fieldForAnch[i][j] = 0;
                if (variaty > 60 && variaty <= 80)
                    fieldForAnch[i][j] = 1;
                if (variaty > 80 && variaty <= 90)
                    fieldForAnch[i][j] = 3;
                if (variaty > 90 && variaty <= 97)
                    fieldForAnch[i][j] = 5;
                if (variaty > 97 && variaty <= 100)
                    fieldForAnch[i][j] = 6;
            }

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                int variaty = rand() % 100 + 1;
                if (variaty <= 60)
                    fieldForTar[i][j] = 0;
                if (variaty > 60 && variaty <= 80)
                    fieldForTar[i][j] = 1;
                if (variaty > 80 && variaty <= 90)
                    fieldForTar[i][j] = 3;
                if (variaty > 90 && variaty <= 97)
                    fieldForTar[i][j] = 5;
                if (variaty > 97 && variaty <= 100)
                    fieldForTar[i][j] = 6;
            }
        sumAllAnch = PrintSumAnch();//суммирование общей важности объектов на поле войны
        sumAllTar = PrintSumTar();
    }

    void AtackOnAnch() {//атака на Анчжурию
        int x, y;
        x = rand() % width;
        y = rand() % height;
        while (fieldForAnchProv[x][y] != 0) {
            x = rand() % width;
            y = rand() % height;
        }
        fieldForAnchProv[x][y] = 1;
        cout << "Тарантерия наносит удар по клектке x = " << x << " y = " << y << ". И наносит урон в размере " << fieldForAnch[x][y] << "\n";
        fieldForAnch[x][y] = 0;
        minCountAnch++;
    }

    void AtackOnTar() {//атака на Тарантерию
        int x, y;
        x = rand() % width;
        y = rand() % height;
        while (fieldForTarProv[x][y] != 0) {
            x = rand() % width;
            y = rand() % height;
        }
        fieldForTarProv[x][y] = 1;
        cout << "Анчурия наносит удар по клектке x = " << x << " y = " << y << ". И наносит урон в размере " << fieldForTar[x][y] << "\n";
        fieldForTar[x][y] = 0;
        minCountTar++;
    }

    void PrintAnch() {//изображаем поле Анчжурии
        for (int i = 0; i < height; i++) {
            cout << "\n";
            for (int j = 0; j < width; j++)
                cout << fieldForAnch[i][j];
        }
    }

    void PrintTar() {//изображаем поле Тарантерии
        for (int i = 0; i < height; i++) {
            cout << "\n";
            for (int j = 0; j < width; j++)
                cout << fieldForTar[i][j];
        }
    }

    int PrintSumAnch() {
        int sum = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                sum += fieldForAnch[i][j];
        }
        return sum;
    }

    int PrintSumTar() {
        int sum = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                sum += fieldForTar[i][j];
        }
        return sum;
    }
};

WarField wf = WarField();
mutex wlock;

//метод для создания потока, имитация перехода хода с помощью мьютексов и предоставления управления к полю войны только для одного потока
void WarAnch() {
    while (!(wf.minCountAnch * 2 >= wf.sumAllAnch || wf.PrintSumAnch() == 0) && !wf.winAnch) {
        wlock.lock();
        if (wf.flagWar && !wf.winAnch) {
            wf.AtackOnAnch();
            wf.ChangeFlag();
        }
        wlock.unlock();
    }
    if (!wf.winAnch) {
        wf.winTar = true;
        cout << "Тарантерия выиграла войну!";
    }
}

void WarTar() {
    while (!(wf.minCountTar * 2 >= wf.sumAllTar || wf.PrintSumTar() == 0) && !wf.winTar) {
        wlock.lock();
        if (!wf.flagWar && !wf.winTar) {
            wf.AtackOnTar();
            wf.ChangeFlag();
        }
        wlock.unlock();
    }
    if (!wf.winTar) {
        wf.winAnch = true;
        cout << "Анчуария  выиграла войну!";
    }
}

int main()
{
    setlocale(LC_ALL, "RUSSIAN");//считываем параметри симуляции
    cout << "Введите параметр для генерации рнадомных чисел при генерации поля:";
    int sr;
    cin >> sr;
    srand(9);
    cout << "Введите размеры поля ширину и длину через пробел:";
    int width, height;
    cin >> width;
    cin >> height;
    wf = WarField(width, height);//создаем объект поля войны
    cout << "Поле для Анчуари:";
    wf.PrintAnch();
    cout << "\n"<< "Поле для Тарантерии:";
    wf.PrintAnch();
    cout << "\n";
    thread anch = thread(WarAnch);//создаем потоки
    thread tar = thread(WarTar);
    while (!wf.winAnch && !wf.winTar) {

    }
    anch.join();
    tar.join();//ждем завершения потоков, чтобы не вылетела ошибка
}
