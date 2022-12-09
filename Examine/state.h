#pragma once
#include <iostream> 
#include <time.h>
#include <stdlib.h>

/*���ģʽ��״̬ģʽ
  ���ܣ����Բ�����ͬ״̬��ת��*/
namespace lsr {
	// ״̬�����
	class Part;
	class State
	{
	public:
		virtual void Qualified() {}
		virtual void NotQualified() {}
		virtual void NotExamined() {}
		virtual void CurrentState(Part* part) {}
	};
	//���
	class  Part
	{
	private:
		State* examine_state;  //Ŀǰ״̬
		int examine_result;
	public:
		Part(State* state) : examine_state(state), examine_result(0) {}
		~Part() { delete examine_state; }
		int GetExamineResult() { return examine_result; }
		void SetExamineResult(int  result) { examine_result = result; }
		void SetState(State* state) { delete examine_state; examine_state = state; }
		void GetState() { examine_state->CurrentState(this); }
	};

	// �����״̬��
	//�ϸ�
	class QualifiedState : public State
	{
	public:
		void Qualified(Part* part)
		{
			std::cout << "������������ϸ�" << std::endl;
		}
		void CurrentState(Part* part) { Qualified(part); }
	};
	//���ϸ�
	class NotQualifiedState : public State
	{
	public:
		void NotQualified(Part* part)
		{
			std::cout << "��������������ϸ�" << std::endl;
		}
		void CurrentState(Part* part) { NotQualified(part); }
	};

	//δ���
	class NotExaminedState : public State
	{
	public:
		void NotExamined(Part* part)
		{
			if (part->GetExamineResult() == 0)
				std::cout << "�����ǰ״̬����δ���飡" << std::endl;
			else if (part->GetExamineResult() == 1)
			{
				part->SetState(new QualifiedState());
				part->GetState();
			}
			else if (part->GetExamineResult() == -1)
			{
				part->SetState(new NotQualifiedState());
				part->GetState();
			}
		}
		void CurrentState(Part* part) { NotExamined(part); }
	};


	//���԰���
	void client_state()
	{
		std::cout << "-----state pattern test begin----" << std::endl;
		Part* part1 = new Part(new NotExaminedState());
		part1->GetState();
		std::cout << "-------parts examine begin-------" << std::endl;
		std::cout << "please input examine result\n1:qualified;2:not qualified   ";
		char result = getchar();
		if (result == '1')
		{
			part1->SetExamineResult(1);
			part1->GetState();
		}
		else if (result == '2')
		{
			part1->SetExamineResult(-1);
			part1->GetState();
		}
	}
}