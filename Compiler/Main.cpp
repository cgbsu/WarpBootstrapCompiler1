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
	//if(argc >= 2)
	//	warp_code_path = std::filesystem::path(args[1]);
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
	//const static std::string default_triple = HOST_TRIPLET
	const static std::string default_triple = llvm::sys::getDefaultTargetTriple();
	std::cout << "Compiling Triplet: " << default_triple << "\n";
	std::cout << "Compiling Code: \n" << warp_code_.value() << "\n";
	bool success = LLVM::compile(&context, warp_code_.value(), object_code_path, default_triple);
	if(success == true)
		std::cout << "Success!\n";
	else
		std::cout << "Failed to compile\n";
	return 0;
}

std::optional<std::string> read_file(std::filesystem::path file_path)
{
	std::cout << "DEBUG READ FILE 0\n";
	if(std::filesystem::exists(file_path) == false)
		return std::nullopt;
	std::cout << "DEBUG READ FILE 1\n";
	std::ifstream input_media_stream(file_path);
	std::cout << "DEBUG READ FILE 2\n";
	if(input_media_stream.is_open() == false && input_media_stream.good() == false)
		return std::nullopt;
	std::cout << "DEBUG READ FILE 3\n";
	std::stringstream input_media_buffer;
	std::cout << "DEBUG READ FILE 4\n";
	std::string line;
	std::cout << "DEBUG READ FILE 5\n";
	while(input_media_stream.eof() == false) {
	std::cout << "DEBUG READ FILE 6\n";
		std::getline(input_media_stream, line);
	std::cout << "DEBUG READ FILE 7\n";
		input_media_buffer << line << "\n";
	std::cout << "DEBUG READ FILE 8\n";
	}
	std::cout << "DEBUG READ FILE 9\n";
	return input_media_buffer.str();
}

LLVM::Context context = LLVM::Context();

