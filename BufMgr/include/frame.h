#ifndef FRAME_H
#define FRAME_H

#include "page.h"

#define INVALID_FRAME -1

class Frame 
{
	private :
	
		PageID pid;
		Page   *data;
		int    pinCount;
		int    dirty;
		Bool referenced;

	public :
		
		Frame();
		~Frame();
		void Pin();
		void Unpin();
		void EmptyIt();
		void DirtyIt();
		void UnDirtyIt();
		void SetPageID(PageID pid);
		Bool IsDirty();
		Bool IsValid();
		Status Write();
		Status Read(PageID pid);
		Status Free();
		Bool NotPinned();
		Bool HasPageID(PageID pid);
		PageID GetPageID();
		Page *GetPage();
		int GetPinCount();
		void SetReferenced(bool i);
		Bool IsReferenced();
		Bool IsVictim();

};

#endif
