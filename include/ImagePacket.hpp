#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace proto
{
	struct Coord
	{
		float lat;
		float lon;
		Coord()
		{

		}
		Coord(float lat, float lon)
		{
			this->lat = lat;
			this->lon = lon;
		}
	};

	// provides an image message class to serialize images
	class ImagePacket
	{
	public:
		enum ImageType
		{
			MONO8,
			BGR24,
			BGRA32
		};

		Coord center;      // center of image in lat/lon
        int zoom;          // web mercator zoom level
		int width;		 // image width
		int height;		 // image height
		int bpp;		 // bits per pixel
		int type;		 // pixel type
		int size;		 // data size
		std::string data;// image data buffer

        ImagePacket(Coord center, int zoom, int width, int height, int bpp, int type, void* data);
		ImagePacket(std::string raw);

		// returns packet buffer
		std::string GetImage(){return data;}
		std::string GetPacket(){return packet;}

	private:
		std::string packet; // packet buffer

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & center.lat;
			ar & center.lon;
            ar & zoom;

			ar & width;
			ar & height;
			ar & bpp;
			ar & type;
			ar & size;
			ar & data;
		}
	};
};
