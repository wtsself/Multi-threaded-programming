#include<iostream>
#include<thread>
#include<string>
#include<mutex>
static Log* log = nullptr;
static std::once_flag once;
//call_once函数，只能在线程函数中使用（在main()函数中会报错），用于保护单例类只能同时被实例化一次
class Log {
public:
	Log() {};
	Log(const Log&log) = delete;//禁止拷贝构造
	Log& operator=(const Log& log) = delete;//禁止赋值拷贝
	static Log& GetInstance() {//单例模式下用该函数去获得单例类的对象
	/*	static Log log;
		return log;
		//懒汉模式，提前构造好对象，来了就给
		*/
		//饿汉模式，来了才构造对象
		/*static Log* log = nullptr;
		if (!log) {
			log = new Log;
		}*/
		
		std::call_once(once, init);//使用call_once进行初始化，传入两个参数，第一个参数是once_flag() 的对象，第二个是负责初始化的函数
		return *log;
	}
	static void init() {
		if (!log) {
			log = new Log;
		}
	}
	void PrintLog(std::string msg) {
		std::cout << __TIME__ << msg << std::endl;//宏定义时间
	}
};
void Logpt() {
	Log::GetInstance().PrintLog("my mult_threads");
}

int main(){
	//单例模式，指一个类只有一个实例，上面以一个日志类为例
	std::thread t1(Logpt);
	std::thread t2(Logpt);
	t1.join();
	t2.join();


	return 0;
}