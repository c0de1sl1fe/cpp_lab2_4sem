﻿#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>


struct stats
{
    size_t comparison_count = 0;
    size_t copy_count = 0;
    size_t time = 0;
    void operator+=(const stats& rhs)
    {
        comparison_count += rhs.comparison_count;
        copy_count += rhs.copy_count;
        time += rhs.time;
    }
};


stats InsertSort(std::vector<int>& arr) 
{
    stats res;
    for (int i = 1; i < arr.size(); i++) {
        for (int j = i; j > 0 && arr[j - 1] > arr[j]; j--, res.comparison_count++) {
            res.copy_count+=3;
            int tmp = arr[j - 1];
            arr[j - 1] = arr[j];
            arr[j] = tmp;
        }
    }
    return res;
}

stats InsertSort_iter(std::vector<int>& arr)
{
    stats res;
    for (std::vector<int>::iterator i = ++arr.begin(); i < arr.end(); i++) 
    {
        for (std::vector<int>::iterator j = i; j > arr.begin() && *((--j)++) > *j; j--, res.comparison_count++)
        {
            res.copy_count+=3;
            std::iter_swap((--j)++, j);
        }
    }
    return res;
}

stats QuickSort(std::vector<int>& arr, int begin, int end)
{
    stats res;
    int left = begin, right = end - 1;
    int mid = (left + right) / 2;
    while (left <= right)
    {
        while (arr[left] < arr[mid])
        {
            res.comparison_count++;
            left++;
        }
        res.comparison_count++;
        while (arr[right] > arr[mid])
        {
            res.comparison_count++;
            right--;
        }
        res.comparison_count++;
        if (left <= right)
        {
            res.copy_count+=3;
            std::swap(arr[left], arr[right]);
            left++;
            right--;
        }
    }
    if (begin < right)
    {
        res += QuickSort(arr, begin, right+1);
    }
    if (end > left)
    {
        res += QuickSort(arr, left+1, end);
    } 
    return res;
}

stats QuickSort_iter(std::vector<int>& arr, std::vector<int>::iterator begin, std::vector<int>::iterator end)
{
    stats res;
    if (end <= begin)
        return res;
    std::vector<int>::iterator pivot = begin, middle = begin + 1;
    for (std::vector<int>::iterator i = begin + 1; i < end; i++)
    {
        res.comparison_count++;
        if (*i < *pivot)
        {
            res.copy_count++;
            std::iter_swap(i, middle);
            middle++;
        }
    }
    res.copy_count++;
    std::iter_swap(begin, middle - 1);
    res += QuickSort_iter(arr, begin, middle - 1);
    res += QuickSort_iter(arr, middle, end);

    return res;
}
size_t lcg() {
    static size_t x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}

stats Test_InsertSort_Random(int numb)
{
    //int res = 0;
    //stats res_stats;
    stats res;
    for (int j = 0; j < 100; j++)
    {   
        std::vector<int> array;
        for (int i = 0; i < numb; i++)
        {
            array.push_back(lcg());
        }

        auto start = std::chrono::high_resolution_clock::now();
        res += InsertSort(array);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        //res += duration.count();
        res.time += duration.count();
    }
    std::cout << "InsertSort for RANDOM array with len: " << numb << 
        "\n\ttime: " << (double)res.time / 100 << " microseconds" <<
        "\n\tcomparison_count : " << res.comparison_count / 100 <<
        "\n\tcopy_count : " << res.copy_count / 100 << std::endl;
    return res;
}
stats Test_InsertSort_Sorted(int numb)
{
    //int res = 0;
    //stats res_stats;
    stats res;
    std::vector<int> array;
    for (int i = 0; i < numb; i++)
    {
        array.push_back(i);
    }

    auto start = std::chrono::high_resolution_clock::now();
    res += InsertSort(array);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    res.time += duration.count();

    std::cout << "InsertSort for SORTED array with len: " << numb <<
        "\n\ttime: " << (double)res.time << " microseconds" <<
        "\n\tcomparison_count : " << res.comparison_count <<
        "\n\tcopy_count : " << res.copy_count << std::endl;
    return res;

}stats Test_InsertSort_ReverseSorted(int numb)
{
    //int res = 0;
    //stats res_stats;
    stats res;
    std::vector<int> array;
    for (int i = numb; i > 0; i--)
    {
        array.push_back(i);
    }

    auto start = std::chrono::high_resolution_clock::now();
    res += InsertSort(array);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    res.time += duration.count();

    std::cout << "InsertSort for REVERSE-SORTED array with len: " << numb <<
        "\n\ttime: " << (double)res.time << " microseconds" <<
        "\n\tcomparison_count : " << res.comparison_count <<
        "\n\tcopy_count : " << res.copy_count << std::endl;
    return res;
}
stats Test_QuickSort_Random(int numb)
{
    //int res = 0;
    //stats res_stats;
    stats res;
    for (int j = 0; j < 100; j++)
    {
        std::vector<int> array;
        for (int i = 0; i < numb; i++)
        {
            array.push_back(lcg());
        }

        auto start = std::chrono::high_resolution_clock::now();
        res += QuickSort(array, 0, array.size());
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        res.time += duration.count();
    }
    std::cout << "QuickSort for RANDOM array with len: " << numb <<
        "\n\ttime: " << (double)res.time / 100 << " microseconds" <<
        "\n\tcomparison_count : " << res.comparison_count / 100 <<
        "\n\tcopy_count : " << res.copy_count / 100 << std::endl;
    return res;
}
stats Test_QuickSort_Sorted(int numb)
{
    //int res = 0;
    //stats res_stats;
    stats res;
    std::vector<int> array;
    for (int i = 0; i < numb; i++)
    {
        array.push_back(i);
    }

    auto start = std::chrono::high_resolution_clock::now();
    res += QuickSort(array, 0, array.size());
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    res.time += duration.count();

    std::cout << "QuickSort for SORTED array with len: " << numb <<
        "\n\ttime: " << (double)res.time << " microseconds" <<
        "\n\tcomparison_count : " << res.comparison_count <<
        "\n\tcopy_count : " << res.copy_count << std::endl;
    return res;

}stats Test_QuickSort_ReverseSorted(int numb)
{
    //int res = 0;
    //stats res_stats;
    stats res;
    std::vector<int> array;
    for (int i = numb; i > 0; i--)
    {
        array.push_back(i);
    }

    auto start = std::chrono::high_resolution_clock::now();
    res += QuickSort(array, 0, array.size());
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    res.time += duration.count();

    std::cout << "QuickSort for REVERSE-SORTED array with len: " << numb <<
        "\n\ttime: " << (double)res.time << " microseconds" <<
        "\n\tcomparison_count : " << res.comparison_count <<
        "\n\tcopy_count : " << res.copy_count << std::endl;
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
            test.push_back(5);
            test.push_back(4);
            test.push_back(3);
            test.push_back(2);
            test.push_back(1);
            stats tmp = InsertSort(test);
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
            std::vector<int> test;
            test.push_back(5);
            test.push_back(3);
            test.push_back(4);
            test.push_back(3);
            stats tmp = QuickSort(test, 0, test.size());
            std::cout << tmp.comparison_count << " " << tmp.copy_count << std::endl;
            for (int i = 0; i < test.size(); i++)
            {
                std::cout << test[i] << " ";
            }
            system("pause");
            break;
        }
        case(3):
        {
            Test_InsertSort_Random(1000);
            Test_InsertSort_Sorted(1000);
            Test_InsertSort_ReverseSorted(1000);
            
            Test_QuickSort_Random(1000);
            Test_QuickSort_Sorted(1000);
            Test_QuickSort_ReverseSorted(1000);
            std::vector<int> numbs = { 1000,  2000, 3000, 4000, 5000, 6000, 7000,8000,9000, 10000, 25000, 50000, 100000 };
            std::ofstream myfile;
            myfile.open("Test_InsertSort_Random.txt");
            for (auto i : numbs)
            {
                stats tmp;
                tmp  = Test_InsertSort_Random(i);
                myfile << i << " " << tmp.comparison_count << " " << tmp.copy_count << " " << tmp.time << "\n";
            }
            myfile.close();

            myfile.open("Test_InsertSort_Sorted.txt");
            for (auto i : numbs)
            {
                stats tmp;
                tmp = Test_InsertSort_Sorted(i);
                myfile << i << " " << tmp.comparison_count << " " << tmp.copy_count << " " << tmp.time << "\n";
            }
            myfile.close();

            myfile.open("Test_InsertSort_ReverseSorted.txt");
            for (auto i : numbs)
            {
                stats tmp;
                tmp = Test_InsertSort_ReverseSorted(i);
                myfile << i << " " << tmp.comparison_count << " " << tmp.copy_count << " " << tmp.time << "\n";
            }
            myfile.close();

            // -------------------------------------------//

            myfile.open("Test_QuickSort_Random.txt");
            for (auto i : numbs)
            {
                stats tmp;
                tmp = Test_QuickSort_Random(i);
                myfile << i << " " << tmp.comparison_count << " " << tmp.copy_count << " " << tmp.time << "\n";
            }
            myfile.close();

            myfile.open("Test_QuickSort_Sorted.txt");
            for (auto i : numbs)
            {
                stats tmp;
                tmp = Test_QuickSort_Sorted(i);
                myfile << i << " " << tmp.comparison_count << " " << tmp.copy_count << " " << tmp.time << "\n";
            }
            myfile.close();

            myfile.open("Test_QuickSort_ReverseSorted.txt");
            for (auto i : numbs)
            {
                stats tmp;
                tmp = Test_QuickSort_ReverseSorted(i);
                myfile << i << " " << tmp.comparison_count << " " << tmp.copy_count << " " << tmp.time << "\n";
            }
            myfile.close();


            system("pause");
            break;
        }
        case(4):
        {
            std::cout << "check inter and no iter version" << std::endl;
            std::vector<int> test;
            test.push_back(5);
            test.push_back(3);
            test.push_back(4);
            test.push_back(3);
            std::cout << "No iter InsertSort: " << std::endl;

            stats tmp = InsertSort(test);
            std::cout << tmp.comparison_count << " " << tmp.copy_count << std::endl;
            for (int i = 0; i < test.size(); i++)
            {
                std::cout << test[i] << " ";
            }
            std::cout << std::endl;
            tmp.comparison_count = 0;
            tmp.copy_count = 0;
            test.clear();
            test.push_back(5);
            test.push_back(3);
            test.push_back(4);
            test.push_back(3);

            std::cout << "iter InsertSort: " << std::endl;
            tmp = InsertSort_iter(test);
            std::cout << tmp.comparison_count << " " << tmp.copy_count << std::endl;
            for (int i = 0; i < test.size(); i++)
            {
                std::cout << test[i] << " ";
            }
            tmp.comparison_count = 0;
            tmp.copy_count = 0;
            test.clear();

            std::cout << std::endl << "-----------------" << std::endl;
            test.push_back(5);
            test.push_back(3);
            test.push_back(4);
            test.push_back(3);
            std::cout << "No iter QuickSort: " << std::endl;

            tmp = QuickSort(test, 0, test.size());
            std::cout << tmp.comparison_count << " " << tmp.copy_count << std::endl;
            for (int i = 0; i < test.size(); i++)
            {
                std::cout << test[i] << " ";
            }
            std::cout << std::endl;
            tmp.comparison_count = 0;
            tmp.copy_count = 0;
            test.clear();
            test.push_back(5);
            test.push_back(3);
            test.push_back(4);
            test.push_back(3);

            std::cout << "iter QuickSort: " << std::endl;
            tmp = QuickSort_iter(test, test.begin(), test.end());
            std::cout << tmp.comparison_count << " " << tmp.copy_count << std::endl;
            for (int i = 0; i < test.size(); i++)
            {
                std::cout << test[i] << " ";
            }
            tmp.comparison_count = 0;
            tmp.copy_count = 0;
            test.clear();

            exit = true;
            break;
        }
        default:
            std::cout << "Unable to recognize your request" << std::endl;
            break;
        }
    }
}