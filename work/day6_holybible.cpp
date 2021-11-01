#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <fstream>
#include <map>
#include <unordered_map>
using namespace std;

class Dictionary
{
public:
    void read(const std::string &filename);
    void store(const std::string &filename);

private:
	unordered_map<string,int> _dict;
};

void Dictionary:: read(const std::string &filename){
	ifstream ifs(filename);
	if(!ifs.good()){ 
        cerr << "ifstream is not good" << endl;
        return;
    }
	string word;
    while(ifs >> word)//对于文件输入流而言，默认情况以空格为分隔符
    {
        int len = word.size();
		int i;
		for(i =0; i< len; ++i){ 
			if(word[i] >='a' && word[i]<='z' || word[i] >='A' && word[i]<='Z'  ){

			}else{
				break;
			}
		}
		if(i == len ){  // 不是单词不计数
			auto it = _dict.find(word);
			if(it != _dict.end()){
				it->second++;
			}else{
				_dict.insert({word,1});
			}
		}
    }
    ifs.close();
	return;
}

void Dictionary::store(const std::string &filename){
	ofstream ofs(filename);
    if(!ofs.good())
    {
        cerr << "ofstream is not good" << endl;
        return;
    }
	for(auto it = _dict.begin(); it != _dict.end(); ++it){
		ofs<< it->first <<"  "<< it->second<<endl;
	}
	ofs.close();
	return;
}

int main(){
	Dictionary holybible;
	string str1 = "C:\\Users\\hao\\Desktop\\C++Day6\\The_Holy_Bible.txt";
	string str2 = "C:\\Users\\hao\\Desktop\\C++Day6\\fileout1.txt";
	holybible.read(str1);
	holybible.store(str2);
	system("pause");
	return 0;
}