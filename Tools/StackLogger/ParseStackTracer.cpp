#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <variant>
#include <vector>
#include <queue>
#include <optional>
#include <filesystem>
#include <fstream>
#include <source_location>
#include <charconv>

#include <ctpg/ctpg.hpp>

#include <Warp/Utilities.hpp>

using namespace Warp::Utilities;

constexpr static const auto log_entry_start_pattern = FixedString{"\\[\\d+:\\d+\\]"};
constexpr static const auto regex_match_start_pattern = FixedString{"\\[\\d+:\\d+\\]\\s+"};
constexpr static const auto word_pattern = FixedString{"\\w"};
constexpr static const auto number_pattern = FixedString{"\\d+"};
constexpr static const auto single_anything_pattern = FixedString{"."};

constexpr static const auto reduce_rule_string = FixedString{"Reduced using rule"};
constexpr static const auto rr_conflict_string = FixedString{"R/R conflict encountered"};
constexpr static const auto end_of_file_string = FixedString{"<eof>"};
constexpr static const auto shift_to_string = FixedString{"Shift to"};
constexpr static const auto term_string = FixedString{"term:"};
constexpr static const auto reduce_operator_string = FixedString{"<-"};
constexpr static const auto parse_log_string = FixedString{"PARSE:"};
constexpr static const auto regex_match_log_string = FixedString{"REGEX MATCH:"};

constexpr static const auto log_entry_start = ctpg::regex_term<log_entry_start_pattern.string>("LogEntryStart");
constexpr static const auto number = ctpg::regex_term<number_pattern.string>("Number");
constexpr static const auto word = ctpg::regex_term<word_pattern.string>("Word");
constexpr static const auto single_anything = ctpg::regex_term<single_anything_pattern.string>("SingleAnything");


constexpr static const auto regex_match = ctpg::regex_term<regex_match_log_string.string>("RegexMatchStart");
constexpr static const auto parse_log = ctpg::regex_term<parse_log_string.string>("RegexMatchStart");
constexpr static const auto reduce_rule = ctpg::string_term(reduce_rule_string.string);
constexpr static const auto rr_conflict = ctpg::string_term(rr_conflict_string.string);
constexpr static const auto end_of_file = ctpg::string_term(end_of_file_string.string);
constexpr static const auto shift_to = ctpg::string_term(shift_to_string.string);
constexpr static const auto term = ctpg::string_term(term_string.string);
constexpr static const auto reduce_operator = ctpg::string_term(reduce_operator_string.string);

constexpr static const auto comma = ctpg::char_term(',');
constexpr static const auto colon = ctpg::char_term(':');

struct Reduction {
	std::string what;
	std::vector<std::string> from;
};

struct RRConflict {}; // Just to to help distinguish it //

using StackElement = std::variant<Reduction, std::string, RRConflict>;
using Stack = std::vector<StackElement>;

struct StackLog {
	Stack stack;
	std::queue<Stack> log;
};

constexpr static const auto reduction = ctpg::nterm<Reduction>("Reduction");
constexpr static const auto shift = ctpg::nterm<std::string>("Shift");
constexpr static const auto rr_conflict_log = ctpg::nterm<Reduction>("RRConflictLog");
constexpr static const auto stack = ctpg::nterm<Stack>("Stack");

consteval auto rules()
{
	return ctpg::rules(
			reduction(log_entry_start, parse_log, reduce_rule, number, word, reduce_operator, word)
			>= [](auto, auto, auto, auto, auto to, auto, auto from) {
				return Reduction{std::string{to}, std::vector{std::string{from}}};
			}, 
			stack(reduction)
			>>= [](auto& log, auto reduction) {
				log.stack.push_back(StackElement{Reduction{reduction}});
			}
		);
}

constexpr ctpg::parser parser_log_parser(
		stack, 
		ctpg::terms(
				log_entry_start, 
				number, 
				word, 
				regex_match, 
				parse_log, 
				regex_match, 
				single_anything, 
				reduce_rule, 
				reduce_operator
			), 
		ctpg::nterms(
				reduction, 
				shift, 
				rr_conflict_log, 
				stack
			), 
		rules()
	);

Stack parse_log_from_string(const std::string_view log);

std::string read_file(const std::filesystem::path log_path);

void test_parse_reduction();

int main(int argc, char** args)
{
	//const auto sample_log = read_file(std::filesystem::path("sample_log.ctpg"));
	//parse_log_from_string(sample_log);
	test_parse_reduction();
	return 0;
}

Stack parse_log_from_string(const std::string_view log) {
	return Stack{};
}

std::string read_file(const std::filesystem::path log_path) {
	return std::string{};
}

template<auto TestParameterConstant, auto CompareParameterConstant>
bool check(auto expected, std::source_location location = std::source_location::current())
{
	StackLog log;
	if(auto result = parser_log_parser.context_parse(
					log, 
					ctpg::buffers::string_buffer(TestParameterConstant.string), 
					std::cerr
				);
				result.has_value() == true
			)
	{
		if(CompareParameterConstant(log, expected) == false) {
			throw std::string{"Match Failed! " } + std::to_string(location.line());
			return false;
		}
		return true;
	}
	else {
		throw std::string{"Invalid Result! " } + std::to_string(location.line());
		return false;
	}
}

bool last_reduction_string_value(const StackLog& actual, std::string expected)
{
	if(std::holds_alternative<Reduction>(actual.stack.back()) == true)
		return std::get<Reduction>(actual.stack.back()).what == expected;
	return false;
}

void test_parse_reduction()
{
	check<
			FixedString{"[1:17] PARSE: Reduced using rule 0  Digits <- Base10Digits"}, 
			last_reduction_string_value
		>(std::string{"Digits"});
}

