#include "PayloadData.hpp"

namespace proto
{
	PayloadData::PayloadData()
	{

	}

	PayloadData::PayloadData(std::string serializedPayloadData)
	{
		std::istringstream iss(serializedPayloadData);
		boost::archive::text_iarchive archive(iss);
		archive >> *this;
	}

	void PayloadData::LoadLLA(float lat, float lon, float alt)
	{
		this->lla[0] = lat;
		this->lla[1] = lon;
		this->lla[2] = alt;
	}

	void PayloadData::LoadAttitude(float yaw, float pitch, float roll)
	{
		this->attitude[0] = yaw;
		this->attitude[1] = pitch;
		this->attitude[2] = roll;
	}

	void PayloadData::LoadImage(void * image, int channels, int width, int height, int zoom, ImageType type)
	{
		int size = channels * width * height;
		this->image = std::string( (const char*) image, size );
		this->image_data = ImageData( channels, width, height, zoom, type );
	}

	void PayloadData::SerializeData()
	{
		std::ostringstream oss;
		boost::archive::text_oarchive archive(oss);
		archive << *this;
		this->serializedPayloadData = oss.str();
	}

	/*
	void PayloadData::setImageType(ImageType type)
	{
		this->_type = type;
	}
	*/
	
};