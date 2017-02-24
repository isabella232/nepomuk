#include "tool/io/binary_decoder.hpp"
#include "tool/io/binary_encoder.hpp"
#include "tool/io/serialisation.hpp"
#include "tool/io/std_stream.hpp"
#include "tool/io/stream_errors.hpp"

#include <fstream>
#include <string>

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(write_and_read_bytes)
{
    std::string test{"Don't panic!"};

    { // write data to test file
        std::ofstream ofs("write_and_read_bytes.tmp");
        transit::tool::io::StdOutputStream outstream(ofs);
        auto binary_encoder = transit::tool::io::makeBinaryEncoder(outstream);
        transit::tool::io::Serialize(test, binary_encoder);
    }

    std::string read_data = []() {
        std::ifstream ifs("write_and_read_bytes.tmp");
        transit::tool::io::StdInputStream instream(ifs);
        auto binary_decoder = transit::tool::io::makeBinaryDecoder(instream);
        std::string result;
        transit::tool::io::Deserialize(result, binary_decoder);
        return result;
    }();

    BOOST_CHECK_EQUAL(test, read_data);
}

BOOST_AUTO_TEST_CASE(handle_unopened_streams)
{
    std::ofstream ofs;
    transit::tool::io::StdOutputStream outstream(ofs);
    auto binary_encoder = transit::tool::io::makeBinaryEncoder(outstream);
    std::string test{"Don't panic!"};

    BOOST_CHECK_THROW(transit::tool::io::Serialize(test, binary_encoder), transit::tool::io::OutputStreamError);

    std::ifstream ifs;
    transit::tool::io::StdInputStream instream(ifs);
    auto binary_decoder = transit::tool::io::makeBinaryDecoder(instream);
    std::string result;
    BOOST_CHECK_THROW(transit::tool::io::Deserialize(result, binary_decoder), transit::tool::io::InputStreamError);
}

BOOST_AUTO_TEST_CASE(zero_length)
{
    std::string test;

    { // write data to test file
        std::ofstream ofs("write_and_read_bytes.tmp");
        transit::tool::io::StdOutputStream outstream(ofs);
        auto binary_encoder = transit::tool::io::makeBinaryEncoder(outstream);
        transit::tool::io::Serialize(test, binary_encoder);
    }

    std::string read_data = []() {
        std::ifstream ifs("write_and_read_bytes.tmp");
        transit::tool::io::StdInputStream instream(ifs);
        auto binary_decoder = transit::tool::io::makeBinaryDecoder(instream);
        std::string result;
        transit::tool::io::Deserialize(result, binary_decoder);
        return result;
    }();

    BOOST_CHECK_EQUAL(test, read_data);
}

BOOST_AUTO_TEST_CASE(broken_encode)
{
    std::ofstream ofs("write_and_read_bytes.tmp");
    transit::tool::io::StdOutputStream outstream(ofs);
    BOOST_CHECK_NO_THROW(outstream.write_bytes(NULL, 64));

    std::ifstream ifs("write_and_read_bytes.tmp");
    transit::tool::io::StdInputStream instream(ifs);
    BOOST_CHECK_NO_THROW(instream.read_bytes(NULL, 64));

    BOOST_CHECK(static_cast<bool>(ofs));
    BOOST_CHECK(static_cast<bool>(ifs));
}