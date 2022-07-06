#include <Warp/Utilities/Conversions.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__LOGGING__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__LOGGING__HPP
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
				bool LogFunctionSignitureParameterConstant = true, 
				bool LogSourceLocationParameterConstant = false, 
				bool EnableDebuggingParameterConstant
			>
		void log(
				auto& log, 
				std::source_location location = std::source_location::current(), 
				auto... to_log
			)
		{
			if constexpr((LogTypeParameterConstant == LogType::Debug 
					&& EnableDebuggingParameterConstant == true)
					|| LogTypeParameterConstant != LogType::Debug)
			{
				log << enum_string<LogTypeParameterConstant> << "::";
				if constexpr(LogSourceLocationParameterConstant == true)
				{
					log << location.file_name() 
						<< "[" << location.line() << "]" 
						<< "[" << location.column() << "]";
				}
				if constexpr(LogFunctionSignitureParameterConstant == true)
					log << location.function_name() << ": ";
				(log << ... << to_log); 
				log << "\n";
			}
		}
	}
	template<
			auto LogTypeParameterConstant, 
			bool LogFunctionSignitureParameterConstant = true, 
			bool LogSourceLocationParameterConstant = false, 
			bool EnableDebuggingParameterConstant = Warp::Utilities::Detail::debug
		>
	struct Log
	{
		Log(std::source_location location = std::source_location::current()) 
				: location(location) {}

		void operator[](auto& log, auto... to_log)
		{
			Warp::Utilities::Detail::log<
					LogTypeParameterConstant, 
					LogFunctionSignitureParameterConstant, 
					LogSourceLocationParameterConstant, 
					EnableDebuggingParameterConstant
				>(log, location, to_log... );
		}

		protected: 
			std::source_location location;
	};
}
#endif // WARP__UTILITIES__HEADER__UTILITIES__LOGGING__HPP

