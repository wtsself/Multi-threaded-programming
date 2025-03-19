#include<iostream>
#include<thread>
#include<mutex>//互斥锁
std::mutex L;
std::mutex m1 ,m2;//两个互斥量
std::mutex mtx;//这里的互斥量被lock_guard()自动管理，类似于智能指针
std::timed_mutex mtx_time;//这里是可以参与时间相关操作的互斥量
int global=0;//定义一个全局共享资源
void foo(int &a) {
	
	for (int i = 0;i < 100;i++) {
		L.lock();//加锁操作，其它线程不能同时访问共享数据，只能一个线程使用
		a++;
		L.unlock();//解锁操作
	}
}

void fooly() {

	for (int i = 0;i < 100;i++) {
		std::lock_guard<std::mutex> lg(mtx);//构造互斥量封装类模板
		//lock_guard(_Mutex& _Mtx, adopt_lock_t) noexcept // strengthened
		//有两参数，第二参数表示让lock_guard接管你已经加锁的互斥量，在析构时由lock_guard解锁:std::lock_guard<std::mutex> lg(mtx,std::adopt_lock);
		global++;

	}
}

void fooly_u() {

	for (int i = 0;i < 2;i++) { 
		//std::unique_lock<std::mutex> lg(mtx);  与  std::lock_guard<std::mutex> lg(mtx);一样作用
		std::unique_lock<std::timed_mutex> lg(mtx_time,std::defer_lock);//延迟加锁
		if (lg.try_lock_for(std::chrono::seconds(2))) {//这里是尝试加锁，若阻塞则等待2秒，超过时间则跳过，返回flase
			std::this_thread::sleep_for(std::chrono::seconds(1));//线程等待1秒
			global++;//结果为3的情况是A线程连续两次都拿到数据进行两次加操作且一共sleep了2秒，而B线程在等待2秒后跳过一次循环，最后进行了一次加操作
		}
	}
}

void fooa() {
	m1.lock();
	m2.lock();
	m2.unlock();
	m1.unlock();
}

void foob() {
	m2.lock();
	m1.lock();
	m1.unlock();
	m2.unlock();
	//这样的时候会出现t1线程m1加锁后要m2加锁但m2被线程t2加锁了，反而t2要对m1加速也拿不到，出现循环等待的死锁情况
	//情况正常的原因是一个线程速度更快，在其它线程还没执行就执行完了，导致死锁没发生
	//解决方法是调整锁的逻辑，如这里就可以同时锁一样的互斥量：
	/*m1.lock();
	m2.lock();
	m2.unlock();
	m1.unlock();*/
}

int main() {
#if 0
	//1.互斥量解决多线程数据共享问题
	//如果不加锁，多线程会随机的访问共享变量，导致最终的结果各不相同
	int a = 100;
	std::thread t1(foo, std::ref(a));
	std::thread t2(foo, std::ref(a));
	t1.join();
	t2.join();
	std::cout << a << std::endl;

	//2.互斥量死锁
	//循环 等待 资源占有 互斥 排外
	std::thread t1(fooa);
	std::thread t2(foob);
	t1.join();
	t2.join();
	std::cout << "over" << std::endl;


	//3.lock_guard 

	/*std::lock_guard 是 C++ 标准库中的一种互斥量封装类，用于保护共享数据，防止多个线程同时访问同一资源而导致的数据竞争问题。
	std::lock_guard 的特点如下：
	当构造函数被调用时，该互斥量会被自动锁定。
	当析构函数被调用时，该互斥量会被自动解锁。
	std::lock_guard 对象不能复制或移动，因此它只能在局部作用域中使用。*/
	std::thread t1(fooly);
	std::thread t2(fooly);
	t1.join();
	t2.join();
	std::cout << global << std::endl;


#endif
	//4. std::unique_lock
	//是lock_guard的升级版，支持更多的操作
	/*std::unique_lock 是 C++ 标准库中提供的一个互斥量封装类，用于在多线程程序中对互斥量进行加锁和解锁操作。它的主要特点是可以对互
	斥量进行更加灵活的管理，包括延迟加锁、条件变量、超时等。*/
	//std::unique_lock 提供了以下几个成员函数：
	//lock()：尝试对互斥量进行加锁操作，如果当前互斥量已经被其他线程持有，则当前线程会被阻塞，直到互斥量被成功加锁。
	//try_lock()：尝试对互斥量进行加锁操作，如果当前互斥量已经被其他线程持有，则函数立即返回 false，否则返回 true。
	//try_lock_for(const std::chrono::duration<Rep, Period>&rel_time)：尝试对互斥量进行加锁操作，如果当前互斥量已经被其他线程持有，则当前线程会被阻塞，直到互斥量被成功加锁，或者超过了指定的时间。
	//try_lock_until(const std::chrono::time_point<Clock, Duration>&abs_time)：尝试对互斥量进行加锁操作，如果当前互斥量已经被其他线程持有，则当前线程会被阻塞，直到互斥量被成功加锁，或者超过了指定的时间点。
	//unlock()：对互斥量进行解锁操作。
	std::thread t1(fooly_u);
	std::thread t2(fooly_u);
	t1.join();
	t2.join();
	std::cout << global << std::endl;


	return 0;
}