#include <stdio.h>
#include "commands.h"
#pragma warning(disable : 4996)
int getM() {
    FILE* fp;
    Department department;
    int id;

    if ((fp = fopen(DEPARTMENT_PATH, "rb")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }

    printf("Enter the Department Id: ");
    scanf("%d", &id);
    fseek(fp, sizeof(struct Department)* (id-1), SEEK_SET);
    fread(&department, sizeof(struct Department) , 1, fp);
    printf("\n------------------------\n", id);
    if (department.id != id || department.isDeleted)
    {
        printf("Department with this id is not found\n");
        return 1;
    }
        
       
    else 
    {

        printf("Department Id   : %d\nDepartment Name : %s\nDirector        : %s\nProject         : %s\n"
        ,department.id, department.departmentName, department.director, department.project);
      
    } 
    printf("--------------------------\n", id);
    fclose(fp);
    return 0;
}

int getS() {
    FILE* employees;
    FILE* departments;
     Employee employee;
     Department department;
    int id;

    if ((employees = fopen(EMPLOYEE_PATH, "rb")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }
    if ((departments = fopen(DEPARTMENT_PATH, "rb")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }
    printAllMasters();
    printf("Enter the Department Id: ");
    scanf("%d", &id);
    fseek(departments, sizeof(struct Department) * (id - 1), SEEK_SET);
    fread(&department, sizeof(struct Department), 1, departments);
    if (department.id != id || department.isDeleted)
    {
        printf("Department with this id is not found\n");
        return 1;
    }
    printf("%s", "\n------------Department Employees------------\n");
    if (department.firstEmployeeId!=0)
    {
        fseek(employees, sizeof(struct Employee) * (department.firstEmployeeId - 1), SEEK_SET);
        fread(&employee, sizeof(struct Employee), 1, employees);
        if (!employee.isDeleted && employee.departmentID == department.id)
        {

            printf("ID           : %d\nDepartmentID : %d\nName         : %s\nSpeciality   : %s\n", employee.id, employee.departmentID, employee.name, employee.speciality);
            printf("%s", "--------------------------------------------\n");
        }
        while (employee.nextEmployeeID != 0)
        {
            fseek(employees, sizeof(struct Employee) * (employee.nextEmployeeID - 1), SEEK_SET);
            fread(&employee, sizeof(struct Employee), 1, employees);
            printf("ID           : %d\nDepartmentID : %d\nName         : %s\nSpeciality   : %s\n", employee.id, employee.departmentID, employee.name, employee.speciality);
            printf("%s", "--------------------------------------------\n");
        }
    }
           
        
   
 
    fclose(employees);
    fclose(departments);
    return 0;
}

void delM(DeletedElements &delEl) {
    int id;

    mastersShow();
    printf("Enter Department ID:\n");
    scanf("%d", &id);

    deleteMasterByID(id, delEl);
}

void delS(DeletedElements& delEl) {
    int id;

    slaveShow();
    printf("Enter employee -[number]-: ");
    scanf("%d", &id);
    printf("\n");
    deleteSlaveByID(id, delEl);
}

int mUpdate() {
    int id;

    mastersShow();
    printf("Enter an [id]:\n");
    scanf("%d", &id);

    return masterUpdateByID(id);
};

int sUpdate() {
    int id;
    slaveShow();
    FILE* employees;
    if ((employees = fopen(EMPLOYEE_PATH, "rb")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }
    printf("Enter employee [ID]: ");
    scanf("%d", &id);
    printf("\n");
    return slaveUpdateByID(id);
};

int insertM(DeletedElements & delEl) {
    FILE* departmentsFile;
    Department department;
    int id=-1;
    double intSize = sizeof(int);
   
    if ((departmentsFile = fopen(DEPARTMENT_PATH, "a+b")) == NULL) {
        perror("Error occured while opening file");
        return 1;
    }
   
    if (delEl.sizeDeletedDepartment>=1)
    {
        id = delEl.deletedDepartmentID[delEl.sizeDeletedDepartment - 1];
        delEl.deletedDepartmentID[delEl.sizeDeletedDepartment - 1] = 0;
        delEl.sizeDeletedDepartment--;
      
    }
        
    
    if (id!=-1) 
    {
            fclose(departmentsFile);
            departmentsFile = fopen(DEPARTMENT_PATH, "r+b");
               
            department.id = id;
         
          
            fseek(departmentsFile, sizeof(struct Department) * (department.id - 1), SEEK_SET);
            id = -1;
            fread(&department, sizeof(struct Department), 1, departmentsFile);
           
               
        
    }
    else
    {
        while (fread(&department, sizeof(struct Department), 1, departmentsFile) == 1);
        if (department.id >= 1)
        {
           department.id++;
        }
        else
        {
            department.id = 1;

        }
    }

    printf("%s", "Enter Department Name: ");
    scanf(" %s", department.departmentName);
    printf("%s", "Enter Director: ");
    scanf(" %s", department.director);
    printf("%s", "Enter Project: ");
    scanf(" %s", department.project);
    if (!department.isDeleted) // delete this check if you don't wanna move employees to new department 
    {
        department.firstEmployeeId = 0;
    }
    else
    {
        fseek(departmentsFile, sizeof(struct Department) * (department.id - 1), SEEK_SET);
        department.isDeleted = false;
        fwrite(&department, sizeof(struct Department), 1, departmentsFile);
        fclose(departmentsFile);
        return 0;
    }
    department.isDeleted = false;
    fwrite(&department, sizeof(struct Department), 1, departmentsFile);
    fclose(departmentsFile);
    return 0;
}

int insertS(DeletedElements& delEl) {
    FILE* employeesFile;
    Employee employee;
    int id=-1;
    if ((employeesFile = fopen(EMPLOYEE_PATH, "a+b")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }
    mastersShow();
    if (delEl.sizeDeletedEmployees >= 1)
    {
        id = delEl.deletedEmployeesID[delEl.sizeDeletedEmployees - 1];
        delEl.deletedEmployeesID[delEl.sizeDeletedEmployees - 1] = 0;
        delEl.sizeDeletedEmployees--;
  
    }
    if (id != -1)
    {
        employeesFile = fopen(EMPLOYEE_PATH, "r+b");

        employee.id = id;
        employee.isDeleted = false;

        fseek(employeesFile, sizeof(struct Employee) * (id - 1), SEEK_SET);
       
   
    }
    if (id==-1)
    {
        
  
        while (fread(&employee, sizeof(struct Employee), 1, employeesFile) == 1);
        
        if (employee.id >= 1)
        {
           employee.id = employee.id++;
        }
        else
        {
            employee.id = 1;

        }
    }
    employee.nextEmployeeID = 0;
    employee.prevEmpoyeeID = 0;
    printf("Enter Department ID: ");
    scanf("%d", &employee.departmentID);
    printf("%s", "Enter Employee Name: ");
    scanf("%s",employee.name);
    printf("Enter Employee Speciality: ");
    scanf("%s", employee.speciality);
    printf("\n");
    employee.isDeleted = false;
 
    Employee bufferEmployee = employee;
    fwrite(&bufferEmployee, sizeof(Employee), 1, employeesFile);
    fclose(employeesFile);

     FILE* departmentFile;
     Department department;
     if ((departmentFile = fopen(DEPARTMENT_PATH, "r+b")) == NULL) {
         perror("Error. Problems with opening the file");
         return 1;
     }
    if( (employeesFile = fopen(EMPLOYEE_PATH, "r+b")) == NULL)
     {
         perror("Error. Problems with opening the file");
         return 1;
     }
     fseek(departmentFile, sizeof(struct Department) * (bufferEmployee.departmentID - 1), SEEK_SET);
     fread(&department, sizeof(struct Department), 1, departmentFile);
     if (department.firstEmployeeId==0)
     {
         fseek(departmentFile, sizeof(struct Department) * (bufferEmployee.departmentID - 1), SEEK_SET);
         department.firstEmployeeId = bufferEmployee.id;
         fwrite(&department, sizeof(struct Department), 1, departmentFile);
     }
     else
     {
         fseek(employeesFile, sizeof(struct Employee) * (department.firstEmployeeId - 1), SEEK_SET);
         fread(&employee, sizeof(struct Employee), 1, employeesFile);
         while (employee.nextEmployeeID != 0)
         {
             fseek(employeesFile, sizeof(struct Employee) * (employee.nextEmployeeID - 1), SEEK_SET);
             fread(&employee, sizeof(struct Employee), 1, employeesFile);
         }
         fseek(employeesFile, sizeof(struct Employee) * (employee.id - 1), SEEK_SET);
         employee.nextEmployeeID = bufferEmployee.id;
         fwrite(&employee, sizeof(struct Employee), 1, employeesFile);
         fseek(employeesFile, sizeof(struct Employee) * (bufferEmployee.id - 1), SEEK_SET);
         bufferEmployee.prevEmpoyeeID = employee.id;
         fwrite(&bufferEmployee, sizeof(struct Employee), 1, employeesFile);
     }
     
     fclose(employeesFile);
     fclose(departmentFile);
     return 0;
  

  
}

int printAllMasters() { 
    FILE* departmentsFile;
    Department department;

    if ((departmentsFile = fopen(DEPARTMENT_PATH, "rb")) == NULL) 
    {
        perror("Error occured while opening file");
        return 1;
    }

    printf("%s", "-----Departments-----\n");
    while (fread(&department, sizeof(struct Department), 1, departmentsFile) == 1) 
    {
        if (!department.isDeleted)
        {
            printf("Department ID   : %d\nDepartment Name : %s\nDirector        : %s\nProject         : %s\n",
                department.id, department.departmentName, department.director, department.project);
            printf("%s", "---------------------\n");
        }
        
    }


    fclose(departmentsFile);
    return 0;
}

int printAllSlaves() 
{
    FILE* employeesFile;
    Employee employee;

    if ((employeesFile = fopen(EMPLOYEE_PATH, "rb")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }


    printf("%s", "------------Employees--------\n");
    int check=0;
    while (fread(&employee, sizeof(struct Employee), 1, employeesFile) == 1) 
    {
       
        if (!employee.isDeleted && check != employee.id)
        {
            printf("ID                  : %d\nDepartment ID       : %d\nEmployee Name       : %s\nEmployee Speciality : %s\n",employee.id, employee.departmentID, employee.name, employee.speciality);
            printf("%s", "-----------------------------\n");
            check = employee.id;
        }
       
    }

    fclose(employeesFile);
    return 0;
}

int countM() {
    FILE* departmentsFile;
    Department department;
    int count = 0;
    double departmentStructSize = sizeof(Department);
    if ((departmentsFile = fopen(DEPARTMENT_PATH, "rb")) == NULL)
    {
        perror("Error occured while opening file");
        return 1;
    }
   /* fseek(fp, -(officeSize), SEEK_END);
    fread(&office, sizeof(struct Office), 1, fp);
    printf("Amount of Offices : %d\n", office.id);*/

    while (fread(&department, sizeof(struct Department), 1, departmentsFile) == 1) 
    {
        if (!department.isDeleted)
        {
            count++;
        }
       
    }
    printf("\n");
    printf("Amount of Department: %d\n", count);
    printf("\n");
    fclose(departmentsFile);
    return 0;
}

int countS() {
    FILE* employeesFile;
    Employee employee;
    int count = 0;
    int id;

    if ((employeesFile = fopen(EMPLOYEE_PATH, "rb")) == NULL) {
        perror("Error occured while opening file");
        return 1;
    }

    mastersShow();
    printf("Enter the Department ID or -1 for count all employees:\n");
    scanf("%d", &id);
    printf("\n");
    int check=-1;
    while (fread(&employee, sizeof(struct Employee), 1, employeesFile) == 1) 
    {
        if (!employee.isDeleted)
        {
            if (check!=employee.id)
            {
                if (id == -1 || employee.departmentID == id) count++;
                check = employee.id;
            }
           
        }
       
    }

    if (id == -1) printf("count of Employees: %d\n", count);
    else printf("Department [%d] has: %d employees\n", id, count);
    printf("\n");

    fclose(employeesFile);
    return 0;
}

int deleteMasterByID(int id, DeletedElements& delEl) 
{ 
    FILE* departmentFile;
    int bufferId;
    Department department;

    if ((departmentFile = fopen(DEPARTMENT_PATH, "r+b")) == NULL) 
    {
        perror("Error. Problems with opening the file");
        return 1;
    }
  
    fseek(departmentFile, sizeof(struct Department) * (id - 1), SEEK_SET);
    fread(&department, sizeof(struct Department), 1, departmentFile);
    double departmentStructSize = sizeof(Department);
    if (department.id == id )
    {
        fseek(departmentFile, sizeof(struct Department) * (id - 1), SEEK_SET);
        
        department.isDeleted = true;
        //department.firstEmployeeId = 0; if we don't wanna move employees to new department
        fwrite(&department, sizeof(struct Department), 1, departmentFile);
    }
    else
    {
        printf("Department with this id is not founded");
    }

    delEl.deletedDepartmentID[delEl.sizeDeletedDepartment] = id;
    delEl.sizeDeletedDepartment++;
    fclose(departmentFile);


}

int deleteSlaveByID(int id, DeletedElements& delEl)
{ 
    FILE* employeesFile;
    Employee employee;
    if ((employeesFile = fopen(EMPLOYEE_PATH, "r+b")) == NULL)
    {
        perror("Error. Problems with opening the file\n");
        return 1;
    }
    FILE* departmentFile;
    Department department;
    if ((departmentFile = fopen(DEPARTMENT_PATH, "r+b")) == NULL)
    {
        perror("Error. Problems with opening the file\n");
        return 1;
    }
    fseek(employeesFile, sizeof(struct Employee) * (id - 1), SEEK_SET);
    fread(&employee, sizeof(struct Employee), 1, employeesFile);
    int nextID;
    int prevID;
    
  
    if (employee.id == id)
    {
        if (employee.isDeleted)
        {
             printf("Employee with this id is not founded\n");
        }
        
        employee.isDeleted = true;
        nextID = employee.nextEmployeeID;//2
        prevID = employee.prevEmpoyeeID;//0
        employee.nextEmployeeID = 0;
        employee.prevEmpoyeeID = 0;
        fseek(employeesFile, sizeof(struct Employee) * (id - 1), SEEK_SET);
        fwrite(&employee, sizeof(struct Employee), 1, employeesFile);
       
        if (prevID!=0)
        {
            //changing prevEmployee
            fseek(employeesFile, sizeof(struct Employee) * (prevID - 1), SEEK_SET);
            fread(&employee, sizeof(struct Employee), 1, employeesFile);
            fseek(employeesFile, sizeof(struct Employee) * (prevID - 1), SEEK_SET);
            employee.nextEmployeeID = nextID;
            fwrite(&employee, sizeof(struct Employee), 1, employeesFile);
        }
        else
        {
            //if our employee is first we changing department
            fseek(departmentFile, sizeof(struct Department) * (employee.departmentID-1), SEEK_SET);
            fread(&department, sizeof(struct Department), 1, departmentFile);
            if (nextID==0)
            {
                department.firstEmployeeId = 0;
            }
            else
            {
                department.firstEmployeeId = nextID;
            }
            fseek(departmentFile, sizeof(struct Department) * (employee.departmentID - 1), SEEK_SET);
            fwrite(&department, sizeof(struct Department), 1, departmentFile);
            fclose(departmentFile);
        }
        if (nextID!=0)
        {
            fseek(employeesFile, sizeof(struct Employee) * (nextID - 1), SEEK_SET);
            fread(&employee, sizeof(struct Employee), 1, employeesFile);
            employee.prevEmpoyeeID = prevID;
            fwrite(&employee, sizeof(struct Employee), 1, employeesFile);
        }
        
     
       
    }
    else
    {
        printf("Employee with this id is not founded\n");
    }
    delEl.deletedEmployeesID[delEl.sizeDeletedEmployees] = id;
    delEl.sizeDeletedEmployees++;
    fclose(employeesFile);
  
}


int masterUpdateByID(int id) 
{ 
    FILE* departmentFile;
    Department department;
    int property;
    int isFound = 0;

    if ((departmentFile = fopen(DEPARTMENT_PATH, "r+b")) == NULL) {
        perror("Error occured while opening file\n");
        return 1;
    }
    fseek(departmentFile, sizeof(Department)*(id-1), SEEK_SET);
    fread(&department, sizeof(struct Department), 1, departmentFile);
        if (department.id != id || department.isDeleted) 
        {
           printf("Department with this id is not found\n");
            return 0;
        
        }
    departmentPropPrint(&department);
    printf("Enter what you want to change: ");
    scanf("%d", &property);

    switch (property) {
    case 1:
        printf("Enter the new Department Name: ");
        scanf("%s", department.departmentName);
        break;
    case 2:
        printf("Enter the new Director:");
        scanf("%s", department.director);
        break;
    case 3:
        printf("Enter the new Project: ");
        scanf("%s", department.project);
        break;
    default:
        printf("\n[Error. Incorrect Number. Try Again]\n");
        while (getchar() != '\n');
        break;
    }
    printf("\n");
    double departmentStructSize = sizeof(Department);
    fseek(departmentFile, -departmentStructSize, SEEK_CUR);
    fwrite(&department, sizeof(department), 1, departmentFile);

    fclose(departmentFile);
    return 0;
}

int slaveUpdateByID(int id) { 
    

    int property;
    int isFound=false;
    FILE* employeesFile;
    Employee employee;
    FILE* departmentsFile;
    Department department;
   
    if ((employeesFile = fopen(EMPLOYEE_PATH, "r+b")) == NULL)
    {
        perror("Error. Problems with opening the file");
        return 1;
    }
    if ((departmentsFile = fopen(DEPARTMENT_PATH, "r+b")) == NULL)
    {
        perror("Error. Problems with opening the file");
        return 1;
    }
    fseek(employeesFile, sizeof(struct Employee) * (id - 1), SEEK_SET);
    fread(&employee, sizeof(struct Employee), 1, employeesFile);

    if (employee.id == id &&!employee.isDeleted) isFound = true;
    if (!isFound) 
    {
        perror("Employee with this number not found");
        return 0;
    }

    employeePropPrint(&employee);
    printf("Enter what you want to change: ");
    scanf("%d", &property);
 
    Employee bufferEmployee;
    int oldDepartmentID = employee.departmentID;
    int prevEmployeeId = employee.prevEmpoyeeID;
    int nextEmployeeId = employee.nextEmployeeID;
    switch (property) 
    {
    case 1:
        printf("Enter the new DepartmentID: ");
        scanf("%d", &employee.departmentID);
      
        bufferEmployee = employee;
        bufferEmployee.nextEmployeeID = 0;
        bufferEmployee.prevEmpoyeeID = 0;
        if (prevEmployeeId != 0)
        {
            //changing prev employee
            fseek(employeesFile, sizeof(struct Employee) * (prevEmployeeId - 1), SEEK_SET);
            fread(&employee, sizeof(struct Employee), 1, employeesFile);
            fseek(employeesFile, sizeof(struct Employee) * (prevEmployeeId - 1), SEEK_SET);
            employee.nextEmployeeID = nextEmployeeId;
            fwrite(&employee, sizeof(struct Employee), 1, employeesFile);
        }
        else
        {
            //changing old deparment
            fseek(departmentsFile, sizeof(struct Department) * (oldDepartmentID-1), SEEK_SET);
            fread(&department, sizeof(struct Department), 1, departmentsFile);
            if (nextEmployeeId ==0)
            {
                department.firstEmployeeId = 0;
            }
            else
            {
                department.firstEmployeeId = nextEmployeeId;
            }
            fseek(departmentsFile, sizeof(struct Department) * (oldDepartmentID-1), SEEK_SET);
            fwrite(&department, sizeof(struct Department), 1, departmentsFile);
          
        }
        if (nextEmployeeId != 0)
        {
            //changing next employee
            fseek(employeesFile, sizeof(struct Employee) * (nextEmployeeId - 1), SEEK_SET);
            fread(&employee, sizeof(struct Employee), 1, employeesFile);
            employee.prevEmpoyeeID = prevEmployeeId;
            fseek(employeesFile, sizeof(struct Employee) * (nextEmployeeId - 1), SEEK_SET);
            fwrite(&employee, sizeof(struct Employee), 1, employeesFile);
        }
        //changing new department (adding out employee to the end)
        fseek(departmentsFile, sizeof(struct Department) * (bufferEmployee.departmentID - 1), SEEK_SET);
        fread(&department, sizeof(struct Department), 1, departmentsFile);
        if (department.firstEmployeeId ==0)
        {
            fseek(departmentsFile, sizeof(struct Department) * (bufferEmployee.departmentID - 1), SEEK_SET);
            department.firstEmployeeId = bufferEmployee.id;
            fwrite(&department, sizeof(struct Department), 1, departmentsFile);
        }
        else
        {
            fseek(employeesFile, sizeof(struct Employee) * (department.firstEmployeeId - 1), SEEK_SET);
            fread(&employee, sizeof(struct Employee), 1, employeesFile);
            while (employee.nextEmployeeID != 0)
            {
                fseek(employeesFile, sizeof(struct Employee) * (employee.nextEmployeeID - 1), SEEK_SET);
                fread(&employee, sizeof(struct Employee), 1, employeesFile);
            }
            fseek(employeesFile, sizeof(struct Employee) * (employee.id - 1), SEEK_SET);
            employee.nextEmployeeID = bufferEmployee.id;
            bufferEmployee.prevEmpoyeeID = employee.id;
            fwrite(&employee, sizeof(struct Employee), 1, employeesFile);
        }
      

        break;
    case 2:
        printf("Enter the new Name: ");
        scanf("%s", bufferEmployee.name);
        break;
    case 3:
        printf("Enter the new Speciality: ");
        scanf("%d", &bufferEmployee.speciality);
        break;
    default:
        printf("\n[Error. Incorrect Number. Try Again]\n");
        while (getchar() != '\n');
        break;
    }
    printf("\n");
    fseek(employeesFile, sizeof(struct Employee) * (bufferEmployee.id - 1), SEEK_SET);
    fwrite(&bufferEmployee, sizeof(struct Employee), 1, employeesFile);
    fclose(employeesFile);
    fclose(departmentsFile);
    return 0;
}

void departmentPropPrint(struct Department* department) { 
    printf("================ID:%d=================\n",department->id);
    printf("[1] Department Name: %s\n[2] Director:   %s\n[3] Project:    %s\n",department->departmentName, department->director, department->project);
    printf("=====================================\n");
}

void employeePropPrint(struct Employee* employee) 
{ 
   
    printf("=============Employee with ID: %d ==============\n",employee->id);
    printf("[1] Department ID:  %d\n[2] Name:           %s\n[3] Speciality:     %s\n", employee->departmentID,employee->name, employee->speciality);
    printf("==================================\n");
}

int mastersShow() { 
    FILE* departmentsFile;
    Department department;

    if ((departmentsFile = fopen(DEPARTMENT_PATH, "rb")) == NULL) 
    {
        perror("Error. Problems with opening the file");
        return 1;
    }

    printf("===============Departments================\n");
    while (fread(&department, sizeof(struct Department), 1, departmentsFile) == 1) 
    {
        if (!department.isDeleted)
        {
            printf("Department ID:  -[%d]-\nDepartment Name: [%s]\nDirector:        [%s]\nProject:         [%s]\n", department.id, department.departmentName, department.director, department.project);
            printf("====================================\n");
        }
        
    }
    fclose(departmentsFile);
    return 0;
}

int slaveShow() { 
    FILE* employeesFile;
    Employee employee;

    if ((employeesFile = fopen(EMPLOYEE_PATH, "rb")) == NULL) 
    {
        perror("Error. Problems with opening the file\n");
        return 1;
    }
 

    printf("=============Employees=================\n");
    int check = 0;
    while (fread(&employee, sizeof(struct Employee), 1, employeesFile) == 1)
    {
        if (!employee.isDeleted&& check != employee.id)
        {
            printf("======================================\n");
            printf("Employee ID         : [%d]\nDepartment ID       :  %d\nEmployee name       :  %s\nEmployee speciality :  %s\n", employee.id, employee.departmentID, employee.name, employee.speciality);
            check = employee.id;
        }
       
    }
    printf("======================================\n");


    fclose(employeesFile);
    return 0;
}



