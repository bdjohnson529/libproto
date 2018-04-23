#include "include/Client.hpp"

using namespace std;

int main()
{
	string address = "127.0.0.1";
	cout << address << endl;
	proto::Client myClient(address, 5000);
	return 0;
}

