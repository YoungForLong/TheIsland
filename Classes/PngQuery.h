#pragma once

#include"cocos2d.h"
#include"rapidxml.hpp"
#include"rapidxml_iterators.hpp"
#include"rapidxml_print.hpp"
#include"rapidxml_utils.hpp"

using namespace cocos2d;

class PngQuery
{
public:
	PngQuery()
	{
		
	}
	~PngQuery()
	{

	}
	std::string inWhichPng(std::string str)
	{
		rapidxml::xml_document<> _doc;
		rapidxml::file<> _docl(FileUtils::getInstance()->fullPathForFilename("landLink.xml").c_str());
		_doc.parse<0>(_docl.data());
		rapidxml::xml_node<>* node_pngLink = _doc.first_node("pngLink");
		rapidxml::xml_node<>* node_plist = node_pngLink->first_node("plist");

		while (node_plist)
		{
			rapidxml::xml_node<>* node_first_sibling = node_plist->first_node("fileName");
			while (node_first_sibling)
			{
				if (node_first_sibling->value() == str)
				{
					_spng = node_plist->first_attribute("pngFile")->value();
					return _spng;
				}
				else
				{
					node_first_sibling = node_first_sibling->next_sibling("fileName");
				}
			}
			node_plist = node_plist->next_sibling("plist");//define this to "++"
		}

		log("ERROR_NOT_FOUND_PNG_IN_PLIST");

		return NULL;
	}

	std::string inWhichPlist(std::string str)
	{
		rapidxml::xml_document<> _doc;
		rapidxml::file<> _docl(FileUtils::getInstance()->fullPathForFilename("landLink.xml").c_str());
		_doc.parse<0>(_docl.data());
		rapidxml::xml_node<>* node_pngLink = _doc.first_node("pngLink");
		rapidxml::xml_node<>* node_plist = node_pngLink->first_node("plist");

		while (node_plist)
		{
			rapidxml::xml_node<>* node_first_sibling = node_plist->first_node("fileName");
			while (node_first_sibling)
			{
				if (node_first_sibling->value() == str)
				{
					_splist = node_plist->first_attribute("plistFile")->value();
					return _splist;
				}
				else
				{
					node_first_sibling = node_first_sibling->next_sibling("fileName");
				}
			}
			node_plist = node_plist->next_sibling("plist");//define this to "++"
		}

		log("ERROR_NOT_FOUND_PNG_IN_PLIST");

		return NULL;
	}

private:
	std::string _spng;
	std::string _splist;
};


