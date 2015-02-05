RC initBufferPool(BM_BufferPool *const bm,const char *const pageFileName,const int numPages,ReplacementStrategy strategy,void *stratData)
{
	int fd,i=0;
	if((fd=open(pageFileName,O_RDWR))==-1)
		return RC_FILE_NOT_FOUND;
	
	if(curPools>=MAX_POOLS)
		return FALSE;
	
	record[curPools].numRead=0;
	record[curPools].numWrite=0;
	record[curPools].freeAccess=TRUE;
	record[curPools].rPos=0;
	
	BufferFrame *bf.*tempBf;
	(*bm).pageFileName=pageFileName;
	(*bm).numPages=numPages;
	(*bm).strategy=strategy;
	
	bf=(BufferFrame*)malloc(sizeof(BufferFrame));
	bf->fixcount=0;
	bf->dirty=FALSE;
	bf->recordPos=curPools;
	bf->ph.pageNum=NO_PAGE;
	bf->ph.data=(char *)malloc(PAGE_SIZE * sizeof(char));
	bf->next=NULL;
	
	(*bm).mgmtData=bf;
	
	for(i=0;i<numPages-1;i++)
	{
		tempBf=(BufferFrame*)malloc(sizeof(BufferFrame));
		tempBf->fixcount=0;
		tempBf->dirty=FALSE;
		tempBf->recordPos=curPools;
		tempBf->ph.pageNum=NO_PAGE;
		tempBf->ph.data=(char *)malloc(PAGE_SIZE * sizeof(char));
		tempBf->next=NULL;
		
		bf->next=tempBf;
		bf=bf->next;
	}
	close(fd);
	curPools++;
	return RC_OK;
}