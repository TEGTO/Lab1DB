#include <stdio.h>
#include "commands.h"
#pragma warning(disable : 4996)
int getM() {
    FILE* fp;
    struct Office office;
    int id;

    if ((fp = fopen(OFFICE_PATH, "rb")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }

    printf("Enter the Office Id: ");
    scanf("%d", &id);
    fseek(fp, sizeof(struct Office)* (id-1), SEEK_SET);
    fread(&office, sizeof(struct Office) , 1, fp);
    printf("\n------------------------\n", id);
    if (office.id != id||office.isDeleted )
        printf("Office with this id is not found\n");
    else 
    {

        printf("Id : %d\nDepartment : %s\nDirector : %s\nProject: %s\n",office.id, office.department, office.director, office.project);
      
    } 
    printf("------------------------\n", id);
    fclose(fp);
    return 0;
}

int getS() {
    FILE* employees;
    FILE* offices;
     Employee employee;
     Office office;
    int id;

    if ((employees = fopen(EMPLOYEE_PATH, "rb")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }
    if ((offices = fopen(OFFICE_PATH, "rb")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }
    printAllMasters();
    printf("Enter the Office Id: ");
    scanf("%d", &id);
    printf("%s", "------------------------\n");
    fseek(offices, sizeof(struct Office) * (id - 1), SEEK_SET);
    fread(&office, sizeof(struct Office), 1, offices);
    for (int i = 0; i < office.currentCountOfEmployees; i++)
    {
       
    
            fseek(employees, sizeof(struct Employee) * (office.employees_id[i] - 1), SEEK_SET);
            fread(&employee, sizeof(struct Employee), 1, employees);
            if (!employee.isDeleted&& employee.officeID == office.id)
            {
                printf("ID : %d\nOfficeID : %d\nName: %s\nSpeciality: %s\n",employee.id,employee.officeID, employee.name, employee.speciality);
                printf("%s", "------------------------\n");
            }
            
        
    }
 
    fclose(employees);
    fclose(offices);
    return 0;
}

void delM() {
    int id;

    mastersShow();
    printf("Enter Office ID:\n");
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
    FILE* mainFile;
    FILE* delFile;
    struct Office office;
    int id=-1;
    int intSize = sizeof(int);
   
    if ((mainFile = fopen(OFFICE_PATH, "a+b")) == NULL) {
        perror("Error occured while opening file");
        return 1;
    }
    if ((delFile = fopen(OFFICE_DELETED, "a+b")) == NULL)
    {
        perror("Error occured while opening file");
        return 1;
    }
    while (fread(&id, intSize, 1, delFile) == 1)
    {
     
        if (id !=-1)
        {
            break;
        }
    }
    if (id ==-1)
    {
       
        delFile = fopen(OFFICE_DELETED, "r+b");
        remove(OFFICE_DELETED);
    }
    if (id!=-1) 
    {
            mainFile = fopen(OFFICE_PATH, "r+b");
            delFile = fopen(OFFICE_DELETED, "r+b");
            fseek(delFile, -intSize, SEEK_END);
            fread(&id, sizeof(int), 1, delFile);
            while (id == -1)
            {
                fseek(delFile, -intSize * 2, SEEK_CUR);
                fread(&id, sizeof(int), 1, delFile);
              
            }
         
            office.id = id;
            office.isDeleted = false;
           
            fseek(mainFile, sizeof(struct Office) * (id - 1), SEEK_SET);
            id = -1;
         
            
               
        
    }
    else
    {
        while (fread(&office, sizeof(struct Office), 1, mainFile) == 1);
        if (office.id >= 1)
        {
            office.id = office.id++;
        }
        else
        {
            office.id = 1;

        }
    }
    office.currentCountOfEmployees = 0;
    printf("%s", "Enter Department: ");
    scanf(" %s", office.department);
    printf("%s", "Enter Director: ");
    scanf(" %s", office.director);
    printf("%s", "Enter Project: ");
    scanf(" %s", office.project);
    fwrite(&office, sizeof(struct Office), 1, mainFile);
    int size = sizeof(id);
    fseek(delFile, -size, SEEK_CUR);
    fwrite(&id, intSize, 1, delFile);
    fclose(mainFile);
    fclose(delFile);
    return 0;
}

int insertS() {
    FILE* employees;
    FILE* offices;
    Employee employee;
    Office office;
    int officeSize = sizeof(Office);
    if ((employees = fopen(EMPLOYEE_PATH, "a+b")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }
    if ((offices = fopen(OFFICE_PATH, "r+b")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }
    mastersShow();
    while (fread(&employee, sizeof(struct Employee), 1, employees) == 1);
    if (employee.id >= 1)
    {
        employee.id = employee.id++;
    }
    else
    {
        employee.id = 1;

    }
    printf("Enter Office ID: ");
    scanf("%d", &employee.officeID);
    printf("%s", "Enter Employee Name: ");
    scanf("%s",employee.name);
    printf("Enter Employee Speciality: ");
    scanf("%s", employee.speciality);
    printf("\n");
    employee.isDeleted = false;
    fseek(offices, officeSize*(employee.officeID-1), SEEK_SET);
    fread(&office, officeSize, 1, offices);
    for (int i = 0; i < office.departmentSize; i++)
    {
      
        if (office.employees_id[i]==0)
        {
            office.employees_id[i] = employee.id;
            office.currentCountOfEmployees++;
            if (office.currentCountOfEmployees<office.departmentSize)
            {
                fseek(offices, officeSize * (employee.officeID - 1), SEEK_SET);
                fwrite(&office, officeSize, 1, offices);
            }
            else
            {
                perror("Error. Exceeded department size");
                return 1;
            }
            fwrite(&employee, sizeof(struct Employee), 1, employees);
            fclose(employees);
            fclose(offices);
            return 0;
        }
    }

  
}

int printAllMasters() { 
    FILE* file;
    struct Office office;

    if ((file = fopen(OFFICE_PATH, "rb")) == NULL) 
    {
        perror("Error occured while opening file");
        return 1;
    }

    printf("%s", "-----Offices-----\n");
    while (fread(&office, sizeof(struct Office), 1, file) == 1) 
    {
        if (!office.isDeleted)
        {
            printf("Office ID  : %d\nDepartment : %s\nDirector   : %s\nProject    : %s\n",
                office.id, office.department, office.director, office.project);
            printf("%s", "-------------------\n");
        }
        
    }


    fclose(file);
    return 0;
}

int printAllSlaves() 
{
    FILE* file;
    struct Employee employee;

    if ((file = fopen(EMPLOYEE_PATH, "rb")) == NULL) {
        perror("Error. Problems with opening the file");
        return 1;
    }

    printf("%s", "------------Employees--------\n");
    while (fread(&employee, sizeof(struct Employee), 1, file) == 1) 
    {
      
        if (!employee.isDeleted)
        {
            printf("ID: %d\nOffice ID: %d\nEmployee Name: %s\nEmployee Speciality: %s\n", employee.id, employee.officeID, employee.name, employee.speciality);
            printf("%s", "-----------------------------\n");
        }
       
    }

    fclose(file);
    return 0;
}

int countM() {
    FILE* fp;
    struct Office office;
    int count = 0;
    int officeSize = sizeof(Office);
    if ((fp = fopen(OFFICE_PATH, "rb")) == NULL)
    {
        perror("Error occured while opening file");
        return 1;
    }
   /* fseek(fp, -(officeSize), SEEK_END);
    fread(&office, sizeof(struct Office), 1, fp);
    printf("Amount of Offices : %d\n", office.id);*/

    while (fread(&office, sizeof(struct Office), 1, fp) == 1) 
    {
        if (!office.isDeleted)
        {
            count++;
        }
       
    }
    printf("\n");
    printf("Amount of Offices: %d\n", count);
    printf("\n");
    fclose(fp);
    return 0;
}

int countS() {
    FILE* fp;
    struct Employee employee;
    int count = 0;
    int id;

    if ((fp = fopen(EMPLOYEE_PATH, "rb")) == NULL) {
        perror("Error occured while opening file");
        return 1;
    }

    mastersShow();
    printf("Enter the Office ID or -1 for count all employees:\n");
    scanf("%d", &id);
    printf("\n");
    while (fread(&employee, sizeof(struct Employee), 1, fp) == 1) 
    {
        if (!employee.isDeleted)
        {
            if (id == -1 || employee.officeID == id) count++;
        }
       
    }

    if (id == -1) printf("count of Employees: %d\n", count);
    else printf("Office [%d] has: %d employees\n", id, count);
    printf("\n");

    fclose(fp);
    return 0;
}

int deleteMasterByID(int id) 
{ 
    FILE* mainFile;
    FILE* delFile;
    int bufferId;
    Office office;

    if ((mainFile = fopen(OFFICE_PATH, "r+b")) == NULL) 
    {
        perror("Error. Problems with opening the file");
        return 1;
    }
    if ((delFile = fopen(OFFICE_DELETED, "a+b")) == NULL)
    {
        perror("Error. Problems with opening the file");
        return 1;
    }

    fseek(mainFile, sizeof(struct Office) * (id - 1), SEEK_SET);
    fread(&office, sizeof(struct Office), 1, mainFile);
    int officeSize = sizeof(Office);
    if (office.id == id )
    {
        fseek(mainFile, -officeSize, SEEK_CUR);
        
        office.isDeleted = true;
        fwrite(&office, sizeof(struct Office), 1, mainFile);
    
        fwrite(&office.id, sizeof(int), 1, delFile);
    }
    else
    {
        printf("Office with this id is not founded");
    }
    fclose(mainFile);
    fclose(delFile);

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
    FILE* fp;
    struct Office group;
    int property;
    int is_found = 0;

    if ((fp = fopen(OFFICE_PATH, "r+b")) == NULL) {
        perror("Error occured while opening file\n");
        return 1;
    }
    fseek(fp, sizeof(Office)*(id-1), SEEK_SET);
    fread(&group, sizeof(struct Office), 1, fp);
        if (group.id != id || group.isDeleted) 
        {
           printf("Office with this id is not found\n");
            return 0;
        
        }
    officePropPrint(&group);
    printf("Enter what you want to change: ");
    scanf("%d", &property);

    switch (property) {
    case 1:
        printf("Enter the new Department: ");
        scanf("%s", group.department);
        break;
    case 2:
        printf("Enter the new Director:");
        scanf("%s", group.director);
        break;
    case 3:
        printf("Enter the new Project: ");
        scanf("%s", group.project);
        break;
    default:
        printf("\n[Error. Incorrect Number. Try Again]\n");
        while (getchar() != '\n');
        break;
    }
    printf("\n");
    double sizeGroup = sizeof(Office);
    fseek(fp, -sizeGroup, SEEK_CUR);
    fwrite(&group, sizeof(group), 1, fp);

    fclose(fp);
    return 0;
}

int slaveUpdateByID(int id) { 
    

    int property;
    int isFound=false;
    FILE* employeesFile;
    Employee employee;
    FILE* offices;
    Office office;
   
    if ((employeesFile = fopen(EMPLOYEE_PATH, "r+b")) == NULL)
    {
        perror("Error. Problems with opening the file");
        return 1;
    }
    if ((offices = fopen(OFFICE_PATH, "r+b")) == NULL)
    {
        perror("Error. Problems with opening the file");
        return 1;
    }
    fseek(employeesFile, sizeof(struct Employee) * (id - 1), SEEK_SET);
    fread(&employee, sizeof(struct Employee), 1, employeesFile);

        if (employee.id == id &&!employee.isDeleted) 
        {
            isFound = true;
        }
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
        printf("Enter the new OfficeID: ");
        scanf("%d", &employee.officeID);
        fseek(offices, sizeof(struct Office) * (employee.officeID - 1), SEEK_SET);
        fread(&office, sizeof(struct Office), 1, offices);
        for (int i = 0; i < office.departmentSize; i++)
        {   
            if (office.employees_id[i]==employee.id)
            {
               
                break;
            }

            if (office.employees_id[i] == 0 )
            {
                office.employees_id[i] = employee.id;
                office.currentCountOfEmployees++;
                if (office.currentCountOfEmployees < office.departmentSize)
                {
                  
                    fseek(offices, sizeof(struct Office) * (employee.officeID - 1), SEEK_SET);
                    fwrite(&office, sizeof(struct Office), 1, offices);
                  
                }
                else
                {
                    perror("Error. Exceeded department size");
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
    double sizeStudent = sizeof(Employee);
    fseek(employeesFile, -sizeStudent, SEEK_CUR);
    fwrite(&employee, sizeof(struct Employee), 1, employeesFile);
    fclose(employeesFile);
    fclose(offices);
    return 0;
}

void officePropPrint(struct Office* group) { 
    printf("================ID:%d=================\n",group->id);
    printf("[1] Department: %s\n[2] Director:   %s\n[3] Project:    %s\n",group->department, group->director, group->project);
    printf("=====================================\n");
}

void employeePropPrint(struct Employee* employee) 
{ 
   
    printf("=============Employee with ID: %d ==============\n",employee->officeID);
    printf("[1] Office ID:  %d\n[2] Name:       %s\n[3] Speciality: %s\n", employee->officeID,employee->name, employee->speciality);
    printf("==================================\n");
}

int mastersShow() { 
    FILE* file;
    struct Office office;

    if ((file = fopen(OFFICE_PATH, "rb")) == NULL) 
    {
        perror("Error. Problems with opening the file");
        return 1;
    }

    printf("===============Offices================\n");
    while (fread(&office, sizeof(struct Office), 1, file) == 1) 
    {
        if (!office.isDeleted)
        {
            printf("Office ID: -[%d]-\nDepartment: [%s]\nDirector:   [%s]\nProject:    [%s]\n", office.id, office.department, office.director, office.project);
            printf("====================================\n");
        }
        
    }
    fclose(file);
    return 0;
}

int slaveShow() { 
    FILE* file;
    struct Employee employee;

    if ((file = fopen(EMPLOYEE_PATH, "rb")) == NULL) 
    {
        perror("Error. Problems with opening the file\n");
        return 1;
    }

  
    printf("=============Employees=================\n");
    while (fread(&employee, sizeof(struct Employee), 1, file) == 1)
    {
        if (!employee.isDeleted)
        {
            printf("======================================\n");
            printf("Employee ID:       -[%d]-\nOffice ID:           %d\nEmployee name:       %s\nEmployee speciality: %s\n", employee.id, employee.officeID, employee.name, employee.speciality);
          
        }
       
    }
    printf("======================================\n");


    fclose(file);
    return 0;
}



