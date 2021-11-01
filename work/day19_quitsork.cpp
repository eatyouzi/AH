#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>
#include <stdlib.h>
using namespace std;


class Computer
{
    friend std::ostream &operator<<(std::ostream &os, const Computer &rhs);
public:
    Computer(string name, int price)
        : _name(name), _price(price) {}

    int getPrice() const {
        return _price;
    }

private:
    string _name;
    int _price;
};
// print时用到
std::ostream &operator<<(std::ostream &os, const Computer &rhs){
	os<<rhs._price<<" "<<rhs._name;
	return os;
}

template <typename T>  // 重写swap
void myswap(T &lhs, T &rhs)
{
	//cout<<"swap"<<endl;
    auto temp = lhs;
    lhs = rhs;
    rhs = temp;
}

template<>
struct std::less<Computer>
{
	bool operator()(const Computer &lhs,const Computer &rhs){
		return(lhs.getPrice() <= rhs.getPrice());
	}
};

template<typename T,typename Compare=std::less<T>>
class MyQsort
{
public:
	MyQsort(T *arr, size_t size, Compare com);
	void quick(int left, int right, Compare &com);
	int partition(int left, int right, Compare &com);
	void print();
private:
	vector<T> _vec;
};

template <typename T, typename Compare>
MyQsort<T,Compare>::MyQsort(T *arr, size_t size, Compare com){
	for(size_t i=0; i<size; ++i){
		_vec.push_back(arr[i]);
	}
	quick(0,_vec.size()-1,com);
}

template <typename T, typename Compare>
void MyQsort<T,Compare>::quick(int left, int right, Compare &com){
	int privot;
	if(left <right){
		privot = partition(left,right,com);
		quick(left,privot-1,com);
		quick(privot+1,right,com);
	}	
}
template<typename T, typename Compare>
int MyQsort<T,Compare>::partition(int left, int right, Compare &com){
	int i,j;
	for(i = left, j=left; i<right; ++i){
		if(com(_vec[i], _vec[right])){
			myswap(_vec[i], _vec[j]);
			++j;
		}
	}
	myswap(_vec[j],_vec[right]);
	return j;
}

template<typename T, typename Compare>
void MyQsort<T,Compare>:: print(){
	for(auto &e : _vec){
		cout<<e<<" ";
	}
	cout<<endl;
}


int main(){
	Computer arr[5]={{"huawei",6666},{"chuizi",4444},{"xioiami",5555},{"ipone",9999},{"oppo",3333}};
	MyQsort<Computer> q(arr,5,std::less<Computer>());
	q.print();
	system("pause");
	return 0;
}