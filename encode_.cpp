// DecoderCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#define MAX_ASCII 255

using namespace std;



vector<int> encoder(string input)
{
    unordered_map<string, int> dict;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        dict[ch] = i;
    }

    string current = "";
    string next = "";
    current += input[0];
    int coder = MAX_ASCII + 1;
    vector<int> output;
    //cout << "String\tOutput_Code\tAddition\n";
    for (int i = 0; i < input.length(); i++) {
        // check if the last letter
        if (i != input.length() - 1)
            next += input[i + 1];

        // if in dictionary found this combination, then continue to add next char to make new word
        if (dict.find(current + next) != dict.end()) {
            current = current + next;
        }
        else {
            //cout << current << "\t" << dict[current] << "\t\t"
            //    << current + next << "\t" << coder << endl;

            // if not found, add current code to output
            output.push_back(dict[current]);

            // set new word to dictionary to this code
            dict[current + next] = coder;

            // new code, new word
            coder++;

            //set current word to the next char
            current = next;
        }
        // reset next char
        next = "";
    }
    // cout << current << "\t" << dict[current] << endl;
    // add the last word
    return output;
}


string decoder(vector<int> input)
{
    struct Library
    {
        int code;
        string letters = "";
        Library(int code, string letters)
        {
            this->code = code;
            this->letters += letters;
        };
    };
    vector<Library> libs;

    // initialize library with ASCII characters from 0 to 255
    for (int i = 0; i <= MAX_ASCII; i++)
    {
        string s;
        s += char(i);
        libs.push_back(Library(i, s));
    }

    // decoding part

    int coder = MAX_ASCII + 1;
    string current = "";
    string output = "";

    for (int i = 0; i < input.size(); i++)
    {
        current += libs[input[i]].letters;
        output += current;

        // the first half of the decoding string will be 'current'
        Library newLib = Library(coder, current);
        libs.push_back(newLib);

        // the second half will be the first char of the next lib
        if (i < input.size() - 1)
            libs[coder].letters += libs[input[i + 1]].letters[0];

        //cout << "---------------------------"<<endl;
        coder++; // next library
        current = "";
    }

    return output;
}

int main()
{
    std::string input = "ABRACADABRACADA";
    vector<int> output = encoder(input);
    for (auto o : output) {
        cout << o << " ";
    }
    cout << endl << decoder(output);
    //ifstream myfile("text.txt");
    //string input_data;

    //while (getline(myfile, input_data)) {
    //    cout << input_data;
    //}

    //vector<int> out = encoder(input_data);

    //string after_decode = decoder(out);

    //for (auto i : out) cout << i << " ";
    //cout << "\n-----------------------------\n";
    //cout << after_decode;
    return 0;
}
