#include <iostream>
#include <thread>
//thread�̵߳Ļ���ʹ��
//�����߳�std::thread t(function_name, args...);
//`function_name`���߳���ڵ�ĺ�����ɵ��ö���
//`args...`�Ǵ��ݸ������Ĳ���
//�����̺߳����ǿ���ʹ��`t.join()`�����̵߳ȴ����߳���ɣ�����ʹ��`t.detach()`�����̣߳����̲߳��ȴ����̣߳������ں�̨���С�
// 
//������һ���̺߳󣬿���ѡ����� join() �� detach() �����������������ǻ���ģ���һ�������� join()���Ͳ���
//�ٵ��� detach()����֮��Ȼ��joinable() �������������ж��Ƿ��Ѿ����߳̽�������Щ������joinable()��������һ������ֵ
// ������߳̿��Ա�join()��detach()���򷵻�true
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
