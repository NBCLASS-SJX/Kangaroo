// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  xmlformat.cpp
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年01月14日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include "xmlformat.h"

std::string XmlFormat::getXmlString()
{
	XMLPrinter printer;
	doc.Print(&printer);
	return printer.CStr();
}

void XmlFormat::addXmlDeclaration(const char *vesion, const char *encoding)
{
	char declaration[128] = { 0 };
#if defined(__GNUC__)
	sprintf(declaration, "<?xml version=\"%s\" encoding=\"%s\"?>", vesion, encoding);
#elif defined(_MSC_VER)
	sprintf_s(declaration, 128, "<?xml version=\"%s\" encoding=\"%s\"?>", vesion, encoding);
#endif
	doc.Parse(declaration);
}

XMLElement *XmlFormat::addRootElement(const char *rootName)
{
	XMLElement* element = doc.NewElement(rootName);
	doc.InsertEndChild(element);
	return element;
}

XMLElement *XmlFormat::addChildElement(XMLElement *element, const char *childName)
{
	XMLElement* childElement = doc.NewElement(childName);
	element->InsertEndChild(childElement);
	return childElement;
}

XMLElement *XmlFormat::addChildElement(XMLElement *element, const char *childName, const char *context)
{
	XMLElement* childElement = doc.NewElement(childName);
	element->InsertEndChild(childElement);
	childElement->SetText(context);
	return childElement;
}

XMLElement *XmlFormat::addChildElement(XMLElement *element, const char *childName, int32_t context)
{
	XMLElement* childElement = doc.NewElement(childName);
	element->InsertEndChild(childElement);
	childElement->SetText(context);
	return childElement;
}

XMLElement *XmlFormat::addChildElement(XMLElement *element, const char *childName, int64_t context)
{
	XMLElement* childElement = doc.NewElement(childName);
	element->InsertEndChild(childElement);
	childElement->SetText(context);
	return childElement;
}

XMLElement *XmlFormat::addChildElement(XMLElement *element, const char *childName, uint32_t context)
{
	XMLElement* childElement = doc.NewElement(childName);
	element->InsertEndChild(childElement);
	childElement->SetText(context);
	return childElement;
}

XMLElement *XmlFormat::addChildElement(XMLElement *element, const char *childName, double context)
{
	XMLElement* childElement = doc.NewElement(childName);
	element->InsertEndChild(childElement);
	childElement->SetText(context);
	return childElement;
}

XMLElement *XmlFormat::addChildElement(XMLElement *element, const char *childName, float context)
{
	XMLElement* childElement = doc.NewElement(childName);
	element->InsertEndChild(childElement);
	childElement->SetText(context);
	return childElement;
}
	
XMLElement *XmlFormat::addChildElement(XMLElement *element, const char *childName, bool context)
{
	XMLElement* childElement = doc.NewElement(childName);
	element->InsertEndChild(childElement);
	childElement->SetText(context);
	return childElement;
}

void XmlFormat::addAttribute(XMLElement *element, const char *attrName, const char *attrValue)
{
	element->SetAttribute(attrName, attrValue);
}

	
void XmlFormat::addAttribute(XMLElement *element, const char *attrName, int32_t attrValue)
{
	element->SetAttribute(attrName, attrValue);
}

void XmlFormat::addAttribute(XMLElement *element, const char *attrName, int64_t attrValue)
{
	element->SetAttribute(attrName, attrValue);
}

void XmlFormat::addAttribute(XMLElement *element, const char *attrName, uint32_t attrValue)
{
	element->SetAttribute(attrName, attrValue);
}

void XmlFormat::addAttribute(XMLElement *element, const char *attrName, double attrValue)
{
	element->SetAttribute(attrName, attrValue);
}

void XmlFormat::addAttribute(XMLElement *element, const char *attrName, float attrValue)
{
	element->SetAttribute(attrName, attrValue);
}

void XmlFormat::addAttribute(XMLElement *element, const char *attrName, bool attrValue)
{
	element->SetAttribute(attrName, attrValue);
}

