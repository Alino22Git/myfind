#include <stdio.h>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <algorithm>
namespace fs = std::filesystem;

void StringCompare(std::string entry, std::string searchingFile, bool caseInSensitive);
    // void outPutFormat(int processID,std::vector<std::string> files,std::vector<std::string> paths);

    int main(int argc, char *argv[])
{
    bool recursive = false, caseInSensitive = false;
    std::string searchPath;
    std::vector<std::string> files;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-R")
        {
            recursive = true;
        }
        else if (arg == "-i")
        {
            caseInSensitive = true;
        }
        else if (searchPath.empty())
        {
            searchPath = argv[i];
        }
        else
        {
            files.push_back(argv[i]);
        }
    }

    if (searchPath.empty() || files.empty())
    {
        std::cout << "Usage: ./myfind [-R] [-i] searchpath filename1 [filename2] ... [filenameN]\n";
        return 1;
    }

    if (recursive)
    {
        // for-each: you get the reference(&) of every entry in the searchpath (without & you would get a copy of the entry)
        for (const auto &rec_entry : fs::recursive_directory_iterator(searchPath))
        {
            if (rec_entry.is_regular_file())
            {
                for (int i = 0; i < (int)files.size(); i++)
                {
                    StringCompare(rec_entry.path().filename().string(), files[i], caseInSensitive);
                }
            }
        }
    }
    else
    {
        // for-each: you get the reference(&) of every entry in the searchpath (without & you would get a copy of the entry)
        for (const auto &dir_entry : fs::directory_iterator(searchPath))
        {
            if (dir_entry.is_regular_file())
            {
                for (int i = 0; i < (int)files.size(); i++)
                {
                    StringCompare(dir_entry.path().filename().string(), files[i], caseInSensitive);
                }
            }
        }
    }

    return 0;
}

void StringCompare(std::string entry, std::string searchingFile, bool caseInSensitive)
{
    if (caseInSensitive)
    {
        //Testing if searchingFile exists in entry 
        if (std::equal(entry.begin(), entry.end(), searchingFile.begin(), searchingFile.end(), [](char c1, char c2){ return std::tolower(c1) == std::tolower(c2); }))
        {
            std::cout << "<pid>: " << searchingFile << " : " << entry << "\n";
        }
    }
    else
    {
    if (entry.find(searchingFile))
    {
        std::cout << "<pid>: " << searchingFile << " : " << entry << "\n";
    }
}
}
