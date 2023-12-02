#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_user(char *username, char *password)
{

    if (check_user(username) == 0)
    {
        printf("user %s exists\n", username);
        return 1;
    }

    FILE *file = fopen(FILE_PATH, "a");
    if (file == NULL)
    {
        perror("can not open file");
        return 1;
    }

    fprintf(file, "%s %s\n", username, password);

    printf("user %s is authorized\n", username);

    fclose(file);

    return 0;
}

int delete_user(char *username)
{

    if (check_user(username) != 0)
    {
        printf("user %s not found\n", username);
        return 1;
    }

    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL)
    {
        perror("can not open temp file");
        return 1;
    }

    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL)
    {
        perror("can not open user config file");
        fclose(temp_file);
        return 1;
    }

    User user;
    int user_found = 0;
    while (fscanf(file, "%s %s", user.username, user.password) != EOF)
    {
        if (strcmp(user.username, username) == 0)
        {
            user_found = 1;
        }
        else
        {
            fprintf(temp_file, "%s %s\n", user.username, user.password);
        }
    }

    fclose(file);
    fclose(temp_file);

    if (!user_found)
    {
        printf("user not found: %s\n", username);
        remove("temp.txt");
        return 1;
    }

    if (remove(FILE_PATH) != 0)
    {
        perror("can not remove origin user config file");
        return 1;
    }

    if (rename("temp.txt", FILE_PATH) != 0)
    {
        perror("can not rename file");
        return 1;
    }

    printf("user %s is deleted\n", username);

    return 0;
}

int check_user(char *username)
{
    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL)
    {
        perror("can not open file");
        return 1;
    }

    User user;
    while (fscanf(file, "%s %s", user.username, user.password) != EOF)
    {
        if (strcmp(user.username, username) == 0)
        {
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}