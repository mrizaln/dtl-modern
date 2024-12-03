
#ifndef DTL_EXAMPLE_COMMON_H
#define DTL_EXAMPLE_COMMON_H

#include <string>
#include <cstdio>

using namespace std;

inline bool isFileExist(string& fs)
{
    FILE* fp;
    if ((fp = fopen(fs.c_str(), "r")) == NULL) {
        return false;
    }
    fclose(fp);
    return true;
}

inline bool isFewArgs(int argc, int limit = 3)
{
    bool ret = false;
    if (argc < limit) {
        ret = true;
    }
    return ret;
}

#endif
