#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

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
	ifstream f_seq0, f_seq1;//, f_genome2, f_blast;//, f2, f_ortho;
	ofstream f_seq0_3way, f_seq1_3way;
	
	string line;
	vector<string> toks, toks2;//, toks3;
	string name, name2;//, name3;
	string name2_a, name2_b;
	
	vector<string> rgn0, rgn1;
	map<string, string> seq0, seq1;
	map<string, int> len0, len1;
	map<string, string> ocr1;
	map<string, int> ocr_flag1;

	f_seq0.open(argv[1], ios::in);
	while (!f_seq0.eof()){
		getline(f_seq0, line);
		if (line.compare("")) {
			if (line[0] == '>') {
				toks = tokenize(line, '\t');
				//cout<<toks.size()<<'\n';
				name = toks[0].substr(1);
				rgn0.push_back(name);
			}
			else {
				seq0[name] = line;
				len0[name] = line.length();
			}
		}
	}
	f_seq0.close();
	
	f_seq1.open(argv[2], ios::in);
	while (!f_seq1.eof()){
		getline(f_seq1, line);
		if (line.compare("")) {
			if (line[0] == '>') {
				toks = tokenize(line, '\t');
				//cout<<toks.size()<<'\n';
				name = toks[0].substr(1);
				toks2 = tokenize(name, ':');
				//cout<<toks2.size()<<" **\n";
				//name2 = toks2[3];
				//toks3 = tokenize(name2, ':');
				//cout<<toks3.size()<<" ##\n";
				//cout<<name2<<'\n';
				name2_a = toks2[2];
				name2_b = toks2[3];
				name2 = name2_a.append(":").append(name2_b);
				rgn1.push_back(name);
				//cout<<name2<<'\n';
				if (ocr_flag1[name2] == 0) {
					ocr1[name2] = name;
					ocr_flag1[name2] = 1;
				}
				else
					ocr_flag1[name2] = 2;
				//strand = toks[1][0];
			}
			else {
				seq1[name] = line;
				len1[name] = line.length();
			}
		}
	}
	f_seq1.close();

	f_seq0_3way.open(argv[3], ios::out);
	f_seq1_3way.open(argv[4], ios::out);
	for (int i = 0; i < rgn0.size(); i++) {
		//cout<<rgn0[i]<<'\n';
		//cout<<ocr_flag1[rgn0[i]]<<'\n';
		if (ocr_flag1[rgn0[i]] == 1) {// && ocr_flag2[rgn0[i]] == 1 ) {
			f_seq0_3way<<'>'<<rgn0[i]<<'\n'<<seq0[rgn0[i]]<<'\n';
			f_seq1_3way<<'>'<<ocr1[rgn0[i]]<<'\n'<<seq1[ocr1[rgn0[i]]]<<'\n';
			//f_seq2_3way<<'>'<<ocr2[rgn0[i]]<<'\n'<<seq2[ocr2[rgn0[i]]]<<'\n';
		}
	}
	f_seq0_3way.close();
	f_seq1_3way.close();
			
	//f_cands2.close();
	//f_igr1.close();
	//f_igr2.close();

	return 0;
}
