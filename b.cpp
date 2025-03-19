#include<iostream>
#include<thread>
//多线程传参未定义的问题
void foo(int &a) {
	std::cout << a << std::endl;
	a++;
	
}

void foo1(int* ptr) {
	std::cout << *ptr << std::endl; // 访问已经被销毁的指针
}
void foo2(int& x) {
	std::cout << x << std::endl; // 访问已经被释放的内存
}

class MyClass {
public:
	void func() {
		std::cout << "Thread " << std::this_thread::get_id()
			<< " started" << std::endl;
		// do some work
		std::cout << "Thread " << std::this_thread::get_id()
			<< " finished" << std::endl;
	}
};

class MyClass {
private:
	friend void myThreadFunc(MyClass* obj);
	void privateFunc() {
		std::cout << "Thread "
			<< std::this_thread::get_id() << " privateFunc" << std::endl;
	}
};

void myThreadFunc(MyClass* obj) {
	obj->privateFunc();
}

int main() {
#if 0
	//1.函数中向线程传入临时变量
	//在线程函数执行时，临时变量a1被销毁，从而导致未定义行为。
	//解决方案是将变量复制到一个持久的对象中，然后将该对象传递给线程。
	//std::ref()函数的作用在于，在函数模板中可以使用引用参数，而不是拷贝参数，这通常可以提高代码的性能。
	//std::ref(x)函数将x对象转换成了一个引用包装器，使函数能够修改x的值，在我看来就是匿名引用，指向临时变量内存，延长生命周期

	int a1 = 199;
	std::thread t1(foo,std::ref(a1));
	t1.join();
	std::cout << a1 << std::endl;

	//2.传递指针或引用指向局部变量
	int x = 1;
	std::thread t(foo1, &x); // 传递指向局部变量的指针
	t.join();


	//3.传递的指针或引用指向已释放的内存
	int* ptr = new int(1);
	std::cout << *ptr;
	std::thread t(foo2, *ptr); // 传递已经释放的内存
	delete ptr;//在t线程调用ptr时ptr已经被释放了，导致内存访问失败，解决方式依然是用ref()函数匿名引用
	t.join();


	//4. 类成员函数作为入口函数，类对象被提前释放
	//使用智能指针延长类对象的生命周期，并自动调用其析构函数来释放类对象
   // MyClass obj;
    //std::thread t(&MyClass::func, &obj);
    // obj 被提前销毁了，会导致未定义的行为
	/*上面的代码中，在创建线程之后，obj 对象立即被销毁了，这会导致在线程执
	行时无法访问 obj 对象，可能会导致程序崩溃或者产生未定义的行为。

	为了避免这个问题，可以使用智能指针 std::shared_ptr 来管理类对象的生命周期，确保在线程执行期
	间对象不会被销毁。具体来说，可以在创建线程之前，将类对象的指针封装在一个 std::shared_ptr 对象
	中，并将其作为参数传递给线程。这样，在线程执行期间，即
	使类对象的所有者释放了其所有权，std::shared_ptr 仍然会保持对象的生命周期，直到线程结束。
	*/

	std::shared_ptr<MyClass> obj = std::make_shared<MyClass>();
	std::thread t(&MyClass::func, obj);
	t.join();
	/*上面的代码中，使用 std::make_shared 创建了一个 MyClass 类对象，并将其封装在
	一个 std::shared_ptr 对象中。*/
	//其实就是智能指针通过一个对象管理需要被自动释放的资源，然后该对象调用析构函数去释放该资源

	
#endif

	//5.入口函数为类的私有成员函数
	//类中使用友元

	MyClass obj;
	std::thread thread_1(myThreadFunc, &obj);
	thread_1.join();


	return 0;
}
