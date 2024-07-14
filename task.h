#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <stdlib.h>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>


#ifndef HW4_TASK_H
#define HW4_TASK_H

struct task
{
    int id;
    std::string name;
    int priority;
    int classify;
    tm notifyTime;
    tm doTime;
    task():id(0),name(" "),priority(1),classify(1), doTime({}),notifyTime({}) {
        notifyTime.tm_isdst=0;notifyTime.tm_sec=0;notifyTime.tm_wday=0;notifyTime.tm_yday=0;
        doTime.tm_isdst=0;doTime.tm_sec=0;doTime.tm_wday=0;doTime.tm_yday=0;
    };
    task(const task& A):id(A.id),name(A.name),priority(A.priority),classify(A.classify), doTime(A.doTime),notifyTime(A.notifyTime){}
};
struct TaskList
{
    int TaskNum;
    std::vector<task>taskList;
    TaskList(){TaskNum=0;}
    TaskList(int n){TaskNum=n;task aTask;taskList.insert(taskList.end(),TaskNum,aTask);}
    TaskList(const TaskList& A){
        TaskNum=A.TaskNum;
        taskList=A.taskList;
    }

};
bool isValidTm(const tm& time);
void  sort_do_time(TaskList &List);
bool time_equal (const tm &A,const tm &B);
bool time_less(const tm &A,const tm &B);
TaskList loadTasks(const  std::string& filename);
bool newTask(const  std::string& filename);
void show_tasks(TaskList &List);
void  DelTask(TaskList &List,std::string &filename);
void checkReminders(const std::string &filename);
void  sort_notify_time(TaskList &List);
#endif //HW4_TASK_H
