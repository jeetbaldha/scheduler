# Task Scheduler
Simple Timer-Based Task Scheduler in C++
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <mutex>
#include <cctype>

class Task 
{
public:
    std::string name;
    int timeRequired;
    Task(const std::string& n, int t) : name(n), timeRequired(t) {}
};

class TaskScheduler 
{
    std::vector<Task> tasks;
    std::mutex mtx;

public:
    void addTask(const std::string& name, int timeRequired) 
    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.emplace_back(name, timeRequired);
  }

    void executeTasks() 
    {
        while (true) 
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::lock_guard<std::mutex> lock(mtx);
            for (auto it = tasks.begin(); it != tasks.end();) 
            {
                if (--(it->timeRequired) <= 0) 
                {
                    std::cout << "Executing: " << it->name << "\n";
                    it = tasks.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }
};

bool isAlphabetic(const std::string& str) 
{
    for (char c : str) if (!std::isalpha(c)) return false;
    return true;
}

int main() 
{
    TaskScheduler scheduler;
    std::thread executor(&TaskScheduler::executeTasks, &scheduler);
    int choice;

    while (true) 
    {
        std::cout << "\n1. Add Task\n2. Exit\nChoose: ";
        std::cin >> choice;
        if (choice == 1) 
        {
            std::string name;
            std::cout << "Task name: ";
            std::cin >> name;
            if (!isAlphabetic(name)) 
            {
                std::cout << "Invalid name.\n";
                continue;
            }
            int timeRequired;
            std::cout << "Time required (seconds): ";
            std::cin >> timeRequired;
            if (timeRequired >= 0) scheduler.addTask(name, timeRequired);
            else std::cout << "Invalid time required.\n";
        } 
        else if (choice == 2) 
        {
            executor.detach();
            return 0;
        } else 
        {
            std::cout << "Invalid choice.\n";
        }
    }
}