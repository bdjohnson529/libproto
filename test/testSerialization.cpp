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

	std::string image_file = "/home/ben/Desktop/testimg.png";

	// load image from disk
	cv::Mat image;
	image = cv::imread(image_file, IMREAD_GRAYSCALE);
	cv::imshow("image", image);
	cv::waitKey(400);

	// cast as char array
	const char * image_char = reinterpret_cast<const char*>(image.data);
	size_t image_size = strlen(image_char);
	std::cout << "original image size = " << image_size << std::endl;

	// load payload data, and serialize
	PayloadData payload_data;
	payload_data.LoadLLA(118, 33, 0);		// fake LLA
	payload_data.LoadAttitude(0, 0, 0);		// fake YPR
	payload_data.LoadImage(image.data, image.channels(), image.cols, image.rows, 20);	// fake zoom
	payload_data.SerializeData();

	// unpack serialized data
	std::string serialized_data = payload_data.GetSerializedData();
	PayloadData received_payload_data = PayloadData(serialized_data);
	std::string received_image = received_payload_data.GetImage();
	ImageData received_image_data = received_payload_data.GetImageData();

	std::cout << "recieved img size = " << received_image.size() << std::endl;
	std::cout << "zoom = " << received_image_data.zoom << std::endl;

	// cast string into cv::Mat
	int n = received_image.size();

	unsigned char * buf = new unsigned char[received_image_data.channels * received_image_data.height * received_image_data.width];
	memset(buf, 0, sizeof buf);
	memcpy(buf, received_image.data(), received_image.size() );
	cv::Mat received_image_mat(received_image_data.height, received_image_data.width, CV_8UC1, buf );

	// display deserialized image
	cv::imshow("unpacked", received_image_mat);
	cv::waitKey(6000);


	return 0;
}