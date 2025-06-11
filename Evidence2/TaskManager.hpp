//
//  TaskManager.hpp
//  TaskManagement
//
//  Created by Developer on 10/06/25.
//

#ifndef TaskManager_hpp
#define TaskManager_hpp

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <chrono>
#include <iomanip>
#include <map>

enum class Priority {
    LOW,
    MEDIUM,
    HIGH,
    URGENT
};

enum class Status {
    PENDING,
    IN_PROGRESS,
    COMPLETED,
    CANCELLED,
    BLOCKED
};

enum class Category {
    WORK,
    PERSONAL,
    STUDY,
    HEALTH,
    SHOPPING,
    OTHER
};

struct Date {
    int day, month, year;
    
    Date() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&time_t);
        day = tm.tm_mday;
        month = tm.tm_mon + 1;
        year = tm.tm_year + 1900;
    }
    
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    
    std::string toString() const {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }
    
    bool isAfter(const Date& other) const {
        if (year != other.year) return year > other.year;
        if (month != other.month) return month > other.month;
        return day > other.day;
    }
    
    bool isBefore(const Date& other) const {
        return other.isAfter(*this);
    }
    
    bool isToday() const {
        Date today;
        return day == today.day && month == today.month && year == today.year;
    }
    
    int daysDifference(const Date& other) const {
        return abs((year * 365 + month * 30 + day) - (other.year * 365 + other.month * 30 + other.day));
    }
};

class Task {
private:
    static int nextTaskId;
    int taskId;
    std::string title;
    std::string description;
    Priority priority;
    Status status;
    Category category;
    Date createdDate;
    Date dueDate;
    Date completedDate;
    int estimatedHours;
    int actualHours;
    std::vector<std::string> tags;
    std::vector<std::string> subtasks;
    bool isRecurring;
    int recurringDays;
    
public:
    Task(const std::string& taskTitle, const std::string& desc = "", 
         Priority prio = Priority::MEDIUM, Category cat = Category::OTHER)
        : taskId(nextTaskId++), title(taskTitle), description(desc), 
          priority(prio), status(Status::PENDING), category(cat),
          estimatedHours(1), actualHours(0), isRecurring(false), recurringDays(0) {
        createdDate = Date();
        dueDate = Date(createdDate.day + 7, createdDate.month, createdDate.year);
    }
    
    ~Task() {}
    
    void markCompleted() {
        if (status != Status::COMPLETED) {
            status = Status::COMPLETED;
            completedDate = Date();
        }
    }
    
    void markInProgress() {
        if (status == Status::PENDING || status == Status::BLOCKED) {
            status = Status::IN_PROGRESS;
        }
    }
    
    void markCancelled() {
        status = Status::CANCELLED;
    }
    
    void markBlocked() {
        status = Status::BLOCKED;
    }
    
    bool isOverdue() const {
        Date today;
        return today.isAfter(dueDate) && status != Status::COMPLETED && status != Status::CANCELLED;
    }
    
    bool isDueToday() const {
        return dueDate.isToday() && status != Status::COMPLETED;
    }
    
    bool isDueTomorrow() const {
        Date tomorrow(dueDate.day - 1, dueDate.month, dueDate.year);
        return tomorrow.isToday() && status != Status::COMPLETED;
    }
    
    int getDaysUntilDue() const {
        Date today;
        if (today.isAfter(dueDate)) return 0;
        return dueDate.daysDifference(today);
    }
    
    int getDaysOverdue() const {
        if (!isOverdue()) return 0;
        Date today;
        return today.daysDifference(dueDate);
    }
    
    void addTag(const std::string& tag) {
        if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
            tags.push_back(tag);
        }
    }
    
    void removeTag(const std::string& tag) {
        tags.erase(std::remove(tags.begin(), tags.end(), tag), tags.end());
    }
    
    void addSubtask(const std::string& subtask) {
        subtasks.push_back(subtask);
    }
    
    void removeSubtask(const std::string& subtask) {
        subtasks.erase(std::remove(subtasks.begin(), subtasks.end(), subtask), subtasks.end());
    }
    
    void setRecurring(int days) {
        isRecurring = true;
        recurringDays = days;
    }
    
    Task createNextRecurrence() const {
        if (!isRecurring) return *this;
        
        Task nextTask = *this;
        nextTask.taskId = nextTaskId++;
        nextTask.status = Status::PENDING;
        nextTask.createdDate = Date();
        nextTask.dueDate = Date(dueDate.day + recurringDays, dueDate.month, dueDate.year);
        nextTask.actualHours = 0;
        
        return nextTask;
    }
    
    double getCompletionRate() const {
        if (subtasks.empty()) {
            return status == Status::COMPLETED ? 100.0 : 0.0;
        }
        // Simplified - in real implementation would track subtask completion
        return status == Status::COMPLETED ? 100.0 : (status == Status::IN_PROGRESS ? 50.0 : 0.0);
    }
    
    // Getters
    int getId() const { return taskId; }
    std::string getTitle() const { return title; }
    std::string getDescription() const { return description; }
    Priority getPriority() const { return priority; }
    Status getStatus() const { return status; }
    Category getCategory() const { return category; }
    Date getCreatedDate() const { return createdDate; }
    Date getDueDate() const { return dueDate; }
    Date getCompletedDate() const { return completedDate; }
    int getEstimatedHours() const { return estimatedHours; }
    int getActualHours() const { return actualHours; }
    std::vector<std::string> getTags() const { return tags; }
    std::vector<std::string> getSubtasks() const { return subtasks; }
    bool getIsRecurring() const { return isRecurring; }
    int getRecurringDays() const { return recurringDays; }
    
    // Setters
    void setTitle(const std::string& taskTitle) { title = taskTitle; }
    void setDescription(const std::string& desc) { description = desc; }
    void setPriority(Priority prio) { priority = prio; }
    void setCategory(Category cat) { category = cat; }
    void setDueDate(const Date& date) { dueDate = date; }
    void setEstimatedHours(int hours) { estimatedHours = hours; }
    void setActualHours(int hours) { actualHours = hours; }
    
    std::string getPriorityString() const {
        switch (priority) {
            case Priority::LOW: return "Low";
            case Priority::MEDIUM: return "Medium";
            case Priority::HIGH: return "High";
            case Priority::URGENT: return "URGENT";
            default: return "Unknown";
        }
    }
    
    std::string getStatusString() const {
        switch (status) {
            case Status::PENDING: return "Pending";
            case Status::IN_PROGRESS: return "In Progress";
            case Status::COMPLETED: return "Completed";
            case Status::CANCELLED: return "Cancelled";
            case Status::BLOCKED: return "Blocked";
            default: return "Unknown";
        }
    }
    
    std::string getCategoryString() const {
        switch (category) {
            case Category::WORK: return "Work";
            case Category::PERSONAL: return "Personal";
            case Category::STUDY: return "Study";
            case Category::HEALTH: return "Health";
            case Category::SHOPPING: return "Shopping";
            case Category::OTHER: return "Other";
            default: return "Unknown";
        }
    }
    
    void printTask() const {
        std::cout << "Task [" << taskId << "] " << title << std::endl;
        std::cout << "  Description: " << description << std::endl;
        std::cout << "  Priority: " << getPriorityString() << " | Status: " << getStatusString() << std::endl;
        std::cout << "  Category: " << getCategoryString() << std::endl;
        std::cout << "  Created: " << createdDate.toString() << " | Due: " << dueDate.toString() << std::endl;
        
        if (status == Status::COMPLETED) {
            std::cout << "  Completed: " << completedDate.toString() << std::endl;
        }
        
        if (isOverdue()) {
            std::cout << "  ⚠️  OVERDUE by " << getDaysOverdue() << " days!" << std::endl;
        } else if (isDueToday()) {
            std::cout << "  📅 Due TODAY!" << std::endl;
        } else if (isDueTomorrow()) {
            std::cout << "  ⏰ Due tomorrow!" << std::endl;
        }
        
        std::cout << "  Time: " << actualHours << "/" << estimatedHours << " hours" << std::endl;
        
        if (!tags.empty()) {
            std::cout << "  Tags: ";
            for (size_t i = 0; i < tags.size(); ++i) {
                std::cout << "#" << tags[i];
                if (i < tags.size() - 1) std::cout << " ";
            }
            std::cout << std::endl;
        }
        
        if (!subtasks.empty()) {
            std::cout << "  Subtasks (" << subtasks.size() << "):" << std::endl;
            for (const auto& subtask : subtasks) {
                std::cout << "    - " << subtask << std::endl;
            }
        }
        
        if (isRecurring) {
            std::cout << "  🔄 Recurring every " << recurringDays << " days" << std::endl;
        }
    }
    
    void printCompact() const {
        std::cout << std::setw(4) << taskId << " | " 
                  << std::setw(25) << title.substr(0, 25) << " | "
                  << std::setw(8) << getPriorityString() << " | "
                  << std::setw(12) << getStatusString() << " | "
                  << std::setw(10) << dueDate.toString();
        
        if (isOverdue()) {
            std::cout << " ⚠️";
        } else if (isDueToday()) {
            std::cout << " 📅";
        }
        
        std::cout << std::endl;
    }
};

int Task::nextTaskId = 1;

class Project {
private:
    static int nextProjectId;
    int projectId;
    std::string projectName;
    std::string description;
    Date startDate;
    Date endDate;
    Status status;
    std::vector<int> taskIds;
    double budget;
    double spentAmount;
    
public:
    Project(const std::string& name, const std::string& desc = "")
        : projectId(nextProjectId++), projectName(name), description(desc),
          status(Status::PENDING), budget(0.0), spentAmount(0.0) {
        startDate = Date();
        endDate = Date(startDate.day + 30, startDate.month, startDate.year);
    }
    
    ~Project() {}
    
    void addTask(int taskId) {
        if (std::find(taskIds.begin(), taskIds.end(), taskId) == taskIds.end()) {
            taskIds.push_back(taskId);
        }
    }
    
    void removeTask(int taskId) {
        taskIds.erase(std::remove(taskIds.begin(), taskIds.end(), taskId), taskIds.end());
    }
    
    // Getters
    int getId() const { return projectId; }
    std::string getName() const { return projectName; }
    std::string getDescription() const { return description; }
    Date getStartDate() const { return startDate; }
    Date getEndDate() const { return endDate; }
    Status getStatus() const { return status; }
    std::vector<int> getTaskIds() const { return taskIds; }
    double getBudget() const { return budget; }
    double getSpentAmount() const { return spentAmount; }
    
    // Setters
    void setName(const std::string& name) { projectName = name; }
    void setDescription(const std::string& desc) { description = desc; }
    void setStartDate(const Date& date) { startDate = date; }
    void setEndDate(const Date& date) { endDate = date; }
    void setStatus(Status projectStatus) { status = projectStatus; }
    void setBudget(double amount) { budget = amount; }
    void addExpense(double amount) { spentAmount += amount; }
    
    void printProject() const {
        std::cout << "Project [" << projectId << "] " << projectName << std::endl;
        std::cout << "  Description: " << description << std::endl;
        std::cout << "  Duration: " << startDate.toString() << " to " << endDate.toString() << std::endl;
        std::cout << "  Status: " << (status == Status::COMPLETED ? "Completed" : "Active") << std::endl;
        std::cout << "  Tasks: " << taskIds.size() << std::endl;
        if (budget > 0) {
            std::cout << "  Budget: $" << std::fixed << std::setprecision(2) << spentAmount 
                      << " / $" << budget << std::endl;
        }
    }
};

int Project::nextProjectId = 1;

class TaskManager {
private:
    std::vector<std::unique_ptr<Task>> tasks;
    std::vector<std::unique_ptr<Project>> projects;
    std::map<Category, int> categoryStats;
    std::map<Priority, int> priorityStats;
    int totalTasks;
    int completedTasks;
    int pendingTasks;
    
public:
    TaskManager() : totalTasks(0), completedTasks(0), pendingTasks(0) {}
    
    ~TaskManager() {}
    
    void addTask(std::unique_ptr<Task> task) {
        if (task) {
            categoryStats[task->getCategory()]++;
            priorityStats[task->getPriority()]++;
            totalTasks++;
            
            if (task->getStatus() == Status::COMPLETED) {
                completedTasks++;
            } else {
                pendingTasks++;
            }
            
            tasks.push_back(std::move(task));
        }
    }
    
    void addProject(std::unique_ptr<Project> project) {
        if (project) {
            projects.push_back(std::move(project));
        }
    }
    
    Task* findTask(int taskId) {
        auto it = std::find_if(tasks.begin(), tasks.end(),
            [taskId](const std::unique_ptr<Task>& task) {
                return task->getId() == taskId;
            });
        return (it != tasks.end()) ? it->get() : nullptr;
    }
    
    Project* findProject(int projectId) {
        auto it = std::find_if(projects.begin(), projects.end(),
            [projectId](const std::unique_ptr<Project>& project) {
                return project->getId() == projectId;
            });
        return (it != projects.end()) ? it->get() : nullptr;
    }
    
    bool completeTask(int taskId) {
        Task* task = findTask(taskId);
        if (task && task->getStatus() != Status::COMPLETED) {
            task->markCompleted();
            completedTasks++;
            pendingTasks--;
            
            // Create next occurrence if recurring
            if (task->getIsRecurring()) {
                auto nextTask = std::make_unique<Task>(task->createNextRecurrence());
                addTask(std::move(nextTask));
            }
            
            return true;
        }
        return false;
    }
    
    bool deleteTask(int taskId) {
        auto it = std::find_if(tasks.begin(), tasks.end(),
            [taskId](const std::unique_ptr<Task>& task) {
                return task->getId() == taskId;
            });
        
        if (it != tasks.end()) {
            if ((*it)->getStatus() == Status::COMPLETED) {
                completedTasks--;
            } else {
                pendingTasks--;
            }
            totalTasks--;
            
            tasks.erase(it);
            return true;
        }
        return false;
    }
    
    std::vector<Task*> getTasksByCategory(Category category) {
        std::vector<Task*> result;
        for (const auto& task : tasks) {
            if (task->getCategory() == category) {
                result.push_back(task.get());
            }
        }
        return result;
    }
    
    std::vector<Task*> getTasksByPriority(Priority priority) {
        std::vector<Task*> result;
        for (const auto& task : tasks) {
            if (task->getPriority() == priority) {
                result.push_back(task.get());
            }
        }
        return result;
    }
    
    std::vector<Task*> getTasksByStatus(Status status) {
        std::vector<Task*> result;
        for (const auto& task : tasks) {
            if (task->getStatus() == status) {
                result.push_back(task.get());
            }
        }
        return result;
    }
    
    std::vector<Task*> getOverdueTasks() {
        std::vector<Task*> result;
        for (const auto& task : tasks) {
            if (task->isOverdue()) {
                result.push_back(task.get());
            }
        }
        return result;
    }
    
    std::vector<Task*> getTodaysTasks() {
        std::vector<Task*> result;
        for (const auto& task : tasks) {
            if (task->isDueToday()) {
                result.push_back(task.get());
            }
        }
        return result;
    }
    
    std::vector<Task*> searchTasks(const std::string& query) {
        std::vector<Task*> result;
        for (const auto& task : tasks) {
            if (task->getTitle().find(query) != std::string::npos ||
                task->getDescription().find(query) != std::string::npos) {
                result.push_back(task.get());
            }
        }
        return result;
    }
    
    void printDashboard() {
        std::cout << "=== Task Manager Dashboard ===" << std::endl;
        std::cout << "Total Tasks: " << totalTasks << std::endl;
        std::cout << "Completed: " << completedTasks << " | Pending: " << pendingTasks << std::endl;
        
        if (totalTasks > 0) {
            double completionRate = (static_cast<double>(completedTasks) / totalTasks) * 100;
            std::cout << "Completion Rate: " << std::fixed << std::setprecision(1) 
                      << completionRate << "%" << std::endl;
        }
        
        auto overdue = getOverdueTasks();
        auto today = getTodaysTasks();
        
        std::cout << "Overdue Tasks: " << overdue.size() << std::endl;
        std::cout << "Due Today: " << today.size() << std::endl;
        
        std::cout << "\n=== Quick Actions Needed ===" << std::endl;
        if (!overdue.empty()) {
            std::cout << "⚠️  Overdue Tasks:" << std::endl;
            for (Task* task : overdue) {
                std::cout << "  - " << task->getTitle() << " (Due: " 
                          << task->getDueDate().toString() << ")" << std::endl;
            }
        }
        
        if (!today.empty()) {
            std::cout << "📅 Due Today:" << std::endl;
            for (Task* task : today) {
                std::cout << "  - " << task->getTitle() << std::endl;
            }
        }
    }
    
    void printAllTasks() {
        std::cout << "=== All Tasks ===" << std::endl;
        if (tasks.empty()) {
            std::cout << "No tasks found." << std::endl;
            return;
        }
        
        std::cout << std::setw(4) << "ID" << " | " 
                  << std::setw(25) << "Title" << " | "
                  << std::setw(8) << "Priority" << " | "
                  << std::setw(12) << "Status" << " | "
                  << std::setw(10) << "Due Date" << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        
        for (const auto& task : tasks) {
            task->printCompact();
        }
    }
    
    void printTasksByCategory() {
        std::cout << "=== Tasks by Category ===" << std::endl;
        for (const auto& pair : categoryStats) {
            if (pair.second > 0) {
                std::cout << "Category: ";
                switch (pair.first) {
                    case Category::WORK: std::cout << "Work"; break;
                    case Category::PERSONAL: std::cout << "Personal"; break;
                    case Category::STUDY: std::cout << "Study"; break;
                    case Category::HEALTH: std::cout << "Health"; break;
                    case Category::SHOPPING: std::cout << "Shopping"; break;
                    case Category::OTHER: std::cout << "Other"; break;
                }
                std::cout << " (" << pair.second << " tasks)" << std::endl;
                
                auto categoryTasks = getTasksByCategory(pair.first);
                for (Task* task : categoryTasks) {
                    std::cout << "  - " << task->getTitle() << " [" 
                              << task->getStatusString() << "]" << std::endl;
                }
                std::cout << std::endl;
            }
        }
    }
    
    void printStatistics() {
        std::cout << "=== Task Statistics ===" << std::endl;
        std::cout << "Total Tasks: " << totalTasks << std::endl;
        std::cout << "Completed: " << completedTasks << std::endl;
        std::cout << "Pending: " << pendingTasks << std::endl;
        
        std::cout << "\nBy Priority:" << std::endl;
        for (const auto& pair : priorityStats) {
            std::cout << "  ";
            switch (pair.first) {
                case Priority::LOW: std::cout << "Low"; break;
                case Priority::MEDIUM: std::cout << "Medium"; break;
                case Priority::HIGH: std::cout << "High"; break;
                case Priority::URGENT: std::cout << "URGENT"; break;
            }
            std::cout << ": " << pair.second << std::endl;
        }
        
        std::cout << "\nBy Category:" << std::endl;
        for (const auto& pair : categoryStats) {
            std::cout << "  ";
            switch (pair.first) {
                case Category::WORK: std::cout << "Work"; break;
                case Category::PERSONAL: std::cout << "Personal"; break;
                case Category::STUDY: std::cout << "Study"; break;
                case Category::HEALTH: std::cout << "Health"; break;
                case Category::SHOPPING: std::cout << "Shopping"; break;
                case Category::OTHER: std::cout << "Other"; break;
            }
            std::cout << ": " << pair.second << std::endl;
        }
    }
};

int main() {
    std::cout << "=== Task Management System ===" << std::endl;
    
    TaskManager manager;
    
    // Create sample tasks
    auto task1 = std::make_unique<Task>("Complete project proposal", "Write and submit the Q4 project proposal", Priority::HIGH, Category::WORK);
    task1->setEstimatedHours(8);
    task1->addTag("urgent");
    task1->addTag("project");
    task1->addSubtask("Research requirements");
    task1->addSubtask("Draft proposal");
    task1->addSubtask("Review and edit");
    task1->addSubtask("Submit proposal");
    
    auto task2 = std::make_unique<Task>("Buy groceries", "Weekly grocery shopping", Priority::MEDIUM, Category::SHOPPING);
    task2->setEstimatedHours(2);
    task2->addTag("weekly");
    task2->setRecurring(7);
    
    auto task3 = std::make_unique<Task>("Exercise", "30 minutes cardio workout", Priority::HIGH, Category::HEALTH);
    task3->setEstimatedHours(1);
    task3->addTag("fitness");
    task3->addTag("daily");
    task3->setRecurring(1);
    
    auto task4 = std::make_unique<Task>("Study C++ templates", "Learn advanced template concepts", Priority::MEDIUM, Category::STUDY);
    task4->setEstimatedHours(4);
    task4->addTag("programming");
    task4->addTag("cpp");
    
    auto task5 = std::make_unique<Task>("Call dentist", "Schedule annual checkup", Priority::LOW, Category::HEALTH);
    task5->setEstimatedHours(1);
    
    // Add tasks to manager
    manager.addTask(std::move(task1));
    manager.addTask(std::move(task2));
    manager.addTask(std::move(task3));
    manager.addTask(std::move(task4));
    manager.addTask(std::move(task5));
    
    // Display dashboard
    manager.printDashboard();
    
    std::cout << "\n";
    manager.printAllTasks();
    
    std::cout << "\n";
    manager.printTasksByCategory();
    
    // Complete some tasks
    std::cout << "=== Completing Tasks ===" << std::endl;
    manager.completeTask(2); // Complete grocery shopping (will create recurring task)
    manager.completeTask(5); // Complete call dentist
    
    // Print updated dashboard
    std::cout << "\n=== Updated Dashboard ===" << std::endl;
    manager.printDashboard();
    
    std::cout << "\n";
    manager.printStatistics();
    
    return 0;
}

#endif /* TaskManager_hpp */