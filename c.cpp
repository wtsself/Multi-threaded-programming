#include<iostream>
#include<thread>
#include<mutex>//������
std::mutex L;
std::mutex m1 ,m2;//����������
std::mutex mtx;//����Ļ�������lock_guard()�Զ���������������ָ��
std::timed_mutex mtx_time;//�����ǿ��Բ���ʱ����ز����Ļ�����
int global=0;//����һ��ȫ�ֹ�����Դ
void foo(int &a) {
	
	for (int i = 0;i < 100;i++) {
		L.lock();//���������������̲߳���ͬʱ���ʹ������ݣ�ֻ��һ���߳�ʹ��
		a++;
		L.unlock();//��������
	}
}

void fooly() {

	for (int i = 0;i < 100;i++) {
		std::lock_guard<std::mutex> lg(mtx);//���컥������װ��ģ��
		//lock_guard(_Mutex& _Mtx, adopt_lock_t) noexcept // strengthened
		//�����������ڶ�������ʾ��lock_guard�ӹ����Ѿ������Ļ�������������ʱ��lock_guard����:std::lock_guard<std::mutex> lg(mtx,std::adopt_lock);
		global++;

	}
}

void fooly_u() {

	for (int i = 0;i < 2;i++) { 
		//std::unique_lock<std::mutex> lg(mtx);  ��  std::lock_guard<std::mutex> lg(mtx);һ������
		std::unique_lock<std::timed_mutex> lg(mtx_time,std::defer_lock);//�ӳټ���
		if (lg.try_lock_for(std::chrono::seconds(2))) {//�����ǳ��Լ�������������ȴ�2�룬����ʱ��������������flase
			std::this_thread::sleep_for(std::chrono::seconds(1));//�̵߳ȴ�1��
			global++;//���Ϊ3�������A�߳��������ζ��õ����ݽ������μӲ�����һ��sleep��2�룬��B�߳��ڵȴ�2�������һ��ѭ������������һ�μӲ���
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
	//������ʱ������t1�߳�m1������Ҫm2������m2���߳�t2�����ˣ�����t2Ҫ��m1����Ҳ�ò���������ѭ���ȴ����������
	//���������ԭ����һ���߳��ٶȸ��죬�������̻߳�ûִ�о�ִ�����ˣ���������û����
	//��������ǵ��������߼���������Ϳ���ͬʱ��һ���Ļ�������
	/*m1.lock();
	m2.lock();
	m2.unlock();
	m1.unlock();*/
}

int main() {
#if 0
	//1.������������߳����ݹ�������
	//��������������̻߳�����ķ��ʹ���������������յĽ��������ͬ
	int a = 100;
	std::thread t1(foo, std::ref(a));
	std::thread t2(foo, std::ref(a));
	t1.join();
	t2.join();
	std::cout << a << std::endl;

	//2.����������
	//ѭ�� �ȴ� ��Դռ�� ���� ����
	std::thread t1(fooa);
	std::thread t2(foob);
	t1.join();
	t2.join();
	std::cout << "over" << std::endl;


	//3.lock_guard 

	/*std::lock_guard �� C++ ��׼���е�һ�ֻ�������װ�࣬���ڱ����������ݣ���ֹ����߳�ͬʱ����ͬһ��Դ�����µ����ݾ������⡣
	std::lock_guard ���ص����£�
	�����캯��������ʱ���û������ᱻ�Զ�������
	����������������ʱ���û������ᱻ�Զ�������
	std::lock_guard �����ܸ��ƻ��ƶ��������ֻ���ھֲ���������ʹ�á�*/
	std::thread t1(fooly);
	std::thread t2(fooly);
	t1.join();
	t2.join();
	std::cout << global << std::endl;


#endif
	//4. std::unique_lock
	//��lock_guard�������棬֧�ָ���Ĳ���
	/*std::unique_lock �� C++ ��׼�����ṩ��һ����������װ�࣬�����ڶ��̳߳����жԻ��������м����ͽ���������������Ҫ�ص��ǿ��ԶԻ�
	�������и������Ĺ��������ӳټ�����������������ʱ�ȡ�*/
	//std::unique_lock �ṩ�����¼�����Ա������
	//lock()�����ԶԻ��������м��������������ǰ�������Ѿ��������̳߳��У���ǰ�̻߳ᱻ������ֱ�����������ɹ�������
	//try_lock()�����ԶԻ��������м��������������ǰ�������Ѿ��������̳߳��У������������� false�����򷵻� true��
	//try_lock_for(const std::chrono::duration<Rep, Period>&rel_time)�����ԶԻ��������м��������������ǰ�������Ѿ��������̳߳��У���ǰ�̻߳ᱻ������ֱ�����������ɹ����������߳�����ָ����ʱ�䡣
	//try_lock_until(const std::chrono::time_point<Clock, Duration>&abs_time)�����ԶԻ��������м��������������ǰ�������Ѿ��������̳߳��У���ǰ�̻߳ᱻ������ֱ�����������ɹ����������߳�����ָ����ʱ��㡣
	//unlock()���Ի��������н���������
	std::thread t1(fooly_u);
	std::thread t2(fooly_u);
	t1.join();
	t2.join();
	std::cout << global << std::endl;


	return 0;
}