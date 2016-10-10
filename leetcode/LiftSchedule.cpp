//���ݵ���ģ��

#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <mutex>
#include <thread>
#include <assert.h>

using namespace std;

struct LiftInfo {
	int id;
	int floor;
	int velocity;		// with sign, +ve for up, -ve for down
	vector<int> destination;	
	LiftInfo():id(0), floor(0), velocity(0)
	{}
};

struct Command {
	enum Type {
		Outer,
		Inner,
		Stop
	}type;

	union Data {
		struct Outer
		{
			enum Dir
			{
				Up,
				Down
			}dir;
			int floor;	// which floor click the button?
		}outer;

		struct Inner
		{
			int destination;
		}inner;
	}d;
};

void testLift() {
	//std::vector<std::thread> grp;
	//int numFloor = 30;
	//int numLift = 5;
	//int velocity = 1;
	//vector<LiftInfo> vLifts(numLift);

	//vector<mutex> commandMtx(numLift);
	//vector<deque<Command> > commands(numLift);

	//for (size_t i = 0; i < numLift; i++)
	//{
	//	grp.push_back(std::thread([&] {
	//		auto &lift = vLifts[i];
	//		lift.id = i;

	//		while (true)
	//		{
	//			//������������ϲ�������²㣬��ͦס

	//			Command cmd;
	//			
	//			std::lock_guard<std::mutex> lock(commandMtx[i]);
	//			if (commands[i].empty()) {
	//				continue;
	//			}
	//			cmd = commands[i].front();
	//			
	//			switch (cmd.type) {
	//			case Command::Outer:
	//				
	//				//��������ϣ�
	//				if (cmd.d.outer.dir == Command::Data::Outer::Up)
	//				{
	//					//�������ͣ�ˣ����Ұ�ť¥�������棬�������������ϣ�����
	//					if (lift.velocity == 0) {}
	//					commands[i].pop_front();
	//				}
	//			}
	//			
	//		}
	//	}));
	//}
	//
	//for (auto& thread : grp)
	//	if (thread.joinable())
	//		thread.join();
}
