//
// MainPage.xaml.cpp
// MainPage 类的实现。
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace CppTest2;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

#include <assert.h>


//“空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409 上有介绍

#pragma comment(lib, "../Debug/OpenCCx/OpenCCx.lib")

MainPage::MainPage()
{
	InitializeComponent();

	opencc_dll_handle_ = LoadPackagedLibrary(L"OpenCCx.dll", 0);

	typedef void*(*opencc_open_fun)(const char* cfg);
	opencc_open_fun open_fun = (opencc_open_fun)GetProcAddress(opencc_dll_handle_, "opencc_open");


		s2t_ = open_fun(OPENCC_DEFAULT_CONFIG_SIMP_TO_TRAD);
		t2s = open_fun(OPENCC_DEFAULT_CONFIG_TRAD_TO_SIMP);

	assert(s2t_ != nullptr);
}


void CppTest2::MainPage::btn2s_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	String^ str = textBox->Text->ToString();
	typedef char* (*opencc_convert_utf8_t)(opencc_t opencc, const char* input, size_t length);
	opencc_convert_utf8_t fun = (opencc_convert_utf8_t)GetProcAddress(opencc_dll_handle_, "opencc_convert_utf8");
	char* multi = new char[str->Length() * 3+ 2];
	int len = WideCharToMultiByte(CP_UTF8, 0, str->Data(), -1, multi, str->Length() * 3 + 2, NULL, FALSE);
	multi[len] = 0;

	char* ret = fun(s2t_, multi, -1);
	if (ret != nullptr) {
		wchar_t* utf8Str = new wchar_t[str->Length() + 1];
		MultiByteToWideChar(CP_UTF8, 0, ret, -1, utf8Str, str->Length() + 1);

		String^ x = ref new String(utf8Str);
		textBox->Text = x;

		opencc_convert_utf8_free(ret);
		delete[] utf8Str;
	}
}


void CppTest2::MainPage::btn2t_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}
