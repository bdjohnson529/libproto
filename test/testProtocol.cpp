#include <cassert>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Message.hpp"
#include "ImagePacket.hpp"
#include "Client.hpp"
#include "Server.hpp"

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

	// create client
	std::cout << "Starting Client on " << ADDRESS << std::endl;
	Client client("client", ADDRESS);

	// create server
	std::cout << "Starting Server on " << ADDRESS << std::endl;
    Server server(ADDRESS);

	// create an image packet
    cv::Mat iimage = cv::imread(image_path);
	Coord a(0,1);
	Coord b(2,3);
	Coord c(4,5);
	Coord d(6,7);
    ImagePacket ipacket(a, b, c, d, iimage.cols, iimage.rows, iimage.elemSize() * 8, iimage.data);
	Message wmsg(MessageType::IMAGE, ipacket.GetPacket(), COMPRESSION);

	// IMPORTANT : the server will miss the first message from a client
	client.Send(wmsg.Get());
	client.Send(wmsg.Get());
	s_sleep(1000);

	// receive on the server
	std::vector<AddressedMessage> msgs;
	do
	{
		msgs = server.Recv();
	} while(msgs.front().message.size() == 0);

	// create the message object
	Message rmsg(msgs.front().message, COMPRESSION);

	// decode the packet
	assert(rmsg.Header().type == MessageType::IMAGE);
    ImagePacket opacket(rmsg.Body());
    cv::Mat oimage = cv::Mat(opacket.height, opacket.width, iimage.type(), (void*) opacket.GetImage().data()).clone();
    cv::imshow("Image", oimage);
	std::cout << a.lat << "," << a.lon << std::endl;
	std::cout << b.lat << "," << b.lon << std::endl;
	std::cout << c.lat << "," << c.lon << std::endl;
	std::cout << d.lat << "," << d.lon << std::endl;
    cv::waitKey(0);
}