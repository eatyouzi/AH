#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Point
{
public:
	Point(int x,int y)
	:_x(x),_y(y){

	}

	void show(){
		cout<<"x=  "<<_x
			<<"Y=  "<<_y<<endl;
	}
private:
	int _x,_y;
};

class Computer
{
public:
	Computer(const string &name,const int &price )
	:_price(price)
	,_name(name)
	{

	}

	void show(){
		cout<<"price "<<_price
			<<"name "<<_name<<endl;
	}
private:
	int _price;
	string _name;
};

template <typename T>  
class Singleton
{
public: 
	template<typename ...Args> // 模板参数包
	static T* getInstance(Args ...args){   // 函数参数包
		if(!pInstance){
			pInstance = new T(args...);  // 解包
		}
		return pInstance;
	}

	static void destory(){
		if(pInstance){
			delete [] pInstance;
			pInstance = nullptr;
		}
	}
private:
	Singleton();
	~Singleton();
	static T* pInstance;
};

template <typename T>
T* Singleton<T>:: pInstance = nullptr;

int main(){
	Computer *pc1 = Singleton<Computer>::getInstance("Xiaomi", 6666);
    Computer *pc2 = Singleton<Computer>::getInstance("Xiaomi", 6666);
	cout<<pc1<<endl;
	cout<<pc2<<endl;

	Point *pt3 = Singleton<Point>::getInstance(1,2);
	Point *pt4 = Singleton<Point>::getInstance(1,2);
	cout<<pt3<<endl;
	cout<<pt4<<endl;

	system("pause");
	return 0;
	// Point　*pt3 = Singleton<Point>::getInstance(1, 2);
    // Point　*pt4 = Singleton<Point>::getInstance(1, 2);

}