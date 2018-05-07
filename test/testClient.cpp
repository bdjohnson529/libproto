#include "Client.hpp"
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

	proto::Client client(ip_address, port);

	std::string image_file = "/home/nuc4/Desktop/test.png";

	while(1)
	{
		// load image from disk
		cv::Mat image;
		image = cv::imread(image_file);
		cv::imshow("image", image);
		cv::waitKey(2000);

		// cast as char array
		const char * image_char = reinterpret_cast<const char*>(image.data);
		size_t image_size = strlen(image_char);
		std::cout << "Image is " << image_size << " kb" << std::endl;

		// load payload data
		PayloadData payload_data;
		payload_data.LoadLLA(118, 33, 0);		// fake LLA
		payload_data.LoadAttitude(0, 0, 0);		// fake YPR
		payload_data.LoadImage(image.data, image_size, 8, image.cols, image.rows, 0);	// CV_TYPE set to zero
		payload_data.SerializeData();

		std::string serialized_data = payload_data.GetSerializedData();
		PayloadData received_payload_data = PayloadData(serialized_data);
		std::string received_image = received_payload_data.GetImage();
		ImageData received_image_data = received_payload_data.GetImageData();

		// cast string into cv::Mat
		unsigned char * buf = new unsigned char[received_image.size()];
		memcpy(buf, received_image.data(), received_image.size() );
		cv::Mat received_image_mat(received_image_data.height, received_image_data.width, CV_8UC1, buf);

		cv::imshow("unpacked", received_image_mat);
		cv::waitKey(1000);




	}

	return 0;
}