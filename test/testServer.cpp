#include <cassert>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Message.hpp"
#include "ImagePacket.hpp"
#include "Server.hpp"

using namespace proto;

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		std::cout << "usage : testServer ip port" << std::endl;
		return -1;
	}

	// get client info
	std::string ip = argv[1];
	std::string port = argv[2];
	std::string address = "tcp://" + ip + ":" + port;

	// create server
	std::cout << "Starting Server on " << address << std::endl;
	Server server(address);

	while(true)
	{
		// receive on the server
		AddressedMessage msg = server.Recv();

		// create the message object
		std::cout << "Received " << msg.message.size() << " bytes..." << std::endl;
		Message rmsg(msg.message, PROTO_COMPRESSION);

		// decode the packet
		assert(rmsg.Header().type == MessageType::IMAGE);
		ImagePacket opacket(rmsg.Body());

		int type = ImagePacket::ImageType::BGR24;
		if(opacket.type == ImagePacket::ImageType::MONO8)
			type = CV_8UC1;
		else if(opacket.type == ImagePacket::ImageType::BGR24)
			type = CV_8UC3;
		else if(opacket.type == ImagePacket::ImageType::BGRA32)
			type = CV_8UC4;
		cv::Mat oimage = cv::Mat(opacket.height, opacket.width, type, (void*) opacket.GetImage().data()).clone();
		cv::imshow("Image", oimage);
		std::cout << "Scale: " << opacket.pixel_scale << std::endl;
		std::cout << opacket.center.lat << "," << opacket.center.lon << std::endl;
		cv::waitKey(10);
	}
}
