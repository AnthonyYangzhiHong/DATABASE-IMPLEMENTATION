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
