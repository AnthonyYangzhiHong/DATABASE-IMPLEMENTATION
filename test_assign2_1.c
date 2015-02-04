main(void){
	initStorageManager();
	testName="";
	
	testCreatingAndReadingDummyPages();
	testReadPage();
	testFIFO();
	testLRU();
}

testCreatingAndReadingDummyPages(void){
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