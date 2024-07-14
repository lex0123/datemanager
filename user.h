#ifndef USER_H
#define USER_H

#include <fstream>
#include <iostream>
#include <string>
#include "picosha2.h"
#include <unordered_map>
#include <iomanip>
std::unordered_map<std::string, std::string> loadUsers(const std::string& filename);
void saveUser(const std::string& filename, const std::string& username, const std::string& password);
void registerUser(const std::string& filename);
std::string loginUser(const std::string& filename,const std::string& username,const std::string &password);
std::unordered_map<std::string, std::string> loadUsers(const std::string& filename) {
    std::unordered_map<std::string, std::string> users;
    std::ifstream infile(filename);
    std::string username, HashPassWd;
    while (infile >> username >> HashPassWd) {
        users[username] = HashPassWd;
    }
    return users;
}
// Function to save users to a file
void saveUser(const std::string& filename, const std::string& username, const std::string& password) {
    std::ofstream outfile(filename, std::ios_base::app);
    std::string HashPassWd;
    picosha2::hash256_hex_string(password,HashPassWd);
    outfile << username <<'\t'<< HashPassWd << std::endl;
}

// Function to register a new user
void registerUser(const std::string& filename) {
    std::unordered_map<std::string, std::string> users = loadUsers(filename);
    std::string username, password;
    std::cout << "Enter new username: ";
    std::cin >> username;
    while(users.count(username)!=0) {
        std::cout<<"The username already exists,Please input again!\nEnter new username:";
    }
    std::cout << "Enter new password: ";
    std::cin>>password;
    while(password.size()<4) {
        std::cout<<"Error! The size of password must be greater than 4\n Please input password again"<<std::endl;
    }
    saveUser(filename, username, password);
    std::fstream taskFile(username,std::ios_base::app);
    taskFile<<0<<std::endl;
    std::cout << "User registered successfully!" << std::endl;
}

// Function to log
std::string loginUser(const std::string& filename,const std::string& username,const std::string &password) {
    std::unordered_map<std::string, std::string> users = loadUsers(filename);
    std::string HashPassWd;
    picosha2::hash256_hex_string(password,HashPassWd);
    if (users.find(username) != users.end() && users[username] == HashPassWd) {
        std::cout << "Login successful!" << std::endl;
        return username;
    } else {
        std::cout << "Invalid username or password!" << std::endl;
        return " ";
    }
}//
//
#endif