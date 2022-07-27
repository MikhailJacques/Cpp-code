
#include <map>
#include <mutex>
#include <vector>
#include <string>
#include <thread>
#include <fstream>
#include <iostream>
#include <algorithm>
#include<windows.h>

using namespace std;

// User input options
typedef struct
{
    unsigned char order;            // a = ascending, d = descending
    unsigned char spliting_char;    // s = white space, c = coma, n = new line
} Options;

typedef struct
{
    Options options;
    vector<string> input_files_paths;
    string output_file_path;
} Params;

void SpawnReadingThreads(const vector<string>& paths);
void ReadTextFile(const std::string& file_path);

void SpawnWritingThread(const Params& params);
void WriteTextFile(const Params& params);

void ReadUserInput(Options& options);
void RemoveSpecialCharacters(string& token);

std::mutex mtx;

CRITICAL_SECTION file_cs;

std::map<std::string, unsigned int> ordered_map_asc;

int main(int argc, char** argv) 
{
    Params params;
    params.output_file_path.assign("F4.txt");

    // No error checking is done
    for (int i = 1; i < argc; ++i)
    {
        params.input_files_paths.push_back(string(argv[i]));
    }

    ReadUserInput(params.options);

    InitializeCriticalSection(&file_cs);

    SpawnReadingThreads(params.input_files_paths);

    SpawnWritingThread(params);

	return 0;
}

void SpawnReadingThreads(const vector<string>& input_files_paths)
{
    std::vector<std::thread> threads;
    threads.reserve(input_files_paths.size());

    for (int i = 0; i < input_files_paths.size(); i++)
    {
        std::thread th(&ReadTextFile, input_files_paths[i]);
        threads.push_back(std::move(th));
    }

    for (std::thread& th : threads)
    {
        if (th.joinable())
            th.join();
    }
}

void ReadTextFile(const std::string& file_path)
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

                // std::lock_guard<std::mutex> guard(mtx); // RAII, exeption safe
                mtx.lock();
                ordered_map_asc[token]++;
                mtx.unlock();
            }

            file.close();
        }
    }
}

void SpawnWritingThread(const Params& params)
{
    std::thread th(&WriteTextFile, params);

    if (th.joinable())
        th.join();
}

// void WriteTextFile(const std::string& file_path)
void WriteTextFile(const Params& params)
{
    ofstream output_file(params.output_file_path, ios::out);

    if (output_file.is_open())
    {
        string most_frequent_word;
        unsigned int most_frequent_word_cnt = 1;

        if (params.options.order == 'a')
        {
            for (auto const& pair : ordered_map_asc)
            {
                if (most_frequent_word_cnt < pair.second)
                {
                    most_frequent_word.assign(pair.first);
                    most_frequent_word_cnt = pair.second;
                }

                output_file << pair.first << "\n";
            }
        }
        else
        {
            std::map<std::string, unsigned int, greater<string>> ordered_map_desc(ordered_map_asc.begin(), ordered_map_asc.end());

            for (auto const& pair : ordered_map_desc)
            {
                if (most_frequent_word_cnt < pair.second)
                {
                    most_frequent_word.assign(pair.first);
                    most_frequent_word_cnt = pair.second;
                }

                output_file << pair.first << "\n";
            }
        }

        output_file << "The most frequent word in the text is: '"
            << most_frequent_word << "', count: " << most_frequent_word_cnt;

        output_file.close();
    }
}

void ReadUserInput(Options& options)
{
    bool go = false;
    std::string user_input;

    options.order = ' ';
    options.spliting_char = ' ';

    cout << "Valid options: \n"
        << "    sort[-a, -d]\n"
        << "    split[-s, -c, -n]\n"
        << "    go\n" << endl;

    cout << "Enter your options: \n";
    getline(cin, user_input);

    do
    {
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
                << "    go\n"
                << "Enter your options: \n";
        }

        if (go == false)
            getline(cin, user_input);

    } while (go == false);
}

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