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
	ifstream f_igr, f_clusters;//, f2, f_ortho;
	ofstream f_cand;
	
	string line;
	vector<string> toks;
	string name;
	char strand;

	map<string, string> chr;
	map<string, int> len;

	f_igr.open(argv[1], ios::in);
	while (!f_igr.eof()){
		getline(f_igr, line);
		if (line.compare("")) {
			if (line[0] == '>') {
				toks = tokenize(line, '\t');
				name = toks[0].substr(1);
				//strand = toks[1][0];
			}
			else {
				chr[name] = line;
				len[name] = line.length();
			}
		}
	}
	f_igr.close();
	
	int start, end;
	f_clusters.open(argv[2], ios::in);
	f_cand.open(argv[3], ios::out);
	while (!f_clusters.eof()){
		getline(f_clusters, line);
		if (line.compare("") ) {
			toks = tokenize(line, '\t');
			{
				start = atoi(toks[1].c_str())-100 > 0 ? atoi(toks[1].c_str())-100 : 0;
				end = atoi(toks[2].c_str())+100 < len[toks[0]]-1 ? atoi(toks[2].c_str())+100 : len[toks[0]]-1;
				f_cand<<'>'<<toks[0]<<'|'<<start<<'|'<<end<<'|'<<toks[3]<<'|'<<toks[6]<<'\n';
				f_cand<<chr[toks[0]].substr(start, end-start+1)<<'\n';
			}
		}
	}
	f_clusters.close();
	f_cand.close();
	
	return 0;
}
