#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct Employee
{
    int id;
    char name[50];
    char password[30];
    float hourlyrate;
    int isadmin;
};

struct AttendanceRecord
{
    int empid;
    char date[15];
    float hoursworked;
};

Employee *employees = nullptr;
AttendanceRecord *attendance = nullptr;
int empcount = 0;
int attcount = 0;

void initializesystem();
void cleanupsystem();
void saveemployeestofile();
void loademployeesfromfile();
void saveattendancetofile();
void loadattendancefromfile();
int login();
void addemployee();
void viewallemployees();
void markattendance(int empId);
void viewattendance(int empId);
void calculatepayroll(int empId);
void adminmenu(int empId);
void usermenu(int empId);
void mainmenu();

void loademployeesfromfile()
{
    ifstream file("employees.txt");
    if (!file)
        return;
    file >> empcount;
    file.ignore();
    if (empcount > 0)
    {
        employees = new Employee[empcount * 2];
        for (int i = 0; i < empcount; i++)
        {
            file >> employees[i].id;
            file.ignore();
            file.getline(employees[i].name, 50);
            file.getline(employees[i].password, 30);
            file >> employees[i].hourlyrate >> employees[i].isadmin;
            file.ignore();
        }
    }
    file.close();
}
void loadattendancefromfile()
{
    ifstream file("attendance.txt");
    if (!file)
        return;

    file >> attcount;
    file.ignore();

    if (attcount > 0)
    {
        attendance = new AttendanceRecord[attcount * 2];
        for (int i = 0; i < attcount; i++)
        {
            file >> attendance[i].empid;
            file.ignore();
            file.getline(attendance[i].date, 15);
            file >> attendance[i].hoursworked;
            file.ignore();
        }
    }
    file.close();
}
void initializesystem()
{
    loademployeesfromfile();
    loadattendancefromfile();
    if (empcount == 0)
    {
        employees = new Employee[1];
        employees[0].id = 1;
        strcpy(employees[0].name, "admin");
        strcpy(employees[0].password, "admin123");
        employees[0].hourlyrate = 25.0;
        employees[0].isadmin = 1;
        empcount = 1;
        saveemployeestofile();
        cout << "\n[Default admin created: username=admin, password=admin123]\n";
    }
}
void cleanupsystem()
{
    delete[] employees;
    delete[] attendance;
}
void saveemployeestofile()
{
    ofstream file("employees.txt");
    file << empcount << endl;
    for (int i = 0; i < empcount; i++)
    {
        file << employees[i].id << endl
             << employees[i].name << endl
             << employees[i].password << endl
             << employees[i].hourlyrate << endl
             << employees[i].isadmin << endl;
    }
    file.close();
}
void saveattendancetofile()
{
    ofstream file("attendance.txt");
    file << attcount << endl;
    for (int i = 0; i < attcount; i++)
    {
        file << attendance[i].empid << endl
             << attendance[i].date << endl
             << attendance[i].hoursworked << endl;
    }
    file.close();
}
int login()
{
    char name[50], pass[30];
    cout << "\n----- LOGIN -----\n";
    cout << "Username: ";
    cin >> name;
    cout << "Password: ";
    cin >> pass;
    for (int i = 0; i < empcount; i++)
    {
        if (strcmp(employees[i].name, name) == 0 && strcmp(employees[i].password, pass) == 0)
        {
            cout << "Login successful!\n";
            return i;
        }
    }
    cout << "Invalid credentials!\n";
    return -1;
}
void addemployee()
{
    Employee *temp = new Employee[empcount + 1];
    for (int i = 0; i < empcount; i++)
    {
        temp[i] = employees[i];
    }
    delete[] employees;
    employees = temp;
    employees[empcount].id = empcount + 1;
    cout << "\nEnter employee name: ";
    cin >> employees[empcount].name;
    cout << "Enter password: ";
    cin >> employees[empcount].password;
    cout << "Enter hourly rate: $";
    cin >> employees[empcount].hourlyrate;
    cout << "Is admin? (1/0): ";
    cin >> employees[empcount].isadmin;
    empcount++;
    saveemployeestofile();
    cout << "Employee added successfully!\n";
}
void viewallemployees()
{
    cout << "\n----- EMPLOYEE LIST -----\n";
    cout << "ID\tName\t\tRate\t\tRole\n";
    cout << "--------------------------------------------\n";
    for (int i = 0; i < empcount; i++)
    {
        cout << employees[i].id << "\t" << employees[i].name << "\t\t$" << employees[i].hourlyrate << "\t\t";
        if (employees[i].isadmin)
        {
            cout << "Admin";
        }
        else
        {
            cout << "Employee";
        }
        cout << endl;
    }
}
void markattendance(int empid)
{
    AttendanceRecord *temp = new AttendanceRecord[attcount + 1];
    for (int i = 0; i < attcount; i++)
    {
        temp[i] = attendance[i];
    }
    delete[] attendance;
    attendance = temp;
    attendance[attcount].empid = employees[empid].id;
    cout << "\nEnter date (YYYY-MM-DD): ";
    cin >> attendance[attcount].date;
    cout << "Enter hours worked: ";
    cin >> attendance[attcount].hoursworked;
    attcount++;
    saveattendancetofile();
    cout << "Attendance marked successfully!\n";
}
void viewattendance(int empid)
{
    int empidnum = employees[empid].id;
    cout << "\n----- ATTENDANCE RECORD -----\n";
    cout << "Date\t\tHours Worked\n";
    cout << "-----------------------------\n";
    float totalhours = 0;
    int recordcount = 0;
    for (int i = 0; i < attcount; i++)
    {
        if (attendance[i].empid == empidnum)
        {
            cout << attendance[i].date << "\t" << attendance[i].hoursworked << endl;
            totalhours += attendance[i].hoursworked;
            recordcount++;
        }
    }
    if (recordcount == 0)
    {
        cout << "No attendance records found.\n";
    }
    cout << "-----------------------------\n";
    cout << "Total Records: " << recordcount << endl;
    cout << "Total Hours: " << totalhours << endl;
}
void calculatepayroll(int empid)
{
    int empidnum = employees[empid].id;
    char month[8];
    cout << "\nEnter month (YYYY-MM): ";
    cin >> month;
    float totalhours = 0;
    for (int i = 0; i < attcount; i++)
    {
        if (attendance[i].empid == empidnum)
        {
            if (strncmp(attendance[i].date, month, 7) == 0)
            {
                totalhours += attendance[i].hoursworked;
            }
        }
    }
    float grosssalary = totalhours * employees[empid].hourlyrate;
    float tax = grosssalary * 0.10;
    float netsalary = grosssalary - tax;
    cout << "\n----- PAYROLL REPORT -----\n";
    cout << "Employee: " << employees[empid].name << endl;
    cout << "Month: " << month << endl;
    cout << "Total Hours: " << totalhours << endl;
    cout << "Hourly Rate: $" << employees[empid].hourlyrate << endl;
    cout << "Gross Salary: $" << grosssalary << endl;
    cout << "Tax (10%): $" << tax << endl;
    cout << "Net Salary: $" << netsalary << endl;
}
void adminmenu(int empid)
{
    int choice = -1;
    while (choice != 0)
    {
        cout << "\n----- ADMIN MENU -----\n";
        cout << "1. Add Employee\n";
        cout << "2. View All Employees\n";
        cout << "3. Mark My Attendance\n";
        cout << "4. View My Attendance\n";
        cout << "5. My Payroll\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;
        if (choice == 1)
            addemployee();
        else if (choice == 2)
            viewallemployees();
        else if (choice == 3)
            markattendance(empid);
        else if (choice == 4)
            viewattendance(empid);
        else if (choice == 5)
            calculatepayroll(empid);
        else if (choice == 0)
            cout << "Logging out...\n";
        else
            cout << "Invalid choice!\n";
    }
}
void usermenu(int empid)
{
    int choice = -1;
    while (choice != 0)
    {
        cout << "\n----- EMPLOYEE MENU -----\n";
        cout << "1. Mark Attendance\n";
        cout << "2. View My Attendance\n";
        cout << "3. View My Payroll\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
            markattendance(empid);
        else if (choice == 2)
            viewattendance(empid);
        else if (choice == 3)
            calculatepayroll(empid);
        else if (choice == 0)
            cout << "Logging out...\n";
        else
            cout << "Invalid choice!\n";
    }
}
void mainmenu()
{
    int choice = -1;
    while (choice != 0)
    {
        cout << "\n---- PAYROLL MANAGEMENT SYSTEM ----\n";
        cout << "1. Login\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        if (choice == 1)
        {
            int empid = login();
            if (empid != -1)
            {
                if (employees[empid].isadmin)
                    adminmenu(empid);
                else
                    usermenu(empid);
            }
        }
        else if (choice != 0)
            cout << "Invalid choice! Try again.\n";
    }
    cout << "Exiting Payroll System. Goodbye!\n";
}
int main()
{
    initializesystem();
    mainmenu();
    cleanupsystem();
    cout << "\nThank you for using the system!\n";
    return 0;
}