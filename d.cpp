#include<iostream>
#include<thread>
#include<string>
#include<mutex>
static Log* log = nullptr;
static std::once_flag once;
//call_once������ֻ�����̺߳�����ʹ�ã���main()�����лᱨ�������ڱ���������ֻ��ͬʱ��ʵ����һ��
class Log {
public:
	Log() {};
	Log(const Log&log) = delete;//��ֹ��������
	Log& operator=(const Log& log) = delete;//��ֹ��ֵ����
	static Log& GetInstance() {//����ģʽ���øú���ȥ��õ�����Ķ���
	/*	static Log log;
		return log;
		//����ģʽ����ǰ����ö������˾͸�
		*/
		//����ģʽ�����˲Ź������
		/*static Log* log = nullptr;
		if (!log) {
			log = new Log;
		}*/
		
		std::call_once(once, init);//ʹ��call_once���г�ʼ��������������������һ��������once_flag() �Ķ��󣬵ڶ����Ǹ����ʼ���ĺ���
		return *log;
	}
	static void init() {
		if (!log) {
			log = new Log;
		}
	}
	void PrintLog(std::string msg) {
		std::cout << __TIME__ << msg << std::endl;//�궨��ʱ��
	}
};
void Logpt() {
	Log::GetInstance().PrintLog("my mult_threads");
}

int main(){
	//����ģʽ��ָһ����ֻ��һ��ʵ����������һ����־��Ϊ��
	std::thread t1(Logpt);
	std::thread t2(Logpt);
	t1.join();
	t2.join();


	return 0;
}