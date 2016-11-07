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
	ifstream f_fwd;//, f2, f_ortho;
	ofstream f_nuc;
	
	bool first = true;
	string line;
	vector<string> toks;
	bool rev;
	f_fwd.open(argv[1], ios::in);
	f_nuc.open(argv[2], ios::out);
	while (!f_fwd.eof()){
		getline(f_fwd, line);
		if (line[0] == '>'){
			f_nuc<<line<<'\n';
			toks=tokenize(line, '|');
			if (toks[1].compare("+"))
				rev=true;
			else
				rev=false;
		}
		else {
			if (rev)
				for (int i = line.length()-1; i >= 0; i--) 
					switch (line[i]) {
						case 'A': f_nuc<<'T';
							break;
						case 'T': f_nuc<<'A';
							break;
						case 'C': f_nuc<<'G';
							break;
						case 'G': f_nuc<<'C';
							break;
						case 'a': f_nuc<<'t';
							break;
						case 't': f_nuc<<'a';
							break;
						case 'c': f_nuc<<'g';
							break;
						case 'g': f_nuc<<'c';
							break;
						case 'R': f_nuc<<'Y';
							break;
						case 'Y': f_nuc<<'R';
							break;
						case 'r': f_nuc<<'y';
							break;
						case 'y': f_nuc<<'r';
							break;
						default: f_nuc<<'N';
					}
			else
				f_nuc<<line;
			f_nuc<<'\n';
		}
	}
	f_nuc.close();
	f_fwd.close();

	return 0;
}
