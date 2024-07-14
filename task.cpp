#include "task.h"
#include <thread>
using namespace std;
string Classifier[]={"    learning   "," entertainment ","      rest     "};
string Priority[]={"  high  priority "," medium priority ","  low  priority  "};
bool isValidTm(const  tm& time) {
    if (time.tm_year < 0) return false; // 年份从1900年开始计算
    if (time.tm_mon < 0 || time.tm_mon > 12) return false; // 月份范围是0-12
    if (time.tm_mday < 1 || time.tm_mday > 31) return false; // 日期范围是1-31
    if (time.tm_hour < 0 || time.tm_hour > 23) return false; // 小时范围是0-23
    if (time.tm_min < 0 || time.tm_min > 59) return false; // 分钟范围是0-59

    // 检查每个月的天数
    static const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int monthDays = daysInMonth[time.tm_mon];

    // 闰年检查
    if (time.tm_mon == 1) { // 2月
        int year = time.tm_year + 1900;
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            monthDays = 29;
        }
    }

    if (time.tm_mday > monthDays) return false;
    return true;
}
bool time_equal (const tm &A,const tm &B)
{
    return (A.tm_year == B.tm_year
            &&A.tm_mon == B.tm_mon
            &&A.tm_mday == B.tm_mday
            &&A.tm_hour == B.tm_hour
            &&A.tm_min == B.tm_min);
}
bool time_less(const tm &A,const tm &B)
{
    if (A.tm_year < B.tm_year)return true;
    else if (A.tm_year > B.tm_year)return false;
    if (A.tm_mon < B.tm_mon)return true;
    else if (A.tm_mon > B.tm_mon)return false;
    if (A.tm_mday < B.tm_mday)return true;
    else if (A.tm_mday > B.tm_mday)return false;
    if (A.tm_hour < B.tm_hour)return true;
    else if (A.tm_hour > B.tm_hour)return false;
    if (A.tm_min < B.tm_min)return true;
    else return false;
}
void  sort_do_time(TaskList &List)
{
    sort(List.taskList.begin(), List.taskList.end(), [](const task&A,const task&B){return time_less(A.doTime,B.doTime);});
}
void  sort_notify_time(TaskList &List)
{
    sort(List.taskList.begin(), List.taskList.end(), [](const task&A,const task&B){return time_less(A.notifyTime,B.notifyTime);});
}
TaskList loadTasks(const  string& filename) {
    ifstream infile(filename);
    int n;
    task aTask;
    infile>>n;
    TaskList usertask(n);
    for(int i=0;i<usertask.TaskNum;i++)
    {
        infile>>usertask.taskList[i].id >> usertask.taskList[i].name>>usertask.taskList[i].priority>>usertask.taskList[i].classify;
        infile>>usertask.taskList[i].notifyTime.tm_year>>usertask.taskList[i].notifyTime.tm_mon>>usertask.taskList[i].notifyTime.tm_mday>>usertask.taskList[i].notifyTime.tm_hour>>usertask.taskList[i].notifyTime.tm_min;
        infile>>usertask.taskList[i].doTime.tm_year>>usertask.taskList[i].doTime.tm_mon>>usertask.taskList[i].doTime.tm_mday>>usertask.taskList[i].doTime.tm_hour>>usertask.taskList[i].doTime.tm_min;
    }
    return usertask;
}
void saveTask(const  string& filename,task &Atask) {
    ofstream outfile(filename,  ios_base::app);
    outfile<<Atask.id <<'\t'<< Atask.name<<'\t'<<Atask.priority<<'\t'<<Atask.classify<<endl;
    outfile<<Atask.notifyTime.tm_year<<'\t'<<Atask.notifyTime.tm_mon<<'\t';
    outfile<<Atask.notifyTime.tm_mday<<'\t'<<Atask.notifyTime.tm_hour<<'\t';
    outfile<<Atask.notifyTime.tm_min<<endl;
    outfile<<Atask.doTime.tm_year<<'\t'<<Atask.doTime.tm_mon<<'\t'<<Atask.doTime.tm_mday<<'\t';
    outfile<<Atask.doTime.tm_hour<<'\t'<<Atask.doTime.tm_min<<endl;
}
bool newTask(const  string& filename)
{
    task aTask;
    string input;
    cin.ignore();
    cout << "Enter task name: "<<endl;
    getline( cin, aTask.name);
    TaskList check=loadTasks(filename);
    for(int i=0;i<check.TaskNum;i++)
    {
        if (check.taskList[i].name==aTask.name)
        {cout<<"the task exists"<<endl;
            return false;}
    }
    // Input and validate aTask priority
    cout << "1 indicates the highest priority\n"
            "2 indicates the second highest priority\n"
            "3 indicates the lowest priority\nEnter task priority (1-3): \n";
    cin >> aTask.priority;
    if (aTask.priority < 1 || aTask.priority > 3) {
        cerr << "Invalid priority, must be between 1 and 3." <<  endl;
        return false;
    }

    // Input and validate aTask classify
    cout << "1 for learning\n"
            "2 for entertainment\n"
            "3 for rest\nEnter task classify (1-3):\n ";
    cin >> aTask.classify;
    if (aTask.classify < 1 || aTask.classify > 3) {
        cerr << "Invalid classify, must be between 1 and 3." <<  endl;
        return false;
    }
    cout << "Enter notice time (YYYY MM DD HH MM)"<< endl;cin >> aTask.notifyTime.tm_year  >> aTask.notifyTime.tm_mon >> aTask.notifyTime.tm_mday  >> aTask.notifyTime.tm_hour >>aTask.notifyTime.tm_min;
    if (!isValidTm(aTask.notifyTime)) {
        cerr << "Invalid notify time format." <<  endl;
        return false;
    }

    // Input and validate do time
    cout << "Enter do time (YYYY MM DD HH MM): "<< endl;
    cin >> aTask.doTime.tm_year>> aTask.doTime.tm_mon  >> aTask.doTime.tm_mday  >> aTask.doTime.tm_hour  >>aTask.doTime.tm_min;
    for(int i=0;i<check.TaskNum;i++) {
        if (time_equal(aTask.doTime,check.taskList[i].doTime)) {
            cout << "You have a task to handle during this time period,the task's name is" <<' '<<check.taskList[i].name
                 << endl;
            return false;
        }
    }
    if (!isValidTm(aTask.doTime)) {
        cerr << "Invalid do time format." <<  endl;
        return false;
    }
    // 打开文件进行读写
    fstream file(filename,  ios::in |  ios::out);
    if (!file) {
        cerr << "无法打开文件: " << filename <<  endl;
        return false;
    }
    // 读取第一个数值
    int firstNumber;
    file >> firstNumber;

    // 对第一个数值加1
    firstNumber += 1;
    aTask.id=firstNumber;
    // 移动文件定位指针到文件开头
    file.seekp(0);
    // 写回修改后的数值
    file << firstNumber;
    file.close(); // 关闭文件
    saveTask(filename,aTask);
    return true;
}

void show_tasks(TaskList &List)
{
    int choice;
    cout << "1. Display in chronological order:" <<  endl;
    cout << "2. Display in order by ID:" <<  endl;
    cout << "Enter choice: " <<  endl;
    cin>>choice;
    if(choice==1) {
        sort_do_time(List);
    }
    else if(choice==2){
        sort(List.taskList.begin(), List.taskList.end(), [](const task &task1, const task &task2) {
            return task1.id<task2.id;
        });
    }
    else {
        cout<<"Invalid choice!"<<endl;
        return;
    }
    cout<<"task id"<<" |  task   name  "<<setw(10)<<"|    priority     |"<<setw(10)<<"    classify   |  "<<setw(12)<<"reminder time"<<setw(10)<<"   |     do time      |"<<endl;
    for(int i=0;i<List.taskList.size();i++)
    {
        cout<<List.taskList[i].id<<"       |  "<<setw(10)<<setfill(' ')<< left<<List.taskList[i].name<<"   |"<<Priority[List.taskList[i].priority-1]<<"|"<<Classifier[List.taskList[i].classify-1]<<"|";
        cout<<" "<<List.taskList[i].notifyTime.tm_year<<"-"<< setw(2)<<setfill('0')<<right<<List.taskList[i].notifyTime.tm_mon<<"-"<<  setw(2)<<setfill('0')<<right<<List.taskList[i].notifyTime.tm_mday<<' '<<  setw(2)<<setfill('0')<<right<<List.taskList[i].notifyTime.tm_hour<<":"<<  setw(2)<<setfill('0')<<right<<List.taskList[i].notifyTime.tm_min<<" |";
        cout<<" "<<List.taskList[i].doTime.tm_year<<"-"<< setw(2)<<setfill('0')<<right<<List.taskList[i].doTime.tm_mon<<"-"<<  setw(2)<<setfill('0')<<right<<List.taskList[i].doTime.tm_mday<<' '<<  setw(2)<<setfill('0')<<right<<List.taskList[i].doTime.tm_hour<<":"<<  setw(2)<<setfill('0')<<right<<List.taskList[i].doTime.tm_min<<" |"<<endl;
    }
}
void  DelTask(TaskList &List,string &filename)
{
    int n;
    cout<<"Please input task id:"<<endl;
    cin>>n;
    sort(List.taskList.begin(),List.taskList.end(),[](const task &A,const task &B){return A.id<B.id;});
    if(n>=0&&n<=List.TaskNum)List.taskList.erase(List.taskList.begin()+n-1);
    else {cout<<"Error!Invalid id!";return;}
    List.TaskNum--;
    ofstream  outfile(filename,ofstream::trunc);
    outfile<<List.TaskNum<<endl;
    for(int i=0;i<List.TaskNum;i++)
    {
        if (List.taskList[i].id>n)List.taskList[i].id--;
        outfile<<List.taskList[i].id <<'\t'<< List.taskList[i].name<<'\t'<<List.taskList[i].priority<<'\t'<<List.taskList[i].classify<<endl;
        outfile<<List.taskList[i].notifyTime.tm_year<<'\t'<<List.taskList[i].notifyTime.tm_mon<<'\t';
        outfile<<List.taskList[i].notifyTime.tm_mday<<'\t'<<List.taskList[i].notifyTime.tm_hour<<'\t';
        outfile<<List.taskList[i].notifyTime.tm_min<<endl;
        outfile<<List.taskList[i].doTime.tm_year<<'\t'<<List.taskList[i].doTime.tm_mon<<'\t'<<List.taskList[i].doTime.tm_mday<<'\t';
        outfile<<List.taskList[i].doTime.tm_hour<<'\t'<<List.taskList[i].doTime.tm_min<<endl;
    }
    cout<<"Your task has been deleted and the task id has changed, please recheck"<<endl;
    show_tasks(List);
    outfile.close();
}

void checkReminders(const string &filename) {
    TaskList List= loadTasks(filename);
    sort_do_time(List);
    while (true) {
        std::this_thread::sleep_for(chrono::seconds (5));
        std::time_t currentTime = std::time(nullptr);
        std::tm* localTime = std::localtime(&currentTime);
        int i=0;
        tm givenTime = {};
        for (;i<List.TaskNum;i++)
        {
            givenTime.tm_year = List.taskList[i].notifyTime.tm_year-1900; // 年份从1900开始
            givenTime.tm_mon = List.taskList[i].notifyTime.tm_mon-1;            // 0-based, 7月是6
            givenTime.tm_mday = List.taskList[i].notifyTime.tm_mday;          // 日期
            givenTime.tm_hour = List.taskList[i].notifyTime.tm_hour;
            givenTime.tm_min = List.taskList[i].notifyTime.tm_min;           // 分钟
            if (time_less(givenTime,*localTime)) {
                i++;
            } else if (time_equal(givenTime,*localTime)) {
                cout<<"Don't forget that you need to complete "<<List.taskList[i].name<<" at ";
                cout<<List.taskList[i].doTime.tm_year<<'-'<<List.taskList[i].doTime.tm_mon<<'-';
                cout<<List.taskList[i].doTime.tm_mday<< " "<<List.taskList[i].doTime.tm_hour<<':';
                cout<<List.taskList[i].doTime.tm_min<<endl;
                cout<<"Please continue..."<<endl;
                std::this_thread::sleep_for(chrono::seconds(10));
            }
        }
    }
}

