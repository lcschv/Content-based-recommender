#include "../lib/input.h"



LoadInput::LoadInput(string &ratingsfile, string &targetsfile, string &contentfile){
	readInputFiletoBuffer(ratingsfile);
	readTargetFile(targetsfile);
	loadUsers();
	loadStopWords();
	readContentSimple(contentfile);
	CalcUsersAverageRating();
	RecommenderImdb();
	
}


//Get user feature matrix..
void LoadInput::getUsermatrix(){
	for (auto it1:UsersMap){
		for (auto it2:tokensHash){
			double num = 0;
			int den = 0;
			for (auto it3:ItemTokens){
				if (it1.second.find(stoi(it3.first))!= it1.second.end() && it2.second.find(it3.first) != it2.second.end()){
					den++;
					num += it1.second[stoi(it3.first)] * it2.second[it3.first];
				}
			}
			(den!=0) ? userfeaturematrix[MapCorrectUserId[it1.first]][it2.first] = num/den:userfeaturematrix[MapCorrectUserId[it1.first]][it2.first] = 0;
			cout << it1.first <<" "<<it2.first <<" --> "<<userfeaturematrix[MapCorrectUserId[it1.first]][it2.first] << endl;
		}
	}
}

// Return list of tokens of each item of a user
void LoadInput::getlistoftokesofuser(){
	for (auto i:UsersMap){
		for (auto j:i.second){
			for (auto it:ItemTokens[MapCorrectItemId[j.first]]){
				ListOfTokensofaUser[i.first][it.first] = 1;		
			}
		}		
	}
}

// Aux function that prints ItemsTokensMap
void LoadInput::printItemTokens(){
	for (auto it:ItemTokens){
			cout << it.first<< "DocId ---> Tokens: ";
		for (auto it2:it.second){
			cout <<it2.first<<" ";
		}
		cout <<endl;
	}
}

// Aux function that prints MeansBased(Values from imdb ratings)
void LoadInput::printmeanBased(){
	for (auto it:meanBased){
			cout  <<"Item: "<< it.first<<"Rating: "<<it.second << endl;
		cout <<endl;
	}
}

// Aux function that prints all tokens of a user
void LoadInput::printListOfTokensofaUser(){
	for (auto it:ListOfTokensofaUser){
		cout <<"user: " <<it.first<< " ---> ";
		for (auto it2:it.second){
			cout <<it2.first<<" ";
		}
		cout <<endl;
	}
}

// Aux function that prints TokensHash(contains tokens->itens->tf-idf)
void LoadInput::printtokensHash(){
	for (auto it:tokensHash){
		cout << it.first<< " ---> ";
		for (auto it2:it.second){
			cout <<it2.first<<":"<<it2.second<< " ";
		}
		cout <<endl;
	}
}
void LoadInput::printTargetMap(){
	for (auto i:targetBuffer){
		cout <<"usuario:"<<i.user <<"	item:"<<i.item <<endl;
	};
};

void LoadInput::printUsersMap(){
	for (auto i:UsersMap){
		for (auto j:i.second){
			cout <<"usuario:"<<i.first <<"	item:"<<j.first<<"	rating:"<<j.second<<endl;
		}		
	}	
}
void LoadInput::printItemsMap(){
	for (auto i:ItemsMap){
		for (auto j:i.second){
			cout <<"item:"<<i.first <<"	usuario:"<<j.first<<"	rating:"<<j.second<<endl;
		}		
	}	
}
// Function that makes the recommendations for each target item.
void LoadInput::RecommenderImdb(){
	tupla nextTarget;
	double r = 0;
	nextTarget = getNextTarget();
	cout << "UserId:ItemId,Prediction"<<endl;
	while (nextTarget.item != -1){

		if (meanBased.find(MapCorrectItemId[nextTarget.item]) != meanBased.end()){
			r = meanBased[MapCorrectItemId[nextTarget.item]];
		}else if (UserAverageRating.find(nextTarget.user) != UserAverageRating.end()) {
			r = UserAverageRating[nextTarget.user];	
		}else{
			r = 6.7;
		}
		cout <<"u"<<MapCorrectUserId[nextTarget.user]<<":i"<<MapCorrectItemId[nextTarget.item]<<","<<r<<endl;
		nextTarget = getNextTarget();
	}
};

// Function that insert each token read from the json content into two maps tokensHash and ItemTokens
void LoadInput::insertToken(string token, string docid){
	auto search = stopwords.find(token);
	if(search == stopwords.end() && token.size() > 1 && token.size() < 20){
		numofwords++;
		DocIdLen[docid] += 1;
		if (tokensHash.find(token) == tokensHash.end()){
			tokensHash[token][docid] = 1;
			ItemTokens[docid][token] = 1;
	    }
		else{
			if (tokensHash[token].find(docid) == tokensHash[token].end()){
				tokensHash[token][docid] = 1;
				ItemTokens[docid][token] = 1;

			}
			else{
				ItemTokens[docid][token] = 1;

				tokensHash[token][docid] += 1;
			}
		}
	}else{
		return;
	}
}


// Makes the parser of each token, remove accents and separators. 
void LoadInput::ParserTokenizer(string s, string docid){
	typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
	boost::char_separator<char> sep{" ,\n\t\r"};
	remove_accents(s);
	tokenizer tok{s, sep};
	for (const auto &token : tok){
			insertToken(token, docid);
		}		
}


// Calculates the Tf*IDF and stores it in two Maps.
void LoadInput::calcTfIDF(){
	double idf = 0;
	float Tf = 0;
	for (auto it:tokensHash){
		float n = it.second.size();
		for (auto it2:it.second){
			idf = 0;
			idf = log2(numOfDocuments/n);
			Tf = it2.second / DocIdLen[it2.first];
			tokensHash[it.first][it2.first] = Tf*idf;
			ItemTokens[it2.first][it.first] = Tf*idf;
		}
		cout <<endl;
	}
}


// This function was used to calculate the Rocchio Recommender, but it wasn`t working very well.
// To slow to compute the recommendation
void LoadInput::readContent(string &contentfile){
	// 	string line;
	// 	ifstream file(contentfile);
	// 	getline(file,line);
	// 	string docid = ""; 
	// 	while (!file.eof()){
	// 		getline(file,line);
	// 		if (line != ""){

	// 			string docid = line.substr(1,7);
	// 			// cout <<"Filme ID: " <<line.substr(1,7) << endl;
	// 			// cout <<"Filme ID: " <<docid << endl;

	// 			// cout << line<<endl;
	// 			// cout << line.substr(9,line.length()-9);

	// 			auto v = json::parse(line.substr(9,line.length()-9));

	// 			//check if theres a value in the json
	// 			json::value response = v["Response"];
	// 			string resp = to_string(response);
	// 			if (response != "False"){		
	// 				numOfDocuments++;
	// 				// std::cout << stringify(v, json::PRETTY_PRINT) << std::endl;
	// 				// std::cout << "----------" << std::endl;
	// 				string s;

	// 				// // get a specific value
	// 				json::value z = v["Genre"];
	// 				// std::cout << to_number(z) << std::endl;
	// 				s = to_string(z);
	// 				// cout << "Generos.." <<s << endl;
	// 				ParserTokenizer(s, line.substr(1,7));
	// 				// cout << s << endl;		
	// 			}else{
	// 				cout << "FALSE"<<endl;
	// 			}
	// 		}
	// 	}
		
}

// Function that reads the JSON file and get the ImdbRating, store it in the meanBased map.
void LoadInput::readContentSimple(string &contentfile){
	string line;
	ifstream file(contentfile);
	getline(file,line);
	string docid = ""; 
	while (!file.eof()){
		getline(file,line);
		if (line != ""){

			string attributes = line.substr(9,line.length()-9);
			json j = json::parse(attributes);
			//check if theres a value in the json
			
			// string resp = to_string(response);
			if(j.find("imdbRating") != j.end() && j["imdbRating"] != "N/A"){
				string docid = line.substr(1,7);
				string x = j["imdbRating"];
				meanBased[docid] = stof(x);
			}
			
		}
	}
		
}

// Load stop words so we can be able to remove then from the tokens list
void LoadInput::loadStopWords(){
	string word;
	ifstream stopWordsFile;
	stopWordsFile.open("stopwords.txt");
	
	while (!stopWordsFile.eof()){
		getline(stopWordsFile,word);
		stopwords.insert(word);
	}
	stopWordsFile.close();
};

/*Method that reads the ratings.csv and load it into an buffer in the format of the quadruple [user,item,rating,timestamp]*/
void LoadInput::readInputFiletoBuffer(string &ratingsfile){
	ifstream inputfile;
	// cout <<ratingsfile<<endl;
	string line;
	vector<string> words (4);
    inputfile.open(ratingsfile);
    // Removing header of the file ..
    getline(inputfile,line);
    quadruple tempQuadruple;
    //Treating fields of the file ..
    while (!inputfile.eof()){
        getline(inputfile,line);
        if (line != ""){
			boost::split(words, line, boost::is_any_of(",:"));
			//removing character U from users.
			words[0].erase(0,1);
			//removing character i from itens.
			words[1].erase(0,1);
			tempQuadruple.user = stoi(words[0]);
			tempQuadruple.item = stoi(words[1]);
			tempQuadruple.rating = stoi(words[2]);
			tempQuadruple.timestamp = stoi(words[3]);
			MapCorrectUserId[stoi(words[0])] = words[0];
			MapCorrectItemId[stoi(words[1])] = words[1];
			bufferedFile.push_back(tempQuadruple);
		};
    };
};

/*Method that reads the target file, targets that we are supposed to estimate ratings*/
void LoadInput::readTargetFile(string &targetsfile){
	ifstream targetFile;
	string line;
	vector<string> words (4);
    targetFile.open(targetsfile);
    // Removing header of the file ..
    getline(targetFile,line);
    tupla tempTuple;
    //Treating fields of the file ..
    while (!targetFile.eof()){

        getline(targetFile,line);
        if (line != ""){
			boost::split(words, line, boost::is_any_of(":"));
			//removing character U from users.
			words[0].erase(0,1);
			//removing character i from itens.
			words[1].erase(0,1);
			tempTuple.user = stoi(words[0]);
			tempTuple.item = stoi(words[1]);
			MapCorrectUserId[stoi(words[0])] = words[0];
			MapCorrectItemId[stoi(words[1])] = words[1];
			targetBuffer.push_back(tempTuple);
		};
    };
};

// Fill up the UsersMap map from file ratings.csv
void LoadInput::loadUsers(){
	for (auto i:bufferedFile){
		UsersMap[i.user][i.item] = i.rating; 
	}
};

// Get the next target to be estimated
tupla LoadInput::getNextTarget(){
	tupla tempTuple;
	if (!targetBuffer.empty()){
		tempTuple = targetBuffer.back();
		targetBuffer.pop_back();
		return tempTuple;
	} else {
		tempTuple.user = -1;
		tempTuple.item = -1;
		return tempTuple;
	}
}

/*Method that was used previously to read the json(to calculate rocchio)*/
void LoadInput::readJson(){
	// string line;
	// ifstream file("teste.json");
	// while (!file.eof()){
	// 	getline(file,line);

	// 	auto v = json::parse(line);

	// 	std::cout << stringify(v, json::PRETTY_PRINT) << std::endl;
	// 	std::cout << "----------" << std::endl;
	// 	string s;
	// 	// get a specific value
	// 	json::value z = v["Plot"];
	// 	// std::cout << to_number(z) << std::endl;
	// 	s = to_string(z);
	// 	cout << s << endl;
	// }
}


// Method that removes accents, special characters, etc..
void LoadInput::remove_accents(string &str) {
    for(unsigned int i=0;i<str.length();i++) {
        str.at(i) = tolower(str.at(i));
        unsigned char c = str.at(i);
        if ((c == 0xc3) && ((i+1)<str.length())) {
            str.erase (i,1);
            c = str.at(i);
            c = tolower(c);
        }
        if ((c >= 0x30 && c <= 0x39) || (str.at(i) >= 0x61 && str.at(i) <= 0x7a)) {
            //à, á, â, ã, ä
        }else if (( c >= 0xa0 && c <= 0xa4)){
            str[i]='a';
            //ç
        }else if (c == 0xa7) {
            str[i]='c';
            //è, é, ê , ë
        } else if ((c >= 0xa8 && c <= 0xab)){
            str[i]='e';
            //ì, í, î, ï
        } else if ((c >= 0xac && c <= 0xaf)){
            str[i]='i';
            //ñ
        } else if (c == 0xb1) {
            str[i]='n';
            //ò, ó, ô, õ, ö
        } else if ((c >= 0xb2 && c <= 0xb6)){
            str[i]='o';
            //ù, ú, û, ü
        } else if ((c >= 0xb9 && c <= 0xbc)){
            str[i]='u';
            //Se nao for espaco
        }
        else if(c!=0x20){
            unsigned int x;
            x=str[i];
            if((x>=4294967265)&&(x<=4294967270)){ str[i]='a';}else
                if((x>=4294967272)&&(x<=4294967275)){ str[i]='e';}else
                    if((x>=4294967276)&&(x<=4294967279)) {str[i]='i';}else
                        if(((x>=4294967282)&&(x<=4294967287))||(x==4294967280)){ str[i]='o';}else
                            if(x==4294967281){ str[i]='n';}else
                                if((x>=4294967289)&&(x<=4294967292)) {str[i]='u';}else
                                    if(x==4294967271){ str[i]='c';}else{str.replace (i,1," ");}
        }
    }
};

// Calculates the average rating of each user
void LoadInput::CalcUsersAverageRating(){
	double sum = 0;
	double sum2 = 0;
	double useraverage = 0;
	int denom = 0;
	int denomb = 0;

	for (auto it:UsersMap){
		for (auto it2:UsersMap[it.first]){
			denom += 1;
			sum += it2.second;
		}
		useraverage = sum / denom;
		UserAverageRating[it.first] = useraverage;
		denom = 0;
		useraverage = 0;
		sum = 0;
	}
};