# Message Streaming Library

This library supports TCP/IP message streaming. We recommend benchmarking performance against the expected message transfer speed based on your message size and bandwidth. Bandwidth between Linux machines can be tested using [iperf](https://iperf.fr/).

#### Dependencies
* Boost
* OpenCV 3.3.0 or higher
* Zlib
* C++11 compiler


## Usage

### Serialization and Compression

The library packs all data in the PayloadData class. PayloadData is currently configured to transmit GPS position, attitude, and an image.

    payload_data.LoadLLA(latitude, longitude, altitude);
    payload_data.LoadAttitude(yaw, pitch, roll);
    payload_data.LoadImage(image_data, channels, width, height, zoom);


Data is serialized using Boost. The user serializes and retrieves the data as a std::string.

    std::string message_string = payload_data.GetSerializedData();

The Message class compresses a string, using zlib. The user will pass the serialized string to the Message class and retrieve a compressed string.

    Message msg(message_type, message_string, true);
    std::string compressed_message = msg.Get();

Once a compressed message is received, it can be decompressed using the Message class. Notice that the Message constructor is overloaded and can be used to both compress and decompress strings.

	Message msg(compressed_string, true);
	std::string decompressed_string = msg.Body();

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
