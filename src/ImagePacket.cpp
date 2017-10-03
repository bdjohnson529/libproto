#include "ImagePacket.hpp"

namespace proto
{
	ImagePacket::ImagePacket(Coord topleft, Coord topright, Coord botleft, Coord botright, int width, int height, int bpp, void* data)
	{
		this->topleft = topleft;
		this->topright = topright;
		this->botleft = botleft;
		this->botright = botright;
	    this->width = width;
		this->height = height;
		this->bpp = bpp;
		this->size = (width * height * bpp) / 8;
		this->data = std::string((const char*) data, size);
		std::ostringstream oss;
		boost::archive::text_oarchive archive(oss);
		archive << *this;
		packet = oss.str();
	}

	ImagePacket::ImagePacket(std::string raw)
	{
		std::istringstream iss(raw);
		boost::archive::text_iarchive archive(iss);
		archive >> *this;
	}
};
