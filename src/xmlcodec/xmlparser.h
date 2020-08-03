// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  xmlparser.h
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年03月24日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#ifndef _XMLParser_H_H_H
#define _XMLParser_H_H_H

#include <string>
#include "tinyxml2.h"
using namespace tinyxml2;

class XmlParser
{
public:
	bool setXmlString(const char *strXml);
public:
	XMLElement *getRootElement();
	XMLElement *getChildElement(XMLElement *element, const char *childName);
	std::string getContentText(XMLElement *element);
	std::string getAttribute(XMLElement *element, const char *attName);
protected:
	tinyxml2::XMLDocument doc;
};

#endif

