#include"VirtualMap.h"
#include"Animal.h"
#include"Plant.h"
#include"Crocodile.h"
#include"PngQuery.h"

VirtualMap::VirtualMap():
VirtualEntity(VIRTUAL_MAP_ID,"virtualMap")
{
	//创建英雄
}

VirtualMap::~VirtualMap()
{

}

//bool VirtualMap::loadLandIndex()
//{
//	加载地图数据
//	rapidxml::xml_document<> doc_;
//	rapidxml::file<> docl_(FileUtils::getInstance()->fullPathForFilename("land.xml").c_str());
//	doc_.parse<0>(docl_.data());
//
//	if (!doc_.first_node())
//	{
//		log("ERROR_LAND_XML_NULL");
//		return false;
//	}
//	else
//	{
//		rapidxml::xml_node<>*  node_root = doc_.first_node("ROOT");//获取根节点
//		rapidxml::xml_node<>* node_land_first = node_root->first_node("land");//指向第一个元素
//		
//		int count = 0;
//		while (node_land_first)
//		{
//			auto land = Land::create();
//			rapidxml::xml_node<>* first_sibling = node_land_first->first_node("landType");
//
//			land->_landType = (LandType)atoi(first_sibling->value());//字符串转换int
//
//			first_sibling = first_sibling->next_sibling("positionX");
//			land->_imgPosition.x = atoi(first_sibling->value());
//
//			first_sibling = first_sibling->next_sibling("positionY");
//			land->_imgPosition.y = atoi(first_sibling->value());
//
//			first_sibling = first_sibling->next_sibling("sprite");
//			land->_imgName = first_sibling->value();
//
//			first_sibling = first_sibling->next_sibling("width");
//			land->_contentSize.x = atoi(first_sibling->value());
//
//			first_sibling = first_sibling->next_sibling("height");
//			land->_contentSize.y = atoi(first_sibling->value());
//
//			land->_landSprite = Sprite::createWithSpriteFrameName(land->_imgName);
//			end
//			_land.pushBack(land);
//			count++;
//			node_land_first = node_land_first->next_sibling("land");
//			移动指针到下一个节点
//		}
//	}
//	加载完毕
//
//	处理像素信息
//	int count = 0;
//	while (count<_land.size())
//	{
//		auto img = new Image;
//		auto path = FileUtils::getInstance()->fullPathForFilename("land_0.png");
//		img->initWithImageFile(path);
//		int w = _land.at(count)->_contentSize.x / RECT_SIZE;
//		int h = _land.at(count)->_contentSize.y / RECT_SIZE;
//		int x = _land.at(count)->_imgPosition.x / RECT_SIZE;
//		int y = _land.at(count)->_imgPosition.y / RECT_SIZE;
//
//		for (int a = 0; a < w; a++)
//		{
//			for (int b = 0; b < h; b++)
//			{
//				if (getAlpha(Point(a*RECT_SIZE, b*RECT_SIZE),img)>0)//>20则为图片非透明内容
//				{
//					globalMap->setLandType(a + x, b + y, _land.at(count)->_landType);
//				}
//			}
//		}
//		count++;
//	}
//	return true;
//}

bool VirtualMap::loadPlantIndex()
{
	rapidxml::xml_document<> doc_;
	rapidxml::file<> docl_(FileUtils::getInstance()->fullPathForFilename("plant.xml").c_str());
	doc_.parse<0>(docl_.data());

	if (!doc_.first_node())
	{
		log("ERROR_LAND_XML_NULL");
		return false;
	}
	else
	{
		rapidxml::xml_node<>* node_root = doc_.first_node("ROOT");//获取根节点
		rapidxml::xml_node<>* node_land_first = node_root->first_node("plant");//指向第一个元素
		int count = 0;
		while (node_land_first)
		{

			rapidxml::xml_node<>* first_sibling = node_land_first->first_node("plantType");

			auto pp = Plant::create();//字符串转换int

			first_sibling = first_sibling->next_sibling("positionX");
			pp->position().x = atoi(first_sibling->value());

			first_sibling = first_sibling->next_sibling("positionY");
			pp->position().y = atoi(first_sibling->value());

			count++;
			node_land_first = node_land_first->next_sibling("plant");
			//移动指针到下一个节点
		}
	}
	return true;
}

bool VirtualMap::loadAnimalIndex()
{
	rapidxml::xml_document<> doc_;
	rapidxml::file<> docl_(FileUtils::getInstance()->fullPathForFilename("animal.xml").c_str());
	doc_.parse<0>(docl_.data());

	if (!doc_.first_node())
	{
		log("ERROR_LAND_XML_NULL");
		return false;
	}
	else
	{
		rapidxml::xml_node<>* node_root = doc_.first_node("ROOT");//获取根节点
		rapidxml::xml_node<>* node_land_first = node_root->first_node("animal");//指向第一个元素
		int count = 0;
		while (node_land_first)
		{

			rapidxml::xml_node<>* first_sibling = node_land_first->first_node("animalType");

			auto pp = Crocodile::create();//字符串转换int

			first_sibling = first_sibling->next_sibling("positionX");
			pp->position().x = atoi(first_sibling->value()) / RECT_SIZE;

			first_sibling = first_sibling->next_sibling("positionY");
			pp->position().y = atoi(first_sibling->value()) / RECT_SIZE;

			count++;
			node_land_first = node_land_first->next_sibling("animal");
			//移动指针到下一个节点
		}
	}
	return true;
}

int VirtualMap::getAlpha(Point position,Image* img)
{
	position.x += 5;
	position.y += 5;
	unsigned char* imgPixelData_ = img->getData();
	int w = img->getWidth();
	int h = img->getHeight();
	int pa = (((h - position.y) - 1)*w + position.x) * 4 + 3;
	unsigned int alpha = imgPixelData_[pa];
	return alpha;
}

bool VirtualMap::setCache()
{
	return true;
}


