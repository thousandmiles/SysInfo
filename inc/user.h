#ifndef USER_H
#define USER_H

#define FILE_PATH "user.conf"

typedef struct
{
    char username[50];
    char password[512];
} User;

/*
 * @brief	add a authorized user
 * @param   username    valid user name
 * @param   password    valid password
 * @return 	success (0) or error (1)
 */
int add_user(char *username, char *password);

/*
 * @brief	delete a authorized user
 * @param   username    valid user name
 * @return 	success (0) or error (1)
 */
int delete_user(char *username);

/*
 * @brief	check a authorized user
 * @param   username    valid user name
 * @return 	success (0) or error (1)
 */
int check_user(char *username);

#endif