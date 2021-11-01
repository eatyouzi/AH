#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include "tinyxml2.h"  // 是库里面的头文件 
#include <regex> // 正则表达式的
#include <fstream>
using namespace std;
using namespace tinyxml2;


struct RssItem  // 结构体 存item
{
    string title;  // 用数组下标当序号
    string link;
    string description;
    string content;
};

class RssReader
{
public:
    RssReader();
	RssReader(const string&filename); 
    void parseRss();//解析
    void dump(const string & filename);//输出到文件
	~RssReader();
private:
	string _filename;
    vector<RssItem> _rss;
 }; 


RssReader::RssReader(const string&filename)
:_filename(filename)
{
	
}

void RssReader::parseRss(){   // 解析函数
	if(_filename.size() == 0) return;
	XMLDocument xml1;
	int ret =0;
	ret = xml1.LoadFile(_filename.c_str());
	if(ret != 0){  //  Loadfile函数的返回 return _errorID 是错误码
		cout<<"xml文件打开失败"<<endl;
		return;
	}
	//调用库函数解析
	XMLElement *root = xml1.FirstChildElement("rss")->FirstChildElement("channel");
	XMLElement *itemnode = root->FirstChildElement("item");
	// 循环提取每一个item到结构体中

	 //正则模板，用于替换掉description和content中间的标签
    std::regex pattern("<.*?>");

	while(nullptr != itemnode){
		RssItem *tempRss = new RssItem();
		(*tempRss).title = (itemnode->FirstChildElement("title")->GetText());
		(*tempRss).link = itemnode->FirstChildElement("link")->GetText();
		XMLElement *description, *content;


		//这2个是要用正则表达式来提取数据的
		description = itemnode->FirstChildElement("description");
		content = itemnode->FirstChildElement("content:encoded");

		(*tempRss).description = string(std::regex_replace(description->GetText(), pattern, ""));
		(*tempRss).content = string(std::regex_replace(content->GetText(), pattern,""));

		_rss.push_back(*tempRss);
		itemnode = itemnode->NextSiblingElement("item");
	}
}

void RssReader:: dump(const string & filename){  //输出到文件中
	ofstream ofs(filename);
	if(!ofs.good())
    {
        cerr << "ofstream is not good" << endl;
        return;
    }
	for(size_t i =0; i< _rss.size(); ++i){
		ofs<<"<doc>"<<endl;
		ofs << "\t<docid>" << i+1 << "</docid>" << endl;  // 就是数组下标
        ofs << "\t<title>" << _rss[i].title << "</title>" << endl;
        ofs << "\t<link>" << _rss[i].link << "</link>" << endl;
        ofs << "\t<description>" << _rss[i].description << "</description>" << endl;
        ofs << "\t<content>" << _rss[i].content << "</content>" << endl;
		ofs << "</doc>" << endl << endl;
	}
	ofs.close();
}

RssReader:: ~RssReader(){
	delete [] &_rss;
}


int main(){

	string filename = "./coolshell.xml";
	RssReader rssread(filename);
	rssread.parseRss();
	rssread.dump("pagelib.txt");
	system("pause");
    return 0;
}