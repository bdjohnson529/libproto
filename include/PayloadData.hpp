#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace proto
{
	enum ImageType
	{
		MONO8,
		BGR24,
		RGBA32
	};

	enum CameraType
	{
		THERMAL,
		VISIBLE
	};

	struct ImageData
	{
		int channels;
		int width;
		int height;
		int zoom;
		ImageType imgType;
		CameraType camType;
		ImageData()		// emtpy constructor necessary for class initialization
		{

		}
		ImageData(int channels, int width, int height, int zoom, ImageType imgType, CameraType camType)
		{
			this->channels = channels;
			this->width = width;
			this->height = height;
			this->zoom = zoom;
            this->imgType = imgType;
            this->camType = camType;
		}
	};

	class PayloadData
	{

		std::string image;
		ImageData image_data;

	public:
        float attitude[3];
        float lla[3];
		
		PayloadData();
		PayloadData(std::string serializedPayloadData);
		void LoadLLA(float lat, float lon, float alt);
		void LoadAttitude(float yaw, float pitch, float roll);
		void LoadImage(void * image, int channels, int width, int height, int zoom, ImageType imgType, CameraType camType);			// change struct to accept char array
		void SerializeData();
		//void setImageType(ImageType type);

		std::string GetImage(){return image;}
		ImageData GetImageData(){return image_data;}
		std::string GetSerializedData()
		{
			this->SerializeData();
			return serializedPayloadData;
		}

	private:
		std::string serializedPayloadData;
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
            ar & image_data.zoom;
            ar & image_data.imgType;
            ar & image_data.camType;
		}

	};


}
