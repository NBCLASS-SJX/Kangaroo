// =====================================================================================
//	Copyright (C) 2018 by Jiaxing Shao.All rights reserved.
//
//	文 件 名:  kgr_rsa.cpp
//	作    者:  Jiaxing Shao, 13315567369@163.com
//	版 本 号:  1.0
//	创建时间:  2018年12月06日 14时10分10秒
//	Compiler:  g++
//	描    述:  
// =====================================================================================

#include "rsa.h"

RSAEncrypt::RSAEncrypt()
{
	srand((unsigned long)time(NULL));
}

RSAEncrypt::~RSAEncrypt()
{

}

void RSAEncrypt::PrintRsaParme()
{
	std::cout << "---------------------------------\n" << std::endl;
	std::cout << "p=" << m_RsaKey.p << std::endl;
	std::cout << "q=" << m_RsaKey.q << std::endl;
	std::cout << "f=(p-1)*(q-1)=" << m_RsaKey.f << std::endl;
	std::cout << "n=p*q=" << m_RsaKey.n << std::endl;
	std::cout << "e=" << m_RsaKey.PubKey << std::endl;
	std::cout << "d=" << m_RsaKey.PriKey << std::endl;
	std::cout << "s=" << m_RsaKey.s << std::endl;
	std::cout << "---------------------------------\n" << std::endl;
}

void RSAEncrypt::RsaGetParme()
{
	m_RsaKey.p = RandomPrime(16);
	m_RsaKey.q = RandomPrime(16);

	m_RsaKey.n = m_RsaKey.p * m_RsaKey.q;
	m_RsaKey.f = (m_RsaKey.p - 1) * (m_RsaKey.q - 1);

	do {
		m_RsaKey.PubKey = GetRondomNum(65536);
		m_RsaKey.PubKey |= 1;
	} while (SteinGcd(m_RsaKey.PubKey, m_RsaKey.f) != 1);

	m_RsaKey.PriKey = EuclidGcd(m_RsaKey.PubKey, m_RsaKey.f);
	m_RsaKey.s = 0;
	unsigned long long t = m_RsaKey.n >> 1;
	while (t) {
		m_RsaKey.s++;	//s=log2(n)
		t >>= 1;
	}
	PrintRsaParme();
}

int RSAEncrypt::Encode(unsigned long long * dst, unsigned long long * src, const int & srclen)
{
	for (unsigned long i = 0; i < srclen; i++) {
		dst[i] = PowMod(src[i], m_RsaKey.PubKey, m_RsaKey.n);
	}
	return 0;
}

int RSAEncrypt::Decode(unsigned long long * dst, unsigned long long * src, const int & srclen)
{
	for (unsigned long i = 0; i < srclen; i++) {
		dst[i] = PowMod(src[i], m_RsaKey.PriKey, m_RsaKey.n);
	}
	return 0;
}

unsigned long long RSAEncrypt::RandomPrime(const int bits)
{
	unsigned long long base;
	do {
		base = ((unsigned long long)1 << (bits - 1));
		base += GetRondomNum(base);
		base |= 0x01;

	} while (!PrimeTest(base, RMTestTimes));
	return base;
}

unsigned long long RSAEncrypt::SteinGcd(unsigned long long &p, unsigned long long &q)
{
	unsigned long long    a = p > q ? p : q;
	unsigned long long    b = p < q ? p : q;
	unsigned long long    t, r = 1;
	if (p == q) {
		return p;
	} else {
		while ((!(a & 1)) && (!(b & 1))) {
			r <<= 1;
			a >>= 1;
			b >>= 1;
		}

		if (!(a & 1)) {
			t = a;
			a = b;
			b = t;
		}

		do {
			while (!(b & 1)) {
				b >>= 1;
			}

			if (b < a) {
				t = a;
				a = b;
				b = t;
			}  
			b = (b - a) >> 1;
		} while (b);
		return r * a;
	}
}

unsigned long long RSAEncrypt::EuclidGcd(unsigned long long &p, unsigned long long &q)
{
	unsigned long long m, e, i, j, x, y;
	long xx, yy;
	m = q; e = p; x = 0; y = 1; xx = 1; yy = 1;
	while (e) {
		i = m / e; j = m % e;
		m = e; e = j; j = y; y *= i;
		if (xx == yy) {
			if (x > y)
				y = x - y;
			else {
				y -= x;
				yy = 0;
			}
		} else {
			y += x;
			xx = 1 - xx;
			yy = 1 - yy;
		}        x = j;
	}
	if (xx == 0)
		x = q - x;
	return x;
}

bool RSAEncrypt::PrimeTest(unsigned long long &n, long loop)
{
	for (long i = 0; i < PrimeTableCount; i++) {
		if (n % PrimeTable[i] == 0) {
			return false;
		}
	}

	for (long i = 0; i < loop; i++) {
		if (!MillerRabin(n)) {
			return false;
		}
	}
	return true;
}

bool RSAEncrypt::MillerRabin(unsigned long long &n)
{
	unsigned long long m = n - 1;
	unsigned long long j = 0;
	
	while (!(m & 0x01)) {
		j += 1;
		m >>= 1;
	}

	unsigned long long b = (GetRondomNum(n - 3)) + 2;

	unsigned long long v = PowMod(b, m, n);
	if (v == 1) {
		return true;
	}
	unsigned long long i = 1;
	
	while (v != n - 1) {
		if (i == j) {
			return false;
		}
		unsigned long long xxx = 2;
		v = PowMod(v, xxx, n);
		++i;
	}
	return true;
}

unsigned long long RSAEncrypt::GetRondomNum(unsigned long long n)
{
	unsigned long long random = rand() * rand();
	return random % n;
}

unsigned long long RSAEncrypt::PowMod(unsigned long long &base, unsigned long long &pow, unsigned long long &n)
{
	unsigned long long tmpA = base;
	unsigned long long tmpB = pow;
	unsigned long long tmpC = 1;

	while (tmpB)
	{
		while (!(tmpB & 1)) {
			tmpB >>= 1;
			tmpA = ((tmpA % n) * (tmpA % n)) % n;	// a * a % n
		}
		tmpB--;
		tmpC = ((tmpA % n) * (tmpC % n)) % n;		// a * c % n
	}
	return tmpC;
}

void RSAEncrypt::PrimeFactorization(unsigned long long x)
{
	unsigned long long src = x;

	g_vecPrime.push_back(1);
	if (x < 2) {
		return;
	}

	while (src % 2 == 0) {
		g_vecPrime.push_back(2);
		src = src / 2;
	}
	unsigned long long n = src;
	unsigned long long temp = src;

	for (int i = 3; i * i <= n; i += 2) {
		if (src % i == 0) {
			src = src / i;
			temp = temp / i;
			n = n / i;
			g_vecPrime.push_back(i);
			i -= 2;
		} else {
			n = (temp / i) + 1;
		}
	}
	g_vecPrime.push_back(src);
}

