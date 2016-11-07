#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stdlib.h>

using namespace std;

vector<string> tokenize(string input_string, char delimiter)
{
    vector<string> tokens;
    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = input_string.find (delimiter, start)) != string::npos)
    {
      {
        string substring=input_string.substr (start, end-start);
	if (end != start)
        tokens.push_back (substring);
      }
      start = end + 1;
    }
    if (start==end-1)
    {
      tokens.push_back("");
    }
    else
    {
      string substring=input_string.substr(start, end-start);
      tokens.push_back(substring);
    }
    return tokens;
}



int main(int argc, char** argv) {
	ifstream f_blast;//, f2, f_ortho;
	ofstream f_filtered;

	string line;
	vector<string> toks;

	double cutoff = atof(argv[2]);
	f_blast.open(argv[1], ios::in);
	f_filtered.open(argv[3], ios::out);
	while (!f_blast.eof()){
		getline(f_blast, line);
		if (line.compare("")) {
			toks = tokenize(line, '\t');
			if (toks.size() >= 11 && atof(toks[10].c_str())<=cutoff)
				f_filtered<<line<<'\n';
		}
	}
	f_blast.close();
	f_filtered.close();
	
	return 0;
}
