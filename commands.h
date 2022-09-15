#pragma once
#define DATABASE_DB_H
#include <stdio.h>
#include <stdlib.h>

#define SIZE (255)
#define EMPLOYEE_PATH ("employee")
#define EMPLOYEE_DELETED ("employee_deleted")
#define OFFICE_PATH ("office")
#define OFFICE_DELETED ("office_deleted")

struct Department {
    int id;
    char departmentName[SIZE];
    char director[SIZE];
    char project[SIZE];
    int departmentEmployeesSize = 255;
    int employeesID[SIZE]{};
    int currentCountOfEmployees = 0;
    bool isDeleted = false;
};

struct Employee {
    int departmentID;
    int id;
    char name[SIZE];
    char speciality[SIZE];
    bool isDeleted = false;
};

int getM();

int getS();

void delM();

void delS();

int mUpdate();

int sUpdate();

int insertM();

int insertS();

int printAllMasters();

int printAllSlaves();

int countM();

int countS();

int deleteMasterByID(int id);

int deleteSlaveByID(int id);

int masterUpdateByID(int id);

int slaveUpdateByID(int id);

void departmentPropPrint(struct Department* office);

void employeePropPrint(struct Employee* employee);

int mastersShow();

int slaveShow();

void commands();



