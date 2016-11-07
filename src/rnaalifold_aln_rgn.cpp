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
	ifstream f_seq, f_aln;//, f2, f_ortho;
	ofstream f_foldin, f_foldout;
	
	string line;
	vector<string> toks;
	string name;
	char strand;

	vector<string> names, rgn2;
	map<string, string> seq1, seq2;
	map<string, int> len1, len2;

	f_seq.open(argv[1], ios::in);
	while (!f_seq.eof()){
		getline(f_seq, line);
		if (line.compare("")) {
			if (line[0] == '>') {
				toks = tokenize(line, '\t');
				name = toks[0].substr(1);
				names.push_back(name);
				//strand = toks[1][0];
			}
			else {
				seq1[name] = line;
				len1[name] = line.length();
			}
		}
	}
	f_seq.close();

	string rm_cmd;
	rm_cmd.append("rm -f ").append(argv[3]).append(" ").append(argv[4]);
	system(rm_cmd.c_str());
	string mkdir_cmd;
	mkdir_cmd.append("mkdir ").append(argv[4]);
	system(mkdir_cmd.c_str());
	string RNAalifold_cmd;
	RNAalifold_cmd.append("RNAalifold --color in.aln >> ").append(argv[3]);
	int cc=0;
	f_aln.open(argv[2], ios::in);
	f_foldin.open("in.aln", ios::out);
	while (!f_aln.eof()){
		getline(f_aln, line);
		if (line.compare("")) {
			toks = tokenize(line, ' ');
			if (!toks[0].compare("CLUSTAL")) {
				if (cc > 0) {
					f_foldout.open(argv[3], ios::app);
					f_foldout<<"### "<<names[cc-1]<<'\n';
					f_foldout.close();
					f_foldin.close();
					system(RNAalifold_cmd.c_str());
					f_foldout.open(argv[3], ios::app);
					f_foldout<<'\n';
					f_foldout.close();
					stringstream mv_cmd_ss;
					mv_cmd_ss<<"mv alirna.ps "<<argv[4]<<"/"<<cc<<".ps";
					system(mv_cmd_ss.str().c_str());
					f_foldin.open("in.aln", ios::out);
				}
				cc++;
			}
		}
		f_foldin<<line<<'\n';
	}
	f_foldout.open(argv[3], ios::app);
	f_foldout<<"### "<<names[cc-1]<<'\n';
	f_foldout.close();
	f_foldin.close();
	system(RNAalifold_cmd.c_str());
	f_foldout.open(argv[3], ios::app);
	f_foldout<<'\n';
	f_foldout.close();
	stringstream mv_cmd_ss;
	mv_cmd_ss<<"mv alirna.ps "<<argv[4]<<"/"<<cc<<".ps";
	system(mv_cmd_ss.str().c_str());
	f_aln.close();

	return 0;
}
