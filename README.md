# cpp_project

Bug Encountered:

1. In BufMgr.h, it the hashtable is declared but not used, it will lead to segmentation fault(core dump)
2. In BufMgr.cpp, if before checking pincount and erase an record from hashmap, it will lead to strange behaviour that the code can pass 
      the test but finally it will fail due to unexpected database error in db.cpp 609, which is caused by the error when pin a page, which 
      is caused by in sufficient frame in the memory.
