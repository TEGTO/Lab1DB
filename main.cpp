#include "commands.h"
#pragma warning(disable : 4996)

void commands() {
    printf("\nChoose what you want:\n");
    printf("[01] get-m\n"
        "[02] get-s\n"
        "[03] del-m\n"
        "[04] del-s\n"
        "[05] update-m\n"
        "[06] update-s\n"
        "[07] insert-m\n"
        "[08] insert-s\n"
        "[09] amount-m\n"
        "[10] amount-s\n"
        "[11] print-m\n"
        "[12] print-s\n"
        "[13] exit\n");

    printf("Enter: ");
}

void menu() {
    int action;
    bool check = true;
    while (check) 
    {
        commands();
        scanf("%d", &action);
        printf("\n");
        switch (action) {
        case 1:
            getM();
            break;
        case 2:
            getS();
            break;
        case 3:
            delM();
            break;
        case 4:
            delS();
            break;
        case 5:
            mUpdate();
            break;
        case 6:
            sUpdate();
            break;
        case 7:
            insertM();
            break;
        case 8:
            insertS();
            break;
        case 9:
            countM();
            break;
        case 10:
            countS();
            break;
        case 11:
            printAllMasters();
            break;
        case 12:
            printAllSlaves();
            break;
        case 13:
            check != check;
            exit(0);
            break;
        default:
            printf("\n[Error. Incorrect Number.Try Again]\n");
            while (getchar() != '\n');
            break;
        }

    }
}

int main() 
{
    menu();
    return 0;
}

