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
	ifstream f_genome;//, f2, f_ortho;
	ofstream f_one;
	
	bool first = true;
	string line;
	f_genome.open(argv[1], ios::in);
	f_one.open(argv[2], ios::out);
	while (!f_genome.eof()){
		getline(f_genome, line);
		if (line[0] == '>'){
			if (!first)
				f_one<<'\n';
			else
				first = false;
			f_one<<line<<'\n';
		}
		else
			f_one<<line;
	}
	f_one.close();
	f_genome.close();

	return 0;
}
