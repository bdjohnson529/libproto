#include "ImagePacket.hpp"

namespace proto
{
    ImagePacket::ImagePacket(Coord center, float yaw, int zoom, int width, int height, int bpp, int type, void* data)
	{
        this->center = center;
        this->yaw = yaw;
        this->zoom = zoom;
	    this->width = width;
		this->height = height;
		this->bpp = bpp;
		this->type = type;
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
