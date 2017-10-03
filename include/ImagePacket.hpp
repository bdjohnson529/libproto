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
		// tie points of image
		Coord topleft;
		Coord topright;
		Coord botleft;
		Coord botright;

	    int width;		 // image width
		int height;		 // image height
		int bpp;		 // bits per pixel
		int size;		 // data size
		std::string data;// image data buffer

	    ImagePacket(Coord topleft, Coord topright, Coord botleft, Coord botright, int width, int height, int bpp, void* data);
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
			ar & topleft.lat;
			ar & topleft.lon;
			ar & topright.lat;
			ar & topright.lon;
			ar & botleft.lat;
			ar & botleft.lon;
			ar & botright.lat;
			ar & botright.lon;

			ar & width;
			ar & height;
			ar & bpp;
			ar & size;
			ar & data;
		}
	};
};
