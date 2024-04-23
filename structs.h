#define STUDENT_ID_LEN 8
#define TUTOR_ID_LEN 3
#define ADMIN_ID_LEN 3

#define USER_ID_LEN STUDENT_ID_LEN
#define PASSWORD_LEN 32
#define USERNAME_LEN 32

#define SESSION_CODE_LEN 6
#define SESSION_TITLE_LEN 32
#define SESSION_WEEKDAY_LEN 9
#define SESSION_START_TIME_LEN 8
#define SESSION_LOCATION_LEN 7

typedef enum UserType {NONE, STUDENT, TUTOR, ADMIN} UserType;
const char USER_TYPE_NAMES[4][8] = {"none", "student", "tutor", "admin"};


struct User {
	char user_ID[USER_ID_LEN + 1];
	char password[PASSWORD_LEN + 1];
	char username[USERNAME_LEN + 1];
	UserType user_type;
};

//DEFAULT_ADMIN constant is used when there are no users in the system
const struct User DEFAULT_ADMIN = {
	.user_ID = "a00",
	.password = "00000000",
	.username = "Default Admin",
	.user_type = ADMIN,
};

//NO_USER is used as a template and initial 'value' for currentUser
const struct User NO_USER = {
	.user_ID = "None",
	.password = "None",
	.username = "None",
	.user_type = NONE,
};


struct Session {
	char session_code[SESSION_CODE_LEN + 1];
	char title[SESSION_TITLE_LEN + 1];
	char weekday[SESSION_WEEKDAY_LEN + 1];
	char start_time[SESSION_START_TIME_LEN + 1];
	char location[SESSION_LOCATION_LEN + 1];
	char tutor_ID[TUTOR_ID_LEN + 1];
};


//holds information about student enrollment, serves as an intermediate entity between a user and a session
struct Access {
	char student_ID[STUDENT_ID_LEN + 1];
	char session_code[SESSION_CODE_LEN + 1];
};


struct UserArray {
	struct User* data;
	int size;
	int count;
};

struct SessionArray {
	struct Session* data;
	int size;
	int count;
};

struct AccessArray {
	struct Access* data;
	int size;
	int count;
};

