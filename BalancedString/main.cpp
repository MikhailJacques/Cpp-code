// C++ program for the above approach

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
#include <unordered_map>
#include <time.h>

using namespace std;

// Function to check if the current string is balanced or not
bool balanced(int small[], int caps[])
{
    // For every character, check if
    // there exists uppercase as well
    // as lowercase characters
    for (int i = 0; i < 26; i++) {

        if (small[i] != 0 && (caps[i] == 0))
            return 0;

        else if ((small[i] == 0) && (caps[i] != 0))
            return 0;
    }
    return 1;
}

int solution(string& S) {
    // write your code in C++14 (g++ 6.2.0)

    // Store frequency of lowercase characters
    int small[26];

    // Stores frequency of  uppercase characters
    int caps[26];

    for (int i = 0; i < 26; i++)
        small[i] = caps[i] = 0;

    // Count frequency of characters
    for (int i = 0; i < S.length(); i++)
    {
        if (S[i] >= 65 && S[i] <= 90)
            caps[S[i] - 'A']++;
        else
            small[S[i] - 'a']++;
    }

    // Mark those characters whichn are not present in both lowercase and uppercase
    std::unordered_map<char, int> mp;
    for (int i = 0; i < 26; i++)
    {
        if (small[i] && !caps[i])
            mp[char(i + 'a')] = 1;

        else if (caps[i] && !small[i])
            mp[char(i + 'A')] = 1;
    }

    // Initialize the frequencies back to 0
    for (int i = 0; i < 26; i++)
        small[i] = caps[i] = 0;

    // Marks the start and end of current substring
    int i = 0, st = 0;

    // Marks the start and end
    // of required substring
    int start = -1, end = -1;

    // Stores the length of smallest balanced substring
    int minm = 2147483647;

    while (i < S.length())
    {
        if (mp[S[i]])
        {
            // Remove all characters
            // obtained so far
            while (st < i)
            {
                if (S[st] >= 65 && S[st] <= 90)
                    caps[S[st] - 'A']--;
                else
                    small[S[st] - 'a']--;

                st++;
            }
            i += 1;
            st = i;
        }
        else
        {
            if (S[i] >= 65 && S[i] <= 90)
                caps[S[i] - 'A']++;
            else
                small[S[i] - 'a']++;

            // Remove extra characters from
            // front of the current substring
            while (1)
            {
                if (S[st] >= 65 && S[st] <= 90
                    && caps[S[st] - 'A'] > 1)
                {
                    caps[S[st] - 'A']--;
                    st++;
                }
                else if (S[st] >= 97 && S[st] <= 122 && small[S[st] - 'a'] > 1)
                {
                    small[S[st] - 'a']--;
                    st++;
                }
                else
                    break;
            }

            // If substring (st, i) is balanced
            if (balanced(small, caps))
            {
                if (minm > (i - st + 1))
                {
                    minm = i - st + 1;
                    start = st;
                    end = i;
                }
            }
            i += 1;
        }
    }


    int balanced_string_len = -1;

    // No balanced substring
    if (start == balanced_string_len || end == balanced_string_len)
    {

    }
    else  // Store answer string
    {
        string ans = "";
        for (int i = start; i <= end; i++)
        {
            ans += S[i];
        }

        balanced_string_len = end - start + 1;

    }

    return balanced_string_len;
}

// Driver Code
int main()
{

    // Given string
    string s1 = "azABaabza";
    string s2 = "AcZCbaBz";

    cout << "Balanced string 1 length: " << solution(s1) << endl;
    cout << "Balanced string 2 length: " << solution(s2) << endl;

    return 0;
}