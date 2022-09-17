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
void readDelFile(DeletedElements& delEl)
{
    FILE* delElFile;
    if ((delElFile = fopen(DELETEDEL_PATH, "a+b")) == NULL) {
        perror("Error. Problems with opening the file");

    }
    fread(&delEl, sizeof(struct DeletedElements), 1, delElFile);
    remove(DELETEDEL_PATH);
    fclose(delElFile);
}
void saveDelFile(DeletedElements& delEl)
{
    FILE* delElFile;
    if ((delElFile = fopen(DELETEDEL_PATH, "a+b")) == NULL) {
        perror("Error. Problems with opening the file");

    }
    fwrite(&delEl, sizeof(struct DeletedElements), 1, delElFile);
    fclose(delElFile);
}
void menu() {
    int action;
    bool check = true;
    DeletedElements delEl;
    readDelFile(delEl);
    
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
            delM(delEl);
            break;
        case 4:
            delS(delEl);
            break;
        case 5:
            mUpdate();
            break;
        case 6:
            sUpdate();
            break;
        case 7:
            insertM(delEl);
            break;
        case 8:
            insertS(delEl);
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
            saveDelFile(delEl);
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

