#include <iostream>
#include <string>
#include <charconv>
#include <ctpg/ctpg.hpp>

constexpr char natural_number_pattern[] = "[1-9][0-9]*";
constexpr ctpg::regex_term<natural_number_pattern> natural("Natural");

template<typename ResultParameterType = size_t>
ResultParameterType to_value(std::string_view literal)
{
	ResultParameterType result;
	std::from_chars(
		literal.data(), 
		literal.data() + literal.size(), 
		result
	);
	return result;
}

constexpr ctpg::nterm<double> value("Value");

consteval auto values()
{
	return ctpg::rules( 
			value(natural)
				>= [](const auto& literal) {
					return static_cast<double>(to_value(literal));
				}, 
			value(value, '*', natural)
				>= [](const auto left, auto, const auto right) {
					return left * static_cast<const double>(to_value(right));
				}, 
			value(value, '/', natural)
				>= [](const auto left, auto, const auto right) {
					return left / static_cast<const double>(to_value(right));
				}
		);
}


constexpr ctpg::parser p( 
		value, 
		ctpg::terms( 
			natural, 
			'*', 
			'/'
		), 
		ctpg::nterms( 
			value 
		), 
		values()
	);

int main(int argc, char** args)
{
	for(std::string input = ""; input != "exit"; std::getline(std::cin, input))
	{
		if(auto result = p.parse(ctpg::buffers::string_buffer(input.data()), std::cerr);
				result.has_value() == true)
			std::cout << "Result: " << result.value() << "\n";
		else
			std::cout << "Could not parse\n";
	}
	return 0;
}

