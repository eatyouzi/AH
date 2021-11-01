#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

template <typename T = int , size_t ksize =10 >
class Queue
{
public:
	Queue()
	:_size(0)
	,_front(0)
	,_rear(0)
	,_data(new T[ksize]())
	{

	}
	~Queue();
	bool full() const;
	bool empty() const;
	void pop() ;
	void push(const T &val);
	T front() const;
	T back() const;
private:
	int _size;
	int _front;
	int _rear;
	T* _data;
};

template <typename T , size_t ksize >
Queue<T,ksize>::~Queue(){
	if(_data){
		delete [] _data;
		_data = nullptr;
	}
}

template <typename T , size_t ksize >
bool Queue<T,ksize>:: empty() const{
	return  (_front == _rear );
}

template <typename T , size_t ksize >
bool Queue<T,ksize>:: full() const{
	return (_rear + 1) % ksize == _front; 
}




template <typename T , size_t ksize >
void Queue<T,ksize>::push(const T &val){
	if(!full()){
        _data[_rear] = val;
        _rear = (_rear + 1) % ksize;
    }
    else{
        cout << "queue is full!" << endl;
        return;
    }
}

template <typename T , size_t ksize >
void Queue<T,ksize>:: pop() {
	if(!empty()){
        _front = (_front + 1) % ksize;
    }
    else{
        cout << "queue is empty!" << endl;
        return;
    }
}

template <typename T , size_t ksize >
T Queue<T,ksize>::front() const{
	return _data[_front];
}

template <typename T , size_t ksize >
T Queue<T,ksize>::back()const {
	return _data[(_rear-1+ksize)%ksize];
}


int main(){
	Queue <string, 10> q;
	cout<<q.empty()<<endl;
	for(int i=0; i<8; ++i){
		q.push(to_string(i));
	}
	cout<<q.front()<<endl;
	cout<<q.back()<<endl;
	q.pop();
	cout<<q.front()<<endl;
	system("pause");
	return 0;
}