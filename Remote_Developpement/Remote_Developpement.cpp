#define _CRT_SECURE_NO_WARNINGS

#define SETTINGS_PATH L"./settings"
#define MAKE_PATH L"./Command_Files/make"
#define RUN_PATH L"./Command_Files/run"
#define KILL_PATH L"./Command_Files/kill"

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <string.h>

int setup();
int build();
int run();
int kill();
int sync();
int ssh();
int explore();

DWORD g_BytesTransferred = 0;


VOID CALLBACK FileIOCompletionRoutine(
	__in  DWORD dwErrorCode,
	__in  DWORD dwNumberOfBytesTransfered,
	__in  LPOVERLAPPED lpOverlapped
);

VOID CALLBACK FileIOCompletionRoutine(
	__in  DWORD dwErrorCode,
	__in  DWORD dwNumberOfBytesTransfered,
	__in  LPOVERLAPPED lpOverlapped)
{
	
	g_BytesTransferred = dwNumberOfBytesTransfered;
}

int main(int argc, char** argv)
{
	if (argc == 2) {
		if (strcmp(argv[1], "-S") == 0){
			if (setup() < 0) {
				fprintf(stderr, "Setup_Failed\n");
			}
			else {
				printf("Setup succesful\n");
			}
		}
		else if (strcmp(argv[1], "-b") == 0) {
			if (build() < 0) {
				fprintf(stderr, "Build_Failed\n");
			}
			else {
				printf("Build succesful\n");
			}
		}
		else if (strcmp(argv[1], "-r") == 0) {
			if (run() < 0) {
				fprintf(stderr, "Run_Failed\n");
			}
			else {
				printf("Run succesful\n");
			}
		}
		else if (strcmp(argv[1], "-k") == 0) {
			if (kill() < 0) {
				fprintf(stderr, "KILL_Failed\n");
			}
			else {
				printf("KILL succesful\n");
			}
		}
		else if (strcmp(argv[1], "-s") == 0) {
			if (sync() < 0) {
				fprintf(stderr, "Sync_Failed\n");
			}
			else {
				printf("Sync succesful\n");
			}
		}
		else if (strcmp(argv[1], "-ssh") == 0) {
			if (ssh() < 0) {
				fprintf(stderr, "Sync_Failed\n");
			}
			else {
				printf("Sync succesful\n");
			}
		}
		else if (strcmp(argv[1], "-file") == 0) {
			if (explore() < 0) {
				fprintf(stderr, "Sync_Failed\n");
			}
			else {
				printf("Sync succesful\n");
			}
		}
		else {
			printf("Usage : remote_developpement [-S | -b | -r | -k | -s | -ssh | -file]");
			return(0);
		}
		
	}
	else{
		if (setup() < 0) {
			fprintf(stderr, "Setup_Failed\n");
		}
		else {
			printf("Setup succesful\n");
		}
	}

    return 0;
}

int setup() {
	printf("Setup Initialized\n");
	HANDLE hFile;

	hFile = CreateFile(	SETTINGS_PATH,           // name of the write
						GENERIC_WRITE,          // open for writing
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,                   // default security
						CREATE_ALWAYS,             // create new file only
						FILE_ATTRIBUTE_NORMAL,  // normal file
						NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr,"Error Creating ./settings\n");
		CloseHandle(hFile);
		return -1;
	}

	char remote_ip[16];
	printf("Enter Remote_ip : ");
	scanf("%15s", &remote_ip);

	char remote_user[100];
	printf("Enter Remote_User : ");
	scanf("%100s", &remote_user);

	char remote_path[300];
	printf("Enter Remote_path : ");
	scanf("%300s", &remote_path);

	char remote_executable[50];
	printf("Enter executable name : ");
	scanf("%50s", &remote_executable);


	char prompt_is_graphical[2];
	printf("Is the program graphical? (y:n) : ");
	scanf("%1s", &prompt_is_graphical);
	int is_Graphical = 0;

	if (prompt_is_graphical[0] == 'y') {
		is_Graphical = 1;
	}

	char prompt_create_bat[2];
	printf("Do you want to create bat files (y:n) : ");
	scanf("%1s", &prompt_create_bat);
	int createBat= 0;

	if (prompt_create_bat[0] == 'y') {
		createBat = 1;
	}

	//--------------------------------------------------------------------------------------------------------------

	DWORD bytesWritten = 0;
	char writeBuffer[500] = "IP=";

	strcat(writeBuffer, remote_ip);
	strcat(writeBuffer, "\n");
	strcat(writeBuffer, "USER=");
	strcat(writeBuffer, remote_user);
	strcat(writeBuffer, "\n");
	strcat(writeBuffer, "PATH=");
	strcat(writeBuffer, remote_path);
	strcat(writeBuffer, "\n");
	strcat(writeBuffer, "EXECUTABLE=");
	strcat(writeBuffer, remote_executable);
	strcat(writeBuffer, "\n");

	bool bErrorFlag = WriteFile(	hFile,           // open file handle
									writeBuffer,      // start of data to write
									strlen(writeBuffer),  // number of bytes to write
									&bytesWritten, // number of bytes that were written
									NULL);            // no overlapped structure

	if (FALSE == bErrorFlag)
	{
		fprintf(stderr,"ERROR_WRITING_TO_./SETTING\n");
		CloseHandle(hFile);
		return(-1);
	}

	CloseHandle(hFile);

	//--------------------------------------------------------------------------------------------------------------


	system("md Command_Files");
	system("md Build_Environment");
	system("md Results");


	hFile = CreateFile(	MAKE_PATH,           // name of the write
						GENERIC_WRITE,          // open for writing
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,                   // default security
						CREATE_ALWAYS,             // create new file only
						FILE_ATTRIBUTE_NORMAL,  // normal file
						NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Error Creating MAKE_PATH\n");
		return -1;
	}

	memset(writeBuffer, '\0', sizeof(writeBuffer));

	strcat(writeBuffer, "cd ");
	strcat(writeBuffer, remote_path);
	strcat(writeBuffer, "\nmkdir results");
	strcat(writeBuffer, "\nmake > ./results/Build_Result.txt 2> ./results/Build_Errors.txt");
	
	bErrorFlag = WriteFile(	hFile,           // open file handle
							writeBuffer,      // start of data to write
							strlen(writeBuffer),  // number of bytes to write
							&bytesWritten, // number of bytes that were written
							NULL);            // no overlapped structure

	if (FALSE == bErrorFlag)
	{
		fprintf(stderr, "ERROR_WRITING_TO_MAKE_PATH\n");
		CloseHandle(hFile);
		return(-1);
	}

	CloseHandle(hFile);


	//--------------------------------------------------------------------------------------------------------------

	hFile = CreateFile(	RUN_PATH,				// name of the write
						GENERIC_WRITE,          // open for writing
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,                   // default security
						CREATE_ALWAYS,          // create new file only
						FILE_ATTRIBUTE_NORMAL,  // normal file
						NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Error Creating RUN_PATH\n");
		return -1;
	}

	memset(writeBuffer, '\0', sizeof(writeBuffer));

	strcat(writeBuffer, "cd ");
	strcat(writeBuffer, remote_path);
	strcat(writeBuffer, "\n");

	if (is_Graphical) {
		strcat(writeBuffer, "DISPLAY=:0 ");
	}

	strcat(writeBuffer, "./");
	strcat(writeBuffer, remote_executable);
	strcat(writeBuffer, " > ./results/Run_Result.txt 2> ./results/Run_Error.txt &");

	bErrorFlag = WriteFile(	hFile,           // open file handle
							writeBuffer,      // start of data to write
							strlen(writeBuffer),  // number of bytes to write
							&bytesWritten, // number of bytes that were written
							NULL);            // no overlapped structure

	if (FALSE == bErrorFlag)
	{
		fprintf(stderr, "ERROR_WRITING_TO_RUN_PATH\n");
		CloseHandle(hFile);
		return(-1);
	}

	CloseHandle(hFile);


	//--------------------------------------------------------------------------------------------------------------

	hFile = CreateFile(KILL_PATH,				// name of the write
		GENERIC_WRITE,          // open for writing
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,                   // default security
		CREATE_ALWAYS,          // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Error Creating KILL_PATH\n");
		return -1;
	}

	memset(writeBuffer, 0, sizeof(writeBuffer));

	strcat(writeBuffer, "pgrep ");
	strcat(writeBuffer, remote_executable);
	strcat(writeBuffer, " | xargs kill");

	bErrorFlag = WriteFile(hFile,           // open file handle
		writeBuffer,      // start of data to write
		strlen(writeBuffer),  // number of bytes to write
		&bytesWritten, // number of bytes that were written
		NULL);            // no overlapped structure

	if (FALSE == bErrorFlag)
	{
		fprintf(stderr, "ERROR_WRITING_TO_KILL_PATH\n");
		CloseHandle(hFile);
		return(-1);
	}

	CloseHandle(hFile);

	//--------------------------------------------------------------------------------------------------------------

	hFile = CreateFile(L"./Build_Environment/main.c",				// name of the write
		GENERIC_WRITE,          // open for writing
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,                   // default security
		CREATE_NEW,          // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Main Already Exists\n");
	}
	else {
		memset(writeBuffer, 0, sizeof(writeBuffer));

		strcat(writeBuffer, "#include <stdio.h>\nint main(){\n\x09printf(\x22Hello World\x22);\n}");

		bErrorFlag = WriteFile(hFile,           // open file handle
			writeBuffer,      // start of data to write
			strlen(writeBuffer),  // number of bytes to write
			&bytesWritten, // number of bytes that were written
			NULL);            // no overlapped structure

		if (FALSE == bErrorFlag)
		{
			fprintf(stderr, "ERROR_WRITING_TO_Main\n");
			CloseHandle(hFile);
			return(-1);
		}
	}

	CloseHandle(hFile);

	//--------------------------------------------------------------------------------------------------------------

	hFile = CreateFile(L"./Build_Environment/makefile",				// name of the write
		GENERIC_WRITE,          // open for writing
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,                   // default security
		CREATE_NEW,          // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Makefile Already Exists\n");
	}
	else {
		memset(writeBuffer, 0, sizeof(writeBuffer));

		strcat(writeBuffer, "main: main.c\n\x09gcc main.c -o main");

		bErrorFlag = WriteFile(hFile,   // open file handle
			writeBuffer,				// start of data to write
			strlen(writeBuffer),		// number of bytes to write
			&bytesWritten,				// number of bytes that were written
			NULL);						// no overlapped structure

		if (FALSE == bErrorFlag)
		{
			fprintf(stderr, "ERROR_WRITING_TO_Make\n");
			CloseHandle(hFile);
			return(-1);
		}
	}

	CloseHandle(hFile);

	if (createBat) {
		hFile = CreateFile(L"Build.bat",				// name of the write
			GENERIC_WRITE,          // open for writing
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,                   // default security
			CREATE_ALWAYS,          // create new file only
			FILE_ATTRIBUTE_NORMAL,  // normal file
			NULL);                  // no attr. template

		if (hFile == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr, "Error Creating\n");
			return -1;
		}

		memset(writeBuffer, 0, sizeof(writeBuffer));

		strcat(writeBuffer, "remote_developpement -b");

		bErrorFlag = WriteFile(hFile,           // open file handle
			writeBuffer,      // start of data to write
			strlen(writeBuffer),  // number of bytes to write
			&bytesWritten, // number of bytes that were written
			NULL);            // no overlapped structure

		if (FALSE == bErrorFlag)
		{
			fprintf(stderr, "ERROR_WRITING_TO_bat_PATH\n");
			CloseHandle(hFile);
			return(-1);
		}

		CloseHandle(hFile);

		hFile = CreateFile(L"Run.bat",				// name of the write
			GENERIC_WRITE,          // open for writing
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,                   // default security
			CREATE_ALWAYS,          // create new file only
			FILE_ATTRIBUTE_NORMAL,  // normal file
			NULL);                  // no attr. template

		if (hFile == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr, "Error Creating\n");
			return -1;
		}

		memset(writeBuffer, 0, sizeof(writeBuffer));

		strcat(writeBuffer, "remote_developpement -r");

		bErrorFlag = WriteFile(hFile,           // open file handle
			writeBuffer,      // start of data to write
			strlen(writeBuffer),  // number of bytes to write
			&bytesWritten, // number of bytes that were written
			NULL);            // no overlapped structure

		if (FALSE == bErrorFlag)
		{
			fprintf(stderr, "ERROR_WRITING_TO_bat_PATH\n");
			CloseHandle(hFile);
			return(-1);
		}

		CloseHandle(hFile);

		hFile = CreateFile(L"Kill.bat",				// name of the write
			GENERIC_WRITE,          // open for writing
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,                   // default security
			CREATE_ALWAYS,          // create new file only
			FILE_ATTRIBUTE_NORMAL,  // normal file
			NULL);                  // no attr. template

		if (hFile == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr, "Error Creating\n");
			return -1;
		}

		memset(writeBuffer, 0, sizeof(writeBuffer));

		strcat(writeBuffer, "remote_developpement -k");

		bErrorFlag = WriteFile(hFile,           // open file handle
			writeBuffer,      // start of data to write
			strlen(writeBuffer),  // number of bytes to write
			&bytesWritten, // number of bytes that were written
			NULL);            // no overlapped structure

		if (FALSE == bErrorFlag)
		{
			fprintf(stderr, "ERROR_WRITING_TO_bat_PATH\n");
			CloseHandle(hFile);
			return(-1);
		}

		CloseHandle(hFile);

		hFile = CreateFile(L"Sync.bat",				// name of the write
			GENERIC_WRITE,          // open for writing
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,                   // default security
			CREATE_ALWAYS,          // create new file only
			FILE_ATTRIBUTE_NORMAL,  // normal file
			NULL);                  // no attr. template

		if (hFile == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr, "Error Creating\n");
			return -1;
		}

		memset(writeBuffer, 0, sizeof(writeBuffer));

		strcat(writeBuffer, "remote_developpement -s");

		bErrorFlag = WriteFile(hFile,           // open file handle
			writeBuffer,      // start of data to write
			strlen(writeBuffer),  // number of bytes to write
			&bytesWritten, // number of bytes that were written
			NULL);            // no overlapped structure

		if (FALSE == bErrorFlag)
		{
			fprintf(stderr, "ERROR_WRITING_TO_bat_PATH\n");
			CloseHandle(hFile);
			return(-1);
		}

		CloseHandle(hFile);


		hFile = CreateFile(L"ssh.bat",				// name of the write
			GENERIC_WRITE,          // open for writing
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,                   // default security
			CREATE_ALWAYS,          // create new file only
			FILE_ATTRIBUTE_NORMAL,  // normal file
			NULL);                  // no attr. template

		if (hFile == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr, "Error Creating\n");
			return -1;
		}

		memset(writeBuffer, 0, sizeof(writeBuffer));

		strcat(writeBuffer, "remote_developpement -ssh");

		bErrorFlag = WriteFile(hFile,           // open file handle
			writeBuffer,      // start of data to write
			strlen(writeBuffer),  // number of bytes to write
			&bytesWritten, // number of bytes that were written
			NULL);            // no overlapped structure

		if (FALSE == bErrorFlag)
		{
			fprintf(stderr, "ERROR_WRITING_TO_bat_PATH\n");
			CloseHandle(hFile);
			return(-1);
		}

		CloseHandle(hFile);


		hFile = CreateFile(L"file.bat",				// name of the write
			GENERIC_WRITE,          // open for writing
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,                   // default security
			CREATE_ALWAYS,          // create new file only
			FILE_ATTRIBUTE_NORMAL,  // normal file
			NULL);                  // no attr. template

		if (hFile == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr, "Error Creating\n");
			return -1;
		}

		memset(writeBuffer, 0, sizeof(writeBuffer));

		strcat(writeBuffer, "remote_developpement -file");

		bErrorFlag = WriteFile(hFile,           // open file handle
			writeBuffer,      // start of data to write
			strlen(writeBuffer),  // number of bytes to write
			&bytesWritten, // number of bytes that were written
			NULL);            // no overlapped structure

		if (FALSE == bErrorFlag)
		{
			fprintf(stderr, "ERROR_WRITING_TO_bat_PATH\n");
			CloseHandle(hFile);
			return(-1);
		}

		CloseHandle(hFile);
	}



	return 0;
}

int build() {
	printf("Building...\n");
	HANDLE hFile;
	DWORD  dwBytesRead = 0;
	char   ReadBuffer[500];
	OVERLAPPED ol = { 0 };

	hFile = CreateFile(SETTINGS_PATH,               // file to open
						GENERIC_READ,          // open for reading
						FILE_SHARE_READ,       // share for reading
						NULL,                  // default security
						OPEN_EXISTING,         // existing file only
						FILE_ATTRIBUTE_NORMAL, // normal file
						NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr,"Can't find settings, use -S to enter Setup");
		CloseHandle(hFile);
		return -1;
	}


	if (FALSE == ReadFile(hFile, ReadBuffer, 500, &dwBytesRead,NULL))
	{
		printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
	}

	CloseHandle(hFile);

	char *pointer, *bufferPointer, *remote_ip, *remote_user, *remote_path, *remote_executable;
	char separator[2] = "\n";
	char identifier[50];

	bufferPointer = ReadBuffer;

	strcpy(identifier, "IP=");
	pointer = strstr(ReadBuffer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_ip = strtok(pointer, separator);
	printf("IP : %s\n", remote_ip);


	bufferPointer = bufferPointer + strlen(remote_ip) + strlen(identifier) + 1;

	strcpy(identifier, "USER=");
	pointer = strstr(bufferPointer, identifier) + strlen(identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	remote_user = strtok(pointer, separator);
	printf("User : %s\n", remote_user);


	bufferPointer = bufferPointer + strlen(remote_user) + strlen(identifier) + 1;

	strcpy(identifier, "PATH=");
	pointer = strstr(bufferPointer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_path = strtok(pointer, separator);
	printf("Path : %s\n", remote_path);


	bufferPointer = bufferPointer + strlen(remote_path) + strlen(identifier) + 1;

	strcpy(identifier, "EXECUTABLE=");
	pointer = strstr(bufferPointer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_executable = strtok(pointer, separator);
	printf("EXECUTABLE : %s\n", remote_executable);

	char command[100];

	strcpy(command, "PSCP -r -i key.ppk ./build_environment/ ");
	strcat(command, remote_user);
	strcat(command, "@");
	strcat(command, remote_ip);
	strcat(command, ":");
	strcat(command, remote_path);

	system(command);


	strcpy(command, "putty -ssh -2 -l ");
	strcat(command, remote_user);
	strcat(command, " -i key.ppk -m Command_Files/make ");
	strcat(command, remote_ip);

	system(command);


	strcpy(command, "PSCP -r -i key.ppk ");
	strcat(command, remote_user);
	strcat(command, "@");
	strcat(command, remote_ip);
	strcat(command, ":");
	strcat(command, remote_path);
	strcat(command, "results/* ./results/");

	system(command);

	hFile = CreateFile(L"./Results/Build_Result.txt",               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Can't find settings, use");
		CloseHandle(hFile);
		return -1;
	}

	memset(ReadBuffer, 0, sizeof(ReadBuffer));

	if (FALSE == ReadFile(hFile, ReadBuffer, 500, &dwBytesRead, NULL))
	{
		printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
	}

	printf("%s", ReadBuffer);

	CloseHandle(hFile);

	hFile = CreateFile(L"./Results/Build_Errors.txt",               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Can't find settings, use");
		CloseHandle(hFile);
		return -1;
	}

	memset(ReadBuffer, 0, sizeof(ReadBuffer));

	if (FALSE == ReadFile(hFile, ReadBuffer, 500, &dwBytesRead, NULL))
	{
		printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
	}

	if (dwBytesRead == 0) {
		CloseHandle(hFile);
		return(0);
	}
	else {
		CloseHandle(hFile);
		fprintf(stderr, "\n%s\n", ReadBuffer);
		return(-1);
	}
	

	CloseHandle(hFile);

	return(0);
}

int run() {
	printf("Runing...\n");
	HANDLE hFile;
	DWORD  dwBytesRead = 0;
	char   ReadBuffer[500];
	OVERLAPPED ol = { 0 };

	hFile = CreateFile(SETTINGS_PATH,               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Can't find settings, use -S to enter Setup");
		CloseHandle(hFile);
		return -1;
	}


	if (FALSE == ReadFile(hFile, ReadBuffer, 500, &dwBytesRead, NULL))
	{
		printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
	}

	CloseHandle(hFile);

	char *pointer, *bufferPointer, *remote_ip, *remote_user, *remote_path, *remote_executable;
	char separator[2] = "\n";
	char identifier[50];

	bufferPointer = ReadBuffer;

	strcpy(identifier, "IP=");
	pointer = strstr(ReadBuffer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_ip = strtok(pointer, separator);
	printf("IP : %s\n", remote_ip);


	bufferPointer = bufferPointer + strlen(remote_ip) + strlen(identifier) + 1;

	strcpy(identifier, "USER=");
	pointer = strstr(bufferPointer, identifier) + strlen(identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	remote_user = strtok(pointer, separator);
	printf("User : %s\n", remote_user);


	bufferPointer = bufferPointer + strlen(remote_user) + strlen(identifier) + 1;

	strcpy(identifier, "PATH=");
	pointer = strstr(bufferPointer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_path = strtok(pointer, separator);
	printf("Path : %s\n", remote_path);


	bufferPointer = bufferPointer + strlen(remote_path) + strlen(identifier) + 1;

	strcpy(identifier, "EXECUTABLE=");
	pointer = strstr(bufferPointer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_executable = strtok(pointer, separator);
	printf("EXECUTABLE : %s\n", remote_executable);

	char command[100];

	strcpy(command, "putty -ssh -2 -l ");
	strcat(command, remote_user);
	strcat(command, " -i key.ppk -m Command_Files/run ");
	strcat(command, remote_ip);

	system(command);

	return(0);
}

int kill() {
	printf("Killing...\n");
	HANDLE hFile;
	DWORD  dwBytesRead = 0;
	char   ReadBuffer[500];
	OVERLAPPED ol = { 0 };

	hFile = CreateFile(SETTINGS_PATH,               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Can't find settings, use -S to enter Setup");
		CloseHandle(hFile);
		return -1;
	}


	if (FALSE == ReadFile(hFile, ReadBuffer, 500, &dwBytesRead, NULL))
	{
		printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
	}

	CloseHandle(hFile);

	char *pointer, *bufferPointer, *remote_ip, *remote_user, *remote_path, *remote_executable;
	char separator[2] = "\n";
	char identifier[50];

	bufferPointer = ReadBuffer;

	strcpy(identifier, "IP=");
	pointer = strstr(ReadBuffer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_ip = strtok(pointer, separator);
	printf("IP : %s\n", remote_ip);


	bufferPointer = bufferPointer + strlen(remote_ip) + strlen(identifier) + 1;

	strcpy(identifier, "USER=");
	pointer = strstr(bufferPointer, identifier) + strlen(identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	remote_user = strtok(pointer, separator);
	printf("User : %s\n", remote_user);


	bufferPointer = bufferPointer + strlen(remote_user) + strlen(identifier) + 1;

	strcpy(identifier, "PATH=");
	pointer = strstr(bufferPointer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_path = strtok(pointer, separator);
	printf("Path : %s\n", remote_path);


	bufferPointer = bufferPointer + strlen(remote_path) + strlen(identifier) + 1;

	strcpy(identifier, "EXECUTABLE=");
	pointer = strstr(bufferPointer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}

	pointer += strlen(identifier);
	remote_executable = strtok(pointer, separator);
	printf("EXECUTABLE : %s\n", remote_executable);

	char command[100];

	strcpy(command, "putty -ssh -2 -l ");
	strcat(command, remote_user);
	strcat(command, " -i key.ppk -m Command_Files/kill ");
	strcat(command, remote_ip);

	system(command);

	return(0);
}

int sync() {
	printf("Sync...\n");
	HANDLE hFile;
	DWORD  dwBytesRead = 0;
	char   ReadBuffer[500];
	OVERLAPPED ol = { 0 };

	hFile = CreateFile(SETTINGS_PATH,               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Can't find settings, use -S to enter Setup");
		CloseHandle(hFile);
		return -1;
	}


	if (FALSE == ReadFile(hFile, ReadBuffer, 500, &dwBytesRead, NULL))
	{
		printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
	}

	CloseHandle(hFile);

	char *pointer, *bufferPointer, *remote_ip, *remote_user, *remote_path, *remote_executable;
	char separator[2] = "\n";
	char identifier[50];

	bufferPointer = ReadBuffer;

	strcpy(identifier, "IP=");
	pointer = strstr(ReadBuffer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_ip = strtok(pointer, separator);
	printf("IP : %s\n", remote_ip);


	bufferPointer = bufferPointer + strlen(remote_ip) + strlen(identifier) + 1;

	strcpy(identifier, "USER=");
	pointer = strstr(bufferPointer, identifier) + strlen(identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	remote_user = strtok(pointer, separator);
	printf("User : %s\n", remote_user);


	bufferPointer = bufferPointer + strlen(remote_user) + strlen(identifier) + 1;

	strcpy(identifier, "PATH=");
	pointer = strstr(bufferPointer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_path = strtok(pointer, separator);
	printf("Path : %s\n", remote_path);


	bufferPointer = bufferPointer + strlen(remote_path) + strlen(identifier) + 1;

	strcpy(identifier, "EXECUTABLE=");
	pointer = strstr(bufferPointer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}

	pointer += strlen(identifier);
	remote_executable = strtok(pointer, separator);
	printf("EXECUTABLE : %s\n", remote_executable);

	char command[100];

	strcpy(command, "PSCP -r -i key.ppk ");
	strcat(command, remote_user);
	strcat(command, "@");
	strcat(command, remote_ip);
	strcat(command, ":");
	strcat(command, remote_path);
	strcat(command, "results/* ./results/");

	system(command);

	return(0);
}

int ssh() {
	printf("Launching SSH connection...\n");
	HANDLE hFile;
	DWORD  dwBytesRead = 0;
	char   ReadBuffer[500];
	OVERLAPPED ol = { 0 };

	hFile = CreateFile(SETTINGS_PATH,               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Can't find settings, use -S to enter Setup");
		CloseHandle(hFile);
		return -1;
	}


	if (FALSE == ReadFile(hFile, ReadBuffer, 500, &dwBytesRead, NULL))
	{
		printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
	}

	CloseHandle(hFile);

	char *pointer, *bufferPointer, *remote_ip, *remote_user, *remote_path, *remote_executable;
	char separator[2] = "\n";
	char identifier[50];

	bufferPointer = ReadBuffer;

	strcpy(identifier, "IP=");
	pointer = strstr(ReadBuffer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_ip = strtok(pointer, separator);
	printf("IP : %s\n", remote_ip);


	bufferPointer = bufferPointer + strlen(remote_ip) + strlen(identifier) + 1;

	strcpy(identifier, "USER=");
	pointer = strstr(bufferPointer, identifier) + strlen(identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	remote_user = strtok(pointer, separator);
	printf("User : %s\n", remote_user);


	bufferPointer = bufferPointer + strlen(remote_user) + strlen(identifier) + 1;

	strcpy(identifier, "PATH=");
	pointer = strstr(bufferPointer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_path = strtok(pointer, separator);
	printf("Path : %s\n", remote_path);


	bufferPointer = bufferPointer + strlen(remote_path) + strlen(identifier) + 1;

	strcpy(identifier, "EXECUTABLE=");
	pointer = strstr(bufferPointer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}

	pointer += strlen(identifier);
	remote_executable = strtok(pointer, separator);
	printf("EXECUTABLE : %s\n", remote_executable);

	char command[100];

	strcpy(command, "putty ");
	strcat(command, remote_user);
	strcat(command, "@");
	strcat(command, remote_ip);
	strcat(command, " -i key.ppk");

	system(command);

	return(0);
}

int explore() {
	printf("Launching explorer connection...\n");
	HANDLE hFile;
	DWORD  dwBytesRead = 0;
	char   ReadBuffer[500];
	OVERLAPPED ol = { 0 };

	hFile = CreateFile(SETTINGS_PATH,               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Can't find settings, use -S to enter Setup");
		CloseHandle(hFile);
		return -1;
	}


	if (FALSE == ReadFile(hFile, ReadBuffer, 500, &dwBytesRead, NULL))
	{
		printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
	}

	CloseHandle(hFile);

	char *pointer, *bufferPointer, *remote_ip, *remote_user, *remote_path, *remote_executable;
	char separator[2] = "\n";
	char identifier[50];

	bufferPointer = ReadBuffer;

	strcpy(identifier, "IP=");
	pointer = strstr(ReadBuffer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_ip = strtok(pointer, separator);
	printf("IP : %s\n", remote_ip);


	bufferPointer = bufferPointer + strlen(remote_ip) + strlen(identifier) + 1;

	strcpy(identifier, "USER=");
	pointer = strstr(bufferPointer, identifier) + strlen(identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	remote_user = strtok(pointer, separator);
	printf("User : %s\n", remote_user);


	bufferPointer = bufferPointer + strlen(remote_user) + strlen(identifier) + 1;

	strcpy(identifier, "PATH=");
	pointer = strstr(bufferPointer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}
	pointer += strlen(identifier);
	remote_path = strtok(pointer, separator);
	printf("Path : %s\n", remote_path);


	bufferPointer = bufferPointer + strlen(remote_path) + strlen(identifier) + 1;

	strcpy(identifier, "EXECUTABLE=");
	pointer = strstr(bufferPointer, identifier);
	if (pointer == NULL) {
		printf("Settings error\n");
		return(-1);
	}

	pointer += strlen(identifier);
	remote_executable = strtok(pointer, separator);
	printf("EXECUTABLE : %s\n", remote_executable);

	char command[100];

	strcpy(command, "explorer \\\\");
	strcat(command, remote_ip);

	system(command);
	printf(command);

	return(0);
}