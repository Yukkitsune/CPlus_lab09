#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
class expression { // Создаём класс выражение
private: // Приватные переменные, видимые только в классе
    std::string exp;
    int count = 0;
    double* numbers; // массивы для хранения чисел и операций
    char* operation;
public: // Публичные переменные, методы класса
    expression() { // Конструктор
        exp = "";
        count = 0;
        numbers = new double[count] {};
        operation = new char[count] {};
    }
    void set(std::string express, int cnt,double* mas_num, char* sign) { // установка значений для i-го объекта класса
        exp = express;
        count = cnt;
        delete[] numbers;
        delete[] operation;
        numbers = new double[count] {};
        operation = new char [count-1] {};
        for (int i = 0; i < count; ++i) {
            numbers[i] = mas_num[i];
        }
        for (int i = 0; i < count-1; ++i) {
            operation[i] = sign[i];
        }
    }
    double get_res() { // Подсчет результата выражения
        double temp = numbers[0];
        for (int i = 1; i < count;++i) {
            switch (operation[i - 1]) { // Выбор действия в зависимости от операции
            case '*':
                temp *= numbers[i];
                break;
            case '/':
                if (numbers[i] == 0) std::cerr << "Division by zero!" << std::endl;
                else {
                    temp /= numbers[i];
                    break;
                }
            case '+':
                temp += numbers[i];
                break;
            case '-':
                temp -= numbers[i];
                break;
            default: std::cerr << "Something went wrong :(" << std::endl;
            }
        }
        return temp;
    }
    void print(std::string expression,double result) { // Вывод результата в консоль
        std::cout << "Result of expression " << expression << " is " << result << std::endl;
        std::cout<<std::endl;
    }
    void print_fout(std::ofstream& fout,std::string str, double res) { // Вывод результата в файл
        fout << "Result of expression " << str << " is " << res << std::endl;
    }
    ~expression() { // Деструктор класса
        if (numbers != nullptr) { delete[] numbers; }
        if (operation != nullptr) { delete[] operation; }
    }
};
int parsed_count(std::string express) { // Подсчет количества чисел в выражении
    int count = 0;
    for (int i = 1; i < express.size();++i) {
        if (express[i] == '-' || express[i] == '+' || express[i] == '*' || express[i] == '/') count += 1;
    }
    return count+1;
}
void fill_massives(std::string& express, double* num, char* sign) { // Заполнение массивов чисел и операций
    std::string temp_num;
    std::string new_exp = express + '+';
    int i = 0;
    int j = 0;
    while (i < new_exp.size()) {
        if (isdigit(new_exp[i])) temp_num += new_exp[i];
        else {
            sign[j] = new_exp[i];
            num[j] = stod(temp_num);      
            ++j;
            temp_num = "";
        }
        ++i;
    }
}
int main(int argc, const char* argv[]) // Ввод аргументов в консоль
{

    if (argc == 3) { // Проверка количества введённых аргументов
        const std::string in = argv[1]; // Название файла ввода и вывода
        const std::string out = argv[2];
        std::ifstream fint(in); // Открываем файл ввода
        if (!fint) { // Ошибка, если файл не найден
            std::cerr << "File isn't found :(" << std::endl;
            return -1;
        }
        std::ofstream fout(out); // Открываем файл вывода
        int n;
        fint >> n; // Считываем количество выражений
        std::string express; // Строка хранит выражение
        std::vector<std::string> str; // Вектор хранит все строки файла 
        std::vector<expression> exp(n); // Вектор хранит объекты класса выражение
        for (int i = 0; i < n; ++i) { // Заполняем вектор строк
            fint >> express;
            str.push_back(express);
        }
        fint.close();
        for (int i = str.size() - 1; i > -1; --i) { // Работаем с выражениями с конца в начало
            std::string express = str[i];
            int count = parsed_count(express); // Количество чисел в выражении
            double* mean = new double[count] {}; // Создание динамических массивов чисел и операций
            char* sign = new char[count] {};
            fill_massives(express, mean, sign); // Заполнение массивов
            exp[i].set(express, count,mean,sign); // Установка атрибутов объекта класса
            double result = exp[i].get_res(); // Результат выражение
            exp[i].print(express, result); // Вывод в консоль
            exp[i].print_fout(fout, express, result); // Вывод в файл
            delete[] mean;
            delete[] sign;
        }
        std::cout << "Have a great day!" << std::endl;
        fout.close();
        
    }
    else {
        std::cerr << "Not enough arguments :(";
        return -1;
    }
    return 0;
}
