//
//  EmployeeManager.hpp
//  EmployeeManagement
//
//  Created by Developer on 10/06/25.
//

#ifndef EmployeeManager_hpp
#define EmployeeManager_hpp

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <map>

enum class Department {
    HR,
    IT,
    SALES,
    MARKETING,
    FINANCE,
    OPERATIONS
};

enum class EmployeeStatus {
    ACTIVE,
    INACTIVE,
    TERMINATED,
    ON_LEAVE
};

enum class Position {
    INTERN,
    JUNIOR,
    SENIOR,
    MANAGER,
    DIRECTOR,
    VP,
    CEO
};

struct Date {
    int day, month, year;
    
    Date() : day(1), month(1), year(2025) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    
    std::string toString() const {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }
    
    bool isAfter(const Date& other) const {
        if (year != other.year) return year > other.year;
        if (month != other.month) return month > other.month;
        return day > other.day;
    }
    
    int getYearsDifference(const Date& other) const {
        return year - other.year;
    }
};

class Employee {
private:
    static int nextEmployeeId;
    int employeeId;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phone;
    Department department;
    Position position;
    double salary;
    Date hireDate;
    Date birthDate;
    EmployeeStatus status;
    std::string address;
    std::string emergencyContact;
    int vacationDays;
    int usedVacationDays;
    double performanceRating;
    std::vector<std::string> skills;
    
public:
    Employee(const std::string& first, const std::string& last, const std::string& emailAddr,
             const std::string& phoneNum, Department dept, Position pos, double empSalary)
        : employeeId(nextEmployeeId++), firstName(first), lastName(last), email(emailAddr),
          phone(phoneNum), department(dept), position(pos), salary(empSalary),
          status(EmployeeStatus::ACTIVE), vacationDays(20), usedVacationDays(0), performanceRating(3.0) {
        hireDate = Date();
        birthDate = Date(1, 1, 1990);
    }
    
    ~Employee() {}
    
    void promote(Position newPosition, double salaryIncrease) {
        position = newPosition;
        salary += salaryIncrease;
    }
    
    bool requestVacation(int days) {
        if (days > 0 && (usedVacationDays + days) <= vacationDays) {
            usedVacationDays += days;
            return true;
        }
        return false;
    }
    
    void addSkill(const std::string& skill) {
        if (std::find(skills.begin(), skills.end(), skill) == skills.end()) {
            skills.push_back(skill);
        }
    }
    
    void removeSkill(const std::string& skill) {
        skills.erase(std::remove(skills.begin(), skills.end(), skill), skills.end());
    }
    
    bool hasSkill(const std::string& skill) const {
        return std::find(skills.begin(), skills.end(), skill) != skills.end();
    }
    
    int getYearsOfService() const {
        Date today;
        return today.getYearsDifference(hireDate);
    }
    
    int getAge() const {
        Date today;
        return today.getYearsDifference(birthDate);
    }
    
    double getAnnualBonus() const {
        return salary * (performanceRating / 5.0) * 0.1; // Max 10% bonus
    }
    
    int getRemainingVacationDays() const {
        return vacationDays - usedVacationDays;
    }
    
    bool isEligibleForPromotion() const {
        return getYearsOfService() >= 2 && performanceRating >= 4.0 && status == EmployeeStatus::ACTIVE;
    }
    
    std::string getFullName() const {
        return firstName + " " + lastName;
    }
    
    // Getters
    int getId() const { return employeeId; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    Department getDepartment() const { return department; }
    Position getPosition() const { return position; }
    double getSalary() const { return salary; }
    Date getHireDate() const { return hireDate; }
    Date getBirthDate() const { return birthDate; }
    EmployeeStatus getStatus() const { return status; }
    std::string getAddress() const { return address; }
    std::string getEmergencyContact() const { return emergencyContact; }
    int getVacationDays() const { return vacationDays; }
    int getUsedVacationDays() const { return usedVacationDays; }
    double getPerformanceRating() const { return performanceRating; }
    std::vector<std::string> getSkills() const { return skills; }
    
    // Setters
    void setFirstName(const std::string& first) { firstName = first; }
    void setLastName(const std::string& last) { lastName = last; }
    void setEmail(const std::string& emailAddr) { email = emailAddr; }
    void setPhone(const std::string& phoneNum) { phone = phoneNum; }
    void setDepartment(Department dept) { department = dept; }
    void setSalary(double empSalary) { salary = empSalary; }
    void setBirthDate(const Date& date) { birthDate = date; }
    void setStatus(EmployeeStatus empStatus) { status = empStatus; }
    void setAddress(const std::string& addr) { address = addr; }
    void setEmergencyContact(const std::string& contact) { emergencyContact = contact; }
    void setVacationDays(int days) { vacationDays = days; }
    void setPerformanceRating(double rating) { 
        performanceRating = std::max(1.0, std::min(5.0, rating)); 
    }
    
    std::string getDepartmentString() const {
        switch (department) {
            case Department::HR: return "Human Resources";
            case Department::IT: return "Information Technology";
            case Department::SALES: return "Sales";
            case Department::MARKETING: return "Marketing";
            case Department::FINANCE: return "Finance";
            case Department::OPERATIONS: return "Operations";
            default: return "Unknown";
        }
    }
    
    std::string getPositionString() const {
        switch (position) {
            case Position::INTERN: return "Intern";
            case Position::JUNIOR: return "Junior";
            case Position::SENIOR: return "Senior";
            case Position::MANAGER: return "Manager";
            case Position::DIRECTOR: return "Director";
            case Position::VP: return "Vice President";
            case Position::CEO: return "CEO";
            default: return "Unknown";
        }
    }
    
    std::string getStatusString() const {
        switch (status) {
            case EmployeeStatus::ACTIVE: return "Active";
            case EmployeeStatus::INACTIVE: return "Inactive";
            case EmployeeStatus::TERMINATED: return "Terminated";
            case EmployeeStatus::ON_LEAVE: return "On Leave";
            default: return "Unknown";
        }
    }
    
    void printEmployee() const {
        std::cout << "Employee [" << employeeId << "] " << getFullName() << std::endl;
        std::cout << "  Email: " << email << " | Phone: " << phone << std::endl;
        std::cout << "  Department: " << getDepartmentString() << std::endl;
        std::cout << "  Position: " << getPositionString() << std::endl;
        std::cout << "  Salary: $" << std::fixed << std::setprecision(2) << salary << std::endl;
        std::cout << "  Hire Date: " << hireDate.toString() << " (" << getYearsOfService() << " years)" << std::endl;
        std::cout << "  Age: " << getAge() << " | Status: " << getStatusString() << std::endl;
        std::cout << "  Performance: " << std::fixed << std::setprecision(1) << performanceRating << "/5.0" << std::endl;
        std::cout << "  Vacation: " << usedVacationDays << "/" << vacationDays << " days used" << std::endl;
        
        if (isEligibleForPromotion()) {
            std::cout << "  🎯 Eligible for promotion!" << std::endl;
        }
        
        if (!skills.empty()) {
            std::cout << "  Skills: ";
            for (size_t i = 0; i < skills.size(); ++i) {
                std::cout << skills[i];
                if (i < skills.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
        }
        
        if (!address.empty()) {
            std::cout << "  Address: " << address << std::endl;
        }
        
        if (!emergencyContact.empty()) {
            std::cout << "  Emergency Contact: " << emergencyContact << std::endl;
        }
    }
    
    void printCompact() const {
        std::cout << std::setw(4) << employeeId << " | " 
                  << std::setw(20) << getFullName().substr(0, 20) << " | "
                  << std::setw(15) << getDepartmentString().substr(0, 15) << " | "
                  << std::setw(12) << getPositionString() << " | "
                  << "$" << std::setw(8) << std::fixed << std::setprecision(0) << salary << " | "
                  << std::setw(8) << getStatusString() << std::endl;
    }
};

int Employee::nextEmployeeId = 1;

class Project {
private:
    static int nextProjectId;
    int projectId;
    std::string projectName;
    std::string description;
    Date startDate;
    Date endDate;
    std::vector<int> assignedEmployeeIds;
    bool isCompleted;
    double budget;
    int managerId;
    
public:
    Project(const std::string& name, const std::string& desc, int manager, double projectBudget)
        : projectId(nextProjectId++), projectName(name), description(desc),
          isCompleted(false), budget(projectBudget), managerId(manager) {
        startDate = Date();
        endDate = Date(startDate.day + 90, startDate.month, startDate.year); // 3 months default
    }
    
    ~Project() {}
    
    void assignEmployee(int employeeId) {
        if (std::find(assignedEmployeeIds.begin(), assignedEmployeeIds.end(), employeeId) == assignedEmployeeIds.end()) {
            assignedEmployeeIds.push_back(employeeId);
        }
    }
    
    void removeEmployee(int employeeId) {
        assignedEmployeeIds.erase(std::remove(assignedEmployeeIds.begin(), assignedEmployeeIds.end(), employeeId), 
                                 assignedEmployeeIds.end());
    }
    
    void completeProject() {
        isCompleted = true;
    }
    
    bool isOverdue() const {
        Date today;
        return today.isAfter(endDate) && !isCompleted;
    }
    
    int getTeamSize() const {
        return assignedEmployeeIds.size();
    }
    
    // Getters
    int getId() const { return projectId; }
    std::string getName() const { return projectName; }
    std::string getDescription() const { return description; }
    Date getStartDate() const { return startDate; }
    Date getEndDate() const { return endDate; }
    std::vector<int> getAssignedEmployeeIds() const { return assignedEmployeeIds; }
    bool getIsCompleted() const { return isCompleted; }
    double getBudget() const { return budget; }
    int getManagerId() const { return managerId; }
    
    // Setters
    void setName(const std::string& name) { projectName = name; }
    void setDescription(const std::string& desc) { description = desc; }
    void setEndDate(const Date& date) { endDate = date; }
    void setBudget(double projectBudget) { budget = projectBudget; }
    void setManagerId(int manager) { managerId = manager; }
    
    void printProject() const {
        std::cout << "Project [" << projectId << "] " << projectName << std::endl;
        std::cout << "  Description: " << description << std::endl;
        std::cout << "  Manager ID: " << managerId << std::endl;
        std::cout << "  Duration: " << startDate.toString() << " to " << endDate.toString() << std::endl;
        std::cout << "  Budget: $" << std::fixed << std::setprecision(2) << budget << std::endl;
        std::cout << "  Team Size: " << getTeamSize() << " employees" << std::endl;
        std::cout << "  Status: " << (isCompleted ? "Completed" : "In Progress");
        if (isOverdue()) {
            std::cout << " ⚠️ OVERDUE";
        }
        std::cout << std::endl;
        
        if (!assignedEmployeeIds.empty()) {
            std::cout << "  Assigned Employees: ";
            for (size_t i = 0; i < assignedEmployeeIds.size(); ++i) {
                std::cout << assignedEmployeeIds[i];
                if (i < assignedEmployeeIds.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
        }
    }
};

int Project::nextProjectId = 1;

class Department_Info {
private:
    Department department;
    std::string departmentHead;
    double budget;
    int employeeCount;
    std::vector<int> employeeIds;
    
public:
    Department_Info(Department dept, const std::string& head, double deptBudget)
        : department(dept), departmentHead(head), budget(deptBudget), employeeCount(0) {}
    
    ~Department_Info() {}
    
    void addEmployee(int employeeId) {
        if (std::find(employeeIds.begin(), employeeIds.end(), employeeId) == employeeIds.end()) {
            employeeIds.push_back(employeeId);
            employeeCount++;
        }
    }
    
    void removeEmployee(int employeeId) {
        auto it = std::find(employeeIds.begin(), employeeIds.end(), employeeId);
        if (it != employeeIds.end()) {
            employeeIds.erase(it);
            employeeCount--;
        }
    }
    
    double getBudgetPerEmployee() const {
        return employeeCount > 0 ? budget / employeeCount : 0.0;
    }
    
    // Getters
    Department getDepartment() const { return department; }
    std::string getDepartmentHead() const { return departmentHead; }
    double getBudget() const { return budget; }
    int getEmployeeCount() const { return employeeCount; }
    std::vector<int> getEmployeeIds() const { return employeeIds; }
    
    // Setters
    void setDepartmentHead(const std::string& head) { departmentHead = head; }
    void setBudget(double deptBudget) { budget = deptBudget; }
    
    std::string getDepartmentString() const {
        switch (department) {
            case Department::HR: return "Human Resources";
            case Department::IT: return "Information Technology";
            case Department::SALES: return "Sales";
            case Department::MARKETING: return "Marketing";
            case Department::FINANCE: return "Finance";
            case Department::OPERATIONS: return "Operations";
            default: return "Unknown";
        }
    }
    
    void printDepartmentInfo() const {
        std::cout << "Department: " << getDepartmentString() << std::endl;
        std::cout << "  Head: " << departmentHead << std::endl;
        std::cout << "  Budget: $" << std::fixed << std::setprecision(2) << budget << std::endl;
        std::cout << "  Employees: " << employeeCount << std::endl;
        std::cout << "  Budget per Employee: $" << std::fixed << std::setprecision(2) << getBudgetPerEmployee() << std::endl;
    }
};

class EmployeeManagementSystem {
private:
    std::vector<std::unique_ptr<Employee>> employees;
    std::vector<std::unique_ptr<Project>> projects;
    std::map<Department, Department_Info> departments;
    std::string companyName;
    int totalEmployees;
    double totalPayroll;
    
public:
    EmployeeManagementSystem(const std::string& company) 
        : companyName(company), totalEmployees(0), totalPayroll(0.0) {
        
        // Initialize departments
        departments.emplace(Department::HR, Department_Info(Department::HR, "TBD", 500000));
        departments.emplace(Department::IT, Department_Info(Department::IT, "TBD", 1000000));
        departments.emplace(Department::SALES, Department_Info(Department::SALES, "TBD", 750000));
        departments.emplace(Department::MARKETING, Department_Info(Department::MARKETING, "TBD", 600000));
        departments.emplace(Department::FINANCE, Department_Info(Department::FINANCE, "TBD", 800000));
        departments.emplace(Department::OPERATIONS, Department_Info(Department::OPERATIONS, "TBD", 900000));
    }
    
    ~EmployeeManagementSystem() {}
    
    void addEmployee(std::unique_ptr<Employee> employee) {
        if (employee) {
            Department dept = employee->getDepartment();
            int empId = employee->getId();
            
            departments[dept].addEmployee(empId);
            totalEmployees++;
            totalPayroll += employee->getSalary();
            
            employees.push_back(std::move(employee));
        }
    }
    
    void addProject(std::unique_ptr<Project> project) {
        if (project) {
            projects.push_back(std::move(project));
        }
    }
    
    Employee* findEmployee(int employeeId) {
        auto it = std::find_if(employees.begin(), employees.end(),
            [employeeId](const std::unique_ptr<Employee>& employee) {
                return employee->getId() == employeeId;
            });
        return (it != employees.end()) ? it->get() : nullptr;
    }
    
    Project* findProject(int projectId) {
        auto it = std::find_if(projects.begin(), projects.end(),
            [projectId](const std::unique_ptr<Project>& project) {
                return project->getId() == projectId;
            });
        return (it != projects.end()) ? it->get() : nullptr;
    }
    
    std::vector<Employee*> getEmployeesByDepartment(Department dept) {
        std::vector<Employee*> result;
        for (const auto& employee : employees) {
            if (employee->getDepartment() == dept) {
                result.push_back(employee.get());
            }
        }
        return result;
    }
    
    std::vector<Employee*> getEmployeesByPosition(Position pos) {
        std::vector<Employee*> result;
        for (const auto& employee : employees) {
            if (employee->getPosition() == pos) {
                result.push_back(employee.get());
            }
        }
        return result;
    }
    
    std::vector<Employee*> getHighPerformers() {
        std::vector<Employee*> result;
        for (const auto& employee : employees) {
            if (employee->getPerformanceRating() >= 4.0) {
                result.push_back(employee.get());
            }
        }
        return result;
    }
    
    bool promoteEmployee(int employeeId, Position newPosition, double salaryIncrease) {
        Employee* employee = findEmployee(employeeId);
        if (employee && employee->isEligibleForPromotion()) {
            employee->promote(newPosition, salaryIncrease);
            totalPayroll += salaryIncrease;
            return true;
        }
        return false;
    }
    
    void calculatePayroll() {
        totalPayroll = 0.0;
        for (const auto& employee : employees) {
            if (employee->getStatus() == EmployeeStatus::ACTIVE) {
                totalPayroll += employee->getSalary();
            }
        }
    }
    
    double getAverageSalary() const {
        int activeEmployees = 0;
        double totalActive = 0.0;
        
        for (const auto& employee : employees) {
            if (employee->getStatus() == EmployeeStatus::ACTIVE) {
                totalActive += employee->getSalary();
                activeEmployees++;
            }
        }
        
        return activeEmployees > 0 ? totalActive / activeEmployees : 0.0;
    }
    
    void printCompanySummary() {
        calculatePayroll();
        
        std::cout << "=== " << companyName << " Employee Summary ===" << std::endl;
        std::cout << "Total Employees: " << totalEmployees << std::endl;
        std::cout << "Total Annual Payroll: $" << std::fixed << std::setprecision(2) << totalPayroll << std::endl;
        std::cout << "Average Salary: $" << std::fixed << std::setprecision(2) << getAverageSalary() << std::endl;
        
        // Department breakdown
        std::cout << "\nDepartment Breakdown:" << std::endl;
        for (const auto& pair : departments) {
            pair.second.printDepartmentInfo();
            std::cout << std::endl;
        }
        
        // High performers
        auto highPerformers = getHighPerformers();
        std::cout << "High Performers (4.0+ rating): " << highPerformers.size() << std::endl;
        
        // Active projects
        int activeProjects = 0;
        for (const auto& project : projects) {
            if (!project->getIsCompleted()) {
                activeProjects++;
            }
        }
        std::cout << "Active Projects: " << activeProjects << std::endl;
    }
    
    void printAllEmployees() {
        std::cout << "=== All Employees ===" << std::endl;
        std::cout << std::setw(4) << "ID" << " | " 
                  << std::setw(20) << "Name" << " | "
                  << std::setw(15) << "Department" << " | "
                  << std::setw(12) << "Position" << " | "
                  << std::setw(10) << "Salary" << " | "
                  << std::setw(8) << "Status" << std::endl;
        std::cout << std::string(85, '-') << std::endl;
        
        for (const auto& employee : employees) {
            employee->printCompact();
        }
    }
    
    void printAllProjects() {
        std::cout << "=== All Projects ===" << std::endl;
        for (const auto& project : projects) {
            project->printProject();
            std::cout << std::endl;
        }
    }
};

int main() {
    std::cout << "=== Employee Management System ===" << std::endl;
    
    EmployeeManagementSystem company("TechCorp Solutions");
    
    // Add employees
    company.addEmployee(std::make_unique<Employee>("John", "Smith", "john@techcorp.com", "555-1001", Department::IT, Position::SENIOR, 85000));
    company.addEmployee(std::make_unique<Employee>("Sarah", "Johnson", "sarah@techcorp.com", "555-1002", Department::HR, Position::MANAGER, 95000));
    company.addEmployee(std::make_unique<Employee>("Mike", "Davis", "mike@techcorp.com", "555-1003", Department::SALES, Position::JUNIOR, 55000));
    company.addEmployee(std::make_unique<Employee>("Lisa", "Wong", "lisa@techcorp.com", "555-1004", Department::IT, Position::JUNIOR, 65000));
    company.addEmployee(std::make_unique<Employee>("David", "Brown", "david@techcorp.com", "555-1005", Department::MARKETING, Position::SENIOR, 75000));
    company.addEmployee(std::make_unique<Employee>("Emily", "Wilson", "emily@techcorp.com", "555-1006", Department::FINANCE, Position::MANAGER, 90000));
    
    // Set additional employee details
    Employee* john = company.findEmployee(1);
    if (john) {
        john->addSkill("C++");
        john->addSkill("Python");
        john->addSkill("Database Design");
        john->setPerformanceRating(4.2);
        john->setAddress("123 Tech Street, Silicon Valley");
        john->setEmergencyContact("Jane Smith - 555-2001");
    }
    
    Employee* sarah = company.findEmployee(2);
    if (sarah) {
        sarah->addSkill("Leadership");
        sarah->addSkill("Recruitment");
        sarah->addSkill("Training");
        sarah->setPerformanceRating(4.5);
    }
    
    Employee* mike = company.findEmployee(3);
    if (mike) {
        mike->addSkill("Customer Relations");
        mike->addSkill("Negotiation");
        mike->setPerformanceRating(3.8);
        mike->requestVacation(5);
    }
    
    // Add projects
    company.addProject(std::make_unique<Project>("Website Redesign", "Complete overhaul of company website", 1, 50000));
    company.addProject(std::make_unique<Project>("Sales Campaign Q2", "Quarterly sales campaign for new products", 3, 25000));
    company.addProject(std::make_unique<Project>("HR System Upgrade", "Upgrade employee management system", 2, 75000));
    
    // Assign employees to projects
    Project* websiteProject = company.findProject(1);
    if (websiteProject) {
        websiteProject->assignEmployee(1); // John
        websiteProject->assignEmployee(4); // Lisa
    }
    
    Project* salesProject = company.findProject(2);
    if (salesProject) {
        salesProject->assignEmployee(3); // Mike
        salesProject->assignEmployee(5); // David
    }
    
    // Print initial status
    std::cout << "\n=== Initial Company Status ===" << std::endl;
    company.printCompanySummary();
    
    std::cout << "\n=== Employee Details ===" << std::endl;
    if (john) {
        john->printEmployee();
        std::cout << std::endl;
    }
    
    if (sarah) {
        sarah->printEmployee();
        std::cout << std::endl;
    }
    
    // Test promotion
    std::cout << "=== Testing Promotion ===" << std::endl;
    if (company.promoteEmployee(1, Position::MANAGER, 15000)) {
        std::cout << "John Smith promoted to Manager with $15,000 raise!" << std::endl;
    }
    
    if (company.promoteEmployee(4, Position::SENIOR, 10000)) {
        std::cout << "Lisa Wong promoted to Senior with $10,000 raise!" << std::endl;
    }
    
    // Print all employees
    std::cout << "\n";
    company.printAllEmployees();
    
    // Print all projects
    std::cout << "\n";
    company.printAllProjects();
    
    // Get high performers
    std::cout << "=== High Performers ===" << std::endl;
    auto highPerformers = company.getHighPerformers();
    for (Employee* emp : highPerformers) {
        std::cout << emp->getFullName() << " - Rating: " 
                  << std::fixed << std::setprecision(1) << emp->getPerformanceRating() << std::endl;
    }
    
    // Final summary
    std::cout << "\n=== Final Company Summary ===" << std::endl;
    company.printCompanySummary();
    
    return 0;
}

#endif /* EmployeeManager_hpp */