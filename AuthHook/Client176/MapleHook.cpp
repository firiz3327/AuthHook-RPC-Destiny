#include "Global.h"
#include <intrin.h>

bool Hook_WriteStageLogA(bool bEnable)
{
	typedef int(__cdecl* pWriteStageLogA)(int nIdx, ZXString szMessage);
	static auto _WriteStageLogA = (pWriteStageLogA)(GetFuncAddress("nxgsm", "WriteStageLogA"));

	pWriteStageLogA Hook = [](int nIdx, ZXString szMessage) -> int
	{
		Log("WriteStageLogA: %s", szMessage);
		return 0;
	};

	return SetHook(bEnable, reinterpret_cast<void**>(&_WriteStageLogA), Hook);
}

bool Hook_WriteErrorLogA(bool bEnable)
{
	typedef int(__cdecl* pWriteErrorLogA)(int nIdx, ZXString szMessage);
	static auto _WriteErrorLogA = (pWriteErrorLogA)(GetFuncAddress("nxgsm", "WriteErrorLogA"));

	pWriteErrorLogA Hook = [](int nIdx, ZXString szMessage) -> int
	{
		Log("WriteErrorLogA: %s", szMessage);
		return 0;
	};

	return SetHook(bEnable, reinterpret_cast<void**>(&_WriteErrorLogA), Hook);
}

bool Hook_SetProgramState(bool bEnable)
{
	typedef int(__cdecl* SetProgramState_t)(int nState);
	static auto _SetProgramState = reinterpret_cast<SetProgramState_t>(0x0195F250);

	SetProgramState_t Hook = [](int nState) -> int
	{
		auto ret = _ReturnAddress();
		Log("SetProgramState %d [%#08x]", nState, ret);
		return _SetProgramState(nState);
	};

	return SetHook(bEnable, reinterpret_cast<void**>(&_SetProgramState), Hook);
}

/*bool Hook_StringPool__GetString(bool enable)
{
	typedef ZXString*(__fastcall* StringPool__GetString_t)(void* ecx, void* edx, ZXString* result, unsigned int nIdx, char formal);
	static auto StringPool__GetString = reinterpret_cast<StringPool__GetString_t>(0x00EC3BF0);

	StringPool__GetString_t Hook = [](void* ecx, void* edx, ZXString* result, unsigned int nIdx, char formal) -> ZXString*
	{
		auto ret = StringPool__GetString(ecx, edx, result, nIdx, formal);

		Log("StringPool__GetString: %s", *result);

		return ret;
	};

	return SetHook(enable, reinterpret_cast<void**>(&StringPool__GetString), Hook);
}*/

bool HookMapleApi()
{
	bool bResult = true;

	bResult &= Hook_WriteStageLogA(true);
	bResult &= Hook_WriteErrorLogA(true);
	//bResult &= Hook_SetProgramState(true);
	//bResult &= Hook_StringPool__GetString(true);

	return bResult;
}


bool Hook_IsValidCharacterName()
{
	typedef int(__fastcall* CCurseProcess__IsValidCharacterName_t)(void* a1, void* a2, ZXStringT<char>* sCharacterName, void* a4);
	static auto CCurseProcess__IsValidCharacterName = reinterpret_cast<CCurseProcess__IsValidCharacterName_t>(0x00EC5EF0); //v176
	//static auto CCurseProcess__IsValidCharacterName = reinterpret_cast<CCurseProcess__IsValidCharacterName_t>(0x00C4EB70); //v203.4

	CCurseProcess__IsValidCharacterName_t Hook = [](void* a1, void* a2, ZXStringT<char>* sCharacterName, void* a4) -> int
	{
		auto validName = CCurseProcess__IsValidCharacterName(a1, a2, sCharacterName, a4);
		if (!validName) {
			auto strLength = *(sCharacterName->m_pStr - 1);
			auto strIndex = 0;
			do {
				auto chr = sCharacterName->m_pStr[strIndex];

				if (chr < 0x61 || chr > 0xFB) {
					validName = true;
				}
				else {
					validName = false;
					break;
				}

				++strIndex;
			} while (strIndex < strLength);
		}

		return validName;
	};
	return SetHook(true, reinterpret_cast<void**>(&CCurseProcess__IsValidCharacterName), Hook);
}

typedef void(__fastcall* ZXString_char__Assign_t)(void* pThis, void* edx, const char* s, int n);
ZXString_char__Assign_t ZXString_char__Assign = reinterpret_cast<ZXString_char__Assign_t>(0x0047EDD0); //176
bool Hook_StringPool__GetString() {

	typedef ZXStringT<char>* (__fastcall* StringPool__GetString_t)(void* ecx, void* edx, ZXStringT<char>* result, unsigned int nIdx, char formal);
	static auto StringPool__GetString = reinterpret_cast<StringPool__GetString_t>(0x00EC3BF0); //176
	//static auto StringPool__GetString = reinterpret_cast<StringPool__GetString_t>(0x00C4C980); //203.4

	StringPool__GetString_t Hook = [](void* ecx, void* edx, ZXStringT<char>* result, unsigned int nIdx, char formal) -> ZXStringT<char>*
	{
		StringPool__GetString(ecx, edx, result, nIdx, formal);

		if (0 == strcmp(result->m_pStr, "Arial") || 0 == strcmp(result->m_pStr, "Arial Narrow"))
		{
			//const char* szFontName = "Leelawadee";
			const char* szFontName = "Yu Gothic UI Regular";
#ifdef  _DEBUG
			Log("[MapleLog] [StringPool::GetString-%d] %s -> %s", nIdx, result->m_pStr, szFontName);
#endif

			result->m_pStr = 0;
			ZXString_char__Assign(result, edx, szFontName, -1);
		}

		return result;
	};
	return SetHook(true, reinterpret_cast<void**>(&StringPool__GetString), Hook);
}

void FuckMaple()
{
	Log(__FUNCTION__);

	{	//NGS Removal
		PatchRetZero(0x01960B00);
	}

	{	//MSCRC Bypass
		PatchJmp(0x019DD7AD, 0x019DD844);
	}

	// RipOffLatinCheck
	*(BYTE*)(0x0193EDF8 + 0) = 0xFB; //CWndMan::TranslateMessage Enable Latin 0xFB like THMS
	*(BYTE*)(0x006F4325 + 1) = 0xFB; //CCtrlEdit::InsertString
	*(BYTE*)(0x00EC5787 + 1) = 0xFB; //CCurseProcess::IsValidCharacterName
	*(BYTE*)(0x00EC5A7D + 1) = 0xFB;
	*(BYTE*)(0x018C63FF + 1) = 0xFB;
	*(BYTE*)(0x01AD0C44 + 1) = 0xFB;
	*(BYTE*)(0x01AECA15 + 1) = 0xFB;
	// ???
	*(BYTE*)(0x00EC5F8A + 1) = 0xFB; //CCurseProcess::IsValidCharacterName

	// DisablePacketStringCheck
	PatchNop(0x01547C2C, 7); //CUIStatusBar::OnKey
	PatchNop(0x016FD78D, 9); //CUser::OnChat

	Hook_IsValidCharacterName();
	Hook_StringPool__GetString();

	{	//Logo Skipper
		//PatchNop(0x00B31AE6, 2);
		//PatchNop(0x00B31AEA, 2);
		//PatchNop(0x00B31AF7, 2);
	}
}