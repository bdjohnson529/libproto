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
	Coord a(0,1);
	Coord b(2,3);
	Coord c(4,5);
	Coord d(6,7);
	ImagePacket ipacket(a, b, c, d, iimage.cols, iimage.rows, iimage.elemSize() * 8, iimage.data);
	Message wmsg(MessageType::IMAGE, ipacket.GetPacket(), PROTO_COMPRESSION);

	// send every 1 second
	while(true)
	{
		std::cout << "Sent " << wmsg.Get().size() << " bytes..." << std::endl;
		client.Send(wmsg.Get());
		s_sleep(1000);
	}
}
