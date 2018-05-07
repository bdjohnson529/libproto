#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace proto
{

	struct ImageData
	{
		int channels;
		int width;
		int height;
		ImageData()		// emtpy constructor necessary for class initialization
		{

		}
		ImageData(int channels, int width, int height)
		{
			this->channels = channels;
			this->width = width;
			this->height = height;
		}
	};

	class PayloadData
	{
		float attitude[3];
		float lla[3];
		std::string image;
		ImageData image_data;
		std::string serializedPayloadData;

	public:
		enum ImageType
		{
			MONO8,
			BGR24,
			BGRA32
		};

		PayloadData();
		PayloadData(std::string serializedPayloadData);
		void LoadLLA(float lat, float lon, float alt);
		void LoadAttitude(float yaw, float pitch, float roll);
		void LoadImage(void * image, int channels, int width, int height);			// change struct to accept char array
		void SerializeData();

		std::string GetSerializedData(){return serializedPayloadData;}
		std::string GetImage(){return image;}
		ImageData GetImageData(){return image_data;}

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & attitude;
			ar & lla;
			ar & image;
            ar & image_data.channels;
            ar & image_data.width;
            ar & image_data.height;
		}

	};


}