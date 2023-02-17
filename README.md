# cpp_project

## difference using new and without new 
- Use new

1. Allocates memory for the object on the free store (This is frequently the same thing as the heap)
2. Requires you to explicitly delete your object later. (If you don't delete it, you could create a memory leak)
3. Memory stays allocated until you delete it. (i.e. you could return an object that you created using new)

- Without new

1. Allocates memory for the object on the stack (where all local variables go) There is generally less memory available for the stack; if you allocate too many objects, you risk stack overflow.
2. You won't need to delete it later.
3. Memory is no longer allocated when it goes out of scope. (i.e. you shouldn't return a pointer to an object on the stack)

### when to use or not use
- If you don't want to worry about calling delete, (and the potential to cause memory leaks) you shouldn't use new.
- If you'd like to return a pointer to your object from a function, you must use new

https://stackoverflow.com/questions/655065/when-should-i-use-the-new-keyword-in-c
