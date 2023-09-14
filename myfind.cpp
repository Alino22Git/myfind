#include <stdio.h>
#include <vector>
#include <string>

int main(int argc, char *argv[])
{
    bool reksursive = false, caseSensitive = false;
    std::string searchPath;
    std::vector<std::string> files;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-R")
        {
            reksursive = true;
        }
        else if (arg == "-i")
        {
            caseSensitive = true;
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
    printf("Rekursive: %d\nCaseSensitive:%d\n",reksursive,caseSensitive);
    return 0;
}