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
	ifstream f_genome1, f_genome2, f_cands1;//, f2, f_ortho;
	ofstream f_query, f_database, f_cands2;
	
	string line;
	vector<string> toks;
	string name;
	char strand;

	vector<string> rgn1, rgn2;
	map<string, string> chr1, chr2;
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
				chr1[name] = line;
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
				chr2[name] = line;
				len2[name] = line.length();
			}
		}
	}
	f_genome2.close();

	map<string,string> cons_rgn;
	for (int i = 0; i < chr1.size(); i++)
		cons_rgn[rgn1[i]] = rgn2[i];
	
	string cand1_reg, cand2_reg;
	bool cons_ext;
	f_cands1.open(argv[3], ios::in);
	//f_cands2.open(argv[4], ios::in);
	//f_igr1.open(argv[4], ios::out);
	//f_igr2.open(argv[5], ios::out);
	string rm_cmd;
	rm_cmd.append("rm -f ").append(argv[4]);
	system(rm_cmd.c_str());
	while (!f_cands1.eof()){
		getline(f_cands1, line);
		if (line.compare("") ) {
			if (line[0] == '>') {
				toks = tokenize(line, '|');
				cand1_reg = toks[0].substr(1).append("|").append(toks[1]);
				//cand1_name = line.substr(1);
				cand2_reg = cons_rgn[cand1_reg];
				if (cand2_reg.compare("")) {
					cons_ext = true;
					f_query.open("q.fa", ios::out);
					f_database.open("d.fa", ios::out);
					f_query<<line<<'\n';
					f_database<<'>'<<cand2_reg<<'\n';
				}
				else
					cons_ext = false;
			}
			else {
				if (cons_ext) {
					f_query<<line<<'\n';
					f_database<<chr2[cand2_reg]<<'\n';
					f_query.close();
					f_database.close();
					system("formatdb -p F -i q.fa");
					system("formatdb -p F -i d.fa");
					system("blast -p blastn -i q.fa -d d.fa -m9 -o BLAST.out");
					string cat_cmd;
					cat_cmd.append("cat BLAST.out >>").append(argv[4]);
					system(cat_cmd.c_str());
				}
			}
		}
	}
	f_cands1.close();
	//f_cands2.close();
	//f_igr1.close();
	//f_igr2.close();

	return 0;
}
