#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"

#define LOG_LEVEL spdlog::level::debug

#define LOG_TRACE(...) spdlog::trace(__VA_ARGS__)
#define LOG_DEBUG(...) spdlog::debug(__VA_ARGS__)
#define LOG_INFO(...)  spdlog::info(__VA_ARGS__)
#define LOG_WARNING(...) spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)



	//std::string LOG_UINT_ARRAY(const unsigned int* uint_arr, const int size)
	//{
	//	std::string str_array;
	//	for (int i = 0; i < size; i++)
	//	{
	//		str_array.append(std::to_string(uint_arr[i]));
	//	}
	//	return str_array;
	//}

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