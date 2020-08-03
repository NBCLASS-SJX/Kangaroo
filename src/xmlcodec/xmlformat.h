// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  xmlformat.h
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年01月14日
//  Compiler:  g++
//  描    述:  
// =====================================================================================


#ifndef _XMLFormat_H_H_H
#define _XMLFormat_H_H_H

#include <string>
#include "tinyxml2.h"
using namespace tinyxml2;

class XmlFormat
{
public:
	std::string getXmlString();
public:
	void addXmlDeclaration(const char *vesion, const char *encoding);
	XMLElement *addRootElement(const char *rootName);
	XMLElement *addChildElement(XMLElement *element, const char *childName);
	XMLElement *addChildElement(XMLElement *element, const char *childName, const char *context);
	XMLElement *addChildElement(XMLElement *element, const char *childName, int32_t context);
	XMLElement *addChildElement(XMLElement *element, const char *childName, int64_t context);
	XMLElement *addChildElement(XMLElement *element, const char *childName, uint32_t context);
	XMLElement *addChildElement(XMLElement *element, const char *childName, double context);
	XMLElement *addChildElement(XMLElement *element, const char *childName, float context);
	XMLElement *addChildElement(XMLElement *element, const char *childName, bool context);

	void addAttribute(XMLElement *element, const char *attrName, const char *attrValue);
	void addAttribute(XMLElement *element, const char *attrName, int32_t attrValue);
	void addAttribute(XMLElement *element, const char *attrName, int64_t attrValue);
	void addAttribute(XMLElement *element, const char *attrName, uint32_t attrValue);
	void addAttribute(XMLElement *element, const char *attrName, double attrValue);
	void addAttribute(XMLElement *element, const char *attrName, float attrValue);
	void addAttribute(XMLElement *element, const char *attrName, bool attrValue);
protected:
	tinyxml2::XMLDocument doc;
};

#endif

