#pragma once

#include <Windows.h>

#include <map>

using namespace Platform;



namespace OpenCCxRuntineComponent
{

	typedef void* opencc_t;

	public enum class ConverterType : int32 {
		Simple2Traditional = 0,

		Simple2HK = 1,
		Simple2TW = 2,
		Simple2TWPhrases = 3,

		Traditioan2Simple = 4,
		HK2Simple = 5,

		Traditional2HK = 6,
		Traditional2TW = 7,

		TW2Simple = 8,
		TW2SimplePhrases = 9,
	};



	public ref class OpenCCx sealed
	{
	public:
		OpenCCx();

	public:
		bool Load(ConverterType type);
		void Close();
		String^ Convert(String^ toConvert);
		String^ Error();

	private:
	private:
		bool LoadDll();

		const char * ToUtf8(const char16 * toConvert);
		const char16 * ToUnicode(const char* toConvert, size_t len);

	private:
		typedef opencc_t(*opencc_open_fun_t)(const char* configFileName);
		typedef  int(*opencc_close_fun_t)(opencc_t opencc);
		typedef size_t(*opencc_convert_utf8_to_buffer_fun_t)(opencc_t opencc,
			const char* input,
			size_t length,
			char* output);
		typedef const char* (*opencc_error_fun_t)(void);

		opencc_open_fun_t opencc_open_fun_;
		opencc_close_fun_t opencc_close_fun_;
		opencc_convert_utf8_to_buffer_fun_t opencc_convert_utf8_to_buffer_fun_;
		opencc_error_fun_t opencc_error_fun_;


	private:
		HMODULE dllHandle_;
		std::map<ConverterType, opencc_t> loadedTypes_;
		opencc_t currConverter_;
	};

}
