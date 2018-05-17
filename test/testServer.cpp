#include "Server.hpp"
#include "PayloadData.hpp"
#include "Message.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define DEBUG false

using namespace proto;

int main()
{

	proto::Server server(5000);

	while(1)
	{
		std::string payload_message = server.RecvAll();

		if ( payload_message.empty() ){
			sleep(1);
			continue;
		}

		if(DEBUG)
			printf("--Message received.\n");

		Message msg(payload_message, true);
		if(DEBUG)
			printf("--Message unpacked.\n");

		PayloadData received_payload_data = PayloadData( msg.Body() );
		if(DEBUG)
			printf("--Payload struct unpacked.\n");

		ImageData received_image_data = received_payload_data.GetImageData();
		std::string received_image = received_payload_data.GetImage();

		unsigned char * buf = new unsigned char[received_image_data.channels * received_image_data.height * received_image_data.width];
		memset(buf, 0, sizeof buf);
		memcpy(buf, received_image.data(), received_image.size() );
		cv::Mat received_image_mat(received_image_data.height, received_image_data.width, CV_8UC1, buf );

		std::cout << "height = " << received_image_data.height << std::endl;

		cv::imshow("received", received_image_mat);
		cv::waitKey(500);
	}

	return 0;
}