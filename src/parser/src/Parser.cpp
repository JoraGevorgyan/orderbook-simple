#include "../Parser.hpp"

#include <algorithm>
#include <iostream>
#include <optional>
#include <ranges>

#include "common/types.hpp"

namespace parser {
namespace {

std::optional<ob::cmd_fields_t> split_csv_row(std::string_view row)
{
	ob::cmd_fields_t res{ob::str_t{'\0'}};
	constexpr std::string_view delimiter{", "};
	auto res_ins_it = res.begin();
	for (const auto el : std::views::split(row, delimiter)) {
		std::ranges::copy(
			el | std::views::take(ob::max_str_len - 1), res_ins_it->begin());
		if (res_ins_it != res.end()) {
			++res_ins_it;
		} else {
			// Received more elements than expected
			return std::nullopt;
		}
	}
	return res;
}

ob::request::cmd_t parse_cmd(const std::optional<ob::cmd_fields_t>& fields_opt)
{
	if (!fields_opt.has_value()) {
		return ob::request::Invalid{"failed to parse the request"};
	}
	const auto& fields = fields_opt.value();
	const std::string_view key(fields.front().data());
	if (key == "N") {
		return parse_reqest_new(fields);
	}
	if (key == "C") {
		return parse_request_cancel(fields);
	}
	if (key == "F") {
		return ob::request::Flush{};
	}
	return ob::request::Invalid{"Unknown request"};
}

} // namespace

ob::request::cmd_t parse_csv(std::string_view data)
{
	if (data.empty()) {
		return ob::request::Invalid{"Parser received an empty data!"};
	}
	return parse_cmd(split_csv_row(data));
}

} // namespace parser
