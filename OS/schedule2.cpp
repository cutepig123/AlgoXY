// another kinds schdule
// assume one cpu
// Given n task and their coming time and expect duration, schedule each task and output each tasks waiting time
// less-time schedule
#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <assert.h>

struct Task{
    int startTime;
    int duration;

//    Task(){
//        startTime = duration = 0;
//    }
};

void schedule(std::vector<Task> const &tasks){
  //  std::sort(tasks.begin(), tasks.end(), [](Task const &l, Task const &r)
  //      {return l.startTime<r.startTime;});
    int time = 0;

    std::map<int,Task> mTaskStatus; //record how many active remaining tasks
    std::map<int,std::vector<Task> > mTaskHistory;

    for(int i = 0; i<tasks.size(); i++)
    {
        Task const& t=tasks[i];
        mTaskStatus[i] =t;

        int nextTime;

        if(i+1<tasks.size())
            nextTime = tasks[i+1].startTime;
        else
            nextTime += t.duration;

        assert(nextTime>time);
        int duration = nextTime - time;

        //find tasks with min duration
        printf("\tTask queue status:\n");
        for(auto &t:mTaskStatus )
            printf("\tTaskid %d duration %d\n", t.first, t.second.duration);
        auto p = std::min_element(mTaskStatus.begin(), mTaskStatus.end(), [](auto &a, auto &b){return a.second.duration<b.second.duration;});

        printf("Time %d Execute task %d duration %d\n",time, p->first, duration);

        p->second.duration -= duration;

        Task currTask;
        currTask.startTime = time;
        currTask.duration = duration;
        mTaskHistory[p->first].push_back(currTask);

        // process for next
        if(p->second.duration==0)
        {
            mTaskStatus.erase(p);
        }
        time = nextTime;
    }

    while (!mTaskStatus.empty()) {
        printf("\tTask queue status:\n");
        for(auto &t:mTaskStatus )
            printf("\tTaskid %d duration %d\n", t.first, t.second.duration);

        auto p = std::min_element(mTaskStatus.begin(), mTaskStatus.end(), [](auto &a, auto &b){return a.second.duration<b.second.duration;});
        printf("Time %d Execute task %d duration %d\n",time, p->first, p->second.duration);

        Task currTask;
        currTask.startTime = time;
        currTask.duration = p->second.duration;
        mTaskHistory[p->first].push_back(currTask);

        time += p->second.duration;
        mTaskStatus.erase(p);
    }

	//统计等待时间
	for (auto &t: mTaskHistory)
	{
		assert(t.second.size() > 0);
		int totalDuration = 0;

		for (size_t i = 0; i < t.second.size(); i++)
		{
			if (i == 0)
				totalDuration += t.second[i].startTime - tasks[t.first].startTime;
			else
				totalDuration += t.second[i].startTime - (t.second[i-1].startTime + t.second[i - 1].duration);
		}
		
		printf("Task %d total Waiting time %d\n", t.first, totalDuration);
	}
}

void testSchedule2(){
	schedule({ {0, 7}, {2, 4}, {4,1},{5,3},{6,1} });
}
