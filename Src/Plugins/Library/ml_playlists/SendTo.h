#ifndef NULLSOFT_SEND_TO_HELPER_CLASS_H
#define NULLSOFT_SEND_TO_HELPER_CLASS_H

#include "../../General/gen_ml/ml_ipc.h"
#include "main.h"
#include <strsafe.h>
class SendToMenu
{
public:
	SendToMenu()
	{
		memset(&sendTo, 0, sizeof(sendTo));
	}
	void AddHere(HWND hwnd, HMENU hMenu, int type, int simple = 0, int trueSourceType = 0)
	{
		sendTo.mode = 0;
		sendTo.hwnd = 0;
		sendTo.build_hMenu = 0;

		IPC_LIBRARY_SENDTOMENU = SendMessage(plugin.hwndWinampParent, WM_WA_IPC, (WPARAM)&"LibrarySendToMenu", IPC_REGISTER_WINAMP_IPCMESSAGE);
		if (IPC_LIBRARY_SENDTOMENU > 65536 && SendMessage(plugin.hwndWinampParent, WM_WA_IPC, (WPARAM)0, IPC_LIBRARY_SENDTOMENU) == (LRESULT)-1)
		{
			sendTo.mode = 1;
			sendTo.hwnd = hwnd;
			sendTo.data_type = type; //ML_TYPE_ITEMRECORDLIST;
			sendTo.ctx[1] = simple;
			sendTo.ctx[2] = trueSourceType;
			sendTo.build_hMenu = hMenu;
		}
	}
	bool WasClicked(int popUpReturnVal)
	{
		if (sendTo.mode == 2)
		{
			sendTo.menu_id = popUpReturnVal;
			if (SendMessage(plugin.hwndWinampParent, WM_WA_IPC, (WPARAM)&sendTo, IPC_LIBRARY_SENDTOMENU) == (LRESULT)-1)
				return true;
		}
		return false;
	}
	void Cleanup()
	{
		if (sendTo.mode)
		{
			sendTo.mode = 4;
			SendMessage(plugin.hwndWinampParent, WM_WA_IPC, (WPARAM)&sendTo, IPC_LIBRARY_SENDTOMENU); // cleanup
		}
		sendTo.build_hMenu = 0;
	}

	bool InitPopupMenu(WPARAM wParam)
	{
		if (wParam && (HMENU)wParam == sendTo.build_hMenu && sendTo.mode == 1)
		{
			if (SendMessage(plugin.hwndWinampParent, WM_WA_IPC, (WPARAM)&sendTo, IPC_LIBRARY_SENDTOMENU) == (LRESULT)-1)
				sendTo.mode = 2;

			return true;
		}
		return false;
	}

	// still need to free it on your own
	void SendItemRecordList(itemRecordList *obj)
	{
		sendTo.data_type = ML_TYPE_ITEMRECORDLIST;
		sendTo.mode = 3;
		sendTo.data = (void*) & obj;
		SendMessage(plugin.hwndWinampParent, WM_WA_IPC, (WPARAM)&sendTo, IPC_LIBRARY_SENDTOMENU);
	}

	void SendFilenames(const wchar_t *filenames)
	{
		sendTo.data_type = ML_TYPE_FILENAMESW;
		sendTo.mode = 3;
		sendTo.data = (void*)filenames;
		SendMessage(plugin.hwndWinampParent, WM_WA_IPC, (WPARAM)&sendTo, IPC_LIBRARY_SENDTOMENU);
	}

	LRESULT SendPlaylist(mlPlaylist *playlist)
	{
		sendTo.data_type = ML_TYPE_PLAYLIST;
		sendTo.mode = 3;
		sendTo.data = (void*)playlist;
		return SendMessage(plugin.hwndWinampParent, WM_WA_IPC, (WPARAM)&sendTo, IPC_LIBRARY_SENDTOMENU);
	}

	LRESULT SendPlaylists(mlPlaylist **playlists)
	{
		sendTo.data_type = ML_TYPE_PLAYLISTS;
		sendTo.mode = 3;
		sendTo.data = (void*)playlists;
		return SendMessage(plugin.hwndWinampParent, WM_WA_IPC, (WPARAM)&sendTo, IPC_LIBRARY_SENDTOMENU);
	}

private:
	librarySendToMenuStruct sendTo;
};

#endif