#include "../Listener.hpp"

#include <array>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/this_coro.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <cstdint>
#include <iostream>
#include <limits>
#include <variant>

#include "core/BookManager.hpp"
#include "parser/Parser.hpp"

namespace listener {
namespace {

class BufferStream : public std::streambuf {
public:
	BufferStream(const char* data, std::size_t size)
	{
		char* begin = const_cast<char*>(data);
		setg(begin, begin, begin + size);
	}
};

void process_data_stream(std::istream& in_stream)
{
	std::string line;
	while (std::getline(in_stream, line)) {
		const auto cmd = parser::parse_csv(line);
		std::visit(
			[](const auto& cmd) {
				using cmd_t = std::decay_t<decltype(cmd)>;
				if constexpr (std::is_same_v<cmd_t, ob::request::NewOrder>) {
					std::cout << "got NEW order" << std::endl;
					// orderbook::new_order
					return;
				}
				if constexpr (std::is_same_v<cmd_t, ob::request::CancelOrder>) {
					// orderbook::cancel_order
					std::cout << "got CANCEL order" << std::endl;
					return;
				}
				if constexpr (std::is_same_v<cmd_t, ob::request::Flush>) {
					std::cout << "got FLUSH" << std::endl;
					// orderbook::flush
					return;
				}
				if constexpr (std::is_same_v<cmd_t, ob::request::Invalid>) {
					std::cout << "invalid order: " << cmd.reason << std::endl;
				}
			},
			cmd);
	}
}

boost::asio::awaitable<void> udp_listener()
{
	std::cout << "udp listener started" << std::endl;
	using udp_t = boost::asio::ip::udp;

	auto executor = co_await boost::asio::this_coro::executor;
	udp_t::socket socket(
		executor, udp_t::endpoint(boost::asio::ip::address_v4::loopback(), 1234));
	constexpr uint16_t max_data_size = 4096;
	std::array<char, max_data_size> buf{};

	while (true) {
		udp_t::endpoint sender;
		const auto num = co_await socket.async_receive_from(
			boost::asio::buffer(buf), sender, boost::asio::use_awaitable);
		BufferStream buf_stream(buf.data(), num);
		std::istream in_stream(&buf_stream);
		process_data_stream(in_stream);
	}
}

} // namespace

void start_stdin()
{
	std::cout << "stdin listener started" << std::endl;
	process_data_stream(std::cin);
}

void start_udp()
{
	boost::asio::io_context io;
	boost::asio::co_spawn(io, udp_listener(), boost::asio::detached);
	io.run();
}

} // namespace listener
