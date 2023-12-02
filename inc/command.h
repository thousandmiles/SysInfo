#ifndef COMMAND_H
#define COMMAND_H

/*
 * @brief	print help info to cmd
 * @param   void
 * @return 	void
 */
void show_help(void);

/*
 * @brief	run the service by default
 * @param   void
 * @return 	success (0) or error (1)
 */
int run_default(void);

/*
 * @brief	stop the service
 * @param   void
 * @return 	void
 */
void stop_service(void);

/*
 * @brief	run the service in the background
 * @param   void
 * @return 	void
 */
void run_background(void);

/*
 * @brief	run the service in the debug mode, allow this ip to access the original API
 * @param   ip authorized ip address
 * @return 	void
 */
void run_debug(char *ip);

/*
 * @brief	add a authorized user
 * @param   username valid user name
 * @param   password valid password
 * @return 	success (0) or error (1)
 */
int add_user(char *username, char *password);

/*
 * @brief	delete a authorized user
 * @param   username valid user name
 * @return 	success (0) or error (1)
 */
int delete_user(char *username);

#endif