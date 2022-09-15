#include <stdio.h>
#include "commands.h"
#pragma warning(disable : 4996)
int getM() {
    FILE* fp;
    struct Department department;
    int id;

    if ((fp = fopen(OFFICE_PATH, "rb")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }

    printf("Enter the Department Id: ");
    scanf("%d", &id);
    fseek(fp, sizeof(struct Department)* (id-1), SEEK_SET);
    fread(&department, sizeof(struct Department) , 1, fp);
    printf("\n------------------------\n", id);
    if (department.id != id||department.isDeleted )
        printf("Department with this id is not found\n");
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
    if ((departments = fopen(OFFICE_PATH, "rb")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }
    printAllMasters();
    printf("Enter the Department Id: ");
    scanf("%d", &id);
    printf("%s", "------------Department Employees------------\n");
    fseek(departments, sizeof(struct Department) * (id - 1), SEEK_SET);
    fread(&department, sizeof(struct Department), 1, departments);
    for (int i = 0; i < department.currentCountOfEmployees; i++)
    {
       
    
            fseek(employees, sizeof(struct Employee) * (department.employeesID[i] - 1), SEEK_SET);
            fread(&employee, sizeof(struct Employee), 1, employees);
            if (!employee.isDeleted&& employee.departmentID == department.id)
            {
                printf("ID :           %d\nDepartmentID : %d\nName:          %s\nSpeciality:    %s\n",employee.id,employee.departmentID, employee.name, employee.speciality);
                printf("%s", "--------------------------------------------\n");
            }
            
        
    }
 
    fclose(employees);
    fclose(departments);
    return 0;
}

void delM() {
    int id;

    mastersShow();
    printf("Enter Department ID:\n");
    scanf("%d", &id);

    deleteMasterByID(id);
}

void delS() {
    int id;

    slaveShow();
    printf("Enter employee -[number]-: ");
    scanf("%d", &id);
    printf("\n");
    deleteSlaveByID(id);
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

int insertM() {
    FILE* departmentsFile;
    FILE* departmentsDelFile;
    struct Department department;
    int id=-1;
    double intSize = sizeof(int);
   
    if ((departmentsFile = fopen(OFFICE_PATH, "a+b")) == NULL) {
        perror("Error occured while opening file");
        return 1;
    }
    if ((departmentsDelFile = fopen(OFFICE_DELETED, "a+b")) == NULL)
    {
        perror("Error occured while opening file");
        return 1;
    }
    while (fread(&id, intSize, 1, departmentsDelFile) == 1)
    {
     
        if (id !=-1)
        {
            break;
        }
    }
    if (id ==-1)
    {
       
        departmentsDelFile = fopen(OFFICE_DELETED, "r+b");
        remove(OFFICE_DELETED);
    }
    if (id!=-1) 
    {
            departmentsFile = fopen(OFFICE_PATH, "r+b");
            departmentsDelFile = fopen(OFFICE_DELETED, "r+b");
            fseek(departmentsDelFile, -intSize, SEEK_END);
            fread(&id, intSize, 1, departmentsDelFile);
            while (id == -1)
            {
                fseek(departmentsDelFile, -intSize * 2, SEEK_CUR);
                fread(&id, intSize, 1, departmentsDelFile);
              
            }
         
            department.id = id;
            department.isDeleted = false;
           
            fseek(departmentsFile, sizeof(struct Department) * (id - 1), SEEK_SET);
            id = -1;
         
            
               
        
    }
    else
    {
        while (fread(&department, sizeof(struct Department), 1, departmentsFile) == 1);
        if (department.id >= 1)
        {
            department.id = department.id++;
        }
        else
        {
            department.id = 1;

        }
    }
    department.currentCountOfEmployees = 0;
    printf("%s", "Enter Department Name: ");
    scanf(" %s", department.departmentName);
    printf("%s", "Enter Director: ");
    scanf(" %s", department.director);
    printf("%s", "Enter Project: ");
    scanf(" %s", department.project);
    fwrite(&department, sizeof(struct Department), 1, departmentsFile);
    fseek(departmentsDelFile, -intSize, SEEK_CUR);
    fwrite(&id, intSize, 1, departmentsDelFile);
    fclose(departmentsFile);
    fclose(departmentsDelFile);
    return 0;
}

int insertS() {
    FILE* employeesFile;
    FILE* departmentsFile;
    Employee employee;
    Department department;
    double departmentStructSize = sizeof(Department);
    if ((employeesFile = fopen(EMPLOYEE_PATH, "a+b")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }
    if ((departmentsFile = fopen(OFFICE_PATH, "r+b")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }
    mastersShow();
    while (fread(&employee, sizeof(struct Employee), 1, employeesFile) == 1);
    if (employee.id >= 1)
    {
        employee.id = employee.id++;
    }
    else
    {
        employee.id = 1;

    }
    printf("Enter Department ID: ");
    scanf("%d", &employee.departmentID);
    printf("%s", "Enter Employee Name: ");
    scanf("%s",employee.name);
    printf("Enter Employee Speciality: ");
    scanf("%s", employee.speciality);
    printf("\n");
    employee.isDeleted = false;
    fseek(departmentsFile, departmentStructSize*(employee.departmentID-1), SEEK_SET);
    fread(&department, departmentStructSize, 1, departmentsFile);
    for (int i = 0; i < department.departmentEmployeesSize; i++)
    {
      
        if (department.employeesID[i]==0)
        {
            department.employeesID[i] = employee.id;
            department.currentCountOfEmployees++;
            if (department.currentCountOfEmployees<department.departmentEmployeesSize)
            {
                fseek(departmentsFile, departmentStructSize * (employee.departmentID - 1), SEEK_SET);
                fwrite(&department, departmentStructSize, 1, departmentsFile);
            }
            else
            {
                perror("Error. Exceeded department employees size");
                return 1;
            }
            fwrite(&employee, sizeof(struct Employee), 1, employeesFile);
            fclose(employeesFile);
            fclose(departmentsFile);
            return 0;
        }
    }

  
}

int printAllMasters() { 
    FILE* departmentsFile;
    Department department;

    if ((departmentsFile = fopen(OFFICE_PATH, "rb")) == NULL) 
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
    while (fread(&employee, sizeof(struct Employee), 1, employeesFile) == 1) 
    {
      
        if (!employee.isDeleted)
        {
            printf("ID: %d\Department ID  : %d\nEmployee Name       : %s\nEmployee Speciality : %s\n", employee.id, employee.departmentID, employee.name, employee.speciality);
            printf("%s", "-----------------------------\n");
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
    if ((departmentsFile = fopen(OFFICE_PATH, "rb")) == NULL)
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
    while (fread(&employee, sizeof(struct Employee), 1, employeesFile) == 1) 
    {
        if (!employee.isDeleted)
        {
            if (id == -1 || employee.departmentID == id) count++;
        }
       
    }

    if (id == -1) printf("count of Employees: %d\n", count);
    else printf("Department [%d] has: %d employees\n", id, count);
    printf("\n");

    fclose(employeesFile);
    return 0;
}

int deleteMasterByID(int id) 
{ 
    FILE* departmentFile;
    FILE* departmentsDelFile;
    int bufferId;
    Department department;

    if ((departmentFile = fopen(OFFICE_PATH, "r+b")) == NULL) 
    {
        perror("Error. Problems with opening the file");
        return 1;
    }
    if ((departmentsDelFile = fopen(OFFICE_DELETED, "a+b")) == NULL)
    {
        perror("Error. Problems with opening the file");
        return 1;
    }

    fseek(departmentFile, sizeof(struct Department) * (id - 1), SEEK_SET);
    fread(&department, sizeof(struct Department), 1, departmentFile);
    double departmentStructSize = sizeof(Department);
    if (department.id == id )
    {
        fseek(departmentFile, -departmentStructSize, SEEK_CUR);
        
        department.isDeleted = true;
        fwrite(&department, sizeof(struct Department), 1, departmentFile);
    
        fwrite(&department.id, sizeof(int), 1, departmentsDelFile);
    }
    else
    {
        printf("Department with this id is not founded");
    }
    fclose(departmentFile);
    fclose(departmentsDelFile);

}

int deleteSlaveByID(int id) 
{ 
    FILE* employeesFile;
    Employee employee;
    if ((employeesFile = fopen(EMPLOYEE_PATH, "r+b")) == NULL)
    {
        perror("Error. Problems with opening the file\n");
        return 1;
    }
    fseek(employeesFile, sizeof(struct Employee) * (id - 1), SEEK_SET);
    fread(&employee, sizeof(struct Employee), 1, employeesFile);
   
  
    if (employee.id == id)
    {
        if (employee.isDeleted)
        {
             printf("Employee with this id is not founded\n");
        }
        fseek(employeesFile, sizeof(struct Employee) * (id - 1), SEEK_SET);

        employee.isDeleted = true;
        fwrite(&employee, sizeof(struct Employee), 1, employeesFile);
    }
    else
    {
        printf("Employee with this id is not founded\n");
    }
    fclose(employeesFile);
  
}


int masterUpdateByID(int id) 
{ 
    FILE* departmentFile;
    Department department;
    int property;
    int isFound = 0;

    if ((departmentFile = fopen(OFFICE_PATH, "r+b")) == NULL) {
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
    if ((departmentsFile = fopen(OFFICE_PATH, "r+b")) == NULL)
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

    switch (property) 
    {
    case 1:
        printf("Enter the new DepartmentID: ");
        scanf("%d", &employee.departmentID);
        fseek(departmentsFile, sizeof(struct Department) * (employee.departmentID - 1), SEEK_SET);
        fread(&department, sizeof(struct Department), 1, departmentsFile);
        for (int i = 0; i < department.departmentEmployeesSize; i++)
        {   
            if (department.employeesID[i]==employee.id)
            {
               
                break;
            }

            if (department.employeesID[i] == 0 )
            {
                department.employeesID[i] = employee.id;
                department.currentCountOfEmployees++;
                if (department.currentCountOfEmployees < department.departmentEmployeesSize)
                {
                  
                    fseek(departmentsFile, sizeof(struct Department) * (employee.departmentID - 1), SEEK_SET);
                    fwrite(&department, sizeof(struct Department), 1, departmentsFile);
                  
                }
                else
                {
                    perror("Error. Exceeded department employees size");
                    return 1;
                }
                break;
            }
        }
        fseek(employeesFile, sizeof(struct Employee) * (employee.id - 1), SEEK_SET);
        fwrite(&employee, sizeof(struct Employee), 1, employeesFile);
        break;
    case 2:
        printf("Enter the new Name: ");
        scanf("%s", employee.name);
        break;
    case 3:
        printf("Enter the new Speciality: ");
        scanf("%d", &employee.speciality);
        break;
    default:
        printf("\n[Error. Incorrect Number. Try Again]\n");
        while (getchar() != '\n');
        break;
    }
    printf("\n");
    double sizeStructEmployee = sizeof(Employee);
    fseek(employeesFile, -sizeStructEmployee, SEEK_CUR);
    fwrite(&employee, sizeof(struct Employee), 1, employeesFile);
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
   
    printf("=============Employee with ID: %d ==============\n",employee->departmentID);
    printf("[1] Department ID:  %d\n[2] Name:           %s\n[3] Speciality:     %s\n", employee->departmentID,employee->name, employee->speciality);
    printf("==================================\n");
}

int mastersShow() { 
    FILE* departmentsFile;
    Department department;

    if ((departmentsFile = fopen(OFFICE_PATH, "rb")) == NULL) 
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
    while (fread(&employee, sizeof(struct Employee), 1, employeesFile) == 1)
    {
        if (!employee.isDeleted)
        {
            printf("======================================\n");
            printf("Employee ID         : [%d]\nDepartment ID       :  %d\nEmployee name       :  %s\nEmployee speciality :  %s\n", employee.id, employee.departmentID, employee.name, employee.speciality);
          
        }
       
    }
    printf("======================================\n");


    fclose(employeesFile);
    return 0;
}



