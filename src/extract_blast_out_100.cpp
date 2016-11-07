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
	ifstream f_genome1, f_genome2, f_blast;//, f2, f_ortho;
	ofstream f_cand_core, f_cand_100;
	
	string line;
	vector<string> toks;
	string name;

	//vector<string> rgn;
	map<string, string> seq1, seq2;
	map<string, int> len1, len2;
	
	f_genome1.open(argv[1], ios::in);
	while (!f_genome1.eof()){
		getline(f_genome1, line);
		if (line.compare("")) {
			if (line[0] == '>') {
				toks = tokenize(line, '\t');
				name = toks[0].substr(1);
				//rgn.push_back(name);
				//strand = toks[1][0];
			}
			else {
				seq1[name] = line;
				len1[name] = line.length();
			}
		}
	}
	f_genome1.close();
	
	f_genome2.open(argv[2], ios::in);
	while (!f_genome2.eof()){
		getline(f_genome2, line);
		if (line.compare("")) {
			if (line[0] == '>') {
				toks = tokenize(line, '\t');
				name = toks[0].substr(1);
				//rgn.push_back(name);
				//strand = toks[1][0];
			}
			else {
				seq2[name] = line;
				len2[name] = line.length();
			}
		}
	}
	f_genome2.close();

	map<string, string> rgn, rgn_homo;
	map<string, int> start1, end1, start2, end2;
	set<string> names;
	vector<string> cands;
	string homo;
	
	f_blast.open(argv[3], ios::in);
	while (!f_blast.eof()){
		getline(f_blast, line);
		if (line.compare("")) {
			toks = tokenize(line, '\t');
			name = string(toks[0]).append("::").append(toks[1]);
			if (names.count(name) == 0) {
				names.insert(name);
				cands.push_back(name);
				rgn[name] = toks[1];
				rgn_homo[name] = toks[0];
				start1[name] = atoi(toks[6].c_str());
				end1[name] = atoi(toks[7].c_str());
				start2[name] = atoi(toks[8].c_str());
				end2[name] = atoi(toks[9].c_str());
			}
			else {
				if (atoi(toks[6].c_str()) > end1[name] && atoi(toks[8].c_str()) > end2[name]) {
					end1[name] = atoi(toks[7].c_str());
					end2[name] = atoi(toks[9].c_str());
				}
				else if (atoi(toks[7].c_str()) < start1[name] && atoi(toks[9].c_str()) < start2[name]) {
					start1[name] = atoi(toks[6].c_str());
					start2[name] = atoi(toks[8].c_str());
				}
			}
		}
	}
	f_blast.close();

	f_cand_100.open(argv[4], ios::out);
	for (int i = 0; i < cands.size(); i++) {
		int ss, ee;
		ss = start2[cands[i]]-101;
		if (ss < 0)
			ss = 0;
		ee = end2[cands[i]]+99;
		if (ee > len2[rgn[cands[i]]]-1)
			ee = len2[rgn[cands[i]]]-1;
		f_cand_100<<'>'<<rgn[cands[i]]<<'|'<<ss<<'|'<<ee<<"::"<<rgn_homo[cands[i]]<<'\n';
		f_cand_100<<seq2[rgn[cands[i]]].substr(ss, ee-ss+1)<<'\n';
	}
	f_cand_100.close();
			
	//f_cands2.close();
	//f_igr1.close();
	//f_igr2.close();

	return 0;
}
