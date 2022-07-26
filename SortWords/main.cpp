#include <cmath>
#include <array>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include<windows.h>
#include <vector>
#include <time.h>
#include <map>
#include <locale>

using namespace std;

CRITICAL_SECTION file_cs;

typedef struct
{
    unsigned char order;            // a = ascending, d = descending
    unsigned char spliting_char;    // s = white space, c = coma, n = new line
} Options;

std::map<std::string, unsigned int> ordered_map;
// std::map<std::string, unsigned int, greater<string>> ordered_map;

void RemoveSpecialCharacters(string& token)
{
    for (int i = 0; i < token.size(); i++)
    {
        if (token[i] < 'A' || token[i] > 'Z' && token[i] < 'a' || token[i] > 'z')
        {
            token.erase(i, 1);
            i--;
        }
    }
}

void ReadTextFile(std::string file_path)
{
    if (file_path.empty() == false)
    {
        EnterCriticalSection(&file_cs);
        std::cout << "Thread ID: " << std::this_thread::get_id() << " processes file: " << file_path << endl;
        LeaveCriticalSection(&file_cs);

        ifstream file(file_path);
        if (file.is_open())
        {
            string token;

            while (file >> token)
            {
                RemoveSpecialCharacters(token);
                std::transform(token.begin(), token.end(), token.begin(), ::tolower);
                EnterCriticalSection(&file_cs);
                ordered_map[token]++;
                LeaveCriticalSection(&file_cs);
            }

            file.close();
        }  
    }
}

void SpawnReadingThreads(vector<string> paths)
{
    std::vector<std::thread> threads;
    threads.reserve(paths.size());

    for (int i = 0; i < paths.size(); i++)
    {
        std::thread th(&ReadTextFile, paths[i]);
        threads.push_back(std::move(th));
    }

    for (std::thread& th : threads)
    {
        if (th.joinable())
            th.join();
    }
}

void WriteTextFile(std::string file_path)
{
    ofstream output_file(file_path, ios::out);

    if (output_file.is_open())
    {
        string most_frequent_word;
        unsigned int most_frequent_word_cnt = 1;

        for (auto const& pair : ordered_map)
        {
            if (most_frequent_word_cnt < pair.second)
            {
                most_frequent_word.assign(pair.first);
                most_frequent_word_cnt = pair.second;
            }

            output_file << pair.first << "\n";
        }

        output_file << "The most frequent word in the text is: '" << most_frequent_word << "', count: " << most_frequent_word_cnt;

        output_file.close();
    }
}

void SpawnWritingThread(string path)
{
    std::thread th(&WriteTextFile, path);
    
    if (th.joinable())
        th.join();
}

void ReadUserInput(Options& options)
{
    bool go = false;

    cout << "Valid options: \n"
        << "    sort[-a, -d]\n"
        << "    split[-s, -c, -n]\n"
        << "    go\n" << endl;

    do
    {
        std::string user_input;

        cout << "Enter your options: \n";
        getline(cin, user_input);

        if (user_input.compare("sort -a") == 0)
        {
            options.order = 'a';
        }
        else if (user_input.compare("sort -d") == 0)
        {
            options.order = 'd';
        }
        else if (user_input.compare("split -s") == 0)
        {
            options.spliting_char = 's';
        }
        else if (user_input.compare("split -c") == 0)
        {
            options.spliting_char = 'c';
        }
        else if (user_input.compare("split -n") == 0)
        {
            options.spliting_char = 'n';
        }
        else if (user_input.compare("go") == 0)
        {
            if ((options.order != ' ') && (options.spliting_char != ' '))
            {
                go = true;
            }
            else
            {
                cout << "Please select sort and split options first." << endl;
            }
        }
        else
        {
            cout << "Invalid entry! Please try again.\n"
                << "Valid options: \n"
                << "    sort[-a, -d]\n"
                << "    split[-s, -c, -n]\n"    
                << "    go\n" << endl;
        }

    } while (go == false);
}


int main(int argc, char** argv) 
{
    //Options options;
    //options.order = ' ';
    //options.spliting_char = ' ';

    //ReadUserInput(options);

    vector<string> input_file_paths;
    string output_file_path("F4.txt");

    for (int i = 1; i < argc; ++i)
    {
        input_file_paths.push_back(string(argv[i]));
    }

    InitializeCriticalSection(&file_cs);

    SpawnReadingThreads(input_file_paths);

    SpawnWritingThread(output_file_path);

    
    //std::cout << "Ordered map: " << endl;
    //for (auto const& pair : ordered_map)
    //{
    //    std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    //}

    // Do some important work in the main thread

	return 0;
}



