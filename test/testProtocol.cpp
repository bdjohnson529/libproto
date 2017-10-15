#include <cassert>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Message.hpp"
#include "ImagePacket.hpp"
#include "Client.hpp"
#include "Server.hpp"

using namespace proto;

#define ADDRESS std::string("tcp://127.0.0.1:20000")

int main(int argc, char *argv[])
{
	std::string image_path = "../images/opencv.png";
	if(argc > 2)
	{
		std::cout << "usage : testProtocol image_path" << std::endl;
		return -1;
	} else if(argc == 2) {
		image_path = argv[1];
	}

	// create server
	std::cout << "Starting Server on " << ADDRESS << std::endl;
	Server server(ADDRESS);

	// create client
	std::cout << "Starting Client on " << ADDRESS << std::endl;
	Client client("client", ADDRESS);

	// create an image packet
	cv::Mat iimage = cv::imread(image_path);
	float pixel_scale = 1.0f;
	Coord center(0,1);
	ImagePacket ipacket(pixel_scale, center, iimage.cols, iimage.rows, iimage.elemSize() * 8, iimage.type(), iimage.data);
	Message wmsg(MessageType::IMAGE, ipacket.GetPacket(), PROTO_COMPRESSION);

	// IMPORTANT : the server will miss the first message from a client
	for(int i = 0; i < 10; ++i)
		client.Send(wmsg.Get());

	// receive on the server
	AddressedMessage msg = server.Recv();

	// create the message object
	Message rmsg(msg.message, PROTO_COMPRESSION);

	// decode the packet
	assert(rmsg.Header().type == MessageType::IMAGE);
	ImagePacket opacket(rmsg.Body());
	cv::Mat oimage = cv::Mat(opacket.height, opacket.width, opacket.type, (void*) opacket.GetImage().data()).clone();
	cv::imshow("Image", oimage);
	std::cout << "Scale: " << opacket.pixel_scale << std::endl;
	std::cout << opacket.center.lat << "," << opacket.center.lon << std::endl;
	cv::waitKey(0);
}
