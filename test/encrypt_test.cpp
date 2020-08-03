// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  encrypt_test.cpp
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年03月24日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include "../src/encrypt/des.h"
#include "../src/encrypt/md5.h"
#include "../src/encrypt/base64.h"
#include "../src/common/kgr_logger.h"


#include <iostream>
using namespace std;

void base64_test();
void des_test();
void md5_test();

int main(int argc, char *argv[])
{
	base64_test();
	des_test();
	md5_test();
	return 0;
}

void base64_test()
{
	log_debug("*********************** BASE64 ************************");
	char srcBuf[1024] = "abcdefghijklmnopqrstuvwxyz";
	char tmpBuf[2048] = { 0 };
	char dstBuf[1024] = { 0 };
	int srcLen = strlen(srcBuf);
	int tmpLen = 0;
	int dstLen = 0;

	log_debug("src:%s", srcBuf);
	tmpLen = Base64Encode(srcBuf, srcLen, tmpBuf, 2048);
	log_debug("tmp:%s", tmpBuf);
	dstLen = Base64Decode(tmpBuf, tmpLen, dstBuf, 1024);
	log_debug("dst:%s", dstBuf);
}

void des_test()
{
	log_debug("************************ DES **************************");
	char srcBuf[1024] = "123456789";
	char dstBuf[1024] = { 0 };
	char keyBuf[1024] = "zxht1234abcdefgh12345678";
	int srcLen = strlen(srcBuf);
	int dstLen = 0;
	int keyLen = strlen(keyBuf);

	SecretKey *secretKey = GetSecretKey(keyBuf, keyLen, true);
	log_debug("%d, %s", srcLen, srcBuf);
	string base64 = DesEncryptBase64(secretKey, srcBuf, srcLen, ZeroPadding);
	log_debug("%s", base64.c_str());
	dstLen = DesDecryptBase64(secretKey, base64, dstBuf, 1024, ZeroPadding);
	log_debug("%d, %s", dstLen, dstBuf);
}

void md5_test()
{
	log_debug("************************ MD5 **************************");
	char srcBuf[1024] = "123456789";
	int srcLen = strlen(srcBuf);
	std::string msg = MD5EncryptHex((unsigned char*)srcBuf, srcLen);
	log_debug("%s", msg.c_str());
}

