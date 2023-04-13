#include <iostream>
#include <vector>


struct stats
{
    size_t comparison_count = 0;
    size_t copy_count = 0;    void operator+=(const stats& rhs)
    {
        comparison_count += rhs.comparison_count;
        copy_count += rhs.copy_count;
    }
};


stats InsertSort(std::vector<int>& arr, int n) //сортировка вставками
{
    stats res;
    for (int i = 1; i < n; i++) {
        for (int j = i; j > 0 && arr[j - 1] > arr[j]; j--, res.comparison_count++) {
            res.copy_count++;
            int tmp = arr[j - 1];
            arr[j - 1] = arr[j];
            arr[j] = tmp;
        }
    }
    return res;
}
void printMenu()
{
    system("cls");
    std::cout << "Hello, here your menu, choose one:" << std::endl;
    std::cout << "1. Insert_sort" << std::endl;
    std::cout << "2. Quick_sort " << std::endl;
    std::cout << "3. Check stats" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "choice: ";
}
int EnterNumber()
{
    double tmp;
    char stmp[24];
    bool exit1 = false;
    while (!exit1)
    {
        bool exit = true;
        gets_s(stmp);
        char* endptr = stmp;
        tmp = strtod(stmp, &endptr);
        if (endptr == stmp)
        {
            exit = false;
            continue;
        }
        if ((tmp == DBL_MAX || tmp == DBL_MIN) && errno == ERANGE)
        {
            exit = false;
            continue;
        }
        while (*endptr)
        {
            if ((*endptr != ' ') && (*endptr != '\n'))
            {
                exit = false;
                break;
            }
            endptr++;
        }
        if (exit)
        {
            exit1 = true;
        }
        else
        {
            exit1 = false;
            printf("\nsomething wrong with number\n");
        }
    }
    return (int)tmp;
}
void main()
{

    int choice = 0;
    bool exit = false;
    while (!exit)
    {
        printMenu();
        choice = EnterNumber();
        switch (choice)
        {   
        case(1):
        {
            std::vector<int> test;
            test.push_back(4);
            test.push_back(5);
            test.push_back(3);
            stats tmp = InsertSort(test, test.size());
            std::cout << tmp.comparison_count << " " << tmp.copy_count << std::endl;
            for (int i = 0; i < test.size(); i++)
            {
                std::cout << test[i] << " ";
            }
            system("pause");
            break;
        }
        case(2):
        {
            break;
        }
        case(3):
        {
            break;
        }
        case(4):
        {
            exit = true;
            break;
        }
        default:
            std::cout << "Unable to recognize your request" << std::endl;
            break;
        }
    }
}