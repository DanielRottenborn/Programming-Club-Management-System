#if defined(_MSC_VER)
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include "./structs.h"
#include "./functions.h"

int main() {

	struct User currentUser = NO_USER;

	//arrays initialization & loading
	struct UserArray userArray = { .data = NULL, .size = -1, .count = -1 };
	struct SessionArray sessionArray = { .data = NULL, .size = -1, .count = -1 };
	struct AccessArray accessArray = { .data = NULL, .size = -1, .count = -1 };
	void* tempUserData = NULL;
	void* tempSessionData = NULL;
	void* tempAccessData = NULL;

	printf("<---------------------------------------->\n");

	int ret_code = LoadStructsFromFile("./data/users.dat", &tempUserData, &userArray.size, &userArray.count, sizeof(struct User));
	if (ret_code < 0) {
		if (ret_code == -1) { fprintf(stderr, "Could not read file users.dat\n"); }
		else if (ret_code == -2) { fprintf(stderr, "Could not allocate space for user data.\n"); }
		printf("<---------------------------------------->\n");
		return ret_code;
	}

	userArray.data = (struct User*)tempUserData;
	if (userArray.count == 0) { AppendUser(&DEFAULT_ADMIN, &userArray); }

	ret_code = LoadStructsFromFile("./data/sessions.dat", &tempSessionData, &sessionArray.size, &sessionArray.count, sizeof(struct Session));
	if (ret_code < 0) {
		if (ret_code == -1) { fprintf(stderr, "Could not read file sessions.dat\n"); }
		else if (ret_code == -2) { fprintf(stderr, "Could not allocate space for session data.\n"); }
		printf("<---------------------------------------->\n");
		free(userArray.data);
		return ret_code;
	}

	sessionArray.data = (struct Session*)tempSessionData;


	ret_code = LoadStructsFromFile("./data/access.dat", &tempAccessData, &accessArray.size, &accessArray.count, sizeof(struct Access));
	if (ret_code < 0) {
		if (ret_code == -1) { fprintf(stderr, "Could not read file access.dat\n"); }
		else if (ret_code == -2) { fprintf(stderr, "Could not allocate space for session access data.\n"); }
		printf("<---------------------------------------->\n");
		free(userArray.data);
		free(sessionArray.data);
		return ret_code;
	}

	accessArray.data = (struct Access*)tempAccessData;


	printf("Welcome to the Programming Cafe' management system!\n");

	//main menu
	while (1) {
		printf("Enter the corresponding keyword to select the prefered action\n\n");

		if (currentUser.user_type == NONE) {
			printf("login  -  Login into the system\n\n");

		} else if (currentUser.user_type == STUDENT) {
			printf("show_sessions  -  Display the session schedule\n\n");
			printf("enroll  -  Enroll in a session\n\n");
			printf("withdraw  -  Withdraw from a session\n\n");
			printf("logout  -  Save and logout\n\n");

		} else if (currentUser.user_type == TUTOR) {
			printf("show_sessions  -  Display the session schedule\n\n");
			printf("logout  -  Logout\n\n");

		} else if (currentUser.user_type == ADMIN) {
			printf("show_sessions  -  Display the session schedule\n\n");
			printf("add_user  -  Register a new user\n\n");
			printf("remove_user  -  Remove a user from the system\n\n");
			printf("add_session  -  Add a new session\n\n");
			printf("remove_session  -  Remove a session from the schedule\n\n");
			printf("assign_tutor  -  Assign a tutor to a session\n\n");
			printf("dismiss_tutor  -  Suspend a tutor from conducting a session\n\n");
			printf("enroll  -  Enroll a student at a session\n\n");
			printf("withdraw  -  Withdraw a student from a session\n\n");
			printf("logout  -  logout\n\n");
		}

		printf("quit  -  Save and quit\n\n");
		printf("<---------------------------------------->\n");
		
		char user_command[15];
		int ret_code = InputWord(user_command, 14);

		printf("\n<---------------------------------------->\n");

		if (ret_code == -1) {
			printf("Invalid keyword\n");
			printf("\n<---------------------------------------->\n");
			continue;
		}

		StrToLower(user_command);

		if (currentUser.user_type == NONE) {
			if (strcmp(user_command, "login") == 0) { Login(&currentUser, &userArray); continue; }

		} else if (currentUser.user_type == STUDENT) {
			if		(strcmp(user_command, "show_sessions") == 0) { ShowSessions(&currentUser, &userArray, &sessionArray, &accessArray); continue; }
			else if	(strcmp(user_command, "enroll") == 0) { Enroll(&currentUser, &userArray, &sessionArray, &accessArray); continue; }
			else if (strcmp(user_command, "withdraw") == 0) { Withdraw(&currentUser, &userArray, &sessionArray, &accessArray); continue; }
			else if (strcmp(user_command, "logout") == 0) { Logout(&currentUser); continue; }

		} else if (currentUser.user_type == TUTOR) {
			if		(strcmp(user_command, "show_sessions") == 0) { ShowSessions(&currentUser, &userArray, &sessionArray, &accessArray); continue; }
			else if	(strcmp(user_command, "logout") == 0) { Logout(&currentUser); continue; }

		} else if (currentUser.user_type == ADMIN) {
			if		(strcmp(user_command, "show_sessions") == 0) { ShowSessions(&currentUser, &userArray, &sessionArray, &accessArray); continue; }
			else if (strcmp(user_command, "add_user") == 0) { RegisterUser(&userArray); continue; }
			else if (strcmp(user_command, "remove_user") == 0) { RemoveUser(&userArray); continue; }
			else if (strcmp(user_command, "add_session") == 0) { AddSession(&sessionArray); continue; }
			else if (strcmp(user_command, "remove_session") == 0) { RemoveSession(&sessionArray); continue; }
			else if (strcmp(user_command, "assign_tutor") == 0) { AssignTutor(&userArray, &sessionArray); continue; }
			else if (strcmp(user_command, "dismiss_tutor") == 0) { DismissTutor(&sessionArray); continue; }
			else if (strcmp(user_command, "enroll") == 0) { Enroll(&currentUser, &userArray, &sessionArray, &accessArray); continue; }
			else if (strcmp(user_command, "withdraw") == 0) { Withdraw(&currentUser, &userArray, &sessionArray, &accessArray); continue; }
			else if (strcmp(user_command, "logout") == 0) { Logout(&currentUser); continue; }
		}
		
		if (strcmp(user_command, "quit") == 0) { break; }

		printf("Invalid keyword\n");
		printf("\n<---------------------------------------->\n");
	}

	SaveData(&userArray, &sessionArray, &accessArray);
	return 0;
}
