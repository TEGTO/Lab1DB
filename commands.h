#pragma once
#define DATABASE_DB_H
#include <stdio.h>
#include <stdlib.h>

#define SIZE (255)
#define EMPLOYEE_PATH ("employees")
#define DELETEDEL_PATH ("deleted_el")
#define DEPARTMENT_PATH ("departments")
#define DEPARTMENTS_DELETED ("departments_deleted")
struct DeletedElements
{
    int deletedDepartmentID[200]{};
    int sizeDeletedDepartment=0;
    int deletedEmployeesID[200]{};
    int sizeDeletedEmployees = 0;
};
struct Department {
    int id;
    char departmentName[SIZE];
    char director[SIZE];
    char project[SIZE];
    int firstEmployeeId=0;
    bool isDeleted = false;
};

struct Employee {
    int departmentID;
    int id;
    int nextEmployeeID=0;
    int prevEmpoyeeID=0;
    char name[SIZE];
    char speciality[SIZE];
    bool isDeleted = false;
};

int getM();

int getS();

void delM(DeletedElements& delEl);

void delS(DeletedElements& delEl);

int mUpdate();

int sUpdate();

int insertM(DeletedElements& delEl);

int insertS(DeletedElements& delEl);

int printAllMasters();

int printAllSlaves();

int countM();

int countS();

int deleteMasterByID(int id, DeletedElements& delEl);

int deleteSlaveByID(int id, DeletedElements& delEl);

int masterUpdateByID(int id);

int slaveUpdateByID(int id);

void departmentPropPrint(struct Department* office);

void employeePropPrint(struct Employee* employee);

int mastersShow();

int slaveShow();

void commands();



