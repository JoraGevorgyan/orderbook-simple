#include "../Listener.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <variant>

#include "executor/Executor.hpp"
#include "parser/Parser.hpp"

namespace listener {
namespace {

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
					return;
				}
				if constexpr (std::is_same_v<cmd_t, ob::request::CancelOrder>) {
					std::cout << "got CANCEL order" << std::endl;
					return;
				}
				if constexpr (std::is_same_v<cmd_t, ob::request::Flush>) {
					std::cout << "got FLUSH" << std::endl;
					return;
				}
				if constexpr (std::is_same_v<cmd_t, ob::request::Invalid>) {
					std::cout << "invalid order: " << cmd.reason << std::endl;
				}
			},
			cmd);
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
	std::cout << "upd listener started" << std::endl;
	process_data_stream(std::cin);
}

} // namespace listener
