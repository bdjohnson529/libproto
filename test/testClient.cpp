#include "Client.hpp"

using namespace proto;

int main()
{
	std::string ip_address = "192.168.103.87";
	std::string port = "5000";

	proto::Client client(ip_address, port);

	return 0;
}