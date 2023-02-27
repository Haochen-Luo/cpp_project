// #include "hash.h"
#include <unordered_map>
#include "../include/frame.h"
/**
 * Class to implement the buffer replacement policy.
 * There is only one main function to be implemented, PickVictim() which returns an integer corresponding to the frame to be
 * replaced. Remember, when a page in a frame is replaced, you need to write the page back to the disk if the page has been
 * modified. It is up to you to decide whether this is a replacer's responsibility or this has to be left to some other classes.
 *
 * Here we have defined a Clock replacer which should implement the clock replacement policy. Feel free to modify this interface,
 * or add any other replacement policy as you like
 *
 */
class Replacer 
{
	public :

		// virtual Replacer();
		// virtual ~Replacer();

		virtual int PickVictim() = 0;
};

class Clock : public Replacer
{
	private :
		
		int current;
		int numOfBuf;
		Frame **frames;
		//HashTable *hashTable;
		unordered_map<int,int> hashmap;
		
	public :
		
		Clock( int bufSize, Frame **frames, unordered_map<int,int>& hashmap);
		~Clock();
		int PickVictim();
};
 