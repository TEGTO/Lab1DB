#pragma once
#define DATABASE_DB_H
#include <stdio.h>
#include <stdlib.h>

#define SIZE (255)
#define EMPLOYEE_PATH ("employee")
#define EMPLOYEE_DELETED ("employee_deleted")
#define OFFICE_PATH ("office")
#define OFFICE_DELETED ("office_deleted")

struct Office {
    int id;
    char department[SIZE];
    char director[SIZE];
    char project[SIZE];
    bool isDeleted = false;
};

struct Employee {
    int officeID;
    char name[SIZE];
    char speciality[SIZE];
};

int getM();

int getS();

void delM();

void delS();

int mUpdate();

int sUpdate();

int insertM();

int insertS();

int printM();

int printS();

int countM();

int countS();

int deleteMasterByID(int id);

int deleteSlaveByID(int id);

int masterUpdateByID(int id);

int slaveUpdateByID(int id);

void officePropPrint(struct Office* office);

void employeePropPrint(struct Employee* employee);

int mastersShow();

int slaveShow();

void commands();



