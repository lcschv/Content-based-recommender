#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <set>
#include "json.hpp"
using json = nlohmann::json;
// #include "cpp-json/json.h"

using namespace std;

struct quadruple{
	int user;
	int item;
	int rating;
	int timestamp;
};
struct tupla{
	int user;
	int item;
};
struct tuplaUserScore{
	int user;
	int rating;
};
struct tuplaItemScore{
	int item;
	int rating;
};
struct idDf{
	int id;
	int df;
};
class LoadInput{
	private:
		std::vector<quadruple> bufferedFile;
		int vocTermId = 0	; 
		vector<tupla> targetBuffer;
		int numofwords = 0;
		unordered_map <string, unordered_map<string, double>> tokensHash; // Hash que reserva para cada token um map;
		int numOfDocuments = 0;
		unordered_map<string, unordered_map<string,double>> userfeaturematrix;
		unordered_map<string, unordered_map<string,int>> ItemTokens;
		unordered_map<int,unordered_map<string,int>> ListOfTokensofaUser;
		unordered_map<string,double> DocIdLen;
		unordered_map<int,unordered_map<int,double>> UsersMap;
		unordered_map<int,unordered_map<int,int>> ItemsMap;
		unordered_map<int,string> MapCorrectUserId;
		unordered_map<int,string> MapCorrectItemId;
		unordered_map<int,unordered_map<int,double>> AnswerMap;
		unordered_map <string, idDf> VocabularyIdRef;
		set<string> stopwords;
		unordered_map <int, string> TermIdRef; // hash que guarda termIdRef[IDDOTERMO] = token; = 0	; 
		unordered_map<string , float> meanBased;
		unordered_map<int, double> UserAverageRating;

	public:
		
		LoadInput(string &ratingsfile, string &targetsfile, string &contentfile);
	
		void readInputFiletoBuffer(string &ratingsfile);
		void readTargetFile(string &targetsfile);
		void calcUserFeatureMatrix();
		void loadUsers();
		void loadItems();
		void printTargetMap();
		void printUsersMap();
		void printItemsMap();
		tupla getNextTarget();
		void insertToken(string token, string docid);
		void loadStopWords();
		void readJson();
		void remove_accents(string &str);
		void readContent(string &contentfile);
		void ParserTokenizer(string s, string docid);
		void printtokensHash();
		void calcTfIDF();
		void getlistoftokesofuser();
		void printListOfTokensofaUser();
		void printItemTokens();
		void getUsermatrix();
		void readContentSimple(string &contentfile);
		void printmeanBased();
		void CalcUsersAverageRating();
		void RecommenderImdb();
};

#endif