/**********************************************************\

  Auto-generated CamStudioHelperAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "CamStudioHelperAPI.h"

#include <Windows.h>
#include <ShellAPI.h>
#include <atlbase.h>
#include <atlconv.h>

///////////////////////////////////////////////////////////////////////////////
/// @fn FB::variant CamStudioHelperAPI::echo(const FB::variant& msg)
///
/// @brief  Echos whatever is passed from Javascript.
///         Go ahead and change it. See what happens!
///////////////////////////////////////////////////////////////////////////////
FB::variant CamStudioHelperAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo("So far, you clicked this many times: ", n++);

    // return "foobar";
    return msg;
}

///////////////////////////////////////////////////////////////////////////////
/// @fn CamStudioHelperPtr CamStudioHelperAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
CamStudioHelperPtr CamStudioHelperAPI::getPlugin()
{
    CamStudioHelperPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

// Read/Write property testString
std::string CamStudioHelperAPI::get_testString()
{
    return m_testString;
}

void CamStudioHelperAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string CamStudioHelperAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

void CamStudioHelperAPI::testEvent()
{
    fire_test();
}

/************************************************************************/
/* return value: error:0, successfully:1                                */
/************************************************************************/
int CamStudioHelperAPI::callCamStudio()
{
	// 	ShellExecute( NULL, NULL, L"notepad.exe", L"", NULL, SW_SHOW);
	std::string location = "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{04B83666-3A62-452B-85D3-70F8117F2329}_is1";
	std::string name = "InstallLocation";
	std::string value;
	value = getRegKey(location, name);
	if(value.length() == 0)
		return 0;

	value.append("Recorder.exe");
	LPCSTR exe = value.c_str();
	// 	std::string exe_str = "D:\\Program\ Files\ (x86)\\Mozilla\ Firefox\\firefox.exe";
	// 	LPCSTR exe = exe_str.c_str();
	// 	LPCWSTR w_exe = A2CW(exe);
	// 	std::wstring some = new std::wstring(exe_str.c_str());
	ShellExecuteA( NULL, NULL, exe, "", NULL, SW_SHOW);

	return 1;
}

/************************************************************************/
/* return value: error:0, successfully:1                                */
/************************************************************************/
int CamStudioHelperAPI::callTencent()
{
	// 	ShellExecute( NULL, NULL, L"notepad.exe", L"", NULL, SW_SHOW);
	std::string location = "Software\\Tencent\\RTXC";
	std::string name = "INSTDIR";
	std::string value;
	value = getRegKey(location, name);
	if(value.length() == 0)
		return 0;

	value.append("RTX.exe");
	LPCSTR exe = value.c_str();
	// 	std::string exe_str = "D:\\Program\ Files\ (x86)\\Mozilla\ Firefox\\firefox.exe";
	// 	LPCSTR exe = exe_str.c_str();
	// 	LPCWSTR w_exe = A2CW(exe);
	// 	std::wstring some = new std::wstring(exe_str.c_str());
	ShellExecuteA( NULL, NULL, exe, "", NULL, SW_SHOW);

	return 1;
}


/**
* @param location The location of the registry key. For example "Software\\Bethesda Softworks\\Morrowind"
* @param name the name of the registry key, for example "Installed Path"
* @return the value of the key or an empty string if an error occured.
*/
std::string CamStudioHelperAPI::getRegKey(const std::string& location, const std::string& name){
	HKEY key;
	char value[1024];
	memset(value, 0, sizeof(value));
	DWORD bufLen = 1024*sizeof(char);
	long ret;
	REGSAM flag = KEY_WOW64_32KEY;
	// 	ret = RegOpenKeyExA(HKEY_LOCAL_MACHINE, location.c_str(), 0, KEY_ALL_ACCESS | flag, &key);
	ret = RegOpenKeyExA(HKEY_LOCAL_MACHINE, location.c_str(), 0, KEY_QUERY_VALUE | flag, &key);
	if( ret != ERROR_SUCCESS ){
		flag = KEY_WOW64_64KEY;
		ret =  RegOpenKeyExA(HKEY_LOCAL_MACHINE, location.c_str(), 0, KEY_QUERY_VALUE | flag, &key);
		if(ret != ERROR_SUCCESS)
			return std::string();
	}
	ret = RegQueryValueExA(key, name.c_str(), 0, 0, (LPBYTE) value, &bufLen);
	RegCloseKey(key);
	if ( (ret != ERROR_SUCCESS) || (bufLen > 1024*sizeof(char)) ){
		return std::string();
	}
	std::string stringValue = std::string(value);
	return stringValue;
}

int CamStudioHelperAPI::loginRTX(std::string server, long port, std::string usr, std::string pwd)
{
	HRESULT hr = NULL;
	IDispatch* pIDispatch = NULL;
	wchar_t progid[] = L"rtxclient.rtxapi";
	CLSID clsid;

	hr = ::CLSIDFromProgID(progid, &clsid);
	hr = ::CoInitialize(NULL);
	hr = ::CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void **)&pIDispatch);
	if(!SUCCEEDED(hr))
	{
		return 0;
	}
	DISPID dispid;
	OLECHAR* name = L"GetObject";
	hr = pIDispatch->GetIDsOfNames(IID_NULL, &name, 1, GetUserDefaultLCID(), &dispid);
	if(!SUCCEEDED(hr))
		return -2; // get method error

	UINT iError = -1;
	VARIANT rarg;
	::VariantInit(&rarg);
	VARIANT var[1];
	::VariantInit(&var[0]);
	var[0].vt = VT_BSTR;
	var[0].bstrVal = ::SysAllocString(L"KernalRoot");

	DISPPARAMS param;
	param.cArgs = 1;
	param.rgvarg = var;
	param.cNamedArgs = 0;
	param.rgdispidNamedArgs = NULL;

	hr = pIDispatch->Invoke(dispid, IID_NULL, GetUserDefaultLCID(), DISPATCH_METHOD, &param, &rarg, NULL, &iError);
	if(!SUCCEEDED(hr))
		return -3; // get kernelRoot fail

	IRTXCRootPtr rootPtr = rarg.pdispVal;
	rootPtr->Logout();
// 	BSTR server = ::SysAllocString(L"172.16.39.188");
	BSTR b_server = A2BSTR(server.c_str());

	VARIANT userVar;
	::VariantInit(&userVar);
	userVar.vt = VT_BSTR;
// 	userVar.bstrVal = ::SysAllocString(L"wanghao");
	userVar.bstrVal = A2BSTR(usr.c_str());
// 	BSTR pwd = ::SysAllocString(L"123456");
	BSTR b_pwd = A2BSTR(pwd.c_str());

	rootPtr->Login(b_server, port, userVar, b_pwd);
	::VariantClear(&rarg);
	::VariantClear(&var[0]);
	::CoInitialize(NULL);

	return 1;
}
