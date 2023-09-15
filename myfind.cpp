#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <sys/wait.h>

void outputTitle();
void findFile(std::string searchpath, std::string filename, bool recursive, bool caseInsensitive);

int main(int argc, char *argv[])
{
    int opt = 0;
    bool recursive = false, caseInSensitive = false;

    while ((opt = getopt(argc, argv, "Ri")) != -1)
    {
        switch (opt)
        {
        case 'R':
            recursive = true;
            break;
        case 'i':
            caseInSensitive = true;
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " [-R] [-i] searchpath filename1 [filename2] ... [filenameN]" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    // Savind the Searchpath (optind is the next argument after the options)
    std::string searchPath = argv[optind];
    outputTitle();
    for (int i = optind + 1; i < argc; i++)
    {
        std::string filename = argv[i];
        // pid_t is the process id (if fork fails -> id is negative)
        pid_t childPid = fork();
        if (childPid == -1)
        {
            perror("Fork failed!");
            exit(EXIT_FAILURE);
        }
        else if (childPid == 0)
        {
            // Child process
            findFile(searchPath, filename, recursive, caseInSensitive);
            exit(EXIT_SUCCESS);
        }
    }

    int status;
    pid_t childPid;
    // wait() waits for the child processes to finish
    while ((childPid = wait(&status)) != -1)
    {
        if (WIFEXITED(status))
        {
            // WEXITSTATUS() is a macro, to terminate a childprocess normally
            std::cout << "Child process " << childPid << " exited with status " << WEXITSTATUS(status) << std::endl;
        }
        else if (WIFSIGNALED(status))
        {
            // WTERMSIG() is a macro, to terminate a childprocess abnormaly (Signal) due to a error
            std::cerr << "Child process " << childPid << " terminated due to signal " << WTERMSIG(status) << std::endl;
        }
    }

    return 0;
}

void findFile(std::string searchpath, std::string filename, bool recursive, bool caseInsensitive)
{
    // Open directory with the path (c_str() = std::string -> const char*)
    DIR *dir = opendir(searchpath.c_str());
    if (!dir)
    {
        perror("opendir failed!");
        exit(EXIT_FAILURE);
    }

    // dirent* is commonly used in C/CPP programs to search for information in directories
    /*
    struct dirent {
    ino_t d_ino;            // Inode number
    off_t d_off;            // Offset for the next directory entry
    unsigned short d_reclen; // Length of the entry
    unsigned char d_type;   // File type (e.g., directory or regular file)
    char d_name[];          // Null-terminated filename
    };
    */
    struct dirent *entry;
    while ((entry = readdir(dir)))
    {
        // If entry is a directory
        if (entry->d_type == DT_DIR)
        {
            // If it represents the same or the directory above -> continue
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            if (recursive)
            {
                char subdirPath[1024];
                // snprintf is a save option to format a string into the buffer (safe for bufferoverflow)
                snprintf(subdirPath, sizeof(subdirPath), "%s/%s", searchpath.c_str(), entry->d_name);
                findFile(subdirPath, filename, true, caseInsensitive);
            }
        }
        else
        {
            std::string currentFilename = entry->d_name;
            // Case-Insensitive
            if (caseInsensitive)
            {
                // strcasecmp() is like strcmp but ignores the case
                if (strcasecmp(currentFilename.c_str(), filename.c_str()) == 0)
                {
                    char fullPath[1024];
                    snprintf(fullPath, sizeof(fullPath), "%s/%s", searchpath.c_str(), currentFilename.c_str());
                    std::cout << getpid() << ":   " << filename << ": " << fullPath << std::endl;
                }
            }
            else
            {
                if (strcmp(currentFilename.c_str(), filename.c_str()) == 0)
                {
                    char fullPath[1024];
                    snprintf(fullPath, sizeof(fullPath), "%s/%s", searchpath.c_str(), currentFilename.c_str());
                    std::cout << getpid() << ":   " << filename << ": " << fullPath << std::endl;
                }
            }
        }
    }

    closedir(dir);
}

void outputTitle()
{
    std::cout << "  PID || FILENAME || FULL-PATH\n";
}