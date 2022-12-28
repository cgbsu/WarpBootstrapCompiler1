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
			std::cerr << "Error Looking Up Target: " << error << "\n";
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
		if(error_code) {
			std::cerr << "Failed to open file: " << error_code.message();
			llvm::errs() << "Failed to open file: " << error_code.message();
		}
		llvm::legacy::PassManager pass;
		auto file_type = llvm::CGFT_ObjectFile;
		if(target_machine->addPassesToEmitFile(pass, output_media_stream, nullptr, file_type)) {
			std::cerr << "Error: The target machine can not emit files of this type!\n";
			llvm::errs() << "Error: The target machine can not emit files of this type!\n";
			return false;
		}
		pass.run(to_write);
		output_media_stream.flush();
		std::cout << "Wrote object code to " << file_name << "\n";
		llvm::outs() << "Wrote object code to " << file_name << "\n";
		return true;
	}
			

	bool compile(
			Context* context, 
			std::string code, 
			std::filesystem::path output_file_path, 
			std::string target_triple, 
			std::string target_cpu, 
			std::string target_features, 
			llvm::TargetOptions target_options, 
			llvm::Optional<llvm::Reloc::Model> target_reloc_model, 
			bool debug
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
		if(llvm::verifyModule(context->module) == false) {
			std::cerr << "Error: Failed to verify module!";
			return false;
		}
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

