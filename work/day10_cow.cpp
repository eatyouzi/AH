#include <string.h>
#include <iostream>
#include <stdlib.h>
using std::cout;
using std::endl;

//只有= 需要写时复制    << 是不用的  那对于[]就用2种情况  所以用一个子类
class String
{
public:
    String()
    : _pstr(new char[5]() + 4) // 5是int + '\0'  +4是偏移到字符串开始
    {
        cout << "String()" << endl;
        initRefcount(); // 初始化引用计数
    }

    //String s1 = "hello";
    String(const char *pstr)
    : _pstr(new char[strlen(pstr) + 5]() + 4)
    {
        cout << "String(const char *)" << endl;
        strcpy(_pstr, pstr);
        initRefcount();
    }

    //String s2 = s1;
    String(const String &rhs)
    : _pstr(rhs._pstr)//浅拷贝
    {
        cout << "String(const String &)" << endl;
        /* ++*(int *)(_pstr - 4);//引用计数++操作 */
        increaseRefcount();
    }

    String &operator=(const String &rhs)
    {
        cout << "String &operator=(const String &)" << endl;
        if(this != &rhs)
        {
            release();//2、释放左操作数
            _pstr = rhs._pstr;//3、浅拷贝
            increaseRefcount();
        }
        return  *this;
    }

    ~String()
    {
        cout << "~String()" << endl;
        release();
    }

private:
	class Char{
	public:
		Char(String &strtemp, size_t pos)
		:_strtemp(strtemp)
		,_pos(pos){

		}

        char &operator=(const char &ch);  //=号重载  
		//friend std::ostream &operator<<(std::ostream os, const Char &rhs);   // <<重载
	public:
		String &_strtemp;
		size_t _pos;
	};

public:
	Char operator[] (size_t pos){
		return Char(*this,pos);
	}

    // char &operator[](size_t idx)
    // {
    //     if(idx < size())
    //     {
    //         if(getRefcount() > 1)//是不是共享的
    //         {
    //             char *ptmp = new char[size() + 5]() + 4;
    //             strcpy(ptmp, _pstr);
    //             decreaseRefcount();
    //             _pstr = ptmp;
    //             initRefcount();
    //         }
    //         return _pstr[idx];
    //     }
    //     else
    //     {
    //         static char charnull = '\0';
    //         return charnull;
    //     }
    // }
 
private:
    void release()
    {
        decreaseRefcount();
        if(0 == getRefcount())
        {
            delete [] (_pstr - 4);
        }

    }

    void initRefcount(){
        *(int *)(_pstr - 4) = 1;  //强转  解引用
    }

    void increaseRefcount(){
        ++*(int *)(_pstr - 4);
    }
    void decreaseRefcount(){
        --*(int *)(_pstr - 4);
    }

public:
    size_t size(){
        return strlen(_pstr);
    }

    const char *c_str(){
        return _pstr;
    }

    int getRefcount(){
        return *(int *)(_pstr - 4);
    }

    friend std::ostream &operator<<(std::ostream &os, const String &rhs);
	friend std::ostream &operator<<(std::ostream &os, const String::Char &rhs);
private:
    char *_pstr;
};

std::ostream &operator<<(std::ostream &os, const String &rhs)
{
    if(rhs._pstr)
    {
        os << rhs._pstr;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const String::Char&rhs){
	os<<rhs._strtemp._pstr[rhs._pos];  // 就是找到str[i]  的值  利用char找回去
	return os;
}

char &String:: Char:: operator=(const char &ch){
	{
        if(_pos < _strtemp.size())
        {
            if(_strtemp.getRefcount() > 1)//是共享的
            {
                char *ptmp = new char[_strtemp.size() + 5]() + 4;
                strcpy(ptmp, _strtemp._pstr);
                _strtemp.decreaseRefcount();
                _strtemp._pstr = ptmp;
                _strtemp.initRefcount();
            }
			_strtemp._pstr[_pos] = ch;
            return _strtemp._pstr[_pos];
        }
        else
        {
            static char charnull = '\0';
            return charnull;
        }
	}
}


void test(){
    String s1("hello");
    cout << "s1 = " << s1 << endl;
    cout << "s1.getRefcount() = " << s1.getRefcount() << endl;
    printf("s1'address : %p\n", s1.c_str());

    cout <<endl;
    String s2 = s1;
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s1.getRefcount() = " << s1.getRefcount() << endl;
    cout << "s2.getRefcount() = " << s2.getRefcount() << endl;
    printf("s1'address : %p\n", s1.c_str());
    printf("s2'address : %p\n", s2.c_str());

    cout << endl;
    String s3("world");
    cout << "s3 = " << s3 << endl;
    cout << "s3.getRefcount() = " << s3.getRefcount() << endl;
    printf("s3'address : %p\n", s3.c_str());

    cout << endl;
    s3 = s1;
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s1.getRefcount() = " << s1.getRefcount() << endl;
    cout << "s2.getRefcount() = " << s2.getRefcount() << endl;
    cout << "s3.getRefcount() = " << s3.getRefcount() << endl;
    printf("s1'address : %p\n", s1.c_str());
    printf("s2'address : %p\n", s2.c_str());
    printf("s3'address : %p\n", s3.c_str());

    cout << endl << "对s3[0]执行写操作" << endl;
    s3[0] = 'H';//s3.operator[](0)//char = char
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s1.getRefcount() = " << s1.getRefcount() << endl;
    cout << "s2.getRefcount() = " << s2.getRefcount() << endl;
    cout << "s3.getRefcount() = " << s3.getRefcount() << endl;
    printf("s1'address : %p\n", s1.c_str());
    printf("s2'address : %p\n", s2.c_str());
    printf("s3'address : %p\n", s3.c_str());

    cout << endl << "对s1[0]执行读操作" << endl;
    cout << "s1[0] = " << s1[0] << endl;//s1.operator[](0)
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s1.getRefcount() = " << s1.getRefcount() << endl;
    cout << "s2.getRefcount() = " << s2.getRefcount() << endl;
    cout << "s3.getRefcount() = " << s3.getRefcount() << endl;
    printf("s1'address : %p\n", s1.c_str());
    printf("s2'address : %p\n", s2.c_str());
    printf("s3'address : %p\n", s3.c_str());

    //问题所在：不能区分读操作还是写操作？
    
}
int main(int argc, char **argv)
{
    test();
	system("pause");
    return 0;
}

