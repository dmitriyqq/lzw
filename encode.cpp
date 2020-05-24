#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#define MAX_ASCII 255

using namespace std;




string decoder(vector<int>& input)
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
    f = fopen("smallText.txt", "rb");
    if (f == NULL) cout << "Error opening file";

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buffer = new char[size];
    fread(buffer, 1, size, f);
    fclose(f);
    //cout << buffer;
    cout << "Size : " << size;
    cout << "\n\n";

    vector<int> output = encoder(buffer);
    for (auto o : output) {
        cout << o << " ";
    }

    f = fopen("encoded.bin", "wb");
    fwrite(&output[0], sizeof(output[0]), output.size(), f);
    fclose(f);
    /*ofstream writef("encoded.bin", ios::out, ios::binary);
    writef.write(reinterpret_cast<const char*>(output.data()), output.size() * sizeof(output[0]));
    writef.close();*/

    ifstream readf("encoded.bin", ios::in | ios::binary);
    if (!readf) {
        cout << "Cannot open file!" << endl;
        return 1;
    }
    readf.seekg(0, readf.end);
    int length = readf.tellg();
    cout << "lenth :" << length << endl;
    readf.seekg(0, readf.beg);

    vector<int> encoded(length);
    readf.read(reinterpret_cast<char*>(encoded.data()), length * sizeof(encoded[0]));
    readf.close();

    f = fopen("decoded.txt", "wb");
    cout << "encoded: "<< encoded.size() << endl << endl;
    for (auto e : encoded) {
        cout << e << " ";
    }
    string decoded = decoder(encoded);
    //cout << decoded;
    fwrite(&decoded[0], sizeof(decoded[0]), decoded.size(), f);
    fclose(f);

    delete[] buffer;
    return 0;
}
