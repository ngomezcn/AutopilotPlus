#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"

#define LOG_LEVEL spdlog::level::trace
//#define LOG_TRACE(...) spdlog::trace(__VA_ARGS__)
//#define Log::debug(...) spdlog::debug(__VA_ARGS__)
//#define Log::info(...)  spdlog::info(__VA_ARGS__)
//#define Log::warning(...) spdlog::warn(__VA_ARGS__)
//#define Log::error(...) spdlog::error(__VA_ARGS__)

class Log
{
public:
	//std::string LOG_UINT_ARRAY(const unsigned char* uchar_arr, const int size)
	//{
	//	std::string str_array;
	//	for (int i = 0; i < size; i++)
	//	{
	//		str_array.append(std::to_string(uchar_arr[i]));
	//	}
	//	return str_array;
	//}

	static void setSettings()
	{
		const char* log_pattern = "[%t] %^[%l]%$ %v";
		spdlog::set_pattern(log_pattern);
		spdlog::set_level(LOG_LEVEL);

		info("Setting logger => Level:{0}, Pattern: '{1}'", LOG_LEVEL, log_pattern);
	}

	template<typename... Args>
	static void trace(spdlog::format_string_t<Args...> fmt, Args &&... args)
 	{
		spdlog::trace(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void debug(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		spdlog::debug(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void info(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		spdlog::info(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void info_nl(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		std::cout << "\n" << std::endl;
		spdlog::info(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void warning(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		spdlog::warn(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void error(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		spdlog::error(fmt, std::forward<Args>(args)...);
	}
};

	//std::string LOG_CHAR_ARRAY(const unsigned char* uint_arr, const int size)
	//{
	//	std::string str_array;
	//	for (int i = 0; i < size; i++)
	//	{
	//		str_array.append(std::to_string(uint_arr[i]));
	//	}
	//	return str_array;
	//}

#endif /* LOGGER_H */