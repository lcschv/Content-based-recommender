#include <fstream>
#include <limits>
#include <iostream>
#include <string>
#include <istream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include "lib/input.h"
using namespace std;
// #include "lib/cpp-json/json.h"



int main(int argc,char *argv[]){
	

	if (argc != 4){
		cout << "Wrong parameters, please use ./recommender ratings.csv targets.csv content.csv > submission.csv" <<endl;
	return 0;
	}

		string ratingsfile(argv[1]);
		string targetsfile(argv[2]);
		string contentfile(argv[3]);

	LoadInput loadinput(ratingsfile,targetsfile, contentfile);



	return 0;
}

