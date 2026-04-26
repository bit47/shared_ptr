#include<iostream>
#include<functional>

using namespace std;

namespace xyc
{
    template<class T>
    class shared_ptr
    {
    public:
        void release()
        {
            if(--(*_pcount) == 0)
            {
                _del(_ptr);
                delete _pcount;
            }
        }
        shared_ptr(T* ptr = nullptr)
            :_ptr(ptr)
            ,_pcount(new int(1))
        {}
        template<class D>//删除器的函数指针类
        shared_ptr(T* ptr,D del)//构造
            :_ptr(ptr)
            ,_pcount(new int(1))
            ,_del(del)
        {}

        shared_ptr(const shared_ptr<T>& sp)
            :_ptr(sp._ptr)
            ,_pcount(sp._pcount)
            ,_del(sp._del)
        {
            ++(*_pcount);
        }
        ~shared_ptr()
        {
            release();
        }

        shared_ptr operator=(const shared_ptr& sp)
        {//_ptr要指向新的资源空间，需要释放旧的空间
            if(_ptr != sp._ptr)
            {
                release();
                _ptr = sp._ptr;
                _pcount = sp._pcount;
                _del = sp._del;
                ++(*_pcount);
            }
            return *this;
        }

        T& operator*(){return *_ptr;}

        T& operator[](int i){return _ptr[i];}

        T* operator->(){return _ptr;}

        T* get(){ return _ptr;}

    private:
        T* _ptr;
        int* _pcount;
        function<void(T*)> _del = [](T* ptr){ delete ptr;};//function包装器
    };

}

int main()
{
    xyc::shared_ptr<int> p1(new int(1));
    xyc::shared_ptr<int> p2(p1);
    xyc::shared_ptr<int> p3(new int(2));
    xyc::shared_ptr<int> p4(new int[10],[](int* ptr){delete[] ptr;});


    p1 = p1;
    p1 = p2;
    p1 = p3;

    return 0;
}