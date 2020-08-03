// =====================================================================================
//	Copyright (C) 2018 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  rsa.h
//	作    者:  Jiaxing Shao, 13315567369@163.com
//	版 本 号:  1.0
//	创建时间:  2018年12月06日 14时10分10秒
//	Compiler:  g++
//	描    述:  
// =====================================================================================

#ifndef _RSA_ENCRYPT_H_H_H
#define _RSA_ENCRYPT_H_H_H

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#define PrimeTableCount 24
#define RMTestTimes		30

typedef struct {
	unsigned long long p;
	unsigned long long q;
	unsigned long long f;		//f=(p-1)*(q-1)
	unsigned long long n;
	unsigned long long PriKey;
	unsigned long long PubKey;
	unsigned long long s;
} stuRSA_PARAM;

const static long PrimeTable[PrimeTableCount] =
{
	3 ,5 ,7 ,11,13,17,
	19,23,29,31,37,41,
	43,47,53,59,61,67,
	71,73,79,83,89,97
};

class RSAEncrypt
{
public:
	RSAEncrypt();
	~RSAEncrypt();
	void RsaGetParme();
	unsigned long long GetPubKey() {
		return m_RsaKey.PubKey;
	}
	int Encode(unsigned long long * dst, unsigned long long * src, const int &srclen);
	int Decode(unsigned long long * dst, unsigned long long * src, const int &srclen);
private:
	unsigned long long RandomPrime(const int bits);
	bool PrimeTest(unsigned long long &n, long loop);
	bool MillerRabin(unsigned long long &n);
	unsigned long long PowMod(unsigned long long &base, unsigned long long &pow, unsigned long long &n);
	unsigned long long SteinGcd(unsigned long long &p, unsigned long long &q);
	unsigned long long EuclidGcd(unsigned long long &p, unsigned long long &q);

	unsigned long long GetRondomNum(unsigned long long n);
	void PrintRsaParme();
	void PrimeFactorization(unsigned long long x);
private:
	stuRSA_PARAM m_RsaKey;
	std::vector<unsigned long long> g_vecPrime;
};


#endif

