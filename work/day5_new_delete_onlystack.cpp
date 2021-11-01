#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>

using namespace std;
/*
只能在栈上创建， 不能在堆上 那就要让new失效   在堆上要用new标准函数，在栈上只要用构造函数就行
new的步骤  先 new得到内存空间  再调用构造函数   最后返回一个指针
所以只要把第一步new失效  Student *pstu = new Student(123, "Lucy"); 就会报错了
*/

class student{

public:
	student(int id, const char* name)  // 构造函数
	:_id(id)
	,_name( new char[strlen(name)+1]() )  //先申请空间   再strcpy
	{
		strcpy(_name,name);
		cout<<"student(int id, const char* name)"<<endl;
	}

private:

	static void *operator new(size_t sz){  //加static是因为 this指针不能用  那肯定是一个静态函数  
		cout<<"new"<<endl;				   //要显式的写出来  参数size_t 也是固定的
		void *pret = malloc(sz);          //底层调用malloc
		return pret;
	}


	static void operator delete(void  *pret){
		free(pret);
		cout<<"delete"<<endl;
	}
public:
	void print(){
		cout<<"_id "<<_id<<endl;
		cout<<"_name "<<_name<<endl;
	}

	~student(){  // 析构函数
		if(_name){
			delete [](_name);
			_name = nullptr;
		}
		cout<<"~student"<<endl;
	}

private:
	int _id;
	char * _name;
};

int main(){
	//student *stu1 = new student(1,"lihua");
	student stu2(2,"liming");
	stu2.print();
	system("pause");
	return 0;
}