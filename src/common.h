/*
 * common.h
 *
 *  Created on: 2012-6-23
 *      Author: apple
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *genDstPath(const char *path) {

	if (NULL == path) {
		printf("%s,%s,%d:%s:\n", __FILE__, "genDstPath", __LINE__,
				"bad input parameter!");
		return NULL;
	}

	char *dstFileName;

	char *pch;

	dstFileName = (char *) malloc(sizeof(char) * 255);
	memset(dstFileName, '\0', sizeof(dstFileName));

	pch = strrchr(path, '.');

	memcpy(dstFileName, path, pch-path);
	strcat(dstFileName, ".txt");

	return dstFileName;
}

// get the src file extension name
char *getExtName(const char *path) {

	char *ext ;
	char *pch;

	ext = (char *) malloc(sizeof(char) * 16);
	memset(ext, '\0', sizeof(char)*16);

	pch = strrchr(path, '.');

	memcpy(ext, pch+1, strlen(path)-(pch-path));

	return ext;
}

FILE *openFile(const char *path, const char* opt) {
	if (NULL == path || NULL == opt) {

		printf("%s,%s,%d:%s:\n", __FILE__, "openFile", __LINE__,
				"bad input parameter!");
		return NULL;

	}

	FILE *fp;

	fp = fopen(path, opt);

	if (NULL == fp) {

		printf("%s,%s,%d,%s\n", __FILE__, "openFile", __LINE__,
				"Failed to open the srt file!");
		perror(path);
		return NULL;

	}

	return fp;
}



void Usage() {

	printf("Transfer srt_file_name\n");
}

#endif /* COMMON_H_ */
