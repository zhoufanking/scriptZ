#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "common.h"
#include "assproc.h"
#include "srtproc.h"
#include "ssaproc.h"

int main(int argc, char *argv[]) {

	if (argc != 2) {
		perror("NO INPUT FILE FOUND!");
		Usage();
		return -1;
	}

	char *FileType = NULL;

	FileType = getExtName(argv[1]);
	if(strcmp(FileType,"ass") == 0)
		ass_proc(argv[1]);

	if(strcmp(FileType,"srt") == 0)
		srt_proc(argv[1]);

	if(strcmp(FileType,"ssa") == 0)
		ssa_proc(argv[1]);

	puts("over\n");
	return 0;
}

