/*
 * assproc.h
 *
 *  Created on: 2012-6-23
 *      Author: apple
 */

#ifndef ASSPROC_H_
#define ASSPROC_H_

#include "common.h"

#pragma pack(1)
typedef struct {
	unsigned char byte0;
	unsigned char byte1;
} TCHAR;
#pragma pack()


int getText(TCHAR **text, const char *buffer, const int len) {

	int textLen = len;
	TCHAR *pch;
	TCHAR flag[1];
	unsigned char * pbuffer = buffer;

	flag->byte0 = ',';
	flag->byte1 = '\0';

	int cnt = 0;

	while (textLen > 0 && cnt != 9) {
		if (memcmp(pbuffer, flag, sizeof(TCHAR)) == 0) {
			cnt++;
		}
		pbuffer += 2;
		textLen -= 2;
	}

	*text = pbuffer;

	return textLen;
}


int trimFont(TCHAR **res, TCHAR *text, const int textLen) {
	TCHAR start[1], stop[1];
	TCHAR *pbegine, *pend;
	TCHAR *temp;

	temp = (TCHAR*) malloc(sizeof(TCHAR) * 1024);
	memset(temp, '\0', (sizeof(TCHAR))*1024);

	TCHAR *ptemp = temp;

	*res = temp;
	start->byte0 = '{';
	start->byte1 = '\0';

	stop->byte0 = '}';
	stop->byte1 = '\0';

	pbegine = pend = text;
	int len = textLen;
	int totalLen = 0;
	while (len > 0) {

		while (memcmp(pend, start, sizeof(TCHAR)) != 0) {
			pend++;
			len -= 2;

			if( pend->byte0 =='\\'&& (pend+1)->byte0=='N'){
				pend->byte0 ='\r';
				(pend+1)->byte0 = '\n';
			}


			if (len == 0)
				break;

		}

		memcpy(ptemp, pbegine, (pend - pbegine)*2);
		totalLen += pend - pbegine;

		if (len == 0) {

			return totalLen;

		} else {

			ptemp += pend - pbegine;

		}
		while (memcmp(pend, stop, sizeof(TCHAR)) != 0) {

			pend++;
			len -= 2;

		}

		pend++;
		pbegine = pend;
		len -= 2;

	}

	return totalLen;

}

int readLine(FILE* fp, unsigned char *buffer) {

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
		ch = fgetc(fp);
		*pbuffer = ch;
		len += 2;
	}
	return len;
}

int ass_proc(const char *path) {
	FILE *fin = openFile(path, "r");
	FILE *fout;

	unsigned char *buffer = (unsigned char *) malloc(
			sizeof(unsigned char) * 1024);
	char *dstPath = genDstPath(path);

	fout = openFile(dstPath, "wb");

	int len;

	TCHAR tch;
	TCHAR *pstart, *pend;
	TCHAR buff_ch[1024] = { 0, };
	TCHAR buff_en[1024] = { 0, };
	TCHAR HEAD[1];
	HEAD[0].byte0 = 0xff;
	HEAD[0].byte1 = 0xfe;
	fwrite(HEAD, sizeof(TCHAR), 1, fout);

	while ((len = readLine(fin, buffer)) > 0) {

		TCHAR flag[1];
		flag->byte0 = 'D';
		flag->byte1 = '\0';
		TCHAR *text;
		TCHAR *pureText;
		int pureTextLen = 0;

		int res = 0;
		if (memcmp(buffer, flag, sizeof(TCHAR)) == 0) {
			// here we got a dialog

			int textLen = getText(&text, buffer, len);
			if (textLen > 0) {
				pureTextLen = trimFont(&pureText, text, textLen);

				res = fwrite((void*) pureText, sizeof(TCHAR), pureTextLen/*/2*/,
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



#endif /* ASSPROC_H_ */
