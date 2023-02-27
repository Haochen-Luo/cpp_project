#include "../include/replacer.h"
#include "../include/frame.h"

int Clock::PickVictim(){
    int count = 0;
    // cout<<"hello"<<endl;
    
    while(count++<numOfBuf){
        cout<<count<<endl;
        current = (current+1)%numOfBuf;
        if(!frames[current]->IsReferenced()&&frames[current]->GetPinCount()==0){
            if(frames[current]->IsDirty()){
                frames[current]->Write();
            }
            cout<<!frames[current]->IsReferenced()<<frames[current]->GetPinCount()<<endl;
            return current;
        }

        if(frames[current]->IsReferenced()){
            frames[current]->SetReferenced(0);
        }
    }
    return INVALID_FRAME; //If it is not found
    
}

Clock::Clock( int _bufSize, Frame **_frames, unordered_map<int,int>& _hashmap){
    cout<<"init clock replacer"<<endl;
    current = -1;
    frames = _frames;
    numOfBuf = _bufSize;
    hashmap = _hashmap;
}
Clock::~Clock(){
    
}
