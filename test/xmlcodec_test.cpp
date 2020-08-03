// =====================================================================================
//  Copyright (C) 2020 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  xmlcodec_test.cpp
//  作    者:  Jiaxing Shao, 13315567369@163.com
//  版 本 号:  1.0
//  创建时间:  2020年03月24日
//  Compiler:  g++
//  描    述:  
// =====================================================================================

#include "../src/xmlcodec/xmlformat.h"
#include "../src/xmlcodec/xmlparser.h"
#include "../src/common/kgr_logger.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	XmlFormat xml;
	xml.addXmlDeclaration("1.0", "GB2312");
	XMLElement *root = xml.addRootElement("message");
	xml.addAttribute(root, "System", "ATMS");
	xml.addAttribute(root, "Ver", "1.0");
	XMLElement *child = xml.addChildElement(root, "notice");
	xml.addAttribute(child, "abc", "example");
	XMLElement *subtype = xml.addChildElement(child, "subtype", "SiteDwell");
	XMLElement *child2 = xml.addChildElement(root, "control", "this is control");

	std::string strXml = xml.getXmlString();
	log_debug("\n%s", strXml.c_str());
	return 0;
}

