#include "Client.hpp"
#include "Message.hpp"
#include "PayloadData.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace proto;
using namespace cv;

int main()
{
	std::string ip_address = "192.168.1.7";
	std::string port = "5000";

	proto::Client client(ip_address, port);
	//sleep(1);

	std::string image_file = "/home/ben/Desktop/test.jpg";

	sleep(2);

	while(1)
	{
		// load image from disk
		cv::Mat image;
		image = cv::imread(image_file, IMREAD_GRAYSCALE);
		//cv::imshow("image", image);
		//cv::waitKey(2000);

		// load payload data
		PayloadData payload_data;
		payload_data.LoadLLA(118, 33, 0);		// fake LLA
		payload_data.LoadAttitude(0, 0, 0);		// fake YPR
		payload_data.LoadImage(image.data, 1, image.cols, image.rows);	// CV_TYPE set to zero

		// serialize and send
		std::string payload_message = payload_data.GetSerializedData();
		std::cout << "message size = " << payload_message.size() << std::endl;
		Message msg(IMAGE, payload_message, true);
		int bytes_sent = client.Send( msg.Get() );


		//cv::imshow("unpacked", received_image_mat);
		//cv::waitKey(1000);


		sleep(1);

	}

	return 0;
}