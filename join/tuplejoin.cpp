#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/minirel.h"
#include "../include/heapfile.h"
#include "../include/scan.h"
#include "../include/join.h"
#include "../include/relation.h"
#include "../include/bufmgr.h"


//---------------------------------------------------------------
// Each join method takes in at least two parameters :
// - specOfS
// - specOfR
//
// They specify which relations we are going to join, which 
// attributes we are going to join on, the offsets of the 
// attributes etc.  specOfS specifies the inner relation while
// specOfR specifies the outer one.
//
//You can use MakeNewRecord() to create the new result record.
//
// Remember to clean up before exiting by "delete"ing any pointers
// that you "new"ed.  This includes any Scan/BTreeFileScan that 
// you have opened.
//---------------------------------------------------------------



void TupleNestedLoopJoin(JoinSpec specOfR, JoinSpec specOfS, long& pinRequests, long& pinMisses, double& duration)
{

	Status s;
	time_t start,end;
	time(&start);
    // above init a time wrapper, core code begin

	HeapFile* result;
	result = new HeapFile(NULL, s); // create a temp HeapFile
	if (s != OK){
		cerr << "Cannot create new file for storing result\n";
	}
	HeapFile* R = specOfR.file; 
	HeapFile* S = specOfS.file; 
	int r_len = specOfR.recLen;
	int s_len = specOfS.recLen;
	int join_len = r_len+s_len;
	
	
	char* rRecord = new char[r_len];
	
	char* sRecord = new char[s_len];
	
	int r_offset = specOfR.offset/sizeof(int);
	int r_att = specOfR.numOfAttr;
	int s_offset = specOfS.offset/sizeof(int);
	int s_att = specOfS.numOfAttr;
	RecordID r_rid;
	// cout<<"<<-"<<r_rid.pageNo<<" "<<r_rid.slotNo<<" endl"<<endl;
	char* newRecord = new char[join_len]; // otherwise sgement fault
	RecordID join_rid;
	Scan *r_scan;
	r_scan = R->OpenScan(s);
	if (s != OK)
	{
		cerr << "ERROR : cannot open scan on the heapfile to sort.\n";
	}
	
	while (r_scan->GetNext(r_rid, rRecord, r_len) == OK)
	{	
		
		int* r_arr = (int*)(rRecord);
		
		// cout<<"==="<<r_rid.pageNo<<" "<<r_rid.slotNo<<"searching "<<r_arr[r_offset]<<"==="<<endl;
		
		Scan *s_scan;
		s_scan = S->OpenScan(s);
		RecordID s_rid;
		if (s != OK)
		{
			cerr << "ERROR : cannot open scan on the heapfile to sort.\n";
		}
		int count = 0;
		while (s_scan->GetNext(s_rid, sRecord, s_len) == OK){
			int* s_arr = (int*)(sRecord);			
			if(r_arr[r_offset]==s_arr[s_offset]){
				// cout<<"==="<<s_rid.pageNo<<" "<<s_rid.slotNo<<"searching "<<s_arr[s_offset]<<"==="<<endl;
				MakeNewRecord(newRecord,rRecord,sRecord,r_len,s_len);
				result->InsertRecord(newRecord, join_len,join_rid) ;
				// cout<<" "<<join_rid.pageNo<<" "<<join_rid.slotNo<<" "<<endl;				
				/**check the new record is created correctly
				int* jarr = (int*) newRecord;
				for(int i =0;i<r_att+s_att;i++){cout<<jarr[i]<<" ";}
				*/
			}
			
		}


	}
	delete[] newRecord;	
	delete[] rRecord;
	delete[] sRecord;

	// r_scan->GetNext(r_rid, rRecord, r_len); if continue there would be still the same record
	
	// delete r_scan;


	// while (btreeScan->GetNext(rid, &key) == OK)
	// {
	//     S->GetRecord (rid, recPtr, recLen);
	    
	// }

	
	
	// insert(recPtr + offset, rid);

	
	// S->InsertRecord(newRecord,join_len,rid);
    // // open a scan
    // // Status s;
	// // Scan *scan;
	// // scan = S->OpenScan(s);
	// // if (s != OK)
	// // {
	// // 	cerr << "ERROR : cannot open scan on the heapfile to sort.\n";
	// // }

	// delete 

	
	
	// end of time wrapper
	time(&end);
	duration = difftime (end,start);
	printf("Elasped time is %.2lf seconds.", duration);

}
