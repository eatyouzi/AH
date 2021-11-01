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
class TextQuery; // 前向声明


// 文本查询类  完成对输入文件的初始化
class TextQuery
{
	friend class Querysult;

public:
	TextQuery() = default;
	TextQuery(ifstream &ifs); //TextQuery的构造函数接受一个ifstream,逐行读取输入文件:
	QueryResult query(const string& word) const;  //查询函数

private:
	shared_ptr <vector<string> > mfile;  //查询的文本  和QueryResult共享的
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

private:
	string q_word;
	shared_ptr<set<int> > lines_; //指向保存单词出现行号的set
	shared_ptr<vector<string> > word_file_; //单词文件
};



class Query_base
{
	friend class Query;

protected:
	virtual ~Query_base() = default;  // 设为默认

private:
	// eval返回与当前TextQuery匹配的QueryResult
	virtual QueryResult eval(const TextQuery&) const =0; //纯虚函数
	virtual string rep() const = 0; // 加std::  ?
	//rep表示查询的一个string
};

/*
接口类
Query是对外的接口  派生类不能直接使用_base   用户更不行 同时隐藏了 Query_base的继承体系
*/
class Query{  // 用来使用_base  所以是友元
	friend Query operator~(const Query &);  // 重载各种运算符时用到
	friend Query operator|(const Query&, const Query&);
	friend Query operator&(const Query&, const Query&);
	friend std::ostream &operator<<(std::ostream &os, const Query &query);

public:
	Query(const string&);  //与其他类型的query不同，WordQuery可以直接由Query类构造得到 
	QueryResult eval(const TextQuery &t) const{
		return query_->eval(t);
	}

	string rep () const{
		return query_->rep(); 
	}

private:
	//构造函数定义为private,不希望一般用户随便定义Query_base对象。 
	Query(shared_ptr<Query_base> query): query_(query) {} // 构造函数
	shared_ptr <Query_base> query_;  // 每一个对象都有指向_base的指针
							//通过智能指针实现动态绑定
};
//派生的类
// 正常查询
class Wordquery  
:public Query_base
{
	friend class Query;  // Query 要用到这里的数据  ？？？
	Wordquery(const string &qword)
	:q_word(qword)
	{

	}

	QueryResult eval(const TextQuery &t ) const{
		return t.query(q_word);
	} // 查询单词
	
	string rep() const{  // 查询的一个string
		return q_word;
	}

private:
	string q_word;
};

//无论哪种查询都是建立在WordQuery的根本上，因此 Query的构造函数用一个WordQuery来初始化  
Query::Query(const string& s)  // 构造函数  ？？？？
:query_(new Wordquery (s))
{

}

class Notquery
:public Query_base
{
	friend Query operator~(const Query&);//~运算符使用了NotQuery的构造函数
	Notquery(const Query& q)
	:query_(q)
	{

	} 

	QueryResult eval(const TextQuery&) const;//其实真正区分各种Query的是eval函数
	string rep() const override
    {
        return "~(" + query_.rep() + ")";
    }

private:
	Query query_;
};

Query operator~(const Query &q)
{
    return shared_ptr<Query_base>(new Notquery(q));
    //就是在这种情况用到了从指针到Query对象的转换，需要Query的构造函数
    //还使用了NotQuery类的构造函数
}

class Binaryquery
:public Query_base
{
protected:
	Binaryquery(const Query &q1, const Query &q2, string ops)
	:lhs(q1)
	,rhs(q2)
	,ops_(ops)
	{

	}
	// eval函数不用声明  已经继承过来了  Binary本身也是基类 不用定义
	string rep() const {
		return "("+lhs.rep() + " "+ops_+" " + rhs.rep()+")";
	}
	Query lhs;
	Query rhs;
	string ops_; // 运算符
};

class AndQuery
:public Binaryquery
{
	friend Query operator&(const Query&, const Query&);
	AndQuery(const Query&q1, const Query &q2)
	:Binaryquery(q1,q2,"&")
	{

	}
	QueryResult eval(const TextQuery&) const;
	// rep直接用继承就可以
};

Query operator&(const Query &q1,const Query &q2)
{
    return shared_ptr<Query_base>(new AndQuery(q1, q2));
}

class Orquery
:public Binaryquery
{
	friend Query operator|(const  Query&, const Query &);
	Orquery(const Query &q1,const Query &q2)
	:Binaryquery(q1,q2,"|")
	{

	}
	QueryResult eval(const TextQuery&) const;
};

Query operator|(const Query &q1, const Query &q2)
{
    return shared_ptr<Query_base>(new Orquery(q1, q2));
}
//eval实际就是求lines，lines是创建QueryResult的关键

TextQuery::TextQuery(ifstream &ifs)  // 建立查询表
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

QueryResult TextQuery::query(const string &word) const
{
	static shared_ptr<set<int>> nodata(new set<int>);//指向行号集的指针
	auto ret = wordline.find(word);
	if (ret == wordline.end()){
		return QueryResult(word, nodata, mfile);
	}else{
		return QueryResult(word, ret->second, mfile);
	}
}

QueryResult Notquery::eval(const TextQuery& t) const{
	//得到单词出现的行号
	auto result = query_.eval(t);
	auto ret_lines = make_shared<set<int> >();
	//在动态内存中分配一个对象并初始化它，返回指向此对象的shared_ptr;
	//由于是通过shared_ptr管理内存，因此一种安全分配和使用动态内存的方法。
	// 对于输入文件中的每一行，如果没有出现在result中，
	// 则把它添加到ret_lines里
	auto beg = result.begin();
	auto end = result.end();
	auto sz = result.getfile()->size();
	for(size_t i =0; i!= sz; ++i){
		if(beg == end || i != *beg){
			ret_lines->insert(i);
		}else if(beg != end){
			++beg;
		}
	}
	return QueryResult(rep(), ret_lines, result.getfile());  // ？？？
}

QueryResult Orquery::eval(const TextQuery &t) const{
	QueryResult left  = lhs.eval(t);
	QueryResult right = rhs.eval(t);
	auto ret_lines = make_shared<set<int>>(left.begin(), left.end());
	ret_lines->insert(right.begin(), right.end());//set会自动忽略重复的关键字
	return QueryResult(rep(), ret_lines, left.getfile());
}

QueryResult AndQuery:: eval(const TextQuery &t) const{
	auto left = lhs.eval(t);
	auto right = rhs.eval(t);
	auto ret_lines = make_shared<set<int>>();
	set_intersection(left.begin(), left.end(), right.begin(),
	 right.end(), inserter(*ret_lines, ret_lines->begin()));
	// set_intersection的作用是寻找并集
	return QueryResult(rep(), ret_lines, left.getfile());
}


ostream &operator<<(std::ostream &os, const QueryResult &q)
{
	os<<"单词出现了"<<q.lines_->size()<<"  次！"<<endl;
	for(auto it =q.lines_->begin(); it!= q.lines_->end(); ++it){
		os << "  " << "line(" << *it + 1 << ") ";
		os << (*(q.word_file_)) [*it] << endl;
	}
	return os;
}

ostream &operator<<(std::ostream &os, const Query &query)
{
	return os << query.rep();
}

int main(){
	ifstream ifs("C:\\Users\\hao\\Desktop\\C++Day16\\file.txt");
	if(!ifs.good()){ 
        cerr << "ifstream is not good" << endl;
    }

	TextQuery tq(ifs);
	//Query q = ~Query("hair") ;
	//Query q = Query("hair") ;
	//Query q = Query("fiery") & Query("bird") | Query("wind");
	Query q = Query("Alice") | Query("wind");
	cout<<q<<endl;
	cout<<q.eval(tq)<<endl;
	
	system("pause");
	return 0;
}