#include <Warp/Utilities/Conversions.hpp>

namespace Warp::Utilities
{
	enum class LogType
	{
		Debug, 
		Error, 
		Warning, 
		Note
	};


	namespace Detail
	{
		template<LogType = LogType::Debug>
		const char enum_string[] = "Debug";
		template<>
		const char enum_string<LogType::Error>[] = "Error";
		template<>
		const char enum_string<LogType::Warning>[] = "Warning";
		template<>
		const char enum_string<LogType::Note>[] = "Note";
		
		template<
				auto LogTypeParameterConstant, 
				bool VerboseParameterConstant, 
				bool LogSourceLocationParameterConstant
			>
		void log(
				auto& log, 
				std::source_location location = std::source_location::current(), 
				auto... to_log
			)
		{
			log << enum_string<LogTypeParameterConstant> << "::";
			if constexpr(LogSourceLocationParameterConstant == true)
			{
				if constexpr(VerboseParameterConstant == true)
				{
					log << location.file_name() 
						<< "[" << location.line() << "]" 
						<< "[" << location.column() << "]";
				}
				log << location.function_name() << ": ";
			}
			(log << ... << to_log); 
			log << "\n";
		}
	}
	template<
			auto LogTypeParameterConstant, 
			bool VerboseParameterConstant = false, 
			bool LogSourceLocationParameterConstant = true
		>
	struct Log
	{
		Log(std::source_location location = std::source_location::current()) : location(location) {}

		void operator[](auto& log, auto... to_log)
		{
			Warp::Utilities::Detail::log<
					LogTypeParameterConstant, 
					VerboseParameterConstant, 
					LogSourceLocationParameterConstant
				>(log, location, to_log... );
		}

		protected: 
			std::source_location location;
	};
}

