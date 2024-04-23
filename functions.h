#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "./structs.h"

//converts string to lowercase
void StrToLower(char* string) {
	for (size_t i = 0; i < strlen(string); i++) {
		string[i] = tolower(string[i]);
	}
}


//sscanf wrapper for single-word inputs, returns -1 if input would exceed maximum length
int InputWord(char* dest, size_t max_length) {
	char input_buffer[2048];
	char format[5];

	sprintf(format, "%%%zus", max_length);

	fgets(input_buffer, 2048, stdin);
	if (strlen(input_buffer) - 1 > max_length) { return -1; }

	return sscanf(input_buffer, format, dest);
}


//sscanf wrapper for string inputs, returns -1 if input would exceed maximum length
int InputString(char* dest, size_t max_length) {
	char input_buffer[2048];
	char format[8];

	sprintf(format, "%%%zu[^\n]", max_length);

	fgets(input_buffer, 2048, stdin);
	if (strlen(input_buffer) - 1 > max_length) { return -1; }

	return sscanf(input_buffer, format, dest);
}


//user ID input handling, checks for length, returns -1 if input would exceed maximum length
int InputUserID(char* dest) {
	char user_ID[USER_ID_LEN + 1];

	int ret_code = InputWord(user_ID, USER_ID_LEN);
	if (ret_code == -1) {
		printf("UserID is too long.");
		printf("\n<---------------------------------------->\n");
		return -1;
	}

	StrToLower(user_ID);
	strcpy(dest, user_ID);
	return 0;
}


//student ID input handling, checks for length and format, returns -1 on format mismatch
int InputStudentID(char* dest) {
	char student_ID[STUDENT_ID_LEN + 1];

	int ret_code = InputWord(student_ID, STUDENT_ID_LEN);
	if (ret_code == -1) {
		printf("TPNumber is too long.");
		printf("\n<---------------------------------------->\n");
		return -1;
	}
	StrToLower(student_ID);

	if (student_ID[0] != 't' || student_ID[1] != 'p' || !isdigit(student_ID[2]) || !isdigit(student_ID[3]) ||
		!isdigit(student_ID[4]) || !isdigit(student_ID[5]) || !isdigit(student_ID[6]) || !isdigit(student_ID[7])) {

		printf("Invalid TPNumber.\n");
		printf("\n<---------------------------------------->\n");
		return -1;
	}

	strcpy(dest, student_ID);
	return 0;
}


//tutor ID input handling, checks for length and format, returns -1 on format mismatch
int InputTutorID(char* dest) {
	char tutor_ID[TUTOR_ID_LEN + 1];

	int ret_code = InputWord(tutor_ID, TUTOR_ID_LEN);
	if (ret_code == -1) {
		printf("Tutor ID is too long.");
		printf("\n<---------------------------------------->\n");
		return -1;
	}
	StrToLower(tutor_ID);

	if (tutor_ID[0] != 't' || !isdigit(tutor_ID[1]) || !isdigit(tutor_ID[2])) {
		printf("Invalid tutor ID format.\n");
		printf("\n<---------------------------------------->\n");
		return -1;
	}

	strcpy(dest, tutor_ID);
	return 0;
}


//admin ID input handling, checks for length and format, returns -1 on format mismatch
int InputAdminID(char* dest) {
	char admin_ID[ADMIN_ID_LEN + 1];

	int ret_code = InputWord(admin_ID, ADMIN_ID_LEN);
	if (ret_code == -1) {
		printf("Admin ID is too long.");
		printf("\n<---------------------------------------->\n");
		return -1;
	}
	StrToLower(admin_ID);

	if (admin_ID[0] != 'a' || !isdigit(admin_ID[1]) || !isdigit(admin_ID[2])) {
		printf("Invalid admin ID format.\n");
		printf("\n<---------------------------------------->\n");
		return -1;
	}

	strcpy(dest, admin_ID);
	return 0;
}


//password input handling, checks for length and whitespaces presence, returns -1 on format mismatch
int InputPassword(char* dest) {
	char password_input[PASSWORD_LEN + 1];

	int ret_code = InputString(password_input, PASSWORD_LEN);
	if (ret_code == -1) {
		printf("Password is too long.");
		printf("\n<---------------------------------------->\n");
		return -1;
	}

	strcpy(dest, password_input);
	return 0;
}


//session code input handling, checks for length and format, returns -1 on format mismatch
int InputSessionCode(char* dest) {
	char session_code[SESSION_CODE_LEN + 1];

	int ret_code = InputWord(session_code, SESSION_CODE_LEN);
	if (ret_code == -1) {
		printf("Session code is too long.");
		printf("\n<---------------------------------------->\n");
		return -1;
	}
	StrToLower(session_code);

	if (!isalpha(session_code[0]) || !isalpha(session_code[1]) || !isalpha(session_code[2]) ||
		!isdigit(session_code[3]) || !isdigit(session_code[4]) || !isdigit(session_code[5])) {
		printf("Invalid format for a session code.\n");
		printf("\n<---------------------------------------->\n");
		return -1;
	}

	strcpy(dest, session_code);
	return 0;
}



//loads structs from a file, returns -1 if unable to open the file, returns -2 if unable to allocate memory
int LoadStructsFromFile(const char* path, void** p_struct_array, int* p_array_size, int* p_struct_count, size_t struct_size) {
	FILE* p_file = fopen(path, "rb");
	if (p_file == NULL) { return -1; }

	//structure count is stored as an integer at the beginning of the file
	int temp_struct_count = -1;
	int ret_code = fread(&temp_struct_count, sizeof(int), 1, p_file);
	if (ret_code < 1) { temp_struct_count = 0; }

	int temp_array_size = -1;
	void* tempStructArray = NULL;

	if (temp_struct_count > 0) {
		temp_array_size = temp_struct_count * 2 + 1;
		tempStructArray = malloc(temp_array_size * struct_size);
		if (tempStructArray == NULL) {
			fclose(p_file);
			return -2;
		}

		ret_code = fread(tempStructArray, struct_size, temp_struct_count, p_file);
		if (ret_code != temp_struct_count) {
			free(tempStructArray);
			fclose(p_file);
			return -1;
		}

	} else {
		temp_array_size = 10;
		tempStructArray = malloc(10 * struct_size);
		if (tempStructArray == NULL) {
			fclose(p_file);
			return -2;
		}
	}

	*p_struct_array = tempStructArray;
	*p_array_size = temp_array_size;
	*p_struct_count = temp_struct_count;

	fclose(p_file);
	return 0;
}


//saves structs to a file, returns -1 if unable to open the file
int SaveStructsToFile(const char* path, void* structArray, int stuct_count, size_t struct_size) {
	FILE* p_file = fopen(path, "wb");
	if (p_file == NULL) { return -1; }

	int ret_code = fwrite(&stuct_count, sizeof(stuct_count), 1, p_file);
	if (ret_code != 1) {
		fclose(p_file);
		return -1;
	}

	ret_code = fwrite(structArray, struct_size, stuct_count, p_file);
	fclose(p_file);

	if (ret_code != stuct_count) { return -1; }
	return 0;
}


//appends User struct to UserArray, reallocates and expands the array if needed, returns -1 if unable to reallocate
int AppendUser(struct User* user, struct UserArray* userArray) {
	if (userArray->count == userArray->size) {
		void* temp = userArray->data;

		userArray->data = realloc(userArray->data, (userArray->size * 2 + 1) * sizeof(struct User));
		if (userArray->data == NULL) {
			userArray->data = temp;
			return -1;
		}
		userArray->size = userArray->size * 2 + 1;
	}

	userArray->data[userArray->count] = *user;
	userArray->count += 1;
	return 0;
}


//searches for a User in UserArray by user_id, returns pointer to User or NULL if unsuccesfull
struct User* FindUser(char* user_ID, struct UserArray* userArray) {
	for (int i = 0; i < userArray->count; i++) {
		if (strcmp(userArray->data[i].user_ID, user_ID) == 0) {
			return userArray->data + i;
		}
	}

	return NULL;
}


//appends Session struct to SessionArray, reallocates and expands the array if needed, returns -1 if unable to reallocate
int AppendSession(struct Session* session, struct SessionArray* sessionArray) {
	if (sessionArray->count == sessionArray->size) {
		void* temp = sessionArray->data;

		sessionArray->data = realloc(sessionArray->data, (sessionArray->size * 2 + 1) * sizeof(struct Session));
		if (sessionArray->data == NULL) {
			sessionArray->data = temp;
			return -1;
		}
		sessionArray->size = sessionArray->size * 2 + 1;
	}

	sessionArray->data[sessionArray->count] = *session;
	sessionArray->count += 1;
	return 0;
}


//searches for a Session in SessionArray by session_code, returns pointer to Session or NULL if unsuccesfull
struct Session* FindSession(char* session_code, struct SessionArray* sessionArray) {
	for (int i = 0; i < sessionArray->count; i++) {
		if (strcmp(sessionArray->data[i].session_code, session_code) == 0) {
			return sessionArray->data + i;
		}
	}

	return NULL;
}


//appends Access struct to AccessArray, reallocates and expands the array if needed, returns -1 if unable to reallocate
int AppendAccess(struct Access* access, struct AccessArray* accessArray) {
	if (accessArray->count == accessArray->size) {
		void* temp = accessArray->data;

		accessArray->data = realloc(accessArray->data, (accessArray->size * 2 + 1) * sizeof(struct Access));
		if (accessArray->data == NULL) {
			accessArray->data = temp;
			return -1;
		}
		accessArray->size = accessArray->size * 2 + 1;
	}

	accessArray->data[accessArray->count] = *access;
	accessArray->count += 1;
	return 0;
}


//searches for an Access in AccessArray both by student_ID session_code, returns pointer to Access or NULL if unsuccesfull
struct Access* FindAccess(char* student_ID, char* session_code, struct AccessArray* accessArray) {
	for (int i = 0; i < accessArray->count; i++) {
		if (strcmp(accessArray->data[i].student_ID, student_ID) == 0 && strcmp(accessArray->data[i].session_code, session_code) == 0) {
			return accessArray->data + i;
		}
	}

	return NULL;
}



//login procedure, modifies currentUser if succesfull
void Login(struct User* currentUser, struct UserArray* userArray) {
	char user_ID_input[USER_ID_LEN + 1];
	char password_input[PASSWORD_LEN + 1];

	printf("Enter userID: ");
	int ret_code = InputUserID(user_ID_input);
	if (ret_code == -1) { return; }

	printf("Enter password: ");
	ret_code = InputPassword(password_input);
	if (ret_code == -1) { return; }

	struct User* user = FindUser(user_ID_input, userArray);

	if (user != NULL && strcmp(user->password, password_input) == 0) {
		*currentUser = *user;
		printf("\nHello, %s! Logged in succesfully as %s.\n", user->username, USER_TYPE_NAMES[(int)currentUser->user_type]);
		printf("\n<---------------------------------------->\n");
		return;
	}

	printf("Login failed.\n");
	printf("\n<---------------------------------------->\n");
}


//logout procedure, modifies currentUser
void Logout(struct User* currentUser) {
	*currentUser = NO_USER;
	printf("Logged out succesfully.\n");
	printf("\n<---------------------------------------->\n");
}


//registration of a new user, append to userArray on success
void RegisterUser(struct UserArray* userArray) {
	struct User newUser;
	char new_user_type[8];

	printf("Enter the type of the user to be added (student, tutor, admin): ");
	int ret_code = InputWord(new_user_type, 7);
	if (ret_code == -1) {
		printf("Invalid user type.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	StrToLower(new_user_type);

	if (strcmp(new_user_type, "student") == 0) { newUser.user_type = STUDENT; }
	else if (strcmp(new_user_type, "tutor") == 0) { newUser.user_type = TUTOR; }
	else if (strcmp(new_user_type, "admin") == 0) { newUser.user_type = ADMIN; }
	else {
		printf("Invalid user type.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	if (newUser.user_type == STUDENT) {
		printf("Enter a new TPNumber (TPNNNNNN): ");
		ret_code = InputStudentID(newUser.user_ID);
		if (ret_code == -1) { return; }

	} else if (newUser.user_type == TUTOR) {
		printf("Enter a new tutor ID (TNN): ");
		ret_code = InputTutorID(newUser.user_ID);
		if (ret_code == -1) { return; }

	} else if (newUser.user_type == ADMIN) {
		printf("Enter a new admin ID (ANN): ");
		ret_code = InputAdminID(newUser.user_ID);
		if (ret_code == -1) { return; }
	}

	if (FindUser(newUser.user_ID, userArray) != NULL) {
		printf("User with given userID already exists.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	printf("Enter a new password (no longer than 32 characters, without whitespaces): ");
	printf("Enter password: ");
	ret_code = InputPassword(newUser.password);
	if (ret_code == -1) { return; }

	for (int i = 0; i < PASSWORD_LEN; i++) {
		if (newUser.password[i] == ' ') {
			printf("Password contains whitespaces.\n");
			printf("\n<---------------------------------------->\n");
			return;
		}
	}

	printf("Enter a new user username (no longer than 32 characters): ");
	ret_code = InputString(newUser.username, USERNAME_LEN);
	if (ret_code == -1) {
		printf("Username is too long.");
		printf("\n<---------------------------------------->\n");
		return;
	}

	if (AppendUser(&newUser, userArray) == -1) {
		printf("Failed to add a new user.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	printf("Succesfully added %s.\n", newUser.user_ID);
	printf("\n<---------------------------------------->\n");
}


//deletes a user, removes an element from userArray on success
void RemoveUser(struct UserArray* userArray) {
	char user_ID[USER_ID_LEN + 1];

	printf("Enter the userID of the user you want to remove: ");
	int ret_code = InputUserID(user_ID);
	if (ret_code == -1) { return; }

	for (int i = 0; i < userArray->count; i++) {
		if (strcmp(userArray->data[i].user_ID, user_ID) == 0) {
			for (int j = i + 1; j < userArray->count; j++) { 
				userArray->data[j - 1] = userArray->data[j];  //shifts every element after the target by -1
			}
			userArray->count -= 1;

			printf("Succesfully removed user %s from the system.\n", user_ID);
			printf("\n<---------------------------------------->\n");
			return;
		}
	}

	printf("Could not find a user with given ID.\n");
	printf("\n<---------------------------------------->\n");
}


//adds a new session to the schedulle, appends to sessionArray on success
void AddSession(struct SessionArray* sessionArray) {
	struct Session newSession = {
		.session_code = "None",
		.title = "None",
		.weekday = "None",
		.start_time = "None",
		.location = "None",
		.tutor_ID = "n/a",
	};

	printf("Enter a new session code (CCCNNN): ");
	int ret_code = InputSessionCode(newSession.session_code);
	if (ret_code == -1) { return; }

	if (FindSession(newSession.session_code, sessionArray) != NULL) {
		printf("Session with given code already exists.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	printf("Enter a session title (no longer than 32 characters): ");
	ret_code = InputString(newSession.title, SESSION_TITLE_LEN);
	if (ret_code == -1) {
		printf("Session title is too long.");
		printf("\n<---------------------------------------->\n");
		return;
	}

	printf("Enter the weekday (monday, tuesday...): ");
	ret_code = InputWord(newSession.weekday, SESSION_WEEKDAY_LEN);
	if (ret_code == -1) {
		printf("Weekday name is too long.");
		printf("\n<---------------------------------------->\n");
		return;
	}

	StrToLower(newSession.weekday);

	if (strcmp(newSession.weekday, "monday") != 0 && strcmp(newSession.weekday, "tuesday") != 0 && strcmp(newSession.weekday, "wednesday") != 0 &&
		strcmp(newSession.weekday, "thursday") != 0 && strcmp(newSession.weekday, "friday") != 0 && strcmp(newSession.weekday, "saturday") != 0 &&
		strcmp(newSession.weekday, "sunday") != 0) {
		printf("Invalid weekday name.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	printf("Enter the time: ");
	ret_code = InputWord(newSession.start_time, SESSION_START_TIME_LEN);
	if (ret_code == -1) {
		printf("Time string is too long.");
		printf("\n<---------------------------------------->\n");
		return;
	}

	printf("Enter the location (C-NN-NN): ");
	ret_code = InputWord(newSession.location, SESSION_LOCATION_LEN);
	if (ret_code == -1) {
		printf("Location name is too long.");
		printf("\n<---------------------------------------->\n");
		return;
	}

	StrToLower(newSession.location);

	if (!isalpha(newSession.location[0]) || newSession.location[1] != '-' || !isdigit(newSession.location[2]) || !isdigit(newSession.location[3]) ||
			newSession.location[4] != '-' || !isdigit(newSession.location[5]) || !isdigit(newSession.location[6])) {

		printf("Invalid location name.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	if (AppendSession(&newSession, sessionArray) == -1) {
		printf("Failed to add session.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	printf("Succesfully added %s.\n", newSession.session_code);
	printf("\n<---------------------------------------->\n");
}


//deletes a session, removes an element from sessionArray on success
void RemoveSession(struct SessionArray* sessionArray) {
	char session_code[SESSION_CODE_LEN + 1];

	printf("Enter the code of the session you want to remove: ");
	int ret_code = InputSessionCode(session_code);
	if (ret_code == -1) { return; }

	for (int i = 0; i < sessionArray->count; i++) {
		if (strcmp(sessionArray->data[i].session_code, session_code) == 0) {
			for (int j = i + 1; j < sessionArray->count; j++) {
				sessionArray->data[j - 1] = sessionArray->data[j];  //shifts every element after the target by -1
			}
			sessionArray->count -= 1;

			printf("Succesfully removed session %s from the schedule.\n", session_code);
			printf("\n<---------------------------------------->\n");

			return;
		}
	}

	printf("Could not find a session with given code.\n");
	printf("\n<---------------------------------------->\n");
}


//displays all sessions, format is dependant on currentUser (for students, displays the enrollments status for each session)
void ShowSessions(struct User* currentUser, struct UserArray* userArray, struct SessionArray* sessionArray, struct AccessArray* accessArray) {
	if (sessionArray->count == 0) {
		printf("No sessions are currently available\n");
		printf("<---------------------------------------->\n");
		return;
	}

	if (currentUser->user_type == STUDENT) {
		const char column_names[7][21] = { "Session Code", "Title", "Day", "Start Time", "Location", "TutorCode", "Student Status" };
		printf("%-16s%-26s%-16s%-16s%-16s%-16s%-16s\n\n", column_names[0], column_names[1], column_names[2], column_names[3], column_names[4],
			column_names[5], column_names[6]);

		for (int i = 0; i < sessionArray->count; i++) {
			char enrollment[9] = "-";

			//searches for the sessions in which the student is enrolled
			for (int j = 0; j < accessArray->count; j++) {
				if (strcmp(sessionArray->data[i].session_code, accessArray->data[j].session_code) == 0 &&
						strcmp(currentUser->user_ID, accessArray->data[j].student_ID) == 0) {

					strcpy(enrollment, "Enrolled");
				}
			}

			struct Session* session = sessionArray->data + i;
			printf("%-16s%-26s%-16s%-16s%-16s%-16s%-16s\n", session->session_code, session->title, session->weekday,
				session->start_time, session->location, session->tutor_ID, enrollment);

			//prints out students enrolled in each session
			printf("Enrolled Students: ");
			for (int j = 0; j < accessArray->count; j++) {
				struct User* student = FindUser(accessArray->data[j].student_ID, userArray);
				if (strcmp(sessionArray->data[i].session_code, accessArray->data[j].session_code) == 0 && student != NULL) {
					printf("%s, ", student->username);
					
				}
			}
			printf("\n\n");
		}
	} else {
		const char column_names[6][21] = { "Session Code", "Title", "Day", "Start Time", "Location", "TutorCode" };
		printf("%-16s%-26s%-16s%-16s%-16s%-16s\n\n", column_names[0], column_names[1], column_names[2], column_names[3], column_names[4], column_names[5]);

		for (int i = 0; i < sessionArray->count; i++) {
			struct Session* session = sessionArray->data + i;
			printf("%-16s%-26s%-16s%-16s%-16s%-16s\n", session->session_code, session->title, session->weekday,
				session->start_time, session->location, session->tutor_ID);

			//prints out students enrolled in each session
			printf("Enrolled Students: ");
			for (int j = 0; j < accessArray->count; j++) {
				struct User* student = FindUser(accessArray->data[j].student_ID, userArray);
				if (strcmp(sessionArray->data[i].session_code, accessArray->data[j].session_code) == 0 && student != NULL) {
					printf("%s, ", student->username);

				}
			}
			printf("\n\n");
		}
	}

	printf("<---------------------------------------->\n");
}


//assigns tutor, modifies session instance if succesfull
void AssignTutor(struct UserArray* userArray, struct SessionArray* sessionArray) {
	char session_code[SESSION_CODE_LEN + 1];
	char tutor_ID[TUTOR_ID_LEN + 1];

	printf("Enter a session code (CCCNNN): ");
	int ret_code = InputSessionCode(session_code);
	if (ret_code == -1) { return; }

	struct Session* session = FindSession(session_code, sessionArray);

	if (session == NULL) {
		printf("Session not found.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	if (strcmp(session->tutor_ID, "n/a") != 0) {
		printf("The session already has a tutor.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	printf("Enter the code of the tutor to assign in format (TNN): ");
	ret_code = InputTutorID(tutor_ID);
	if (ret_code == -1) { return; }

	struct User* tutor = FindUser(tutor_ID, userArray);

	if (tutor == NULL) {
		printf("Tutor not found.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	strcpy(session->tutor_ID, tutor_ID);
	printf("Succesfully assigned %s to %s.\n", tutor_ID, session_code);
	printf("\n<---------------------------------------->\n");
}


//dismisses tutor, modifies session instance if succesfull
void DismissTutor(struct SessionArray* sessionArray) {
	char session_code[SESSION_CODE_LEN + 1];

	printf("Enter a session code (CCCNNN): ");
	int ret_code = InputSessionCode(session_code);
	if (ret_code == -1) { return; }

	struct Session* session = FindSession(session_code, sessionArray);

	if (session == NULL) {
		printf("Session not found.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	if (strcmp(session->tutor_ID, "n/a") == 0) {
		printf("The session is not yet assigned to any tutor.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	strcpy(session->tutor_ID, "n/a");
	printf("Suspended the tutor from conducting given session.\n");
	printf("\n<---------------------------------------->\n");
}


//enrolls a student in a session, appends to AccessArray if successfull (admins can enroll all students, students can only enroll themselves)
void Enroll(struct User* currentUser, struct UserArray* userArray, struct SessionArray* sessionArray, struct AccessArray* accessArray) {
	struct Access access = { .student_ID = "None", .session_code = "None" };

	if (currentUser->user_type == STUDENT) {
		strcpy(access.student_ID, currentUser->user_ID);

	} else {
		printf("Enter the TPNumber of the student you want to enroll: ");
		int ret_code = InputStudentID(access.student_ID);
		if (ret_code == -1) { return; }

		if (FindUser(access.student_ID, userArray) == NULL) {
			printf("Student not found.\n");
			printf("\n<---------------------------------------->\n");
			return;
		}
	}

	printf("Enter a session code (CCCNNN): ");
	int ret_code = InputSessionCode(access.session_code);
	if (ret_code == -1) { return; }

	if (FindSession(access.session_code, sessionArray) == NULL) {
		printf("Session not found.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	if (FindAccess(access.student_ID, access.session_code, accessArray) != NULL) {
		printf("The student has already been enrolled in the given session.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	AppendAccess(&access, accessArray);

	printf("Enrolled student %s at session %s succesfully.\n", access.student_ID, access.session_code);
	printf("\n<---------------------------------------->\n");

}


//withdraws a student from a session, deletes from AccessArray if successfull (admins can withdraw all students, students can only withdraw themselves)
void Withdraw(struct User* currentUser, struct UserArray* userArray, struct SessionArray* sessionArray, struct AccessArray* accessArray) {
	char student_ID[STUDENT_ID_LEN + 1];
	char session_code[SESSION_CODE_LEN + 1];

	if (currentUser->user_type == STUDENT) {
		strcpy(student_ID, currentUser->user_ID);

	}
	else {
		printf("Enter the TPNumber of the student you want to remove session access from: ");
		int ret_code = InputStudentID(student_ID);
		if (ret_code == -1) { return; }

		if (FindUser(student_ID, userArray) == NULL) {
			printf("Student not found.\n");
			printf("\n<---------------------------------------->\n");
			return;
		}
	}

	printf("Enter the code of the session to have the access denied (CCCNNN): ");
	int ret_code = InputSessionCode(session_code);
	if (ret_code == -1) { return; }

	if (FindSession(session_code, sessionArray) == NULL) {
		printf("Session not found.\n");
		printf("\n<---------------------------------------->\n");
		return;
	}

	for (int i = 0; i < accessArray->count; i++) {
		if (strcmp(accessArray->data[i].session_code, session_code) == 0 && strcmp(accessArray->data[i].student_ID, student_ID) == 0) {
			for (int j = i + 1; j < accessArray->count; j++) {
				accessArray->data[j - 1] = accessArray->data[j];  //shifts each ellement after target by -1
			}
			accessArray->count -= 1;

			printf("Succesfully removed session access to %s from %s.\n", session_code, student_ID);
			printf("\n<---------------------------------------->\n");
			return;
		}
	}

	printf("Could not find an access permission with specified parameters.\n");
	printf("\n<---------------------------------------->\n");
}


//saves data before shutdown, will ask to repeat the process if it fails
void SaveData(struct UserArray* userArray, struct SessionArray* sessionArray, struct AccessArray* accessArray) {
	int ret_code;
	char user_input[2];

	while (1) {
		ret_code = SaveStructsToFile("./data/users.dat", userArray->data, userArray->count, sizeof(struct User));
		if (ret_code == 0) { printf("Saved user data succesfully.\n"); break; }
	
		printf("Could not save user data. Type '0' to skip, type anything except '0' to retry: ");
		ret_code = InputWord(user_input, 1);
		if (ret_code == 0 && user_input[0] == '0') { break; }
	}

	while (1) {
		ret_code = SaveStructsToFile("./data/sessions.dat", sessionArray->data, sessionArray->count, sizeof(struct Session));
		if (ret_code == 0) { printf("Saved session data succesfully.\n"); break; }

		printf("Could not save session data. Type '0' to skip, type anything except '0' to retry: ");
		ret_code = InputWord(user_input, 1);
		if (ret_code == 0 && user_input[0] == '0') { break; }
	}

	while (1) {
		ret_code = SaveStructsToFile("./data/access.dat", accessArray->data, accessArray->count, sizeof(struct Access));
		if (ret_code == 0) { printf("Saved access data succesfully.\n"); break; }

		printf("Could not save access data. Type '0' to skip, type anything except '0' to retry: ");
		ret_code = InputWord(user_input, 1);
		if (ret_code == 0 && user_input[0] == '0') { break; }
	}

	printf("\n<---------------------------------------->\n");
}
