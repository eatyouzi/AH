//友元模式

// #include <iostream>
// #include <stdlib.h>
// using namespace std;

// /*
// 思路
// 自动的进行delete  那就只有在构造函数  和 析构函数中 会自动的调用原本destory中的内容
// 首先  不能直接放到直接的析构函数里面
// 那就放在其他的析构函数里面  就要有另一个类  所以要创建另一个类
// */

// class auto_release; // 前向声明

// class Singleton{
// friend class auto_release;
// public:
// 	static Singleton* getInstance(){
// 		if(nullptr == _pInstance){
// 			//cout<<"getInstance"<<endl;
// 			_pInstance = new Singleton();
// 		}
// 		return _pInstance;
// 	}

// 	static void destory(){ // 这个放在其他类的析构函数里调用  就实现自动删除了
// 		if(_pInstance){
// 			delete [] _pInstance;   // 会调用析构函数
// 			_pInstance = nullptr;
// 		}
// 	}

// private:
// 	Singleton(){
// 		cout<<"Singleton"<<endl;
// 	}
// 	~Singleton(){
// 		cout<<"~Singleton"<<endl;
// 	}
// private:
// 	static Singleton* _pInstance;  // 单例模式
// };

// Singleton *Singleton::_pInstance = nullptr;

// class auto_release{
// public:
// 	auto_release(){
// 		cout<<"auto_release"<<endl;
// 	}
// 	~auto_release(){
// 		cout<<"~auto_release"<<endl;
// 		if(Singleton::_pInstance){
// 			cout<<"~singleton"<<endl;
// 			delete [] Singleton::_pInstance;   
// 			Singleton::_pInstance = nullptr;
// 		}
// 	}
// private:
// };



// int main(){
// 	Singleton *ps1 = Singleton::getInstance();
// 	Singleton *ps2 = Singleton::getInstance();
// 	cout<<ps1<<"  "<<ps2<<endl;
// 	auto_release del;
// 	system("pause");
// 	return 0;
// 	// Singleton::destory(); // 不自动的话  就调用destory
// }




//2、单例模式自动释放的方式2：内部类 + 静态数据成员
// #include <iostream>
// #include <stdlib.h>
// using namespace std;

// class Singleton
// {
// public:
//     static Singleton *getInstance(){
//         if(nullptr == _pInstance){
//             _pInstance = new Singleton();
//         }
// 		return _pInstance;
//     }

//     static void destroy(){
//         if(_pInstance){
//             delete _pInstance;
//             _pInstance = nullptr;
//         }
//     }

// private:
//     class AutoRelease{
//     public:
//         AutoRelease(){
//             cout << "AutoRelease()" << endl;
//         }

//         ~AutoRelease(){
//             cout << "~AutoRelease()" << endl;
//             if(_pInstance){
//                 delete _pInstance;
//                 _pInstance = nullptr;
//             }
//         }
//     };

// private:
//     Singleton(){
//         cout << "Singleton()" << endl;
//     }

//     ~Singleton(){
//         cout << "~Singleton()" << endl;
//     }
// private:
//     static Singleton *_pInstance;
//     static AutoRelease _ar;  // _ar必须加static，不能作为数据成员  不然析构 就死锁了  
// };
// Singleton *Singleton::_pInstance = getInstance();
// Singleton::AutoRelease Singleton::_ar;
// int main(int argc, char **argv)
// {
//     Singleton *ps1 = Singleton::getInstance();
//     Singleton *ps2 = Singleton::getInstance();

//     /* AutoRelease ar;//栈对象,error */

//     /* Singleton::destroy(); */
//     return 0;
// }


// 第三种  单例模式自动释放的方式3：atexit + 饿汉模式
// #include <stdlib.h>
// #include <iostream>

// using std::cout;
// using std::endl;

// //3、单例模式自动释放的方式3：atexit + 饿汉模式
// //
// class Singleton
// {
// public:
//     static Singleton *getInstance()
//     {
//         //注意多线程环境
//         if(nullptr == _pInstance)
//         {
//             _pInstance = new Singleton();
//             atexit(destroy);  // 进程正常结束时，()里注册的函数会被调用 
//         }

//         return _pInstance;
//     }

//     static void destroy()
//     {
//         if(_pInstance)
//         {
//             delete _pInstance;
//             _pInstance = nullptr;
//         }
//     }

// private:
//     Singleton()
//     {
//         cout << "Singleton()" << endl;
//     }

//     ~Singleton()
//     {
//         cout << "~Singleton()" << endl;
//     }
// private:
//     static Singleton *_pInstance;
// };

// /* Singleton *Singleton::_pInstance = nullptr; 饱汉（懒汉）模式*/
// Singleton *Singleton::_pInstance = getInstance();//饿汉模式

// int main(int argc, char **argv)
// {
//     Singleton *ps1 = Singleton::getInstance();
//     Singleton *ps2 = Singleton::getInstance();


//     /* Singleton::destroy(); */
//     return 0;
// }


#include <pthread.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

//4、单例模式自动释放的方式4：pthread_once + atexit
//
class Singleton
{
public:
    static Singleton *getInstance()
    {
        pthread_once(&_once, init);  // 在多线程条件下  once固定  init只会执行一次
        return _pInstance;
    }

    static void init()
    {
        _pInstance = new Singleton();
        atexit(destroy);
    }

    static void destroy()
    {
        if(_pInstance)
        {
            delete _pInstance;
            _pInstance = nullptr;
        }
    }

private:
    Singleton()
    {
        cout << "Singleton()" << endl;
    }

    ~Singleton()
    {
        cout << "~Singleton()" << endl;
    }
private:
    static Singleton *_pInstance;
    static pthread_once_t _once;
};

Singleton *Singleton::_pInstance = nullptr;// 饱汉（懒汉）模式
/* Singleton *Singleton::_pInstance = getInstance();//饿汉模式 */ // 上来就赋值，在多线程中就ok
pthread_once_t Singleton::_once = PTHREAD_ONCE_INIT;

int main(int argc, char **argv)
{
    Singleton *ps1 = Singleton::getInstance();
    Singleton *ps2 = Singleton::getInstance();


    /* Singleton::destroy(); */
    return 0;
}

