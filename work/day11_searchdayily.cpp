#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <fstream>
#include <map>
#include <set>
using namespace std;




class TextQuery
{
public:
    TextQuery()
	:_lines(vector<string> ())
	,_word2Numbers(map<string, set<int> >())
	,_dict(map<string, int>())
	{
		cout<<"TextQuery"<<endl;
	}

    void readFile(const string filename);  // 按行读  写入就行
    void query(const string &word);//查询和打印耦合在一起了
	
private:	
	vector<string> _lines; 
	map<string, set<int> > _word2Numbers;
	map<string, int> _dict;
};

void TextQuery:: readFile(const string filename){
	ifstream ifs(filename);
	if(!ifs.good()){ 
        cerr << "ifstream is not good" << endl;
        return;
    }
	string word;
	string linetemp;
	int i= 1;
	while(getline(ifs,linetemp)){
		_lines.push_back(linetemp);
		int len = linetemp.size();
		int pos=0;
		while(pos <len){
			int j=pos;
			while(linetemp[j] != ' '){
				j++;
			}
			word = linetemp.substr(pos,j-pos);

			auto it1 = _dict.find(word);
			if(it1 == _dict.end()){  // 没有找到  那在_word2Numbers里也没有
				_dict.insert({word,1});
				set <int> s1 = {i};  // 出现的行号
				_word2Numbers.insert({word,s1});
			}else{   // 找到的话  在_dict   _word2Numbers中都有
				it1->second ++;
				auto it2 = _word2Numbers.find(word);
				it2->second.insert(i);
			}
			pos=j+1;
		}

		i++;
	}
}

void TextQuery::  query(const string &word){
	auto it1 = _dict.find(word);
	if(it1 == _dict.end()){
		cout<<word<<" is not  exit"<<endl;
	}else{
		cout<<word<<" occurs "<<it1->second<<" times"<<endl;
		auto it2 = _word2Numbers.find(word);
		auto it3 = it2->second.begin();
		for(it3; it3 != it2->second.end(); it3++){
			cout<<"(line "<<*it3 <<") "
				<<_lines[*it3-1]<<endl;
		}
	}
}

//void print(ostream & os, const QueryResult &);


int main(int argc, char *argv[])
{
   string queryWord("stock");
   TextQuery tq;
   string str1 = "C:\\Users\\hao\\Desktop\\C++Day11\\china_daily.txt";
   tq.readFile(str1);
   tq.query(queryWord);	

   system("pause");
   return 0;
}