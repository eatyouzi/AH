#include<iostream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <fstream>
#include <stdlib.h>
using namespace std;

class Circle{
public:
	Circle();
	Circle( double radius)
	:_radius(radius)
	{
		cout<<"Circle"<<endl;
	}

	double getArea(){
		return 3.14*_radius*_radius;
	}

	double getPerimeter(){
		return 2*3.14*_radius;
	}

	void show(){
		cout<<"_radius  "<<_radius<<endl
		    <<"perimeter  "<<getPerimeter()<<endl
			<<"Area  "<<getArea()<<endl;
	}
private:
	double _radius;
};

class Cylinder
:public Circle  // 继承
{
public:
	Cylinder(double r, double  h)
	:Circle(r)
	,_high(h)
	{
		cout<<"Cylinder"<<endl;
	}

	double getVolume(){
		return _high*getArea();
	}
	void  showVolume(){
		cout<<"Volume  "<<getVolume()<<endl;
	}

private:
	double _high;
};



int main(){
	
	Cylinder cy1(10,2);
	cy1.show();
	cy1.showVolume();

	system("pause");
	return 0;
}