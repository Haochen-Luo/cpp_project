
#include "../include/bufmgr.h"
#include "../include/frame.h"

//--------------------------------------------------------------------
// Constructor for BufMgr
//
// Input   : bufSize  - number of pages in the this buffer manager
// Output  : None
// PostCond: All frames are empty.
//--------------------------------------------------------------------

BufMgr::BufMgr( int bufSize )
{
	
	//TODO: add your code here
	frames = new Frame*[bufSize];
	for(int i = 0;i<bufSize;i++){
		frames[i] = new Frame();
		
	}
	hashmap = new unordered_map<int,int>();
	numOfBuf = bufSize;
	// cout<<totalHit<<endl;
	totalHit = 0; 
	totalCall = 0; 
	numDirtyPageWrites = 0;
	/**
	 TODO: check if this is correct or not
	 not correct!!!!
	 why?
	  Clock cp(bufSize,frames,hashmap);
	  replacer = &cp
	  cp is deallocated after the end of the constructor of 
	 */
	
	replacer = new Clock(bufSize,frames,hashmap);
	// cout<<replacer<<"buf size"<<bufSize<<endl;  
	
}


//--------------------------------------------------------------------
// Destructor for BufMgr
//
// Input   : None
// Output  : None
//--------------------------------------------------------------------

BufMgr::~BufMgr()
{   
	FlushAllPages();
	for(int i = 0;i<numOfBuf;i++){
		delete frames[i];
	}
	delete[] frames;
	delete replacer;
	delete hashmap;
	//TODO: add your code here
}

//--------------------------------------------------------------------
// BufMgr::PinPage
//
// Input    : pid     - page id of a particular page 
//            isEmpty - (optional, default to false) if true indicate
//                      that the page to be pinned is an empty page.
// Output   : page - a pointer to a page in the buffer pool. (NULL
//            if fail)
// Purpose  : Pin the page with page id = pid to the buffer.  
//            Read the page from disk unless isEmpty is true or unless
//            the page is already in the buffer.
// Condition: Either the page is already in the buffer, or there is at
//            least one frame available in the buffer pool for the 
//            page.
// PostCond : The page with page id = pid resides in the buffer and 
//            is pinned. The number of pin on the page increase by
//            one.
// Return   : OK if operation is successful.  FAIL otherwise.
//--------------------------------------------------------------------


Status BufMgr::PinPage(PageID pid, Page*& page, bool isEmpty)
{
	//TODO: add your code here
	totalCall++;
	int fNo = FindFrame(pid);
	// cout<<"pin"<<pid<<endl;

	
	if(fNo!=INVALID_FRAME){
		cout<<"already in buffer"<<endl;
		totalHit++;
		frames[fNo]->Pin();
		page = frames[fNo]->GetPage(); // output from buffer
		// cout<<"show what is in hashmap"<<endl;
		// for(const auto& p:(*hashmap)){
		// 	if(frames[p.second]->GetPinCount()>1){
		// 		cout<<"****************"<<endl;
		// 	}
		// 	cout<<p.first<<" "<<p.second<<" "<<frames[p.second]->GetPinCount()<<endl;
		// }
		return OK;
	}else{
		// cout<<"try to find one"<<endl;
		// cout<<pid<<"not in  buffer"<<endl;
		// cout<<"why fail, show what is in hashmap"<<endl;
			// for(const auto& p:(*hashmap)){
				// cout<<p.first<<" "<<p.second<<endl;
			// }
		fNo = replacer->PickVictim();
		// cout<<"fNo-"<<fNo<<endl;
		if(fNo!=INVALID_FRAME){
			// cout<<"line96"<<endl;
			(*hashmap)[pid] = fNo;
			// cout<<"put"<<pid<<"in the buffer"<<endl;
			// cout<<"show what is in hashmap"<<endl;
			// for(const auto& p:(*hashmap)){
				// cout<<p.first<<" "<<p.second<<endl;
			// }
			// cout<<"line98"<<numOfBuf<<endl;
			frames[fNo]->SetPageID(pid);
			// cout<<"line100"<<endl;
		}else{
			// cout<<"fail to find a available page to pin"<<endl;
			page = NULL;
			return FAIL;
		}
		
		if(!isEmpty){	
			// cout<<"non empty"<<endl;
			/**
			 * TODO:
			 * 
			 */
			
			frames[fNo]->Read(pid);
			// Status s = MINIBASE_DB->ReadPage(pid,page);

			// if(s!=OK){
			// 	page = NULL;
			// 	return s;
			// }					
		}else{
			// cout<<"testing"<<endl;
			frames[fNo]->SetPageID(pid);
			// cout<<"actucally it is empty"<<endl;
		}
		// cout<<"***** fNo is pinned"<<fNo<<endl;
		page = frames[fNo]->GetPage();
		// cout<<page<<endl;
		frames[fNo]->Pin();
		/**TODO: CORRECTED BUG*/
		(*hashmap)[pid] = fNo;
		// for(const auto& p:(*hashmap)){
			// cout<<pid<<" "<<p.first<<" "<<p.second<<endl;
		// }

		
		return OK;
	}
	
} 

//--------------------------------------------------------------------
// BufMgr::UnpinPage
//
// Input    : pid     - page id of a particular page 
//            dirty   - indicate whether the page with page id = pid
//                      is dirty or not. (Optional, default to false)
// Output   : None
// Purpose  : Unpin the page with page id = pid in the buffer. Mark 
//            the page dirty if dirty is true.  
// Condition: The page is already in the buffer and is pinned.
// PostCond : The page is unpinned and the number of pin on the
//            page decrease by one. 
// Return   : OK if operation is successful.  FAIL otherwise.
//--------------------------------------------------------------------


Status BufMgr::UnpinPage(PageID pid, bool dirty)
{
	//TODO: add your code here
	// cout<<"unpin"<<pid<<endl;
	if(!(*hashmap).count(pid)){
		return FAIL;
	}
	if(frames[(*hashmap)[pid]]->NotPinned()){
		return FAIL;
	}
	int fNo = (*hashmap)[pid];
	if(dirty){
		frames[fNo]->DirtyIt();
	}
	if(frames[fNo]->GetPinCount()>1){
		cout<<"\n\n winnnnnnnnnnnnnnnn \n\n\n"<<frames[fNo]->GetPageID()<<" "<<frames[fNo]->GetPinCount()<<endl;
	}
	frames[fNo]->Unpin();
	if(frames[fNo]->NotPinned()){
		(*hashmap).erase(pid);
	}else{
		/*strange bug, if I call (*hashmap).erase(pid); directly without checking NotPinned(), it would lead to unexpected fail
		*** Unexpected error(s) logged, test failed:
		First error occurred: --> DB Manager[from the <<Unknown>>]: /home/scratch/teaching/dsi-ht12/minibase/trunk/minibase-complete/spacemgr/db.cpp:609

		Moreover, it would lead to the effect that the flushing all fail because page 4 is still pineed

		
		*/
		cout<<frames[fNo]->GetPageID()<<" "<<pid<<endl;
		cout<<"not erasing!"<<endl;
	}
	// (*hashmap).erase(pid);
	
	return OK;
}

//--------------------------------------------------------------------
// BufMgr::NewPage
//
// Input    : howMany - (optional, default to 1) how many pages to 
//                      allocate.
// Output   : firstPid  - the page id of the first page (as output by
//                   DB::AllocatePage) allocated.
//            firstPage - a pointer to the page in memory.
// Purpose  : Allocate howMany number of pages, and pin the first page
//            into the buffer. 
// Condition: howMany > 0 and there is at least one free buffer space
//            to hold a page.
// PostCond : The page with page id = pid is pinned into the buffer.
// Return   : OK if operation is successful.  FAIL otherwise.
// Note     : You can call DB::AllocatePage() to allocate a page.  
//            You should call DB:DeallocatePage() to deallocate the
//            pages you allocate if you failed to pin the page in the
//            buffer.
//--------------------------------------------------------------------


Status BufMgr::NewPage(PageID& firstPid, Page*& firstPage, int howMany)
{
	//TODO: add your code here
	if(howMany<=0){
		return FAIL;
	}
	Status s = MINIBASE_DB->AllocatePage(firstPid,howMany);
	if(s!=OK){
		cout<<"allocate fail!"<<endl;
		return FAIL;
	}
	// cout<<"allocate successfully!"<<endl;
	s = PinPage(firstPid,firstPage);
	if(s!=OK){ 
		cout<<"fail to pin"<<endl;
		MINIBASE_DB->DeallocatePage(firstPid,howMany);
		return FAIL;
	}else{
		// cout<<(*hashmap)[firstPid]<<endl;
		firstPage = frames[(*hashmap)[firstPid]]->GetPage();
		return OK;
	}
	
}

//--------------------------------------------------------------------
// BufMgr::FreePage
//
// Input    : pid     - page id of a particular page 
// Output   : None
// Purpose  : Free the memory allocated for the page with 
//            page id = pid  
// Condition: Either the page is already in the buffer and is pinned
//            no more than once, or the page is not in the buffer.
// PostCond : The page is unpinned, and the frame where it resides in
//            the buffer pool is freed.  Also the page is deallocated
//            from the database. 
// Return   : OK if operation is successful.  FAIL otherwise.
// Note     : You can call MINIBASE_DB->DeallocatePage(pid) to
//            deallocate a page.
//--------------------------------------------------------------------


Status BufMgr::FreePage(PageID pid)
{
	//TODO: add your code here
	
	if((*hashmap).count(pid)){ //check if in the buffer
		
		Frame* f = frames[(*hashmap)[pid]]; 
		if(f->GetPinCount()>1){
			return FAIL;
		}
		// f->SetReferenced(0);
		f->EmptyIt();
		hashmap->erase(pid);
	}
	Status s = MINIBASE_DB->DeallocatePage(pid);
	

	return OK;
}


//--------------------------------------------------------------------
// BufMgr::FlushPage
//
// Input    : pid  - page id of a particular page 
// Output   : None
// Purpose  : Flush the page with the given pid to disk.
// Condition: The page with page id = pid must be in the buffer,
//            and is not pinned. pid cannot be INVALID_PAGE.
// PostCond : The page with page id = pid is written to disk if it's dirty. 
//            The frame where the page resides is empty.
// Return   : OK if operation is successful.  FAIL otherwise.
//--------------------------------------------------------------------


Status BufMgr::FlushPage(PageID pid)
{
	//TODO: add your code here
	if((*hashmap).count(pid)&&pid!=INVALID_PAGE){   
		Frame* f = frames[(*hashmap)[pid]]; 
		if(f->NotPinned()){
			if(f->IsDirty()){
				Status s = f->Write();
				if(s!=OK){
					return FAIL;
				}
				numDirtyPageWrites++;
			}
			f->EmptyIt();
			(*hashmap).erase(pid);   
			return OK;
		}
	}
	return FAIL;
		
	
} 

//--------------------------------------------------------------------
// BufMgr::FlushAllPages
//
// Input    : None
// Output   : None
// Purpose  : Flush all pages in this buffer pool to disk.
// Condition: All pages in the buffer pool must not be pinned.
// PostCond : All dirty pages in the buffer pool are written to 
//            disk (even if some pages are pinned). All frames are empty.
// Return   : OK if operation is successful.  FAIL otherwise.
//--------------------------------------------------------------------

Status BufMgr::FlushAllPages()
{
	//TODO: add your code here
	for(int i =0;i<numOfBuf;i++){
		Frame* f = frames[i];
		if(!f->NotPinned()){
			
			cout<<"the page"<<f->GetPageID()<<" is still pined"<<endl;
			return FAIL;
		}
		if(f->IsDirty()){
			Status s = f->Write();
			if(s!=OK){
				return FAIL;
			}
			numDirtyPageWrites++;
		}

		f->EmptyIt();
	}
	(*hashmap).clear();
	return OK;
}


//--------------------------------------------------------------------
// BufMgr::GetNumOfUnpinnedFrames
//
// Input    : None
// Output   : None
// Purpose  : Find out how many unpinned locations are in the buffer
//            pool.
// Condition: None
// PostCond : None
// Return   : The number of unpinned buffers in the buffer pool.
//--------------------------------------------------------------------

unsigned int BufMgr::GetNumOfUnpinnedFrames()
{
	//TODO: add your code here
	int count = 0;
	for(int i =0;i<numOfBuf;i++){
		if(frames[i]->NotPinned()){
			count++;
		}
	}
	return count;
}

void  BufMgr::PrintStat() {
	cout<<"**Buffer Manager Statistics**"<<endl;
	cout<<"Number of Dirty Pages Written to Disk: "<<numDirtyPageWrites<<endl;
	cout<<"Number of Pin Page Requests: "<<totalCall<<endl;
	cout<<"Number of Pin Page Request Misses "<<totalCall-totalHit<<endl;
	cout<<"Number of Pin Page Request Misses "<<totalHit<<endl;
}

//--------------------------------------------------------------------
// BufMgr::FindFrame
//
// Input    : pid - a page id 
// Output   : None
// Purpose  : Look for the page in the buffer pool, return the frame
//            number if found.
// PreCond  : None
// PostCond : None
// Return   : the frame number if found. INVALID_FRAME otherwise.
//--------------------------------------------------------------------

int BufMgr::FindFrame( PageID pid )
{
	//TODO: add your code here
	if((*hashmap).count(pid)){
		return (*hashmap)[pid];
	}else{
		return INVALID_FRAME;
	}
	
}
