#pragma once
#include <iostream>
#include <string>

/*���ģʽ��������ģʽ
  ���ܣ���������У��Բ�ͬ�ͺŵ��Ե�����ṩ������ʽӿ�*/

namespace lsr {
    class Examine
    {
    public:
        std::string Examine_result;

        Examine(std::string examine_result) : Examine_result(examine_result) {}

        // ��ʵ��ʵ���еķ���
        void examine_cpu()
        {
            std::cout << "examine cpu of " << Examine_result << std::endl;
        }

        void examine_gpu()
        {
            std::cout << "examine gpu of " << Examine_result << std::endl;
        }

    };

    // �������ʵ�ʵ��࣬�����ǽӿ�
    class ExamineWithAdapter
    {
    public:
        virtual void examineCpu()
        {

        }
        virtual void examineGpu()
        {

        }
        virtual ~ExamineWithAdapter() = default;
    };

    // �м����������,���̳�һ��Ŀ���࣬ʹ����ϵķ�ʽʵ��
    class Adapter : public ExamineWithAdapter
    {
    private:
        Examine* examine; // ���

    public:
        Adapter(std::string Examine_result)
        {
            this->examine = new Examine(Examine_result);
        }

        void examineCpu() override
        {
            std::cout << "with Adapter,";
            examine->examine_cpu();
        }

        void examineGpu() override
        {
            std::cout << "with Adapter,";
            examine->examine_gpu();
        }

        ~Adapter() = default;
    };


    void client_adapter()
    {
        Examine examine("Computer model 1");
        examine.examine_cpu();
        examine.examine_gpu();

        ExamineWithAdapter* adapter = new Adapter("Computer model 2");
        adapter->examineCpu();
        adapter->examineGpu();
    }
}