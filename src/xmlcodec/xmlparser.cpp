// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  xmlparser.cpp
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年03月24日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include "xmlparser.h"

bool XmlParser::setXmlString(const char *strXml)
{
	if (doc.Parse(strXml) != XML_SUCCESS) {
		return false;
	}
	return true;
}

XMLElement *XmlParser::getRootElement()
{
	XMLElement *rootElement = doc.RootElement();
	return rootElement;
}

XMLElement *XmlParser::getChildElement(XMLElement *element, const char *childName)
{
	XMLElement *childElement = element->FirstChildElement(childName);
	return childElement;
}

std::string XmlParser::getContentText(XMLElement *element)
{
	std::string text;
	if (element->GetText()) {
		text = element->GetText();
	}
	return text;
}

std::string XmlParser::getAttribute(XMLElement *element, const char *attName)
{
	std::string text;
	if (element->Attribute(attName)) {
		text = element->Attribute(attName);
	}
	return text;
}
