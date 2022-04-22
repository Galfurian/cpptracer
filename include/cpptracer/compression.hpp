/// @file compression.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Functions used to compress a stream of characters.

#pragma once

#ifdef COMPRESSION_ENABLED

#include <stdexcept>
#include <sstream>
#include <cstring>
#include <string>
#include <zlib.h>

namespace cpptracer::compression
{

/// Size of the buffer used for the compression.
#define BUFFER_SIZE 32768

/// @brief Compress a STL string using zlib with given compression level and
/// return the binary data.
/// @param str the input string to compress.
/// @param level the compression level.
/// @return the compressed string.
std::string compress(std::string const &str, int level = Z_BEST_COMPRESSION)
{
    // Variable used to track return value from zlib.
    int ret;
    // Create the zlib's control structure.
    z_stream zs;
    std::memset(&zs, 0, sizeof(zs));
    // Add 16 to MAX_WBITS to specify gzip format - it gets taken off again
    // in defaultInit2.
    if ((ret = deflateInit2(&zs, level,
                            Z_DEFLATED,
                            16 + MAX_WBITS,
                            MAX_MEM_LEVEL,
                            Z_DEFAULT_STRATEGY)) != Z_OK) {
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }
    zs.next_in = (Bytef *)str.data();
    // set the z_stream's input
    zs.avail_in = static_cast<uInt>(str.size());
    // Create and set the gzip header.
    gz_header header;
    memset(&header, 0, sizeof(header));
    if ((ret = deflateSetHeader(&zs, &header)) != Z_OK) {
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }
    char outbuffer[BUFFER_SIZE];
    std::string outstring;
    // retrieve the compressed bytes blockwise
    do {
        zs.next_out  = reinterpret_cast<Bytef *>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret          = deflate(&zs, Z_FINISH);
        if (outstring.size() < zs.total_out) {
            // Append the block to the output string
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);
    deflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        // An error occurred that was not EOF.
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }
    return outstring;
}

/// @brief Decompress an STL string using zlib and return the original data.
/// @param str the input string.
/// @return the decompressed string.
std::string decompress(std::string const &str)
{
    // z_stream is zlib's control structure
    z_stream zs;
    std::memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));

    zs.next_in  = (Bytef *)str.data();
    zs.avail_in = static_cast<uInt>(str.size());

    int ret;
    char outbuffer[BUFFER_SIZE];
    std::string outstring;

    // get the decompressed bytes blockwise using repeated calls to inflate
    do {
        zs.next_out  = reinterpret_cast<Bytef *>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) { // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") "
            << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

} // namespace cpptracer::compression

#endif