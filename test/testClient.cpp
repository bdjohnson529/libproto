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
	std::string ip_address = "192.168.103.87";
	std::string port = "5000";

	// load image from disk
	cv::Mat image;


	// set up TCP client
	proto::Client client("127.0.0.1", "5000");
	bool client_exists = client.GetStatus();
	while(!client_exists) 	// loop if unsuccessful
	{
		client = proto::Client("127.0.0.1", "5000");
		client_exists = client.GetStatus();
	}

	std::cout << "Successfully connected to client." << std::endl;

	std::string image_file = "/home/ben/Desktop/testimg.png";

	while(1)
	{
		// load image from disk
		cv::Mat image;
		image = cv::imread(image_file, IMREAD_GRAYSCALE);
		cv::imshow("image", image);
		cv::waitKey(500);

		// load payload data
		PayloadData payload_data;
		payload_data.LoadLLA(34.8687255602, -118.0778732707, 0);		// fake LLA
		payload_data.LoadAttitude(0, 0, 0);		// fake YPR
		payload_data.LoadImage(image.data, 1, image.cols, image.rows, 20, ImageType::MONO8, CameraType::VISIBLE);	// fake zoom

		// serialize and send
		std::string payload_message = payload_data.GetSerializedData();
		std::cout << "message size = " << payload_message.size() << std::endl;
		Message msg(IMAGE, payload_message, true);
		int bytes_sent = client.Send( msg.Get() );

		//cv::imshow("unpacked", received_image_mat);
		//cv::waitKey(1000);


	}

	return 0;
}