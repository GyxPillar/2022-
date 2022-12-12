#pragma once
#include <iostream> 
#include <time.h>
#include <stdlib.h>





/*���ģʽ���Ž�ģʽ
  ���ܣ����Լ��飬����һ��������̵Ľӿ��࣬�����ṩʵ��*/

namespace lsr {
    // ʵ����ӿ� Implementation��ʵ����ӿڣ�������ģʽ
    class ExamineMode {
    public:
        virtual bool security() = 0;
    };

    // ����ʵ���ࣺ�������
    class ExamineParts : public ExamineMode {
    public:
        bool security() override {
            std::cout << "PartExamined, ";
            //���ô�Ʒ��Ϊ1/10
            return rand() % 10;
        }
    };

    // ����ʵ���ࣺ�������
    class ExaminePC : public ExamineMode {
    public:
        bool security() override {
            std::cout << "ComputerExamined, ";
            //���ô�Ʒ��Ϊ1/9
            return rand() % 9;
        }
    };

    // �����ࣺ����
    class Examinewithmode {
    public:
        Examinewithmode(ExamineMode* examineMode) {
            this->examineMode = examineMode;
        }

        virtual ~Examinewithmode() { delete examineMode; }

        virtual std::string examine() = 0;

        ExamineMode* examineMode;
    };

    // ��չ���󻯽�ɫ���������
    class PartsExamine : public  Examinewithmode {
    public:
        PartsExamine(ExamineMode* examineMode) : Examinewithmode(examineMode) { }
        //��չ������ ʵ���˳������ж���ķ���
        std::string examine() {
            bool security = examineMode->security();
            if (!security)
                return "not qualified";

            return "qualified";
        }
    };

    // ��չ���󻯽�ɫ���������
    class PCExamine : public  Examinewithmode {
    public:
        PCExamine(ExamineMode* examineMode) : Examinewithmode(examineMode) { }
        //��չ������ ʵ���˳������ж���ķ���
        std::string examine() {
            bool security = examineMode->security();
            if (!security)
                return "not qualified";

            return "qualified";
        }
    };

    // �ͻ���ʹ��
    void client_bridge() {
        srand((unsigned)time(NULL));
        Examinewithmode* CPUExamine = new PartsExamine(new ExamineParts());
        std::cout << "CPUExamine Result: " << CPUExamine->examine() << std::endl;
        Examinewithmode* GPUExamine = new PartsExamine(new ExamineParts());
        std::cout << "GPUExamine Result: " << GPUExamine->examine() << std::endl;
        Examinewithmode* MouseExamine = new PartsExamine(new ExamineParts());
        std::cout << "MouseExamine Result: " << MouseExamine->examine() << std::endl;
        Examinewithmode* KeyboardExamine = new PartsExamine(new ExamineParts());
        std::cout << "KeyboardExamine Result: " << KeyboardExamine->examine() << std::endl;

        Examinewithmode* ComputerExamine = new PCExamine(new ExaminePC());
        std::cout << "ComputerExamine Result: " << ComputerExamine->examine() << std::endl;
    }
}