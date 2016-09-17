#include "pch.h"
#include "OpenCCx.h"
#include <assert.h>
#include <stdio.h>
#include <string>

using namespace OpenCCxRuntineComponent;
using namespace Platform;

#define TEMP_DATA_LEN	(1024 * 1024 * 5)

static char s_temp_data[TEMP_DATA_LEN];
static wchar_t s_temp_data_w[TEMP_DATA_LEN];
static char s_error_data[1024];

static char* s_cfg_names[] = {
	"opencc/s2t.json",

	"opencc/s2hk.json",
	"opencc/s2tw.json",
	"opencc/s2twp.json",

	"opencc/t2s.json",
	"opencc/hk2s.json",

	"opencc/t2hk.json",
	"opencc/t2tw.json",

	"opencc/tw2s.json",
	"opencc/tw2sp.json"
};

static void log(const char* x) {
	strcpy(s_error_data, x);
}

using namespace OpenCCxRuntineComponent;

OpenCCx::OpenCCx()
	: dllHandle_(NULL)
	, opencc_open_fun_(NULL)
	, opencc_close_fun_(NULL)
	, opencc_convert_utf8_to_buffer_fun_(NULL)
	, opencc_error_fun_ (NULL)
	, currConverter_(NULL)
{
}

bool OpenCCxRuntineComponent::OpenCCx::Load(ConverterType type)
{
	if (dllHandle_ == NULL) {
		if (!LoadDll()) {
			log("load dll failed");
			return false;
		}
	}

	if (opencc_open_fun_ == NULL) {
		log("load fun failed");
		return false;
	}

	auto itFound = loadedTypes_.find(type);
	if (itFound != loadedTypes_.end()) {
		currConverter_ = itFound->second;

		log("allready loaded.");
		return true;
	}

	opencc_t newConverter = opencc_open_fun_(s_cfg_names[(int)type]);
	if (newConverter == (opencc_t)-1) {
		log(opencc_error_fun_());
		return false;
	}

	loadedTypes_[type] = newConverter;
	currConverter_ = newConverter;

	log("load ok");
	return true;
}

void OpenCCxRuntineComponent::OpenCCx::Close()
{
	if (opencc_close_fun_) {
		for (auto it = loadedTypes_.begin(); it != loadedTypes_.end(); ++it) {
			opencc_close_fun_(it->second);
		}
	}
	loadedTypes_.clear();

	if (dllHandle_ != NULL) {
		FreeLibrary(dllHandle_);
		dllHandle_ = NULL;
	}

	opencc_open_fun_ = NULL;
	opencc_close_fun_ = NULL;
	opencc_convert_utf8_to_buffer_fun_ = NULL;
	opencc_error_fun_ = NULL;
}

String ^ OpenCCxRuntineComponent::OpenCCx::Convert(String ^ toConvert)
{
	if (opencc_convert_utf8_to_buffer_fun_ == NULL ||
		toConvert == nullptr ||
		currConverter_ == nullptr
		) {
		log("convert failed-not load");
		return nullptr;
	}

	const char* utf8ToConvert = ToUtf8(toConvert->Data());
	if (utf8ToConvert == nullptr) return nullptr;

	size_t retLen = opencc_convert_utf8_to_buffer_fun_(currConverter_, utf8ToConvert, -1, s_temp_data);
	if (retLen <= 0) {
		log(opencc_error_fun_());
		return nullptr;
	}

	const wchar_t* unicodeRet = ToUnicode(s_temp_data, retLen);

	log("convert ok");
	return ref new String(unicodeRet);
}

String ^ OpenCCxRuntineComponent::OpenCCx::Error()
{
	static wchar_t temp[1024] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, s_error_data, -1, temp, 1024);
	String^ str = ref new String(temp);
	return str;
}

bool OpenCCxRuntineComponent::OpenCCx::LoadDll()
{
	dllHandle_ = LoadPackagedLibrary(L"OpenCCx.dll", 0);
	if (dllHandle_ == NULL) {
		return false;
	}

	opencc_open_fun_ = (opencc_open_fun_t)GetProcAddress(dllHandle_, "opencc_open");
	opencc_close_fun_ = (opencc_close_fun_t)GetProcAddress(dllHandle_, "opencc_close");;
	opencc_convert_utf8_to_buffer_fun_ = (opencc_convert_utf8_to_buffer_fun_t)GetProcAddress(dllHandle_, "opencc_convert_utf8_to_buffer");;
	opencc_error_fun_ = (opencc_error_fun_t)GetProcAddress(dllHandle_, "opencc_error");
	return (
		opencc_open_fun_ != NULL &&
		opencc_close_fun_ != NULL &&
		opencc_convert_utf8_to_buffer_fun_ != NULL
		);
}

const char* OpenCCxRuntineComponent::OpenCCx::ToUtf8(const char16* toConvert) {
	int len = WideCharToMultiByte(CP_UTF8, 0, toConvert, -1, s_temp_data, TEMP_DATA_LEN, NULL, FALSE);
	if (len <= 0) return nullptr;
	s_temp_data[len] = 0;
	return s_temp_data;
}


const char16* OpenCCxRuntineComponent::OpenCCx::ToUnicode(const char* toConvert, size_t len) {
	int retLen = MultiByteToWideChar(CP_UTF8, 0, toConvert, len, s_temp_data_w, TEMP_DATA_LEN);
	if (retLen <= 0) return nullptr;
	s_temp_data_w[retLen] = 0;
	return s_temp_data_w;
}
