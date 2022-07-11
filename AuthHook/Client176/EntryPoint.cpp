//---------------------------------------------------------------------------------------------
// v176.1 Localhost Enabler - Rajan
//---------------------------------------------------------------------------------------------

#include "Global.h"
#include <clocale>
#include <locale>

discord::Core* core{};

VOID WINAPI HeaderProc(DWORD dwPID)
{
	Log("------------------------------------------------");
	Log("[Rajan] [%s] [v%s] [PID: %d]", OPT_APPNAME, OPT_APPVERSION, dwPID);
	Log("[Rajan]       \\    /\\  ");
	Log("[Rajan]        )  ( ')   ");
	Log("[Rajan]       (  /  )    ");
	Log("[Rajan] cat    \\(__)|   ");
	Log("[Rajan] [Built: %s]", __TIMESTAMP__);
	Log("------------------------------------------------");
}

DWORD WINAPI MainProc(PVOID)
{
	#ifdef  _DEBUG
		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);
		printf("Debugging Window:\n");
	#endif

	DWORD dwCurProcId = GetCurrentProcessId();
	HeaderProc(dwCurProcId);

	auto locale = setlocale(LC_ALL, "ja_jp.utf8");
	//auto locale = setlocale(LC_ALL, "Japanese");
	if (!HookSockApi())
		Log("Failed Hooking SockApi");

	if (!HookWinApi())
		Log("Failed Hooking WinApi");

	if (!DestinyHookCanvas())
		Log("Failed Hooking Canvas");

	if (!HookMapleApi())
		Log("Failed Hooking MapleApi");

	if (!DestinyHookNMCO())
		Log("Failed Hooking NMCO");

	Log("All Hooked");

	return 0;
}

void StartRPC() {
	auto result = discord::Core::Create(RPC_CLIENT_ID, DiscordCreateFlags_NoRequireDiscord, &core);
	discord::Activity activity{};
	activity.SetDetails("Currently Grinding SpiritMS");
	activity.SetState("Bera");
	// Refer to Discord Developer Portal on what your ImageKeys are
	activity.GetAssets().SetLargeImage("spiritlogo");
	activity.GetAssets().SetSmallImage("spiritbanner");
	activity.GetAssets().SetLargeText("Spirit");
	activity.GetAssets().SetSmallText("A calm day in Spirit.");
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
	// Do something with results here
	});
	while (true) {
		Sleep(3000); // Update every 3 second
		::core->RunCallbacks();
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&MainProc, NULL, NULL, NULL);
		if (FindWindowA(NULL, "Discord") != NULL)
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&StartRPC, NULL, NULL, NULL);
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		//
	}

	return TRUE;
}