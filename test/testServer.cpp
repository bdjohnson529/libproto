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
	s_sleep(1000);

	while(true)
	{
		// receive on the server
		std::vector<AddressedMessage> msgs = server.Recv();
		for(auto msg : msgs)
		{
			// create the message object
			std::cout << "Received " << msg.message.size() << " bytes..." << std::endl;
			Message rmsg(msg.message, PROTO_COMPRESSION);

			// decode the packet
			assert(rmsg.Header().type == MessageType::IMAGE);
			ImagePacket opacket(rmsg.Body());
			cv::Mat oimage = cv::Mat(opacket.height, opacket.width, CV_8UC3, (void*) opacket.GetImage().data()).clone();
			cv::imshow("Image", oimage);
			cv::waitKey(10);
		}
	}
}
