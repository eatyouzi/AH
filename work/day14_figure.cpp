#include<iostream>
#include <cstdio>
#include <fstream>
#include <stdlib.h>
using namespace std;

class Figure{
public:
	Figure();
	Figure(string name)
	:_typename(name)
	{

	}
	virtual double getArea( )=0;
	virtual string getName( )=0;
	virtual void show()=0;			//打印图形的相关信息
	
protected:
	string  _typename ;	
};

class Cicle
:virtual public Figure
{
public:
	Cicle():_radix(0.0){}    //初始化为0 
	Cicle(double radix)
	:_radix(radix)
	,Figure("Cicle")
	{
	
	}  		
	double getRadius( )   {
		return _radix;
	}//获取圆的半径
	double getPerimeter(){
		return 2*3.14*_radix;
	}      //获取圆的周长
	virtual double getArea() {
		return 3.14*_radix*_radix;
	}  //获取圆的面积
	virtual string getName(){
		return _typename;
	}   //获取圆的名字 
	
	virtual void show(){
		cout<<"name  "<<getName()
			<<"  radix  "<<getRadius()
			<<"  perimeter  "<<getPerimeter()
			<<"  area  "<<getArea()
			<<endl;
	}
	
protected:
	double _radix ;
};

class Cylinder
:public Cicle
{

public:
	Cylinder(double r, double h)
	:Cicle(r)
	,Figure("Cylinder")
	,_high(h)
	{	
	}
	virtual double getArea(){
		return 2 *Cicle::getArea()+ getPerimeter()*_high;
	}

	virtual string getName(){
		return _typename;
	}
	double getHeight(){
		return _high;
	}      //获取圆柱体的高
	double getVolume(){
			return Cicle::getArea()*_high;
	}      //获取圆柱体的体积           
	void  show(){
		cout<<"name  "<<_typename
		<<"  high "<<_high
		<<"  area  "<<getArea()
		<<"  volume  "<<getVolume()
		<<endl;
	}      		 //将圆柱体的高、表面积、体积输出到屏幕 
private:
	double _high;
};
int main(){
	Figure  *f1, *f2;
	Cicle ci1(10.0);
	Cylinder cy1(10.0, 2.0);
	f1 = &ci1;
	f2 = &cy1;
	f1->show();
	f2->show();

	system("pause");
	return 0;
}