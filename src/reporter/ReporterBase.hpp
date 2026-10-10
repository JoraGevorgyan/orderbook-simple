#pragma once

#include "common/AsyncService.hpp"
#include "common/report.hpp"

namespace reporter {

// Singleton service that receives reports from the order books and processes
// them asynchronously on its own worker thread.
class ReporterBase
	: public ob::AsyncService<ReporterBase, ob::report::report_t> {
public:
	// Thread-safe, non-blocking. Return false if the reporter has been stopped.
	static bool acknowledgement(const ob::report::Acknowledgement& report);
	static bool matched_trades(const ob::report::MatchedTrades& report);
	static bool book_change(const ob::report::BookTop& report);

	~ReporterBase();

private:
	friend class ob::AsyncService<ReporterBase, ob::report::report_t>;

	ReporterBase() = default;

	// Report handlers, executed on the worker thread only.
	void handle(const ob::report::report_t& report);
	void handle(const ob::report::Acknowledgement& report);
	void handle(const ob::report::MatchedTrades& report);
	void handle(const ob::report::BookTop& report);
};

} // namespace reporter
