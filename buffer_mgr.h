

typedef enum ReplacementStrategy{
	RS_FIFO=0,
	RS_LRU=1,
	RS_CLOCK=2,
	RS_LFU=3,
	RS_LRU_K=4
}ReplacementStrategy;

typedef int PageNumber;

typedef struct BM_BufferPool{
	char *pageFile;
	int numPages;
	ReplacementStrategy strategy;
	void *mgmtData;
}BM_BufferPool;

typedef struct BM_PageHandle{
	PageNumber pageNum;
	char *data;
}BM_PageHandle;

#define MAKE_POOL()    \
((BM_BufferPool *)malloc(sizeof(BM_BufferPool)))

#define MAKE_PAGE_HANDLE()    \
((BM_PageHandle *)malloc(sizeof(BM_PageHandle)))