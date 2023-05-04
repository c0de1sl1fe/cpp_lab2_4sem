#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>


struct stats
{
    long double comparison_count = 0;
    long double copy_count = 0;
    long double time = 0;
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
        res.comparison_count++;
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
        res.comparison_count++;
        for (std::vector<int>::iterator j = i; j > arr.begin() && *((--j)++) > *j; j--, res.comparison_count++)
        {
            res.copy_count+=3;
            std::iter_swap((--j)++, j);
        }
    }
    return res;
}

stats QuickSort(std::vector<int>& arr, int left, int right)
{
    stats res;
    int i = left, j = right;
    //int mid = (i + j) / 2;
    int pivot = arr[(left + right) / 2];
    while (i <= j)
    {
        while (arr[i] < pivot)
        {
            res.comparison_count++;
            i++;
        }
        res.comparison_count++;
        while (arr[j] > pivot)
        {
            res.comparison_count++;
            j--;
        }
        res.comparison_count++;
        if (i <= j)
        {
            res.copy_count+=3;
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    if (left < j)
    {
        res += QuickSort(arr, left, j);
    }
    if (right > i)
    {
        res += QuickSort(arr, i, right);
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

}

stats Test_InsertSort_ReverseSorted(int numb)
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
        res += QuickSort(array, 0, array.size()-1);
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
    res += QuickSort(array, 0, array.size()-1);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    res.time += duration.count();

    std::cout << "QuickSort for SORTED array with len: " << numb <<
        "\n\ttime: " << (double)res.time << " microseconds" <<
        "\n\tcomparison_count : " << res.comparison_count <<
        "\n\tcopy_count : " << res.copy_count << std::endl;
    return res;

}

stats Test_QuickSort_ReverseSorted(int numb)
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
    res += QuickSort(array, 0, array.size()-1);
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
            test.push_back(8);
            test.push_back(5);
            test.push_back(1);
            test.push_back(2);
            test.push_back(6);
            test.push_back(3);
            stats tmp = QuickSort(test, 0, test.size()-1);
            //stats tmp = QuickSort_iter(test, test.begin(), test.end());
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
            std::vector<int> numbs = { 1000,  2000, 3000, 4000, 5000, 6000, 7000,8000,9000, 10000, 25000, 50000, 100000 };
            std::ofstream myfile;


            std::ofstream myfile1, myfile2, myfile3;
            stats tmp1, tmp2, tmp3;
            myfile.open("Test_InsertSort_Random.txt");
            //for (auto i : numbs)
            //{
            //    stats tmp;
            //    tmp1  = Test_InsertSort_Random(i);
            //    //myfile << i << " " << tmp.comparison_count << " " << tmp.copy_count << " " << tmp.time << "\n";
            //}
            //myfile.close();

            //myfile.open("Test_InsertSort_Sorted.txt");
            //for (auto i : numbs)
            //{
            //    stats tmp;
            //    tmp2 = Test_InsertSort_Sorted(i);
            //    //myfile << i << " " << tmp.comparison_count << " " << tmp.copy_count << " " << tmp.time << "\n";
            //}
            //myfile.close();

            //myfile.open("Test_InsertSort_ReverseSorted.txt");
            //for (auto i : numbs)
            //{
            //    stats tmp;
            //    tmp3 = Test_InsertSort_ReverseSorted(i);
            //    //myfile << i << " " << tmp.comparison_count << " " << tmp.copy_count << " " << tmp.time << "\n";
            //}
            //myfile.close();



            // -------------------------------------------//

            //myfile.open("Test_QuickSort_Random.txt");
            //for (auto i : numbs)
            //{
            //    stats tmp;
            //    tmp4 = Test_QuickSort_Random(i);
            //    //myfile << i << " " << tmp.comparison_count << " " << tmp.copy_count << " " << tmp.time << "\n";
            //}
            //myfile.close();

            //myfile.open("Test_QuickSort_Sorted.txt");
            //for (auto i : numbs)
            //{
            //    stats tmp;
            //    tmp5 = Test_QuickSort_Sorted(i);
            //    //myfile << i << " " << tmp.comparison_count << " " << tmp.copy_count << " " << tmp.time << "\n";
            //}
            //myfile.close();

            //myfile.open("Test_QuickSort_ReverseSorted.txt");
            //for (auto i : numbs)
            //{
            //    stats tmp;
            //    tmp6 = Test_QuickSort_ReverseSorted(i);
            //    //myfile << i << " " << tmp.comparison_count << " " << tmp.copy_count << " " << tmp.time << "\n";
            //}
            //myfile.close();

            myfile1.open("newTest/comparison_count_InsertSort.txt");
            myfile2.open("newTest/copy_count_InsertSort.txt");
            myfile3.open("newTest/time_InsertSort.txt");
            for (auto i : numbs)
            {
                tmp1 = Test_InsertSort_Random(i);
                tmp2 = Test_InsertSort_Sorted(i);
                tmp3 = Test_InsertSort_ReverseSorted(i);
                myfile1 << i << " " << tmp1.comparison_count << " " << tmp2.comparison_count << " " << tmp3.comparison_count << "\n";
                myfile2 << i << " " << tmp1.copy_count << " " << tmp2.copy_count << " " << tmp3.copy_count << "\n";
                myfile3 << i << " " << tmp1.time << " " << tmp2.time << " " << tmp3.time << "\n";
            }
            myfile1.close();
            myfile2.close();
            myfile3.close();
            // -------------------------------------------//
            myfile1.open("newTest/comparison_count_QuickSort.txt");
            myfile2.open("newTest/copy_count_QuickSort.txt");
            myfile3.open("newTest/time_QuickSort.txt");
            for (auto i : numbs)
            {
                tmp1 = Test_QuickSort_Random(i);
                tmp2 = Test_QuickSort_Sorted(i);
                tmp3 = Test_QuickSort_ReverseSorted(i);
                myfile1 << i << " " << tmp1.comparison_count << " " << tmp2.comparison_count << " " << tmp3.comparison_count << "\n";
                myfile2 << i << " " << tmp1.copy_count << " " << tmp2.copy_count << " " << tmp3.copy_count << "\n";
                myfile3 << i << " " << tmp1.time << " " << tmp2.time << " " << tmp3.time << "\n";
            }
            myfile1.close();
            myfile2.close();
            myfile3.close();

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

            tmp = QuickSort(test, 0, test.size()-1);
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