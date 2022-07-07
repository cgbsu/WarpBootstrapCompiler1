#include <Warp/Utilities.hpp>
#include <ctime>
// #include <Warp/Parsing/Numbers.hpp>
#define WARP__PARSING__HEADER__TERMS__HPP__USE_TERM_DEFINITION_MACROS
#include <Warp/Parsing/Terms.hpp>

using namespace Warp::Utilities;

/*constexpr static const char test[] = "ABC123";

constexpr ctpg::parser p(
		size_t, 
		ctpg::terms(
				Warp::Parsing::DigitsTermType
			), 
		ctpg::nterms(
				Warp::Parsing::WholeNonTerminalType
			)
		Warp::Parsing::number_rules()
	);
*/


namespace Warp::Parsing
{
	enum class Test {
		String, 
		Regex, 
		Character, 
		Nterm
	};
	WARP_STRING_TERM(Test::String, "abc")
	WARP_REGEX_TERM(Test::Regex, "[0-9]+", "Number")
	WARP_CHARACTER_TERM(Test::Character, '+')
	WARP_NON_TERMINAL_TERM(Test::Nterm, size_t, "Iteger")
}

constexpr auto a = Warp::Parsing::Term<Warp::Parsing::Test::String>::term();
constexpr auto b = Warp::Parsing::Term<Warp::Parsing::Test::Regex>::term();
constexpr auto c = Warp::Parsing::Term<Warp::Parsing::Test::Character>::term();
constexpr auto d = Warp::Parsing::Term<Warp::Parsing::Test::Nterm>::term();

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

