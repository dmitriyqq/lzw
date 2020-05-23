// DecoderCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <algorithm>

#define MAX_ASCII 255

using namespace std;



vector<unsigned short int> encoder(string input)
{
    unordered_map<string, unsigned short int> dict;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        dict[ch] = i;
    }

    string current = "";
    string next = "";
    current += input[0];
    int coder = MAX_ASCII + 1;
    vector<unsigned short int> output;
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
    // add the last word
    output.push_back(dict[current]);
    return output;
}


string decoder(vector<unsigned short int>& input)
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
    FILE* f;
    f = fopen("largeText.txt", "rb");
    if (f == NULL) cout << "Error opening file";

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buffer = new char[size];
    fread(buffer, 1, size, f);
    fclose(f);
    //cout << buffer;
    cout << "Number of char in input : " << size;
    cout << "\n\n";

    vector<unsigned short int> output = encoder(buffer);
    auto max = output[0];
    for (const auto i : output) {
        if (max >= i) max = i;
    }
    cout << "Max code = " << max << endl;
    /*for (auto o : output) {
        cout << o << " ";
    }*/

    f = fopen("encoded.bin", "wb");
    fwrite(&output[0], sizeof(output[0]), output.size(), f);
    fclose(f);
    /*ofstream writef("encoded.bin", ios::out, ios::binary);
    writef.write(reinterpret_cast<const char*>(output.data()), output.size() * sizeof(output[0]));
    writef.close();*/

    ifstream readf("encoded.bin", ios::in, ios::binary);
    if (!readf) {
        cout << "Cannot open file!" << endl;
        return 1;
    }

    readf.seekg(0, readf.end);
    int length = readf.tellg();
    cout << "lenth :" << length << endl;
    readf.seekg(0, readf.beg);

    vector<unsigned short int> encoded(length);
    readf.read(reinterpret_cast<char*>(encoded.data()), length * sizeof(encoded[0]));
    readf.close();
    cout << endl << endl;
    /*for (auto e : encoded) {
        cout << e << " ";
    }*/

    f = fopen("decoded.txt", "wb");
    string decoded = decoder(encoded);
    //cout << decoded;
    fwrite(&decoded[0], sizeof(decoded[0]), decoded.size(), f);
    fclose(f);

    delete[] buffer;
    return 0;
}
