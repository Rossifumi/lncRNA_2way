#include <iostream>
#include <fstream>
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
	ifstream f1, f2, f_ortho;
	f1.open(argv[1], ios::in);
	f2.open(argv[2], ios::in);
	f_ortho.open(argv[3], ios::in);

	vector<string> gene1, gene2;
	map<string, string> chr1, chr2;
	map<string, char> stn1, stn2;
	map<string, int> ord1, ord2;
	
	string line;
	vector<string> toks;
	while (!f1.eof()){
		getline(f1, line);
		if (line.compare("")) {
			toks = tokenize(line, ' ');
			if (toks.size() > 1) {
				gene1.push_back(toks[0]);
				chr1[toks[0]] = toks[1];
				stn1[toks[0]] = toks[2][0];
				ord1[toks[0]] = atoi(toks[3].c_str());
			}
		}
	}

	while (!f2.eof()){
		getline(f2, line);
		if (line.compare("")) {
			toks = tokenize(line, ' ');
			if (toks.size() > 1) {
				gene2.push_back(toks[0]);
				chr2[toks[0]] = toks[1];
				stn2[toks[0]] = toks[2][0];
				ord2[toks[0]] = atoi(toks[3].c_str());
			}
		}
	}

	map<string, string> ortho;
	while (!f_ortho.eof()){
		getline(f_ortho, line);
		if (line.compare("")) {
			toks = tokenize(line, '\t');
			if (toks.size() > 1) 
				ortho[toks[0]] = toks[1];
		}
	}

	for (int i = 0; i < gene1.size()-1; i++)
		if (!chr1[gene1[i]].compare(chr1[gene1[i+1]]) && stn1[gene1[i]] == stn1[gene1[i+1]])
			if (ortho[gene1[i]].compare("") && ortho[gene1[i+1]].compare(""))
				if (!chr2[ortho[gene1[i]]].compare(chr2[ortho[gene1[i+1]]]) && stn2[ortho[gene1[i]]] == stn2[ortho[gene1[i+1]]])
					if (ord2[ortho[gene1[i]]] + 1 == ord2[ortho[gene1[i+1]]])// || ord2[ortho[gene1[i]]] - 1 == ord2[ortho[gene1[i+1]]])
						cout<<gene1[i]<<'\t'<<gene1[i+1]<<'\t'<<ortho[gene1[i]]<<'\t'<<ortho[gene1[i+1]]<<'\n';

	return 0;
}
