### Serialization and Compression

PayloadData is currently configured to transmit 

    payload_data.LoadLLA(latitude, longitude, altitude);
    payload_data.LoadAttitude(yaw, pitch, roll);
    payload_data.LoadImage(image_data, channels, width, height, zoom);


Data is serialized using Boost. The user serializes and retrieves the data as a string.

    std::string message_string = payload_data.GetSerializedData();



    Message msg(message_type, message_string, true);
    std::string compressed_message = msg.Get();

Once a compressed message is received, it can be decompressed using the Message class. Notice that the Message constructor is overloaded and can be used to both compress and decompress strings.

	Message msg(compressed_string, true);
	std::string decompressed_string = msg.Body();

