#include "ImagePacket.hpp"

namespace proto
{
    ImagePacket::ImagePacket(float pixel_scale, Coord center, int width, int height, int bpp, void* data)
	{
        this->pixel_scale = pixel_scale;
        this->center = center;
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
