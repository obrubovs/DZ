#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Student {
    char id[7];
    char name[20];
    char lastname[25];
    char patronymic[35];
    char faculty[7];
    char speciality[100];
};



int num_lines(FILE* file_name);

void show_all(struct Student *name, int nl);

void find_student(struct Student *name, int nl);

void backup(struct Student *name, int nl);

void save_all(struct Student *name, int nl, FILE* db);



int main() {

    FILE *db;
    db = fopen("students.csv", "r");

    if (db == NULL) { //проверка на открытие файла
        printf("Не удалось открыть файл");
        getchar();
        return 1;
    } else {
        printf("Файл успешно открыт\n");
    }

    int nl = num_lines(db); //колличество строк в файле
    printf("Колличество строк: %d\n", nl);

    char buffer[1024];

    struct Student *BMSTU;
    // выделяем память для считываемой структуры
    BMSTU = (struct Student*)malloc(sizeof(struct Student));

    for (int i = 0; i < nl; i++) {

        fgets(buffer, 1024, db); //переносим данные из файла в буфер
        strcpy(BMSTU[i].id, strtok(buffer, ";"));
        strcpy(BMSTU[i].name, strtok(NULL, ";"));
        strcpy(BMSTU[i].lastname, strtok(NULL, ";"));
        strcpy(BMSTU[i].patronymic, strtok(NULL, ";"));
        strcpy(BMSTU[i].faculty, strtok(NULL, ";"));
        strcpy(BMSTU[i].speciality, strtok(NULL, "\n"));
        BMSTU = (struct Student*)realloc(BMSTU, (i + 2)*sizeof(struct Student)); //расширение памяти
    }

    fclose(db);

        int action = 9;
    printf("Выберите действие\n\n"
           "1 - Добавить студента\n"
           "2 - Удалить сткдента\n"
           "3 - Найти студента по фамилии\n"
           "4 - Вывести весь список студентов\n"
           "5 - Сделать бэкап\n"
           "6 - Восстановление из бэкапа\n"
           "0 - Сохранить и выйти\n");


    while (action == 9) {

        printf("Действие: "); scanf("%d", &action);

        if (action == 1) {

            char new_id[7];
            printf("Введите номер зачетной книги: ");
            scanf("%s", new_id);
            int error = 0;
            for (int i = 0; i < nl; i++) {
                if (strcmp(new_id, BMSTU[i].id) == 0) {
                    printf("Студент с такой зачетной книгой уже есть в базе");
                    error = 1;
                    break;
                }
            }
            if (error != 1) {
                char new_name[20];
                char new_lastname[25];
                char new_patronymic[35];
                char new_faculty[7];
                char new_spec[100];

                printf("Введите имя: ");
                scanf("%s", new_name);
                printf("Введите фамилию: ");
                scanf("%s", new_lastname);
                printf("Введите Отчество: ");
                scanf("%s", new_patronymic);
                printf("Введите факультет: ");
                scanf("%s", new_faculty);

                //ввод специальности
                char c;
                c = getchar();
                printf("Введите специальность: ");
                int n = 0;
                c = getchar();
                while (c != '\n') {
                    new_spec[n++] = c;
                    c = getchar();
                }
                new_spec[n] = '\0';

                nl++; //увеличение счетчика строк

                BMSTU = (struct Student *) realloc(BMSTU, (nl) * sizeof(struct Student)); //увеличение выделенной памяти
                strcpy(BMSTU[nl - 1].id, new_id);
                strcpy(BMSTU[nl - 1].name, new_name);
                strcpy(BMSTU[nl - 1].lastname, new_lastname);
                strcpy(BMSTU[nl - 1].patronymic, new_patronymic);
                strcpy(BMSTU[nl - 1].faculty, new_faculty);
                strcpy(BMSTU[nl - 1].speciality, new_spec);


                printf("Данные успешно записаны\n");

            }
            action = 9;
        }

        if (action == 2) {
            printf("Введите номер зачетной книжки для удаления: ");
            char del_id[6];
            scanf("%s", del_id);

            int line_to_delete = -1;
            for (int i = 0; i < nl; i++) {
                if (strcmp(del_id, BMSTU[i].id) == 0) {
                    printf("Данные найдены\n");
                    line_to_delete = i;
                    break;
                }
            }

            if (line_to_delete == -1) {
                printf("Данные не найдены\n");
            } else {
                strcpy(BMSTU[line_to_delete].id, "NULL");
                printf("Данные успешно удалены\n");
                nl--;
            }

            action = 9;
        }

        if (action == 3) {
            find_student(BMSTU, nl);
            action = 9;
        }

        if (action == 4) {
            show_all(BMSTU, nl);
            action = 9;
        }

        if (action == 5) {
            backup(BMSTU, nl);
            action = 9;
        }

        if (action == 6) {

            char backupname[50];
            printf("Введите название файла бэкапа:\n");
            scanf("%s", backupname);
            FILE *backup_file;
            backup_file = fopen(backupname, "r");

            int nl_backup = num_lines(backup_file);

            if (backup == NULL) { //проверка на открытие файла
                printf("Не удалось открыть файл бэкапа\n");
                getchar();
                return 1;
            } else {
                printf("Файл бэкапа успешно открыт\n");
            }

            struct Student *backup_data;

            backup_data = (struct Student*)malloc(sizeof(struct Student));

            char buffer_backup[1024];

            for (int i = 0; i < nl_backup; i++) {

                fgets(buffer_backup, 1024, backup_file);
                strcpy(backup_data[i].id, strtok(buffer_backup, ";"));
                strcpy(backup_data[i].name, strtok(NULL, ";"));
                strcpy(backup_data[i].lastname, strtok(NULL, ";"));
                strcpy(backup_data[i].patronymic, strtok(NULL, ";"));
                strcpy(backup_data[i].faculty, strtok(NULL, ";"));
                strcpy(backup_data[i].speciality, strtok(NULL, "\n"));
                backup_data = (struct Student*) realloc(backup_data, (i + 2) * sizeof(struct Student));
            }

            fclose(backup_file);

            db = fopen("students.csv", "w");
            if (db == NULL) {
                printf("не удалось открыть файл\n");
            } else {
                for (int i = 0; i < nl_backup; i++) {

                    fprintf(db, "%s;%s;%s;%s;%s;%s", backup_data[i].id, backup_data[i].name, backup_data[i].lastname,
                            backup_data[i].patronymic, backup_data[i].faculty, backup_data[i].speciality);
                    if (i != nl_backup - 1) {
                        fprintf(db, "\n");
                    }

                }
                fclose(db);
                printf("Успешное восстановление из бэкапа\n");
            }

            action = 9;
        }


        if (action == 0) {
            save_all(BMSTU, nl, db);
            action = 9;
        }

    }

    return 0;
}


void show_all(struct Student *name, int nl) { //выводит весь список


    printf("|--ID--|----NAME----|---LAST NAME---|--PATRONYMIC---|----F---|----------SPECIALITY----------------|\n");
    printf("|------|------------|---------------|---------------|--------|------------------------------------|\n");

    for (int i = 0; i < nl; i++) {
        printf("|%6s|%21s|%20s|%14s|%7s|%35s|\n", name[i].id, name[i].name, name[i].lastname, name[i].patronymic, name[i].faculty, name[i].speciality);

    }
}

int num_lines(FILE *file_name) {
    int nl = 1;
    char c = fgetc(file_name);
    while (c != EOF) {
        if (c == '\n') ++nl;
        c = fgetc(file_name);
    }
    rewind(file_name);
    return nl;
}

void backup(struct Student *name, int nl) {
    FILE *backup;
    char filename[70];
    long int ttime = time(NULL);
    struct tm *f_time = localtime(&ttime);
    strftime(filename, 70, "students_%d.%m.%Y_%H.%M.%S.csv", f_time);
    backup = fopen(filename, "w");
    for (int i = 0; i < nl; i++) {

        fprintf(backup, "%s;%s;%s;%s;%s;%s", name[i].id, name[i].name, name[i].lastname, name[i].patronymic,
                name[i].faculty, name[i].speciality);
        if (i != nl - 1) {
            fprintf(backup, "\n");
        }
    }

    fclose(backup);
    printf("Бэкап успешно создан %s .\n", filename);
}

void find_student(struct Student *name, int nl) {

    char lastname_tofind[30];
    printf("Введите фамилию студента, которого хотите найти:\n");
    scanf("%s", lastname_tofind);

    int d = -1;
    for (int j = 0; j < nl; j++) {
        if (strcmp(lastname_tofind, name[j].lastname) == 0) {
            printf("Информация о студенте: \n");
            printf("Номер зачетной книги: %s\n", name[j].id);
            printf("Имя: %s\n", name[j].name);
            printf("Фамилия: %s\n", name[j].lastname);
            printf("Отчество: %s\n", name[j].patronymic);
            printf("Факультет: %s\n", name[j].faculty);
            printf("Специальность: %s\n", name[j].speciality);
            d = j;
        }
    }

    if (d == -1) {
        printf("Студент не найден");
    }
}

void save_all(struct Student *name, int nl, FILE *db) {

    db = fopen("students.csv", "w");
    if (db == NULL) {
        printf("не удалось открыть файл");
    } else {
        for (int i = 0; i < nl; i++) {
            if (strcmp("NULL", name[i].id) != 0) {
                fprintf(db, "%s;%s;%s;%s;%s;%s", name[i].id, name[i].name, name[i].lastname, name[i].patronymic, name[i].faculty, name[i].speciality);
                if(i != nl-1) {
                    fprintf(db, "\n");
                }
            }
        }
        fclose(db);
        printf("Успешно сохранено\n");
    }
}
