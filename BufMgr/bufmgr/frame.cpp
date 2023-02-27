#include "../include/frame.h"
#include "../include/db.h"
/**
 * @brief Construct a new Frame:: Frame object
    	PageID pid;
		Page   *data;
		int    pinCount;
		int    dirty;
		Bool referenced;
 */
Frame::Frame(){
    pid = INVALID_PAGE;	 
	pinCount=0;
    dirty = 0;
	referenced = false;
    data = new Page();
    // memset(data, 0, MAX_SPACE);
}
		
Frame::~Frame(){
    delete data;
    
}
void Frame::Pin(){
    pinCount++;
};
void Frame::Unpin(){
    if(pinCount>0){
        pinCount--;
    }
};
void Frame::EmptyIt(){
    pinCount=0; 
    char* pageData = data->getPageData();
    pid = INVALID_PAGE;	 
    dirty = 0;
	referenced = false;
    memset(pageData, 0, MAX_SPACE);
};
void Frame::DirtyIt(){
    dirty = 1;

};
void Frame::UnDirtyIt(){
    dirty = 0;
};

void Frame::SetPageID(PageID pageNo){
    pid = pageNo;
};
Bool Frame::IsDirty(){
    return dirty;
};
Bool Frame::IsValid(){
    return pid==INVALID_PAGE;
};
Status Frame::Write(){
    Status s = MINIBASE_DB->WritePage(pid,data);
    if(s!=OK){
        cout<<"warning, fail to write by Write() function in Frame"<<endl;
    }
    return s;
};
Status Frame::Read(PageID pid){
    Status s = MINIBASE_DB->ReadPage(pid,data);
    return s;
};
// Status Frame::Free(){

// };
Bool Frame::NotPinned(){
    return pinCount==0;
};  
Bool Frame::HasPageID(PageID pageNo){
    return pid==pageNo;
};
PageID Frame::GetPageID(){
    return pid;
};
Page* Frame::GetPage(){
    return data;
};

void Frame::SetReferenced(bool i){
    referenced=i;
}
Bool Frame::IsReferenced(){
    return referenced;
}
int Frame::GetPinCount(){
    return pinCount;
}
// Bool Frame::IsVictim(){

// }
