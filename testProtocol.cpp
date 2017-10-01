#include <cassert>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Message.hpp"
#include "ImagePacket.hpp"
#include "Client.hpp"
#include "Server.hpp"

#define ADDRESS std::string("127.0.0.1")
#define PORT 20000

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
	std::cout << "Starting Server on " << ADDRESS + ":" + std::to_string(PORT) << std::endl;
    Server server(ADDRESS, PORT);

	// create client
	std::cout << "Starting Client on " << ADDRESS + ":" + std::to_string(PORT) << std::endl;
	Client client("client", ADDRESS, PORT);

	// create an image packet
    cv::Mat iimage = cv::imread(image_path);
	Coord a(0,1);
	Coord b(2,3);
	Coord c(4,5);
	Coord d(7,8);
    ImagePacket ipacket(a, b, c, d, iimage.cols, iimage.rows, iimage.elemSize() * 8, iimage.data);
	Message wmsg(MessageType::IMAGE, ipacket.GetPacket());

	// send the client packet
	client.Send(wmsg.Get());

	// receive on the server
	AddressedMessage amsg = server.Recv();

	// create the message object
	Message rmsg(amsg.message);

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
