#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include <cstring>
#include <memory>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
using namespace std; 

class QueryResult;
class TextQuery;

class TextQuery
{
	friend class QueryResult;
public:
	TextQuery() = default;
	TextQuery(ifstream &ifs); //TextQuery的构造函数接受一个ifstream,逐行读取输入文件:
	QueryResult query(const string& word) const;   //查询函数
private:
	shared_ptr <vector<string> > mfile;   //查询的文本  和QueryResult共享的
	map <string, shared_ptr<set<int> >> wordline;  // 单词和行号对应  
};


// 查询结果类  
class QueryResult  // 保存Query操作的结果
{
	friend class TextQuery;
	// 重载输出<<   便于输出结果
	friend ostream &operator<<(std::ostream &os, const QueryResult &q); 
public:
	QueryResult() = default;
	QueryResult(string qword, shared_ptr<set<int> >lines, shared_ptr<vector<string> > wordfile)
	:q_word(qword)
	,lines_(lines)
	,word_file_(wordfile)
	{

	}

	auto begin() const{
		return lines_->begin();
	}

	auto end() const{
		return lines_->end();
	}

	shared_ptr<vector<string> > getfile() const{
		return word_file_;
	}
	shared_ptr<set<int> > getset() const{
		return lines_;
	}

private:
	string q_word;
	shared_ptr<set<int> > lines_;  //指向保存单词出现行号的set
	shared_ptr<vector<string> > word_file_;  //单词文件
};

TextQuery::TextQuery(ifstream &ifs)
:mfile(new vector<string>)
{
	string word;//单词
	string line;//一行文本
	static int i = 0;//行号
	while(getline(ifs,line)){
		mfile->push_back(line);
		i = mfile->size()-1;  // 单词所在的行号
		istringstream ist(line);
		while(ist >> word){
			auto &li =wordline[word]; // word所在的行号的
			if(!li){
				li.reset(new set<int>);
			}
			li->insert(i);
		}
	}
}

QueryResult TextQuery::query(const string &word) const{
	static shared_ptr<set<int>> nodata(new set<int>);//指向行号集的指针
	auto ret = wordline.find(word);
	if (ret == wordline.end()){
		return QueryResult(word, nodata, mfile);
	}else{
		return QueryResult(word, ret->second, mfile);
	}
}

ostream  &operator<<(std::ostream &os, const QueryResult &q){
	os<<"单词出现了"<<q.getset()->size()<<" 次"<<endl;
	for(auto it =q.getset()->begin(); it!= q.getset()->end(); ++it){
		os << "  " << "line(" << *it + 1 << ") ";
		os << (*(q.getfile())) [*it] << endl;
	}
	return os;
}

int main(){
	ifstream ifs("C:\\Users\\hao\\Desktop\\C++Day16\\file.txt");
	if(!ifs.good()){ 
        cerr << "ifstream is not good" << endl;
    };
	TextQuery tq(ifs);
	QueryResult qr = tq.query("hair");
	cout<<qr;
	system("pause");
	return 0;
}