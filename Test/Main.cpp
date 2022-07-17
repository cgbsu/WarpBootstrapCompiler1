#include <Warp/Utilities.hpp>
#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>

using namespace Warp::Utilities;

constexpr auto b = Warp::Parsing::RegexTerm<"[0-9]+", "Digits">::term;
constexpr auto d = Warp::Parsing::NonTerminalTerm<size_t, "Number">::term;

constexpr ctpg::parser p(
	d, 
	ctpg::terms(
			b
		), 
	ctpg::nterms(
			d
		), 
	ctpg::rules( 
		d(b) >= [](auto num) {
			return std::stoi(std::string{num}.data());
		}
	)
);


int main(int argc, char** args)
{
	auto result = p.parse(ctpg::buffers::string_buffer("12345"), std::cout);
	if(result.has_value() == true)
		std::cout << "RESULT: " << result.value() << "\n";
	return 0;
}

