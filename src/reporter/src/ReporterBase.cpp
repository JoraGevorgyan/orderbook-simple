#include "../ReporterBase.hpp"

#include <variant>

namespace reporter {

ReporterBase::~ReporterBase()
{
	shutdown();
}

bool ReporterBase::acknowledgement(const ob::report::Acknowledgement& report)
{
	return instance().enqueue(report);
}

bool ReporterBase::matched_trades(const ob::report::MatchedTrades& report)
{
	return instance().enqueue(report);
}

bool ReporterBase::book_change(const ob::report::BookChange& report)
{
	return instance().enqueue(report);
}

void ReporterBase::handle(const ob::report::report_t& report)
{
	std::visit([this](const auto& r) { handle(r); }, report);
}

// TODO: implement.
void ReporterBase::handle(const ob::report::Acknowledgement& /*report*/) {}

void ReporterBase::handle(const ob::report::MatchedTrades& /*report*/) {}

void ReporterBase::handle(const ob::report::BookChange& /*report*/) {}

} // namespace reporter
