# Message Streaming Library

This library supports TCP/IP message streaming between Linux machines. Messages can be transferred over any network: wireless, or wired. We recommend testing the bandwidth of your connection using [iperf](https://iperf.fr/).

#### Dependencies
* Boost
* OpenCV 3.3.0 or higher
* Zlib
* C++11 compiler


## Usage

### Serialization and Compression

Sensor data is used to instantiate the [PayloadData](src/PayloadData.cpp) class. The class accepts GPS position, attitude, and an image as attributes. Using these attributes, the [SerializeData](https://github.com/bdjohnson529/libproto/blob/master/src/PayloadData.cpp#L38) method genereates a serialized string. The serialized string is then [compressed](https://github.com/bdjohnson529/libproto/blob/master/src/Message.cpp#L19) and then [sent over the socket](https://github.com/bdjohnson529/libproto/blob/master/src/Client.cpp#L95) using the [Linux OS](https://man7.org/linux/man-pages/man2/send.2.html).

### TCP Client/Server
The TCP stack uses a publisher/subscriber model. One server is created which can subscribe to multiple clients. The server must be created before the clients.

	proto::Server server(port);

Once the server has been successfully created, clients will automatically connect to the server when they are constructed. The Client constructor will loop indefinitely until it is able to connect to the Server.

    proto::Client client(ip_address, port);

Once the Client connects to the Server, the constructor will return. Messages can now be sent from the Client to the Server. The Client::Send method will return the number of bytes sent.

	int bytes_sent = client.Send( msg.Get() );

On the server side, messages are received by the Server::RecvAll method. This method waits for a message to be sent, then reconstructs and returns the message.

    std::string received_message = server.RecvAll();

## Setup

The testClient and testServer should help you diagnose and test your setup.
