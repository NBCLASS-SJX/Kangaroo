// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  des.cpp
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年01月14日
//  Compiler:  g++
//  描    述:  
// =====================================================================================


#include "des.h"

void ByteToBinary(char ch, bool binary[8])
{
	for (int i = 0; i < 8; i++) {
		binary[i] = (ch >> (7 - i)) & 0x01;
	}
}

void KeyLeftShift(bool key56[56], unsigned int round)
{
	bool L28[28] = { 0 };
	bool R28[28] = { 0 };
	for (int i = 0; i < 28; i++) {
		L28[i] = key56[i];
		R28[i] = key56[i + 28];
	}

	for (int i = 0; i < 28; i++) {
		if ((i + KEY_SHIFT[round]) < 28) {
			key56[i] = L28[i + KEY_SHIFT[round]];
			key56[i + 28] = R28[i + KEY_SHIFT[round]];
		}
		else {
			key56[i] = L28[i + KEY_SHIFT[round] - 28];
			key56[i + 28] = R28[i + KEY_SHIFT[round] - 28];
		}
	}
}

void CreateSubKey(SecretKey *secretKey, char *pKey, int keyN)
{
	bool dec64[64] = { 0 };
	bool key64[64] = { 0 };
	bool key56[56] = { 0 };
	for (int i = 0; i < 8; i++) {
		ByteToBinary(pKey[i], dec64 + i * 8);
	}
	for (int i = 0; i < 64; i++) {
		key64[i] = dec64[63 - i];
	}
	for (int i = 0; i < 56; i++) {
		key56[i] = key64[64 - KEY_TABLE[i]];
	}
	for (int i = 0; i < 16; i++) {
		KeyLeftShift(key56, i);
		for (int j = 0; j < 48; j++) {
			secretKey->SubKey[keyN][i][j] = key56[COMPRESS_TABLE[j] - 1];
		}
	}
}

void F_Function(bool r32[32], bool subKey[48], bool out32[32])
{
	bool expandR[48] = { 0 };
	for (int i = 0; i < 48; i++) {
		expandR[i] = r32[EXTEND_TABLE[i] - 1];
		expandR[i] = expandR[i] ^ subKey[i];
	}
	bool t32[32] = { 0 };
	for (int i = 0; i < 8; i++) {
		int row = expandR[i * 6 + 0] * 2 + expandR[i * 6 + 5];
		int col = expandR[i * 6 + 4] + expandR[i * 6 + 3] * 2 + expandR[i * 6 + 2] * 4 + expandR[i * 6 + 1] * 8;
		int num = S_BOX_TABLE[i][row][col];
		t32[i * 4 + 0] = num & 0x08 ? true : false;
		t32[i * 4 + 1] = num & 0x04 ? true : false;
		t32[i * 4 + 2] = num & 0x02 ? true : false;
		t32[i * 4 + 3] = num & 0x01 ? true : false;
	}

	for (int i = 0; i < 32; i++) {
		out32[i] = t32[P_TABLE[i] - 1];
	}
}

void DESEncode64(SecretKey *secretKey, const char * pBuf, char * pDst, int keyN)
{
	bool dec64[64] = { 0 };
	bool tmp64[64] = { 0 };
	for (int i = 0; i < 8; i++) {
		ByteToBinary(pBuf[i], dec64 + i * 8);
	}
	for (int i = 0; i < 64; i++) {
		tmp64[i] = dec64[63 - i];
	}
	bool T64[64] = { 0 };
	for (int i = 0; i < 64; i++) {
		T64[i] = tmp64[64 - IP_TABLE[i]];
	}
	bool L32[32] = { 0 };
	bool R32[32] = { 0 };
	for (int i = 0; i < 32; i++) {
		L32[i] = T64[i];
		R32[i] = T64[i + 32];
	}
	for (int i = 0; i < 16; i++) {
		bool T32[32] = { 0 };
		F_Function(R32, secretKey->SubKey[keyN][i], T32);
		for (int j = 0; j < 32; j++) {
			T32[j] = L32[j] ^ T32[j];
			L32[j] = R32[j];
			R32[j] = T32[j];
		}
	}
	for (int i = 0; i < 32; i++) {
		T64[i] = R32[i];
		T64[i + 32] = L32[i];
	}
	bool bit64[64] = { 0 };
	for (int i = 0; i < 64; i++) {
		bit64[i] = T64[IPEND_TABLE[i] - 1];
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (bit64[i * 8 + j]) {
				pDst[i] |= ((int)pow(2, (7 - j)) & 0xFF);
			}
		}
	}
}

void DESDecode64(SecretKey *secretKey, const char * pBuf, char * pDst, int keyN)
{
	bool dec64[64] = { 0 };
	bool tmp64[64] = { 0 };
	for (int i = 0; i < 8; i++) {
		ByteToBinary(pBuf[i], dec64 + i * 8);
	}
	for (int i = 0; i < 64; i++) {
		tmp64[i] = dec64[63 - i];
	}
	bool T64[64] = { 0 };
	for (int i = 0; i < 64; i++) {
		T64[i] = tmp64[64 - IP_TABLE[i]];
	}
	bool L32[32] = { 0 };
	bool R32[32] = { 0 };
	for (int i = 0; i < 32; i++) {
		L32[i] = T64[i + 32];
		R32[i] = T64[i];
	}
	for (int i = 0; i < 16; i++) {
		bool T32[32] = { 0 };
		F_Function(L32, secretKey->SubKey[keyN][15 - i], T32);
		for (int j = 0; j < 32; j++) {
			T32[j] = R32[j] ^ T32[j];
			R32[j] = L32[j];
			L32[j] = T32[j];
		}
	}
	for (int i = 0; i < 32; i++) {
		T64[i] = L32[i];
		T64[i + 32] = R32[i];
	}
	bool bit64[64] = { 0 };
	for (int i = 0; i < 64; i++) {
		bit64[i] = T64[IPEND_TABLE[i] - 1];
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (bit64[i * 8 + j]) {
				pDst[i] |= ((int)pow(2, (7 - j)) & 0xFF);
			}
		}
	}
}

unsigned int DESEncode(SecretKey *secretKey, const char * pBuf, unsigned int nLen, char * pDst, int keyN)
{
	int nIndex = 0;
	int srcLen = nLen;
	int dstLen = 0;
	memset(pDst, 0, nLen);

	do {
		DESEncode64(secretKey, pBuf + nIndex, pDst + dstLen, keyN);
		nIndex += 8;
		srcLen -= 8;
		dstLen += 8;
	} while (srcLen > 0);
	return dstLen;
}

/* *
 * 
 * */
unsigned int DESDecode(SecretKey *secretKey, const char * pBuf, unsigned int nLen, char * pDst, int keyN)
{
	int nIndex = 0;
	int srcLen = nLen;
	int dstLen = 0;
	memset(pDst, 0, nLen);

	do {
		DESDecode64(secretKey, pBuf + nIndex, pDst + dstLen, keyN);
		nIndex += 8;
		srcLen -= 8;
		dstLen += 8;
	} while (srcLen > 0);
	return dstLen;
}

SecretKey *GetSecretKey(const char *pKey, int nLen, bool is3Des)
{
	SecretKey *secretKey = new SecretKey();
	char keyBuf[24] = { 0 };
	int keyLen = nLen >= 24 ? 24 : nLen;
	memcpy(keyBuf, pKey, keyLen);
	if (is3Des) {
		secretKey->Is3DES = true;
		for (int i = 0; i < 3; i++) {
			CreateSubKey(secretKey, keyBuf + i * 8, i);
		}
	} else {
		secretKey->Is3DES = false;
		CreateSubKey(secretKey, keyBuf, 0);
	}
	return secretKey;
}

int DesEncrypt(SecretKey *secretKey, const char *srcBuf, int srcLen, char *dstBuf, int dstMaxLen, PaddingType padType)
{
	if (secretKey == NULL) {
		return -1;
	}
	char *tmpBuf = NULL;
	int tmpLen = 0;
	int dstLen = 0;

	memset(dstBuf, 0, dstMaxLen);
	if (padType == ZeroPadding) {
		tmpLen = srcLen % 8 == 0 ? srcLen + 8 : (srcLen / 8 + 1) * 8;
		tmpBuf = (char*)malloc(tmpLen);
		memset(tmpBuf, 0, tmpLen);
		memcpy(tmpBuf, srcBuf, srcLen);
	} else if (padType == Pkcs5Padding) {
		return 0;
	} else if (padType == Pkcs7Padding) {
		return 0;
	}

	if (secretKey->Is3DES) {
		dstLen = DESEncode(secretKey, tmpBuf, tmpLen, dstBuf, 0);
		tmpLen = DESDecode(secretKey, dstBuf, dstLen, tmpBuf, 1);
		dstLen = DESEncode(secretKey, tmpBuf, tmpLen, dstBuf, 2);
	} else {
		dstLen = DESEncode(secretKey, tmpBuf, tmpLen, dstBuf, 0);
	}

	if (tmpBuf != NULL) {
		free(tmpBuf);
		tmpBuf = NULL;
	}
	return dstLen;
}

int DesDecrypt(SecretKey *secretKey, const char *srcBuf, int srcLen, char *dstBuf, int dstMaxLen, PaddingType padType)
{
	if (secretKey == NULL || srcLen % 8 != 0) {
		return -1;
	}
	char *tmpBuf = NULL;
	int tmpLen = 0;
	int dstLen = 0;

	memset(dstBuf, 0, dstMaxLen);
	tmpBuf = (char*)malloc(srcLen);
	memset(tmpBuf, 0, srcLen);
	memcpy(tmpBuf, srcBuf, srcLen);
	tmpLen = srcLen;
	if (secretKey->Is3DES) {
		dstLen = DESDecode(secretKey, tmpBuf, tmpLen, dstBuf, 2);
		tmpLen = DESEncode(secretKey, dstBuf, dstLen, tmpBuf, 1);
		dstLen = DESDecode(secretKey, tmpBuf, tmpLen, dstBuf, 0);
	} else {
		dstLen = DESDecode(secretKey, tmpBuf, tmpLen, dstBuf, 0);
	}

	if (tmpBuf != NULL) {
		free(tmpBuf);
		tmpBuf = NULL;
	}

	if (padType == ZeroPadding) {
		int offset = 0;
		for (int i = 0; i < 8; i++) {
			if (dstBuf[dstLen - 1 - i] == 0x00) {
				offset += 1;
			} else {
				break;
			}
		}
		dstLen -= offset;
	} else if (padType == Pkcs5Padding) {
	} else if (padType == Pkcs7Padding) {
	}
	return dstLen;
}

std::string DesEncryptBase64(SecretKey *secretKey, const char *pBuf, int nLen, PaddingType padType)
{
	std::string strMsg = "";
	char *dstBuf = NULL;
	int dstMaxLen = nLen + 9;
	dstBuf = (char*)malloc(dstMaxLen);
	memset(dstBuf, 0, dstMaxLen);
	int dstLen = DesEncrypt(secretKey, pBuf, nLen, dstBuf, dstMaxLen, padType);
	do {
		if (dstLen == -1) {
			break;
		}
		char *tmpBuf = (char*)malloc(dstLen * 2);
		memset(tmpBuf, 0, dstLen * 2);
		if (Base64Encode(dstBuf, dstLen, tmpBuf, dstLen * 2) != -1) {
			strMsg = tmpBuf;
		}
		if (tmpBuf != NULL) {
			free(tmpBuf);
			tmpBuf = NULL;
		}
	} while (0);
	return strMsg;
}

int DesDecryptBase64(SecretKey *secretKey, std::string &strMsg, char *pBuf, int nMaxLen, PaddingType padType)
{
	int tmpMaxLen = strMsg.length();
	char *tmpBuf = (char*)malloc(tmpMaxLen);
	int tmpLen = 0;
	int dstLen = 0;

	memset(tmpBuf, 0, tmpMaxLen);
	tmpLen = Base64Decode(strMsg.c_str(), strMsg.length(), tmpBuf, tmpMaxLen);
	if (tmpLen == -1) {
		return -1;
	}
	dstLen = DesDecrypt(secretKey, tmpBuf, tmpLen, pBuf, nMaxLen, padType);
	if (tmpBuf != NULL) {
		free(tmpBuf);
		tmpBuf = NULL;
	}
	return dstLen;
}

