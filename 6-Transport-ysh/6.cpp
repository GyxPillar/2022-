﻿#include <iostream>
#include <vector>
#include <map>
#include <stack>
using namespace std;

#define MAX_STORE_NUM 3
#define MAX_TRANSPORT_END_NUM 5
namespace YshTransport {
	/**********************************************************************************
	工厂仓库:工厂完成生产后，货物存储在仓库中，等待运输
	***********************************************************************************/
	class FactoryStore {//工厂仓库定义
	public:
		FactoryStore(const int& i, const int& s = 0) :id(i), storage(s) {}//初始化
		int getId() { return id; }
		int getStorgae() { return storage; }
		void addStorage(const int& count) { storage += count; }//增加库存
		void moveStorage(const int& count) { storage -= count; }//搬出库存
	private:
		int id;		//仓库编号
		int storage;//仓库库存

	};

	struct TransportEdge {//存储网表的边的信息
		TransportEdge(const int& d = 0) { dest = d; link = NULL; }
		int dest;
		TransportEdge* link;
	};
	/**********************************************************************************
	迭代器
	***********************************************************************************/
	template <typename T, typename U>
	class Iterator {
	public:
		typedef typename std::vector<T>::iterator iter_type;
		Iterator(U* p_data, bool reverse = false) : m_p_data_(p_data) {
			m_it_ = m_p_data_->begin();
		}
		void First() {
			m_it_ = m_p_data_->begin();
		}
		void Next() {
			m_it_ = m_p_data_->next(m_it_);
		}
		bool IsDone() {
			return (m_it_ == m_p_data_->end());
		}
		iter_type Current() {
			return m_it_;
		}
	private:
		U* m_p_data_;	
		iter_type m_it_;
	};
	/***********************************************************************************
	物流网点:货物运输从仓库运输至物流网点
	************************************************************************************/
	class TransportEnd {
	public:
		TransportEnd(const int& i, const int& s = 0) :id(i), storage(s) { adj = NULL; };//初始化
		int getId()const { return id; }//返回ID
		TransportEdge* getFirstEdge()const { return adj; }
		void addStorage(const int& s) { storage += s; }//添加库存
		void addEdge(const int& d) {//添加新的边
			TransportEdge* p = adj;
			if (adj == NULL) {
				p = new TransportEdge(d);
				adj = p;
				return;
			}
			while (p->dest != d && p->link != NULL)p = p->link;
			if (p->dest == d)return;
			else if (p->link == NULL) {
				p->link = new TransportEdge(d);
				p = p->link;
				p->dest = d;
			}
			return;
		}
	private:
		int id;//网点编号
		int storage;//网点库存
		TransportEdge* adj;//边的头指针
	};
	/***********************************************************************************
	物流网:存储记录整个物流网的联通状况
	************************************************************************************/
	class TransportEndGraph {
	public:
		TransportEndGraph() {};//初始化
		TransportEnd getValue(int id) const {
			for (TransportEnd i : table) {
				if (i.getId() == id)return i;
			}
		}
		vector<TransportEnd>::iterator begin() { vector<TransportEnd>::iterator tmp = table.begin(); return tmp; }
		vector<TransportEnd>::iterator end() { vector<TransportEnd>::iterator tmp = table.end(); return tmp; }
		vector<TransportEnd>::iterator next(vector<TransportEnd>::iterator it) { 
			vector<TransportEnd>::iterator tmp = table.begin();
			for (; tmp != table.end(); tmp++) {
				if (tmp->getId() == it->getId()) {
					tmp++;
					return tmp;
				}
			}
		}
		int size()const { return table.size(); }	//返回当前顶点数
		bool insertVertex(const TransportEnd& vertex) {	//在图中插入一个顶点vertex
			table.push_back(vertex);
			return true;
		}
		bool insertEdge(const int& srcId, const int& endId) {	//在图中插入一条边(v1,v2)
			for (int i = 0; i < table.size(); i++) {
				if (table.at(i).getId() == srcId) {
					table.at(i).addEdge(endId);
					return true;
				}
			}
			return false;
		}
		void iterate(const int& id) {//遍历图，从初始点id开始遍历图
			stack<int> ste;
			vector<int> flag;
			TransportEdge* ptE;
			int tmp = findId(id);
			for (int i = 0; i < table.size(); i++)flag.push_back(0);
			ste.push(tmp);
			while (!ste.empty()) {
				tmp = ste.top();
				cout << table.at(tmp).getId() << endl;
				ste.pop();
				flag.at(tmp) = 1;
				ptE = table.at(tmp).getFirstEdge();
				while (ptE != NULL) {
					tmp = findId(ptE->dest);
					if (flag.at(tmp) == 0) {
						ste.push(tmp);
					}
					ptE = ptE->link;
				}
			}
		}
		Iterator<TransportEnd, TransportEndGraph>* CreateIterator() {
			return new Iterator<TransportEnd, TransportEndGraph>(this);
		}
	private:
		vector<TransportEnd> table;//存储图的邻接表结构
		int findId(const int& id)const {//根据id找到再vector中的位置
			for (int i = 0; i < table.size(); i++) {
				if (table.at(i).getId() == id)return i;
			}
		}
		friend class TransportEndGraphIterator;
	};

	struct route {//物流路线结构体
		route(int s = 0, int e = 0) :src(s), end(e) {}
		int src;//起点：仓库编号
		int end;//重点：网点编号
		bool operator<(const route& r) const
		{
			return (src < r.src) || (src == r.src && end < r.src);
		}
	};
	/***********************************************************************************
	物流公司:负责运输的物流公司，拥有车队流通于仓库和网点
	************************************************************************************/
	class TransportCompany {//物流公司定义
	public:
		TransportCompany(const int& i, const string& s) :id(i), name(s) {};//初始化
		string getName() { return name; }
		int getId() { return id; }
		int getTrunks(const route& r) { return trunks[r]; }
		void addTrunk(const route& r, const int& n) { trunks[r] = n; }
	private:
		int id;//公司编号
		string name;//物流公司名称
		map<route, int> trunks;//车池:<运输路线,货车数量>
	};

	/***********************************************************************************
	顾问接口类，可供main函数调用
	************************************************************************************/
	class Client {
	public:
		static bool inputInt(int& x, const int& limit = 10) {	//读入int型正整数数据
			//x为需要读入的数据，limit为该数据的最大值（默认为10），若不符合要求则不断重新输入，直到输入正确返回true，或输入0返回false
			int n = 0;
			char c = 0;
			while (1) {
				cin >> x;   //读入x的方式必须是cin>>int型变量
				cin.ignore(65535, '\n');
				if (n > 1) {	//判断是否陷入死循环，并给出输入提示
					cout << "请按提示正确输入！（若无反应请按回车键继续）" << endl << endl;
					c = getchar();
					n = 0;
				}
				if (x < 0 || cin.fail())
				{
					cout << "请输入正确的正整数（按0取消操作）" << endl;
					cin.clear();
				}
				else if (x > limit) {
					cout << "输入的数据合理最大值是" << limit << "，请勿超过该限制！（按0取消操作）" << endl;
				}
				else if (x == 0) {
					return false;
				}
				else if (x > 0 && x <= limit) {
					return true;
				}
				cout << "请重新输入：";
				n++;
			}
		}
		static void show_factoryStore(const vector<FactoryStore>& fs) {
			cout << "现有仓库" << fs.size() << "个" << endl;
			cout << "存储情况如下：" << endl;
			for (FactoryStore i : fs) {
				cout << "仓库编号：" << i.getId() << "   ";
				cout << "库存容量：" << i.getStorgae() << "   " << endl;
			}
		}
		static void show_transportCom(const vector<TransportCompany>& tC) {
			cout << "现有物流公司" << tC.size() << "个，分别为：" << endl;
			for (TransportCompany i : tC) { cout << i.getId() << " " << i.getName() << endl; }
		}
		static void show_transportEnd(TransportEndGraph tEG) {
			cout << "现有物流网点" << tEG.size() << "个，编号分别为" << endl;
			//tEG.iterate(1);//从编号为1开始遍历所有节点
			Iterator<TransportEnd, TransportEndGraph>* it = tEG.CreateIterator();//利用迭代器遍历图
			for (it->First(); !it->IsDone(); it->Next()) {
				cout << (*it->Current()).getId() << endl;
			}
		}
		static void add_storage(vector<FactoryStore>& fs) {
			show_factoryStore(fs);		//展示仓库
			int storeId, storeNum;
			cout << "请输入需要存货的仓库编号：";
			inputInt(storeId, fs.size());
			cout << "请输入存货的数量：";
			inputInt(storeNum, 5000);
			vector<FactoryStore>::iterator ifs;
			for (ifs = fs.begin(); ifs != fs.end(); ifs++) {	//迭代寻找仓库
				if (ifs->getId() == storeId) {
					ifs->addStorage(storeNum);
					break;
				}
			}
			cout << "已向" << storeId << "号仓库添加" << storeNum << "件货品" << endl;
			cout << "按回车键继续" << endl;
			while (getchar() != '\n') {}
		}
		static void trans_storage(vector<FactoryStore>& fs, TransportEndGraph& teg, vector<TransportCompany>& tc) {
			//选择仓库和发货数量
			show_factoryStore(fs);//展示仓库
			int storeId, storeNum;
			cout << "请输入要发货的仓库编号：";
			inputInt(storeId, fs.size());
			vector<FactoryStore>::iterator ifs;
			for (ifs = fs.begin(); ifs != fs.end(); ifs++) {	//迭代寻找仓库
				if (ifs->getId() == storeId) { break; }
			}
			cout << "请输入发货的数量：";
			inputInt(storeNum, ifs->getStorgae());

			//选择收货网点
			show_transportEnd(teg);//展示网点信息
			int endId;
			cout << "请输入要收货的网点编号：";
			inputInt(endId, teg.size());

			//选择物流公司
			show_transportCom(tc);//展示物流公司信息	
			while (1) {
				int cpyId;
				cout << "请输入选择的物流公司编号：";
				inputInt(cpyId, tc.size());
				int num = 0;
				vector<TransportCompany>::iterator itc;
				for (itc = tc.begin(); itc != tc.end(); itc++) {	//迭代寻找对应公司
					if (itc->getId() == cpyId) { break; }
				}
				num = itc->getTrunks(route(storeId, endId));//查看公司运力
				if (num * 1000 < storeNum) {				//运力不足
					cout << "物流公司共有" << num << "辆货车，" << "运力不足，输入1补足运力，输入0重新选择" << endl;
					int option;
					if (inputInt(option, 1)) {				//补足运力
						itc->addTrunk(route(storeId, endId), storeNum / 1000 + 1);
						cout << "运力成功补足!" << endl;
					}
					else { continue; }						//重新选择
				}
				break;
			}
			//发货
			ifs->moveStorage(storeNum);

			cout << "发货成功！！" << endl;
			cout << "按回车键继续" << endl;
			while (getchar() != '\n') {}
			return;
		}
	};
}

int main()
{
	//初始化仓库
	vector<YshTransport::FactoryStore> fStores;
	for (int i = 1; i <= MAX_STORE_NUM; i++) { YshTransport::FactoryStore tmp(i, 5000 - i * 1000); fStores.push_back(tmp); }
	//初始化物流网点
	YshTransport::TransportEndGraph tsEndGraph;
	for (int i = 1; i <= MAX_TRANSPORT_END_NUM; i++) { tsEndGraph.insertVertex(YshTransport::TransportEnd(i)); }
	tsEndGraph.insertEdge(1, 2);
	tsEndGraph.insertEdge(1, 3);
	tsEndGraph.insertEdge(1, 5);
	tsEndGraph.insertEdge(3, 4);
	//初始化物流公司
	vector<YshTransport::TransportCompany> tsCompys;
	string tmp = "ZT";
	tsCompys.push_back(YshTransport::TransportCompany(1, tmp));
	tmp = "SF";
	tsCompys.push_back(YshTransport::TransportCompany(2, tmp));
	tmp = "YZ";
	tsCompys.push_back(YshTransport::TransportCompany(3, tmp));
	int option;
	while (1)
	{
		system("cls");
		cout << "===============物流环节===============" << endl;
		cout << "主菜单：" << endl;
		cout << "1.存货" << endl;
		cout << "2.发货" << endl;
		cout << "0.退出" << endl;
		cout << "请选择需要的操作：";
		YshTransport::Client::inputInt(option, 2);
		switch (option) {
			case 1:	//存货
				YshTransport::Client::add_storage(fStores);
				break;
			case 2:	//发货
				YshTransport::Client::trans_storage(fStores, tsEndGraph, tsCompys);
				break;
			case 0:
				exit(0);
				break;
			default:
				break;
		}
	}
}