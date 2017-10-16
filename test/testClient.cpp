#include <cassert>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Message.hpp"
#include "ImagePacket.hpp"
#include "Client.hpp"

using namespace proto;

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		std::cout << "usage : testClient ip port image_path" << std::endl;
		return -1;
	}

	// get client info
	std::string ip = argv[1];
	std::string port = argv[2];
	std::string address = "tcp://" + ip + ":" + port;
	std::string image_path = argv[3];

	// create client
	std::cout << "Starting Client on " << address << std::endl;
	Client client("client", address);

	// create an image packet
	cv::Mat iimage = cv::imread(image_path);
	Coord center(0,0);
	float zoom = 2;

	// find the type of image
	int type = CV_8UC3;
	if(iimage.type() == CV_8UC1)
		type = ImagePacket::ImageType::MONO8;
	else if(iimage.type() == CV_8UC3)
		type = ImagePacket::ImageType::BGR24;
	else if(iimage.type() == CV_8UC4)
		type = ImagePacket::ImageType::BGRA32;

	// send every 1 second
	while(true)
	{
		center.lat += 10;
		center.lat = fmodf(center.lat, 60);
		center.lon += 10;
		center.lon = fmodf(center.lon, 180);
		ImagePacket ipacket(center, zoom, iimage.cols, iimage.rows, iimage.elemSize() * 8, type, iimage.data);
		Message wmsg(MessageType::IMAGE, ipacket.GetPacket(), PROTO_COMPRESSION);
		std::cout << "Sent " << wmsg.Get().size() << " bytes..." << std::endl;
		client.Send(wmsg.Get());
		s_sleep(1000);
	}
}
