#include <iostream>
#include <thread>
//thread线程的基本使用
//创建线程std::thread t(function_name, args...);
//`function_name`是线程入口点的函数或可调用对象
//`args...`是传递给函数的参数
//创建线程后，我们可以使用`t.join()`让主线程等待子线程完成，或者使用`t.detach()`分离线程，主线程不等待子线程，让它在后台运行。
// 
//当创建一个线程后，可以选择调用 join() 或 detach() 方法，这两个方法是互斥的，即一旦调用了 join()，就不能
//再调用 detach()，反之亦然。joinable() 函数可以用来判断是否已经对线程进行了这些操作，joinable()方法返回一个布尔值
// ，如果线程可以被join()或detach()，则返回true
void foo() {
    std::cout << "foo start" << std::endl;
}
void foo_false(){
    std::cout << "foo default" << std::endl;
}
int main() {
    std::thread t(foo);
    std::thread t_f(foo_false);
    t_f.join();
    if (t.joinable()) {
        t.join();
    }
    if (t_f.joinable()) {

    }
    else {
        std::cout << "foo join default " << std::endl;
    }

    return 0;
}
