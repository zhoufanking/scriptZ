/*
 * ssaproc.h
 *
 *  Created on: 2012-6-23
 *      Author: apple
 */

#ifndef SSAPROC_H_
#define SSAPROC_H_

#include <common.h>

int ssaGetText(unsigned char  **text, const char *buffer, const int len) {

	int textLen = len;
	unsigned char *pch;
	unsigned char flag[1];
	unsigned char * pbuffer = buffer;

	flag[0] = ',';

	int cnt = 0;

	while (textLen > 0 && cnt != 9) {
		if (memcmp(pbuffer, flag, sizeof(unsigned char)) == 0) {
			cnt++;
		}
		pbuffer++;
		textLen--;
	}

	*text = pbuffer;

	return textLen;
}

int ssaTrimFont(unsigned char **res, unsigned char *text, const int textLen) {
	unsigned char start[1], stop[1];
	unsigned char *pbegine, *pend;
	unsigned char *temp;

	temp = (unsigned char*) malloc(sizeof(unsigned char) * 1024);
	memset(temp, '\0', (sizeof(unsigned char))*1024);

	unsigned char *ptemp = temp;

	*res = temp;
	start[0] = '{';
	stop[0] = '}';

	pbegine = pend = text;


	int len = textLen;
	int totalLen = 0;
	while (len > 0) {

		while (memcmp(pend, start, sizeof(unsigned char)) != 0) {
			pend++;
			len--;

			if (len == 0)
				break;

		}

		memcpy(ptemp, pbegine, (pend - pbegine));
		totalLen += pend - pbegine;

		if (len == 0) {

			return totalLen;

		} else {

			ptemp += pend - pbegine;

		}
		while (memcmp(pend, stop, sizeof(unsigned char)) != 0) {

			pend++;
			len--;

		}

		pend++;
		pbegine = pend;
		len--;

	}

	return totalLen;

}

int ssaReadLine(FILE* fp, unsigned char *buffer) {

	unsigned char *pbuffer = NULL;

	pbuffer = buffer;

	memset(buffer, '\0', sizeof(unsigned char)*1024);

	int ch = 0;
	int len = 0;

	ch = fgetc(fp);

	if (ch == EOF)
		return 0;

	while (ch != '\n' && ch != EOF) {
		*pbuffer = ch;
		pbuffer++;
		len++;
		ch = fgetc(fp);
	}

	if (ch == EOF)

		return len;

	else {

		*pbuffer = ch;
		pbuffer++;

	}
	printf("in getLen: %s",buffer);

	return len;
}

void ssaReplace(unsigned char *text, int len){
	unsigned char *ptext;
	ptext = text;

	while(len > 0){
		if( *ptext == '\\'&& *(ptext+1) == 'N'){
			*ptext = '\r';
			*(ptext+1) = '\n';
		}

		ptext++;
		len--;
	}

}

int ssa_proc(const char *path) {
	FILE *fin = openFile(path, "r");
	FILE *fout;

	unsigned char *buffer = (unsigned char *) malloc(
			sizeof(unsigned char) * 1024);

	char *dstPath = genDstPath(path);

	fout = openFile(dstPath, "wb");

	int len;

	unsigned char tch;
	unsigned char *pstart, *pend;


	while ((len = ssaReadLine(fin, buffer)) > 0) {

		unsigned char flag[1];
		flag[0] = 'D';

		unsigned char *text;
		unsigned char *pureText;
		int pureTextLen = 0;

		printf("get a line %s",buffer);

		int res = 0;
		if (memcmp(buffer, flag, sizeof(unsigned char)) == 0) {
			// here we got a dialog

			int textLen = ssaGetText(&text, buffer, len);
			printf("text len is %d text is: %s\n",textLen ,text);
			if (textLen > 0) {
				pureTextLen = ssaTrimFont(&pureText, text, textLen);

				ssaReplace(pureText,pureTextLen);

				res = fwrite((void*) pureText, sizeof(unsigned char), pureTextLen/*/2*/,
						fout);
				free(pureText);
				pureText = NULL;

			}
		}

	} //while

	free(buffer);
	buffer = NULL;

	fclose(fin);
	fclose(fout);

	return 0;
}



#endif /* SSAPROC_H_ */
