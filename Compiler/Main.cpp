#include <Warp/SyntaxTranslation/LLVM/Compile.hpp>

using namespace Warp::Utilities;
using namespace Warp::Runtime::Compiler;
using namespace Warp::SyntaxTranslation;

extern LLVM::Context context;
std::optional<std::string> read_file(std::filesystem::path file_path);

#define HOST_TRIPLET "x86_64-linux-gnu"


int main(int argc, char** args)
{
	auto warp_code_path = std::filesystem::path("Warp") / "Test0.warp";
	if(argc >= 2)
		warp_code_path = std::filesystem::path(args[1]);
	std::cout << "Warp Code Path: " << warp_code_path << "\n";
	auto warp_code_ = read_file(warp_code_path);
	if(warp_code_.has_value() == false)
	{
		std::cerr << "Failed to read warp code file " 
				<< warp_code_path.string() 
				<< "\n";
	}
	auto object_code_path = std::filesystem::path(warp_code_path.string() + std::string{".o"});
	std::cout << "Object Code Path: " << object_code_path << "\n";
	const static std::string default_triple = llvm::sys::getDefaultTargetTriple();
	std::cout << "Compiling With Triplet: " << default_triple << "\n";
	bool success = LLVM::compile(&context, warp_code_.value(), object_code_path, default_triple);
	if(success == true)
		std::cout << "Success!\n";
	else
		std::cout << "Failed to compile\n";
	return 0;
}

std::optional<std::string> read_file(std::filesystem::path file_path)
{
	if(std::filesystem::exists(file_path) == false)
		return std::nullopt;
	std::ifstream input_media_stream(file_path);
	if(input_media_stream.is_open() == false && input_media_stream.good() == false)
		return std::nullopt;
	std::stringstream input_media_buffer;
	std::string line;
	while(input_media_stream.eof() == false) {
		std::getline(input_media_stream, line);
		input_media_buffer << line << "\n";
	}
	return input_media_buffer.str();
}

LLVM::Context context = LLVM::Context();

