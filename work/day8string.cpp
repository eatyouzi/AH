#include <iostream>
#include <stdlib.h>
#include <cstring>
using namespace std;


class String 
{
public:

	void print(){
		cout<<_pstr<<endl;
	}
	String();
	String(const char *); // 构造函数
	String(const String &); // 拷贝构造
	~String();  // 析构函数
	String &operator=(const String & str);  // 赋值
	String &operator=(const char * str);  // 初始化

	String &operator+=(const String &); 
	String &operator+=(const char *); 
	
	char &operator[](std::size_t index);
	const char &operator[](std::size_t index) const;
	
	std::size_t size() const;
	const char* c_str() const;
	
	friend bool operator==(const String &, const String &);
	friend bool operator!=(const String &, const String &);
	
	friend bool operator<(const String &, const String &);
	friend bool operator>(const String &, const String &);
	friend bool operator<=(const String &, const String &);
	friend bool operator>=(const String &, const String &);
	
	friend std::ostream &operator<<(std::ostream &os, const String &s);
	friend std::istream &operator>>(std::istream &is, String &s);

private:
	char * _pstr;
};

String:: String(){
	_pstr = nullptr;
}

String::String(const char *str){// 构造函数
	// 先给_pstr 分配空间
	if(str){
		_pstr = new char[strlen(str)+1]();
		strcpy(_pstr, str);
	}
}

String:: ~String(){
	cout<<"~String()"<<endl;
	if(_pstr){
		delete(_pstr);
		_pstr = nullptr;
	}
}

String & String::operator =(const String & str){
	if(this != &str){
		delete _pstr;
		_pstr = nullptr;
	}
	if(str._pstr){
		//深拷贝   
        _pstr=new char[strlen(str._pstr)+1];
        strcpy(_pstr,str._pstr);
    }
    return *this;
}

String &String::operator=(const char * str){
	if(str){
		_pstr = new char [strlen(str)+1];
		strcpy(_pstr,str);
	}
	return *this;
}

String &String::operator+=(const String & str){ // *this变化
	// 还是要深拷贝  原来的长度可能不够
	if(str._pstr == nullptr){
		return *this;
	}

	size_t len = strlen(_pstr) +strlen(str._pstr)+1;
	char *temp  = new char[len]();
	strcpy(temp,_pstr);
    strcat(temp,str._pstr);
	delete _pstr;
	_pstr = temp;
	return *this;
}
String &String::operator+=(const char *str){
	size_t len = strlen(_pstr) +strlen(str)+1;
	char *temp  = new char[len]();
	strcpy(temp,_pstr);
    strcat(temp,str);
	delete _pstr;
	_pstr = temp;
	return *this;
}

char &String::operator[](std::size_t index){// 重载下标访问运算符
	size_t len  = strlen(_pstr);
	if(index < len){
		return _pstr[index];
	}else{ //返回空
		static char temp = '\0';  // 加static是为了保证char & 引用有效  加长生命周期
		return temp;
	}
}
const char &String::operator[](std::size_t index) const{  // 和上面一样？？？
	size_t len  = strlen(_pstr);
	if(index < len){
		return _pstr[index];
	}else{ //返回空
		static char temp = '\0';  // 加static是为了保证char & 引用有效  加长生命周期
		return temp;
	}
}

std::size_t String::size() const{ // 函数重载
	return strlen(_pstr);
}
const char* String::c_str() const{
	return _pstr;  // 就是c字符串分格的
}

bool operator==(const String &str1 , const String &str2){
	return !(strcmp(str1.c_str(), str2.c_str() ) );
}
bool operator!=(const String &str1, const String &str2){
	return (strcmp(str1.c_str(), str2.c_str() ) );
}

bool operator<(const String &str1, const String &str2){
	if(strcmp(str1.c_str(), str2.c_str() )< 0){
		return true;
	}else{
		return false;
	}
}
bool operator >(const String &str1 , const String &str2){
	if(strcmp(str1.c_str(), str2.c_str() ) > 0){
		return true;
	}else{
		return false;
	}
}
bool operator <= (const String &str1, const String &str2){
	if(strcmp(str1.c_str(), str2.c_str() ) > 0){
		return false;
	}else{
		return true;
	}
}
bool operator>=(const String &str1, const String &str2){
	if(strcmp(str1.c_str(), str2.c_str() ) < 0){
		return false;
	}else{
		return true;
	}
}


std::ostream &operator<<(std::ostream &os, const String &s){
	os<<s._pstr;
	return os;
}

std::istream &operator>>(std::istream &is, String &s){
	//默认以空格为分隔符  string就是一个整体
	string temp;  //注意这里是小写的  是string类  
    is>>temp;
	//输入到s
	if(s._pstr){
		delete [] s._pstr;
	}
	s._pstr = new char[temp.size()+1];
    strcpy(s._pstr,temp.c_str());
    return is;
}

String operator+(const String &str1, const String &str2){
	String temp (str1.c_str() );  
	temp += str2;
	return temp;
}
String operator+(const String &str1, const char *str2){
	String temp = (str1.c_str() );
	temp += str2;
	return temp; 
}
String operator+(const char *str1, const String &str2){
	String temp = str1;
	temp += str2;
	return temp;
}

int main(){
	String str1,str2;
	str1 = "wangdao";
	str1.print();

	str2 = "xiaoli";
	// str2 = str1;
	// str2.print();
	str2 += str1;
	str2.print();
	str1 += "nihao";
	str1.print();
	cout<<str1[1]<<endl; 

	cout<<str1.size()<<endl;

	bool flag = str1 == str2;
	cout<<flag<<endl;

	String str3= "aaa";
	String str4 = "bbb";
	bool flag2= str3>str4;
	cout<<flag2<<endl;

	String str5 = str3 + str4;
	str5.print();
	
	str5 = str1 + "111";
	cout<<str5<<endl;

	str5 = "222"+str4;
	cout<<str5<<endl;

	String str6;
	cin>>str6;
	cout<<str6;
	system("pause");
	return 0;
}