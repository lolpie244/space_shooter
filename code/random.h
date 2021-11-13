#include "random"
#include "ctime"
using namespace chrono;
using namespace sf;
int random_int(int min, int max)
{
    return  min + rand() % (max - min + 1);
}
double random_double(double min, double max)
{
    double value;
    value =  rand() % (int)pow(10, 3);
    return min + (value / pow(10, 3)) * (max - min);

}
template <typename T>
T get_random_function(pair<double, double> cords, T func = [](double x){return x;})
{
    vector<T> v;
    //cout << cords.first << ' ' << cords.second << '\n';
    bool type = (cords.second < 0 || cords.second > 1080 * constants::window_coef);
    bool direction = (cords.first > 1920 * constants::window_coef || cords.second > 1080 * constants::window_coef);

    v.push_back([](double x){ return 1 * constants::window_coef;});

    v.push_back([](double x){ x = abs(x); return sqrt(x) * 10;});
    v.push_back([](double x){ x = abs(x); return -1 * sqrt(x) * 10;});

    v.push_back([](double x){ x = abs(x); x /= 240; return sin(x) * 110;});
    v.push_back([](double x){ x = abs(x); x /= 240; return -1 * sin(x) * 110;});

    v.push_back([](double x){ x = abs(x); return (x * x) / 10000;});
    v.push_back([](double x){ x = abs(x); return (-1 * x * x) / 10000;});

    v.push_back([](double x){x = abs(x); return x * 5 / 1000;});
    v.push_back([](double x){x = abs(x); return -1 * x * 5 / 1000;});

    v.push_back([](double x){ x = abs(x); x /= 240; return cos(x) * 110;});
    v.push_back([](double x){ x = abs(x); x /= 240; return -1 * cos(x) * 110;});

    func = v[random_int(0, v.size() - 1)];

    // << func(-1) << '\n';
    return func;
}