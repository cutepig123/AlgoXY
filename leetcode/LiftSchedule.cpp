//电梯调度模拟

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
	//			//如果电梯在最上层或者最下层，则挺住

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
	//				//如果想网上，
	//				if (cmd.d.outer.dir == Command::Data::Outer::Up)
	//				{
	//					//如果电梯停了，并且按钮楼层在下面，则启动电梯向上；否则
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
