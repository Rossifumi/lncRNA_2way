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
	ifstream f_genome1, f_genome2;//, f2, f_ortho;
	ofstream f_seq, f_database, f_cands2;
	
	string line;
	vector<string> toks;
	string name;
	char strand;

	vector<string> rgn1, rgn2;
	map<string, string> seq1, seq2;
	map<string, int> len1, len2;

	f_genome1.open(argv[1], ios::in);
	while (!f_genome1.eof()){
		getline(f_genome1, line);
		if (line.compare("")) {
			if (line[0] == '>') {
				toks = tokenize(line, '\t');
				name = toks[0].substr(1);
				rgn1.push_back(name);
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
				rgn2.push_back(name);
				//strand = toks[1][0];
			}
			else {
				seq2[name] = line;
				len2[name] = line.length();
			}
		}
	}
	f_genome2.close();

	string rm_cmd;
	rm_cmd.append("rm -f ").append(argv[3]);
	system(rm_cmd.c_str());
	for (int i = 0; i < rgn1.size() && i < rgn2.size(); i++){
		f_seq.open("seq.fa", ios::out);
		f_seq<<'>'<<rgn1[i]<<'\n'
			  <<seq1[rgn1[i]]<<'\n'
			  <<'>'<<rgn2[i]<<'\n'
			  <<seq2[rgn2[i]]<<'\n';
		f_seq.close();
		system("clustalw2 seq.fa");
		string cat_cmd;
		cat_cmd.append("cat seq.aln >>").append(argv[3]);
		system(cat_cmd.c_str());
	}

	//f_cands2.close();
	//f_igr1.close();
	//f_igr2.close();

	return 0;
}
