#include<iostream>
#include<thread>
//���̴߳���δ���������
void foo(int &a) {
	std::cout << a << std::endl;
	a++;
	
}

void foo1(int* ptr) {
	std::cout << *ptr << std::endl; // �����Ѿ������ٵ�ָ��
}
void foo2(int& x) {
	std::cout << x << std::endl; // �����Ѿ����ͷŵ��ڴ�
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
	//1.���������̴߳�����ʱ����
	//���̺߳���ִ��ʱ����ʱ����a1�����٣��Ӷ�����δ������Ϊ��
	//��������ǽ��������Ƶ�һ���־õĶ����У�Ȼ�󽫸ö��󴫵ݸ��̡߳�
	//std::ref()�������������ڣ��ں���ģ���п���ʹ�����ò����������ǿ�����������ͨ��������ߴ�������ܡ�
	//std::ref(x)������x����ת������һ�����ð�װ����ʹ�����ܹ��޸�x��ֵ�����ҿ��������������ã�ָ����ʱ�����ڴ棬�ӳ���������

	int a1 = 199;
	std::thread t1(foo,std::ref(a1));
	t1.join();
	std::cout << a1 << std::endl;

	//2.����ָ�������ָ��ֲ�����
	int x = 1;
	std::thread t(foo1, &x); // ����ָ��ֲ�������ָ��
	t.join();


	//3.���ݵ�ָ�������ָ�����ͷŵ��ڴ�
	int* ptr = new int(1);
	std::cout << *ptr;
	std::thread t(foo2, *ptr); // �����Ѿ��ͷŵ��ڴ�
	delete ptr;//��t�̵߳���ptrʱptr�Ѿ����ͷ��ˣ������ڴ����ʧ�ܣ������ʽ��Ȼ����ref()������������
	t.join();


	//4. ���Ա������Ϊ��ں������������ǰ�ͷ�
	//ʹ������ָ���ӳ��������������ڣ����Զ������������������ͷ������
   // MyClass obj;
    //std::thread t(&MyClass::func, &obj);
    // obj ����ǰ�����ˣ��ᵼ��δ�������Ϊ
	/*����Ĵ����У��ڴ����߳�֮��obj ���������������ˣ���ᵼ�����߳�ִ
	��ʱ�޷����� obj ���󣬿��ܻᵼ�³���������߲���δ�������Ϊ��

	Ϊ�˱���������⣬����ʹ������ָ�� std::shared_ptr �������������������ڣ�ȷ�����߳�ִ����
	����󲻻ᱻ���١�������˵�������ڴ����߳�֮ǰ����������ָ���װ��һ�� std::shared_ptr ����
	�У���������Ϊ�������ݸ��̡߳����������߳�ִ���ڼ䣬��
	ʹ�������������ͷ���������Ȩ��std::shared_ptr ��Ȼ�ᱣ�ֶ�����������ڣ�ֱ���߳̽�����
	*/

	std::shared_ptr<MyClass> obj = std::make_shared<MyClass>();
	std::thread t(&MyClass::func, obj);
	t.join();
	/*����Ĵ����У�ʹ�� std::make_shared ������һ�� MyClass ����󣬲������װ��
	һ�� std::shared_ptr �����С�*/
	//��ʵ��������ָ��ͨ��һ�����������Ҫ���Զ��ͷŵ���Դ��Ȼ��ö��������������ȥ�ͷŸ���Դ

	
#endif

	//5.��ں���Ϊ���˽�г�Ա����
	//����ʹ����Ԫ

	MyClass obj;
	std::thread thread_1(myThreadFunc, &obj);
	thread_1.join();


	return 0;
}
