#include "Compress.hpp"
#include "Server.hpp"

namespace proto
{
	std::string Compress(std::string str)
	{
	    z_stream zs;                        // z_stream is zlib's control structure
	    memset(&zs, 0, sizeof(zs));

	    if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK)
	        throw(std::runtime_error("deflateInit failed while compressing."));

	    zs.next_in = (Bytef*) str.data();
	    zs.avail_in = str.size();           // set the z_stream's input

	    int ret;
        int outbuffer_size = 16000000;
        char * outbuffer = new char[16000000];
	    std::string outstring;

	    // retrieve the compressed bytes blockwise
	    do {
	        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = outbuffer_size;//sizeof(outbuffer);

	        ret = deflate(&zs, Z_FINISH);

	        if (outstring.size() < zs.total_out) {
	            // append the block to the output string
	            outstring.append(outbuffer,
	                             zs.total_out - outstring.size());
	        }
	    } while (ret == Z_OK);

	    deflateEnd(&zs);

        if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
	        std::ostringstream oss;
	        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
	        throw(std::runtime_error(oss.str()));
	    }

        delete[] outbuffer;

	    return outstring;
	}

	/** Decompress an STL string using zlib and return the original data. */
	std::string Decompress(std::string str)
	{
	    z_stream zs;                        // z_stream is zlib's control structure
	    memset(&zs, 0, sizeof(zs));

	    if (inflateInit(&zs) != Z_OK)
	        throw(std::runtime_error("inflateInit failed while decompressing."));

	    zs.next_in = (Bytef*)str.data();
	    zs.avail_in = str.size();

	    int ret;
        int outbuffer_size = str.size();
        char *outbuffer = new char[outbuffer_size];
	    std::string outstring;

        int idx = 0;
	    // get the decompressed bytes blockwise using repeated calls to inflate
	    do {
	        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = outbuffer_size;

            ret = inflate(&zs, 0);

            //std::cout << "inflate return = " << ret << std::endl;

	        if (outstring.size() < zs.total_out) {
	            outstring.append(outbuffer,
	                             zs.total_out - outstring.size());

            idx++;
	        }
        } while (ret == Z_OK);
        delete[] outbuffer;

        std::cout << "inflate called " << idx << " times." << std::endl;

	    inflateEnd(&zs);

        if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
	        std::ostringstream oss;
	        oss << "Exception during zlib decompression: (" << ret << ") "
	            << zs.msg;
	        throw(std::runtime_error(oss.str()));
	    }

        //delete outbuffer;

	    return outstring;
	}
};
