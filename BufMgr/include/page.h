#ifndef PAGE_H
#define PAGE_H

#include "minirel.h"
#include <cstring>

const PageID INVALID_PAGE = -1;
const int MAX_SPACE = MINIBASE_PAGESIZE;


class Page
{
public:
    Page(){ 
        data = new char[MAX_SPACE];
        std::memset(data,0,MAX_SPACE);
    };
    ~Page(){delete[] data;};
    inline char* getPageData(){return data;};


private:
    char* data;
};

#endif
