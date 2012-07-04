/*
 * srtproc.h
 *
 *  Created on: 2012-6-23
 *      Author: apple
 */
#include "common.h"
#include <sys/types.h>
#include <regex.h>

#ifndef SRTPROC_H_
#define SRTPROC_H_



unsigned char *ansiReadLine(FILE* fp) {
	// this is for the srt proc
	// don't foget to free the buffer returned by this routine
	unsigned char * buffer = NULL;
	unsigned char *pbuffer = NULL;
	buffer = (unsigned char *) malloc(sizeof(unsigned char) * 1024);
	pbuffer = buffer;

	memset(buffer, '\0', sizeof(unsigned char)*1024);

	int ch = 0;

	ch = fgetc(fp);
	if (ch == EOF)
		return NULL;

	while (ch != '\n') {
		*pbuffer = ch;
		pbuffer++;
		ch = fgetc(fp);
	}
	*pbuffer = ch;
	return buffer;
}


int srt_proc(const char * path) {

	int ch;
	int res;
	FILE *fin, *fout;

	unsigned char * buffer;

	char *extName;
	char *dstName;

	char pattern[] = "[0-1][0-9]:[0-5][0-9]";

	regex_t oRegex;

	if (NULL == path) {
		perror("NO INPUT FILE FOUND!");
		Usage();
		return -1;
	}

	dstName = genDstPath(path);

	fin = openFile(path, "r");
	fout = openFile(dstName, "w");

	free(dstName);
	dstName = NULL;

	while ((buffer = ansiReadLine(fin)) != NULL) {

		printf("buffer: %s", buffer);
		res = regcomp(&oRegex, pattern, 0);

		if (res != 0) {
			perror("compile regex failed!");
		}

		res = regexec(&oRegex, buffer, 0, NULL, 0);

		if (res == 0) {
			//skip this
			printf("buffer skipped\n");
			free(buffer);
			buffer = NULL;
		} else {
			printf("buffer saved\n");
			fprintf(fout, "%s", buffer);
			free(buffer);
			buffer = NULL;
		}
	}

	fclose(fin);
	fclose(fout);
	return 0;
}

#endif /* SRTPROC_H_ */
