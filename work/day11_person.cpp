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

class Person{
public:
	Person(string name, int age)
	:_name(name)
	,_age(age)
	{
		cout<<"Person"<<endl;
	}

	void display(){
		cout<<"_name  "<<_name<<endl
			<<"_age  "<<_age<<endl;
	}
private:
	string _name;
	int _age;
};

class Employee
:public Person 
{
public:
	Employee(string name, int age,float salary, string department)
	:Person(name,age)
	,_salary(salary)
	,_department(department)
	{
		cout<<"Employee"<<endl;
	}

	void display(){
		Person::display();
		cout<<"_salary  "<<_salary<<endl
			<<"_department  "<<_department<<endl;
	}

	float getsalary(){
		return _salary;
	}

private:
	float _salary;
	string _department;
};


int main(){
	Employee e1("xiaohua",18,5000,"xiaoshou");
	Employee e2("xiaoming",22,6800,"jishubu");
	Employee e3("lihua",30,12000,"zongcai");
	e1.Employee::display();
	e2.Employee::display();

	float avg =(e1.getsalary() + e2.getsalary()+ e3.getsalary())/3;
	cout<<"avg "<<avg<<endl;
	system("pause");
	return 0;
}