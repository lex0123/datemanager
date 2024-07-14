#include <iostream>
#include <thread> // 包含标准线程库
#include <mutex> // 包含标准互斥量库
#include <condition_variable> // 包含条件变量库
#include <cstring>
#include "user.h"
#include "task.h"
// Function to load users from a file
using namespace std;
const std::string filename = "users.txt";
void showHelp(const string& appName) {
    std::cout << "Usage: " << appName << " [options] command\n"
              << "\nOptions:\n"
              << "  -h, --help          Show this help message and exit\n"
              << "\nCommands:\n"
              << "  run                 Run in interactive mode\n"
              << "  username password addtask task           Use the username and password provided to log in and then add task1 in the list\n"
              << "  username password showtask               Use the username and password provided to log in and then show all tasks\n"
              << "  username password deltask task           Use the username and password provided to log in and then delete the task1 \n";
}
void run()
{
    int choice=0;
    bool quit =false;
    string  IfLogin;
    while (choice!=3){
        std::cout << "1. Register" << std::endl;
        std::cout << "2. Login" << std::endl;
        std::cout << "3. Quit" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        if (choice == 1) {
            registerUser(filename);
        } else if (choice == 2) {
            std::string username, password;
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            IfLogin=loginUser(filename,username,password);
        } else if (choice == 3)
        {  IfLogin=" ";
        }else {
            std::cout << "Invalid choice!Please input again:" << std::endl;
        }
        if (IfLogin!=" ")
        {
            thread thread1(checkReminders,IfLogin);
            while (!quit) {
                TaskList userTasks=loadTasks(IfLogin);
                char select;
                std::cout << "1. View tasks" << std::endl;
                std::cout << "2. New task" << std::endl;
                std::cout << "3. Delete task" << std::endl;
                std::cout << "4. Log out" << std::endl;
                std::cout << "Enter choice: " << std::endl;
                std::cin >> select;
                if (select == '1') {
                    show_tasks(userTasks);
                    cin.ignore();
                    cout<<"Press any key and \"Enter\" key to continue";
                    cin.get();
                    cin.ignore();
                } else if (select == '2') {
                    if (!newTask(IfLogin)) {cout<<"Newtask fail!"<<endl;return;}
                    cin.ignore();
                    cout<<"Press any key and \"Enter\" key to continue";
                    cin.get();
                    cin.ignore();
                } else if (select == '3') {
                    DelTask(userTasks,IfLogin);
                    cin.ignore();
                    cout<<"Press any key and \"Enter\" key to continue";
                    cin.get();
                    cin.ignore();
                } else if (select == '4') {
                    quit =true;
                }else
                {
                    std::cout << "Invalid choice!" << std::endl;
                    cout<<"Press any key and \"Enter\" key to continue";
                    cin.ignore();
                    cin.get();
                    cin.ignore();
                }
            }
            thread1.join();
        }
    }
}
int main(int argc,char*argv[]) {
    std::vector<char*> commandArgs;
    if (argc==2) {
       if (strcmp(argv[1],"-h")==0||strcmp(argv[1],"-help")==0) {
           showHelp(argv[0]);
       }
       else if(strcmp(argv[1],"run")==0){
                run();
        }
       else
       {
           cerr<<"invalid parameter!"<<endl;
           return -1;
       }
    }
    else if (argc==3&&strcmp(argv[3],"-showtask")==0)
    {
        if (loginUser("user.txt",argv[1],argv[2])!=" ");
        TaskList task =loadTasks(argv[1]);
        show_tasks(task);
    }
    else if (argc==4&&strcmp(argv[3],"-addtask")==0)
    {
        if (loginUser("user.txt",argv[1],argv[2])!=" ");
        TaskList task =loadTasks(argv[1]);
        show_tasks(task);
    }
    else if (argc==4&&strcmp(argv[3],"-deltask")==0)
    {
        if (loginUser("user.txt",argv[1],argv[2])!=" ");
        TaskList task =loadTasks(argv[1]);
        string file=argv[1];
        DelTask(task,file);
    }
    else  cerr<<"invalid parameter!"<<endl;
    return 0;

}
