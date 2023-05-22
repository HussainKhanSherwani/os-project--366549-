#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <vector>
#include <fcntl.h>

#define TOKENSIZE 100


using namespace std;
void  StrTokenizer(char *line, char **argv);
// void  myExecvp(char **argv);
// int GetEnv();
void RunCatCommand(char **argv);
void RunEchoCommand(char **argv);
void RunMkdirCommand(char **argv);
void RunCmpCommand(char **argv);

void ListDirectoryEntries() {
    DIR *dir;
    struct dirent *entry;

    // Open the current directory
    dir = opendir(".");

    if (dir == NULL) {
        cerr << "ERROR: Failed to open directory" << endl;
        return;
    }

    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        cout << entry->d_name << endl;
    }

    // Close the directory
    closedir(dir);
}
int main()
{

	char *path2;
	char *arr [250];
	char *Tokenized ;
	char input[250];
	char *argv[TOKENSIZE];
	
	// Declare a vector to store command history
	std::vector<std::string> commandHistory;

    cout<<"This terminal is created by \nGhulam hussain Khan \nAbdul Basit Sajid \nMohammad Irfan \n";
    cout<<"for help write help \n";
    cout<<"name of the shell is cwushell \n";
	while(true)
	{
		cout<< "cwushell-> ";
		cin.getline(input,250);
		commandHistory.push_back(input);
		StrTokenizer(input, argv);  
		if (strcmp(input, "exit") == 0)  

		{
			break;
		}
        else if (strcmp(input, "presentdir") == 0)
		{
			char cwd[1024];
			if (getcwd(cwd, sizeof(cwd)) != NULL)
			{
				cout << cwd << endl;
			}
			else
			{
				cout << "ERROR: Failed to get current working directory" << endl;
			}
			continue;
		}
		else if (strcmp(argv[0], "help") == 0)
    {
        cout << "Welcome to cwushell help!" << endl;
        cout << "Available commands:" << endl;
        cout << "  exit                - Exit the shell" << endl;
        cout << "  presentdir          - Print the current working directory" << endl;
        cout << "  cdir <directory>    - Change directory" << endl;
        cout << "  remove <file>       - Remove a file" << endl;
        cout << "  end <pid>           - End a process with the given process ID" << endl;
        cout << "  trackhistory        - Display command history" << endl;
        cout << "  create <file>       - Create or update a file" << endl;
        cout << "  rmdir <directory>   - Remove a directory" << endl;
        cout << "  move <source> <dest>- Move or rename a file" << endl;
        cout << "  list                - List directory entries" << endl;
        cout << "  cat <file>          - Display the contents of a file" << endl;
        cout << "  prin <text>         - Print text" << endl;
        cout << "  mkdir <directory>   - Create a directory" << endl;
        cout << "  cmp <file1> <file2> - Compare two files" << endl;
        cout << "  pidof<process>      - finds pid of process" << endl;
		cout << "  help                - Display this help message" << endl;
        continue; // Skip command execution for help
    }
		 else if (strcmp(argv[0], "cdir") == 0)
        	{
	            if (argv[1] != NULL)
            {	
				if(chdir(argv[1]) == 0){
					cout << "Directory is changed to "<< argv[1] << endl;
				}
                else if (chdir(argv[1]) != 0)
                {
                    cout << "ERROR: Failed to change directory" << endl;
                }
            }
            else
            {
                cout << "ERROR: Missing directory argument for cdir command" << endl;
            }
            continue; // Skip command execution for cd
        }
		else if (strcmp(argv[0], "remove") == 0)
        {
            if (argv[1] != NULL)
            {
                int status = remove(argv[1]);
                if (status == 0)
                {
                    cout << "File " << argv[1] << " removed successfully" << endl;
                }
                else
                {
                    cout << "ERROR: Failed to remove file " << argv[1] << endl;
                }
			}
			else{
				cout << "ERROR: Missing directory argument for remove command" << endl;
			}
		}
		else if (strcmp(argv[0], "pidof") == 0)
   		 {
        if (argv[1] != NULL)
        {
            // Fork a child process
            pid_t pid = fork();

            if (pid == 0)
            {
                // Child process
                // Execute the pidof command
                execlp("pidof", "pidof", argv[1], NULL);

                // If execlp returns, there was an error
                cout << "ERROR: Failed to execute pidof command" << endl;
                exit(EXIT_FAILURE);
            }
            else if (pid > 0)
            {
                // Parent process
                // Wait for the child process to complete
                int status;
                waitpid(pid, &status, 0);
            }
            else
            {
                // Fork failed
                cout << "ERROR: Failed to fork a child process" << endl;
            }
        }
        else
        {
            cout << "ERROR: Missing process name argument for pidof command" << endl;
        }
        continue; // Skip command execution for pidof
    }
		else if (strcmp(argv[0], "end") == 0)
        {
            if (argv[1] != NULL)
            {
                int processId = atoi(argv[1]);
                // Use the kill() function to send a signal to the specified process ID
                if (kill(processId, SIGKILL) == 0)
                {
                    cout << "killed and ended the process with pid"<<processId << endl;
                }
				else{
					cout << "Not able to end or kill"<< endl;
				}
            }
            else
            {
                cout << "ERROR: Missing process ID argument for kill command" << endl;
            }
            continue; // Skip command execution for kill
        }
		else if (strcmp(argv[0], "trackhistory") == 0)
		{
			// Display command history
			for (const std::string& command : commandHistory)
			{
				cout << command << endl;
			}

			continue; // Skip command execution for history
		}

		else if (strcmp(argv[0], "create") == 0)
    {
        if (argv[1] != NULL)
        {
            // Create or update the file
            int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
            if (fd == -1)
            {
                cout << "Failed to create or update the file" << endl;
            }
            else
            {
                close(fd);
                cout << "File created or updated: " << argv[1] << endl;
            }
        }
        else
        {
            cout << "Missing filename argument for touch command" << endl;
        }
        continue; // Skip command execution for touch
    }
		else if (strcmp(argv[0], "rmdir") == 0)
		{
			if (argv[1] != NULL)
		{
			if (rmdir(argv[1]) == 0)
        {
            cout << "Directory removed: " << argv[1] << endl;
        }
        else
        {
            cout << "ERROR: Failed to remove directory: " << argv[1] << endl;
			}
		}
		else
		{
			cout << "ERROR: Missing directory name argument for rmdir command" << endl;
		}
		continue; // Skip command execution for rmdir
		}
		else if (strcmp(argv[0], "move") == 0)
		{
		if (argv[1] != NULL && argv[2] != NULL)
		{
			if (rename(argv[1], argv[2]) == 0)
			{
				cout << "File moved: " << argv[1] << " -> " << argv[2] << endl;
			}
			else
			{
				cout << "ERROR: Failed to move file: " << argv[1] << endl;
			}
		}
		else
		{
			cout << "ERROR: Missing source and/or destination file arguments for mv command" << endl;
		}
		continue; // Skip command execution for mv
		}
		else if (strcmp(argv[0], "list") == 0) {
            ListDirectoryEntries();
            continue; // Skip command execution for ls
        }
		 else if (strcmp(argv[0], "cat") == 0)
        {
            RunCatCommand(argv);
            continue; // Skip command execution for cat
        }
        else if (strcmp(argv[0], "prin") == 0)
        {
            RunEchoCommand(argv);
            continue; // Skip command execution for echo
        }
        else if (strcmp(argv[0], "mkdir") == 0)
        {
            RunMkdirCommand(argv);
            continue; // Skip command execution for mkdir
        }
        else if (strcmp(argv[0], "cmp") == 0)
        {
            RunCmpCommand(argv);
            continue; // Skip command execution for cmp
        }
		else if (strcmp(input, "\n") == 0){
			continue;
		}
		cout<<input<<" command not found"<<endl;
	}
	return 0;
}
void RunCatCommand(char **argv)
{
    int fileDescriptor;
    char buffer[256];
    ssize_t bytesRead;

    if (argv[1] != NULL)
    {
        fileDescriptor = open(argv[1], O_RDONLY);
        if (fileDescriptor < 0)
        {
            cout << "ERROR: Failed to open file: " << argv[1] << endl;
            return;
        }

        while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytesRead] = '\0';
            cout << buffer;
        }

        close(fileDescriptor);
    }
    else
    {
        cout << "ERROR: Missing file argument for cat command" << endl;
    }
}

void RunEchoCommand(char **argv)
{
    for (int i = 1; argv[i] != NULL; i++)
    {
        cout << argv[i] << " ";
    }
    cout << endl;
}

void RunMkdirCommand(char **argv)
{
    if (argv[1] != NULL)
    {
        if (mkdir(argv[1], 0777) == 0)
        {
            cout << "Directory created: " << argv[1] << endl;
        }
        else
        {
            cout << "ERROR: Failed to create directory: " << argv[1] << endl;
        }
    }
    else
    {
        cout << "ERROR: Missing directory argument for mkdir command" << endl;
    }
}

void RunCmpCommand(char **argv)
{
    if (argv[1] != NULL && argv[2] != NULL)
    {
        FILE *file1 = fopen(argv[1], "r");
        FILE *file2 = fopen(argv[2], "r");

        if (file1 == NULL)
        {
            cout << "ERROR: Failed to open file: " << argv[1]<< " it might not exist"<< endl;
            return;
        }

        if (file2 == NULL)
        {
            cout << "ERROR: Failed to open file: " << argv[2] <<" it might not exist"<< endl;
            return;
        }

        int ch1, ch2;
        bool filesEqual = true;

        while ((ch1 = fgetc(file1)) != EOF && (ch2 = fgetc(file2)) != EOF)
        {
            if (ch1 != ch2)
            {
                filesEqual = false;
                break;
            }
        }

        if (ch1 != ch2)
        {
            filesEqual = false;
        }

        fclose(file1);
        fclose(file2);

        if (filesEqual)
        {
            cout << "Files are identical" << endl;
        }
        else
        {
            cout << "Files are different" << endl;
        }
    }
    else
    {
        cout << "ERROR: Missing file arguments for cmp command" << endl;
    }
}
// void  myExecvp(char **argv)
// {
// 	pid_t  pid;
// 	int    status;
// 	int childStatus;
// 	pid = fork();
// 	if(pid == 0)
// 	{
// 		childStatus = execvp(*argv, argv);
// 		if (childStatus < 0){
// 			cout<<"ERROR:wrong input"<<endl;
// 		}
// 		exit(0);

// 	}
// 	else if(pid < 0)
// 	{
// 		cout<< "somthing went wrong!"<<endl;
// 	}
// 	else 

// 	{   int status;
// 		waitpid(pid, &status , 0);

// 	}
// }

void StrTokenizer(char *input, char **argv)
{
	char *stringTokenized;
	stringTokenized = strtok(input, " ");
	while(stringTokenized != NULL)
	{
		*argv++  = stringTokenized;
		stringTokenized = strtok(NULL, " ");
	}

	*argv = NULL;
}

// int GetEnv()
// {
// 	char *path2;
// 	char *arr2[250];
// 	char *Tokenized ;
// 	path2 = getenv("PATH");
// 	Tokenized = strtok(path2, ":");
// 	int k = 0;
// 	while(Tokenized != NULL)
// 	{
// 		arr2[k] = Tokenized;
// 		Tokenized = strtok(NULL, ":");
// 		k++;
// 	}

// 	arr2[k] = NULL;
// }