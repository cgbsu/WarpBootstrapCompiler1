#include <Warp/Common.hpp>
#define WARP__BOOTSTRAP__COMPILER__COMPILE__LATEST__PARSER__CACHE
#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>
#include <Warp/Cache/FunctionParser.hpp>
#include <Warp/SyntaxTranslation/LLVM/Translators/FunctionDeclaration.hpp>

namespace Warp::SyntaxTranslation::LLVM
{
	void initialize_llvm()
	{
		//llvm::InitializeModuleAndPassManager();
		llvm::InitializeAllTargetInfos();
		llvm::InitializeAllTargets();
		llvm::InitializeAllTargetMCs();
		llvm::InitializeAllAsmParsers();
		llvm::InitializeAllAsmPrinters();
	}

	//Triple::llvm::Triple(target_triple).getArch()
	std::optional<llvm::TargetMachine*> initialize_target(
			llvm::Module& to_compile, 
			std::string target_triple,
			std::string target_cpu, 
			std::string target_features, 
			llvm::TargetOptions target_options, 
			llvm::Optional<llvm::Reloc::Model> target_reloc_model
		)
	{
		to_compile.setTargetTriple(target_triple);
		std::string error;
		auto target = llvm::TargetRegistry::lookupTarget(target_triple, error);	
		if(!target) {
			llvm::errs() << error;
			return std::nullopt;
		}
		auto target_machine = target->createTargetMachine(
				target_triple, 
				target_cpu, 
				target_features, 
				target_options, 
				target_reloc_model
			);
		to_compile.setDataLayout(target_machine->createDataLayout());
		return target_machine;
	}

	bool write_object_file(
			std::filesystem::path output_file_path, 
			llvm::Module& to_write, 
			llvm::TargetMachine* target_machine
		)
	{
		std::string file_name = output_file_path.string();
		std::error_code error_code;
		llvm::raw_fd_ostream output_media_stream(file_name, error_code, llvm::sys::fs::OF_None);
		if(error_code)
			llvm::errs() << "Failed to open file: " << error_code.message();
		llvm::legacy::PassManager pass;
		auto file_type = llvm::CGFT_ObjectFile;
		if(target_machine->addPassesToEmitFile(pass, output_media_stream, nullptr, file_type)) {
			llvm::errs() << "Error: The target machine can not emit files of this type!\n";
			return false;
		}
		pass.run(to_write);
		output_media_stream.flush();
		llvm::outs() << "Wrote object code to " << file_name << "\n";
		return true;
	}
			
	//std::filesystem::path default_clang_executable = std::filesystem::path("/") / "usr" / "bin" / "clang++-15";
	//// Thank you https://stackoverflow.com/questions/11657529/how-to-generate-an-executable-from-an-llvmmodule
	//void compile_to_executable(
	//		std::string target_triple, 
	//		std::filesystem::path warp_object_file, 
	//		std::filesystem::path executable_name, 
	//		std::filesystem::path clang_executable = default_clang_executable, 
	//		std::vector<std::filesystem::path> other_files = {}, 
	//		std::vector<std::string> additional_arguments = {}, 
	//	)
	//{
	//	llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diagnostic_options = new clang::DiagnosticOptions;
	//	clang::TestDiagnosticPrinter* dianostic_client = new clang::TextDiagnosticPrinter(llvm::errs(), &*diagnostic_options);
	//	llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diagnostic_identifiers(new clang::DiagnosticIds());
	//	clang::DiagnosticsEngine diagnostics(diagnostic_identifiers, &*diagnostic_options, diagnostic_client);
	//	clang::driver::Driver driver(clang_executable.string(), target_triple, diagnostics);
	//	
	//	std::vector<const char*> arguments;
	//	for(const auto& argument : additional_arguments)
	//		arguments.push_back(argument.data());
	//	std::vector<std::string> file_name_buffers;
	//	for(const auto& other_file : other_files) {
	//		file_name_buffers.push_back(other_file.string());
	//		arguments.push_back(file_name_buffers.back().data())
	//	}
	//	std::string object_file_name_buffer = warp_object_file.string();
	//	arguments.push_back(object_file_name_buffer.data());
	//	std::string output = std::string{"-o "} + executable_name;
	//	arguments.push_back(output.data());
	//	auto arguments_reference = ArrayRef<const char*>{arguments};
	//	std::unique_ptr<clang::driver::Compilation> compilation(dreiver.BuildCompilation(arguments_reference));
	//	if(compilation && !compilation->containsError()) {
	//		llvm::SmallVector<std::pair<int, const clang::driver::Command*>, 4> failing_commands;
	//		driver.ExecuteCompilation(*compilation, failing_commands);
	//	}
	//	else
	//		std::cout << "Error when attempting to generate executable.\n";
	//}

	bool compile(
			Context* context, 
			std::string code, 
			std::filesystem::path output_file_path, 
			std::string target_triple, 
			bool debug, 
			std::string target_cpu, 
			std::string target_features, 
			llvm::TargetOptions target_options, 
			llvm::Optional<llvm::Reloc::Model> target_reloc_model
		)
	{
		auto parse_result_ = parse_context(code);
		if(parse_result_.has_value() == false) {
			std::cerr << "Error: Failed to parse.\n";
			return false;
		}
		const auto parsed_context = std::move(parse_result_.value());
		for(const auto& function : parsed_context.functions)
		{
			if(debug == true)
				std::cout << "Compiling Function: " << function.first << "\n";
			auto llvm_function_ = translate_function(
					context, 
					&parsed_context, 
					debug, 
					*function.second.get()
				);
			if(llvm_function_.has_value() == false)
			{
				std::cerr << "Failed to translate " 
						<< function.second->get_name() 
						<< " to LLVM IR code\n";
				return false;
			}
		}
		//if(llvm::verifyModule(context->module) == false) {
		//	std::cerr << "Error: Failed to verify module!";
		//	return false;
		//}
		initialize_llvm();
		auto target_machine_ = initialize_target(
				context->module, 
				target_triple, 
				target_cpu,  
				target_features, 
				target_options, 
				target_reloc_model
			);
		if(target_machine_.has_value() == false)
			return false;
		auto target_machine = target_machine_.value();
		bool success = write_object_file(
				output_file_path, 
				context->module, 
				target_machine
			);
		if(success == false)
			std::cerr << "Error: Failed to write object code to file!\n";
		return success;
	}
}

