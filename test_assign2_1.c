int main(void){
	initStorageManager();
	testName="";

	testCreatingAndReadingDummyPages();
	testReadPage();
	testFIFO();
	testLRU();
}

void testCreatingAndReadingDummyPages(void){
	BM_BufferPool *bm=MAKE_POOL();
	testName="Creating and Reading Back Dummy Pages";
	CHECK(createPageFile("testbuffer.bin"));
	createDummyPages(bm,22);
	checkDummyPages(bm,20);
	createDummyPages(bm,10000);
	checkDummyPages(bm,10000);

	CHECK(destroyPageFile("testbuffer.bin"));

	free(bm);
	TEST_DONE();
}

void createDummyPages(BM_BufferPool *bm,int num){
	int i;
	BM_PageHandle *h=MAKE_PAGE_HANDLE();
	CHECK(initBufferPool(bm,"testbuffer.bin",3,RS_FIFO,NULL));
	for(i=0;i<num;i++)
	{
		CHECK(pinPage(bm,h,i));
		sprintf(h->data,"%s-%i","Page",h->pageNum);
		CHECK(markDirty(bm,h));
		CHECK(unpinPage(bm,h));
	}
	CHECK(shutdownBufferPool(bm));
	free(h);
}

void testFIFO(){
	const char *poolContents[]={
		"[0 0],[-1 0],[-1 0]",
		"[0 0],[1 0],[-1 0]",
		"[0 0],[1 0],[2 0]",
		"[3 0],[1 0],[2 0]",
		"[3 0],[4 0],[2 0]",
		"[3 0],[4 1],[2 0]",
		"[3 0],[4 1],[5x0]",
        "[6x0],[4 1],[5x0]",
        "[6x0],[4 1],[0x0]",
        "[6x0],[4 0],[0x0]",
        "[6 0],[4 0],[0 0]"
	};
	const int request[]={0,1,2,3,4,4,5,6,0};
	const int numLinRequests=5;
	const int numChangeRequests=3;
	
	int i;
	BM_BufferPool *bm=MAKE_POOL();
	BM_PageHandle *h=MAKE_PAGE_HANDLE();
	testName="Testing FIFO page replacement";
	CHECK(createPageFile("testbuffer.bin"));
	createDummyPages(bm,100);
	CHECK(initBufferPool(bm,"testbuffer.bin",3,RS_FIFO,NULL));
	
	for(i=0;i<numLinRequests;i++){
		pinPage(bm,h,request[i]);
		unpinPage(bm,h);
		ASSERT_EQUALS_POOL(poolContents[i],bm,"check pool content");
	}
	
	i=numLinRequests;
	pinPage(bm,h,requests[i]);
	ASSERT_EQUALS_POOL(poolContents[i],bm,"pool content after pin page");
	
	for(i=numLinRequests+1;i<numLinRequests+numChangeRequests+1;i++){
		pinPage(bm,h,requests[i]);
		markDirty(bm,h);
		unpinPage(bm,h);
		ASSERT_EQUALS_POOL(poolContents[i],bm,"check pool content");
	}
	
	i=numLinRequests+numChangeRequests+1;
	h->pageNum=4;
	unpinPage(bm,h);
	ASSERT_EQUALS_POOL(poolContents[i],bm,"unpin last page");
	
	i++;
	forceFlushPool(bm);
	ASSERT_EQUALS_POOL(poolContents[i],bm,"pool content after flush");
	
	ASSERT_EQUALS_INT(3,getNumWriteIO(bm),"check number of write I/Os");
	ASSERT_EQUALS_INT(8,getNumReadIO(bm),"check number of read I/Os");
	
	CHECK(shutdownBufferPool(bm));
	CHECK(destroyPageFile("testbuffer.bin"));
	
	free(bm);
	free(h);
	TEST_DONE();
}