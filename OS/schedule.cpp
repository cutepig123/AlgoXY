//cpu/resource/process schedule
#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <assert.h>

enum ProcessPriority{
    Low,
    Mid,
    High
};

enum ResourceType{
    Resource,
    Cpu
};

struct ResourceInfo{
    ResourceType type;
    int resourceId;
    int duration;
};

struct ProcessInfo
{
    ProcessPriority prio;
    std::deque<ResourceInfo> vResourceInfo;
};

struct ResourceUsage{
    int processId;
    int processPri;
    int startTime;
    int duration;
};

//可剥夺式优先级调度
//cpu and resouce都是独占资源，并且高优先级的随时可以中断低优先级的任务
//方法，每个资源都有一个队列，上面列出执行时间
void schedule(std::vector<ProcessInfo> const &vprocess){
    std::map<int, std::vector<ResourceUsage>> mresouceusageHistory; //资源使用历史记录
    std::map<int, std::vector<ResourceUsage>> mresouceQueue;    //资源队列

//    int rid=0;
//    for(ProcessInfo const & info: vprocess){
//        for(ResourceInfo const &res: info.vinfo)
//        {
//            ResourceUsage u={rid, info.prio, 0,res.duration};
//            mresouceQueue[res.resourceId].push_back(u);
//        }

//        rid++;
//    }
//    int i=1;

    std::vector<ProcessInfo> vprocess2 (vprocess.begin(), vprocess.end());

    int maxRid= 5;
    int currentTime=0;
    while (1) {
         // 每个资源都找一个合适的可运行的process，从vector删除他。加入到历史
        std::vector<int>  resoucePidSelection(maxRid);
        for(int rid=0; rid<maxRid; rid++){

            //找一个合适的可运行的process,according to priority
            int selectedPidx=-1;
            int maxPrio=-1;
            int pidx=0;
            for(ProcessInfo const &pinfo: vprocess2){
                if(!pinfo.vResourceInfo.empty()){
                    if(pinfo.vResourceInfo[0].resourceId==rid && pinfo.prio>maxPrio ){
                        maxPrio = pinfo.prio;
                        selectedPidx = pidx;
                    }
                }
                pidx++;
            }

            resoucePidSelection[rid] = selectedPidx;
        }

        //select a min time slot
       int duration = 100000;
       for(int rid=0; rid<maxRid; rid++){
           int selectedPidx = resoucePidSelection[rid];

           if(selectedPidx>=0){
               ProcessInfo const &pinfo = vprocess2[selectedPidx];

                   if(!pinfo.vResourceInfo.empty() && pinfo.vResourceInfo[0].resourceId==rid){
                       duration = std::min(duration, pinfo.vResourceInfo[0].duration);
                   }

           }
       }

       if(duration==100000)
       {
           break;
       }

        for(int rid=0; rid<maxRid; rid++){
            int selectedPidx = resoucePidSelection[rid];

            if(selectedPidx>=0)
            {
                ResourceUsage usage;
                usage.duration = duration;
                usage.processId = selectedPidx;
                usage.processPri = vprocess2[selectedPidx].prio;
                usage.startTime = currentTime;
                mresouceusageHistory[rid].push_back(usage);

                ResourceInfo& rinfo = vprocess2[selectedPidx].vResourceInfo[0];
                rinfo.duration -= duration;
                assert(rinfo.duration>=0);
                if(rinfo.duration==0){
                    vprocess2[selectedPidx].vResourceInfo.pop_front();
                }

                printf("Current time: %d, duration %d, Resource %d, pid %d, remaining tasks %d\n",
                       currentTime, duration,rid, selectedPidx, vprocess2[selectedPidx].vResourceInfo.size() );
            }
        }

        currentTime += duration;
    }
}

void testSchedule(){
    std::deque<ResourceInfo> info1={{Resource, 1, 20}, {Cpu, 0, 20}, {Resource, 3, 20}};
    ProcessInfo p1={High, info1};

    std::deque<ResourceInfo> info2={{Resource, 3, 40}, {Cpu, 0, 30}, {Resource, 2, 20}};
    ProcessInfo p2={Mid, info2};

    std::deque<ResourceInfo> info3={{Cpu, 0, 30}, {Resource, 2, 20}, {Cpu, 0, 20}};
    ProcessInfo p3={Low, info3};

    schedule({p1, p2, p3});
}

//#include <QtTest/QtTest>

//class TestQString: public QObject
//{
//    Q_OBJECT
//private slots:
//    void toUpper();
//};

//void TestQString::toUpper()
//{
//    QString str = "Hello";
//    QCOMPARE(str.toUpper(), QString("HELLO"));
//}

//QTEST_MAIN(TestQString)
//#include "TestQString.moc"
