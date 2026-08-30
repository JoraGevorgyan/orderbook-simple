#include "../Parser.hpp"

#include <algorithm>
#include <boost/container/static_vector.hpp>
#include <charconv>
#include <optional>
#include <ranges>

#include "common/types.hpp"

namespace parser {
namespace {

inline constexpr uint8_t max_cmd_len = 7;
using cmd_fields_t =
	boost::container::static_vector<ob::str_t, max_cmd_len + 1>;

std::optional<cmd_fields_t> split_csv_row(std::string_view row)
{
	cmd_fields_t res{};
	constexpr std::string_view delimiter{", "};
	for (const auto el : std::views::split(row, delimiter)) {
		if (res.size() > max_cmd_len) {
			// Received more elements than expected
			return std::nullopt;
		}
		auto in_ref = res.emplace_back();
		std::ranges::copy(
			el | std::views::take(ob::max_str_len - 1), std::back_inserter(in_ref));
	}
	return res;
}

template <typename val_t>
bool parse_num(std::string_view in, val_t& out)
{
	auto [ptr, ec] = std::from_chars(in.data(), in.data() + in.size(), out);
	return ec == std::errc{} && ptr == in.data() + in.size();
}

ob::request::cmd_t parse_request_new(const cmd_fields_t& fields)
{
	if (fields.size() != 7) {
		return ob::request::Invalid{"unexpected input structure of NEW!"};
	}
	ob::id_t user_id = 0;
	if (!parse_num(fields[1], user_id)) {
		return ob::request::Invalid{"failed to parse the user id"};
	}
	auto& symbol = fields[2]; // validate symbol
	ob::price_t price = 0;
	if (!parse_num(fields[3], price)) {
		return ob::request::Invalid{"failed to parse the price"};
	}
	ob::quantity_t quantity = 0;
	if (!parse_num(fields[4], quantity)) {
		return ob::request::Invalid{"failed to parse the quantity"};
	}
	auto side = ob::Side::BUY;
	if (fields[5] == "S") {
		side = ob::Side::SELL;
	} else if (fields[5] != "B") {
		return ob::request::Invalid{"failed to parse the order side"};
	}
	ob::id_t order_id = 0;
	if (!parse_num(fields[6], order_id)) {
		return ob::request::Invalid{"failed to parse the order id"};
	}

	return ob::request::NewOrder{
		.u_id = user_id,
		.symbol = symbol,
		.price = price,
		.quantity = quantity,
		.side = side,
		.o_id = order_id};
}

ob::request::cmd_t parse_request_cancel(const cmd_fields_t& fields)
{
	if (fields.size() != 3) {
		return ob::request::Invalid{"unexpected input structure of CANCEL!"};
	}
	ob::id_t user_id = 0;
	if (!parse_num(fields[1], user_id)) {
		return ob::request::Invalid{"failed to parse user id"};
	}
	ob::id_t order_id = 0;
	if (!parse_num(fields[2], order_id)) {
		return ob::request::Invalid{"failed to parse order id"};
	}
	return ob::request::CancelOrder{.u_id = user_id, .o_id = order_id};
}

ob::request::cmd_t parse_cmd(const std::optional<cmd_fields_t>& fields_opt)
{
	if (!fields_opt.has_value()) {
		return ob::request::Invalid{"failed to parse the request"};
	}
	const auto& fields = fields_opt.value();
	const std::string_view key(fields.front().data());
	if (key == "N") {
		return parse_request_new(fields);
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
