#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

struct Student
{
    std::string name;
    unsigned int age;
    float grade;
};

void recordStudent(const Student &student)
{   
    std::ofstream fout("students.txt", std::ios::app);
    fout << student.name << "\t" << student.age << "\t" << student.grade << std::endl;
    fout.close();
}

void printStudent()
{   
    std::ifstream fin("students.txt");
    Student student;
    std::cout <<"Student list" << std::endl;

    while (fin >> student.name >> student.age >> student.grade)
    {
        std::cout << student.name << "\t" << student.age << "\t" << student.grade << std::endl;
    }
    
    fin.close();
}

void parseCommand(const std::string &commandLine)
{
    std::stringstream ss(commandLine);
    std::string command;
    ss >> command;

    if (command == "student")
    {
        Student student;
        if (ss >> student.name >> student.age >> student.grade)
            recordStudent(student);
        else
            std::cout << "Error: wrong format." << std::endl;
        
    }
    else if (command == "student-list")
    {
        printStudent();
    }
    else if (!command.empty() && command != "exit")
    {
        std::cout << "Unknow command!" << std::endl;
    }
}


int main(int, char **)
{
    std::string commandLine = "";

    std::cout << "Commands:" << std::endl;
    std::cout << "1. student <name> <age> <grade>" << std::endl;
    std::cout << "2. student-list" << std::endl;
    std::cout << "3. exit" << std::endl;

    while (commandLine != "exit")
    {
        std::cout <<"Input command: "<<std::endl;
        std::getline(std::cin, commandLine);
        parseCommand(commandLine);
    }

    return 0;
}