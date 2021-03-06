#include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <kubridge.h>
#include "ids_prx.h"

PSP_MODULE_INFO("TA-095v2_Downgrader", 0x0800, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

#define printf pspDebugScreenPrintf
#define FW(f) ((((f >> 8) & 0xF) << 24) | (((f >> 4) & 0xF) << 16) | ((f & 0xF) << 8) | 0x10)

//#define DEBUG

int ReadKey(int key, char *buffer);
int WriteKey(int key, char *buffer);
int CreateKey(int key);
int DeleteKey(int key);


void wait_release(unsigned int buttons)
{
	SceCtrlData pad;
	sceCtrlReadBufferPositive(&pad, 1);

	while(pad.Buttons & buttons)
	{
		sceKernelDelayThread(100000);
		sceCtrlReadBufferPositive(&pad, 1);
	}
}

unsigned int wait_press(unsigned int buttons)
{
	SceCtrlData pad;
	sceCtrlReadBufferPositive(&pad, 1);

	while(1)
	{
		if(pad.Buttons & buttons)
			return pad.Buttons & buttons;

		sceKernelDelayThread(100000);
		sceCtrlReadBufferPositive(&pad, 1);
	}
	return 0;
}

void Fix51Key()
{
	char buffer51[512];
	int key = ReadKey(0x51, buffer51);

	printf("- Fixing 0x51 IDS key.\n");

	if(key != 0)// Creating new key if missed
	{
		printf("- Key is not founded, creating new key.\n");
		CreateKey(0x51);

		unsigned char new_buffer51[512] = {
			0x36, 0x2E, 0x33, 0x39, 0x00, 0x00, 0x00, 0x00, 0x34, 0x87, 0x6A, 0xE6, 0xF0, 0x6C, 0x89, 0xFA, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};
		
		WriteKey(0x51, (char *)new_buffer51);
	}
	else if(strncmp(buffer51, "6.39", 4) != 0)// Fixing key
	{
		printf("- Key is founded. Key 0x51 version: %c.%c%c. Fixing.\n", buffer51[0], buffer51[2], buffer51[3]);
		strcpy(buffer51, "6.39");
		WriteKey(0x51, buffer51);
	}
	else// Not needed fixing
	{
		printf("- Key is correct. Key 0x51 version: %c.%c%c. Not needed fixing.\n", buffer51[0], buffer51[2], buffer51[3]);
		return;
	}

	// Checking key
	printf("- Checking fixed or created key.\n");
	key = ReadKey(0x51, buffer51);
	if(key != 0)
		printf("- Error! Key is not created.\n");
	else if(strncmp(buffer51, "6.39", 4) != 0)
		printf("- Error! Key is not fixed.\n");
	else
		printf("- Key fixing or creating is complete success. Current key 0x51 version: %c.%c%c.\n\n", buffer51[0], buffer51[2], buffer51[3]);

	return;
}

static u8 g_buf[64 * 1024] __attribute__((aligned(64)));

int CopyIndex()
{
	SceUID fd = -1, fdw = -1;
	int ret = -1;

	printf("- Copying index_09g.dat to index index_04g.dat... ");

	ret = sceIoOpen("flash0:/vsh/etc/index_09g.dat", PSP_O_RDONLY, 777);
	if(ret < 0)
	{
		printf("\n- Error! Not founded index_09g.dat\n");
		goto ERROR;
	}

	fd = ret;

	ret = sceIoOpen("flash0:/vsh/etc/index_04g.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 777);
	if(ret < 0)
	{
		printf("\n- Error! Can't open output file (index_04g.dat) for writing.\n");
		goto ERROR;
	}

	fdw = ret;
	ret = sizeof(g_buf);
	ret = sceIoRead(fd, g_buf, ret);

	while(ret > 0)
	{
		ret = sceIoWrite(fdw, g_buf, ret);
		if(ret < 0)
		{
			printf("\n- Error! Can't write index_04g.dat.\n");
			goto ERROR;
		}

		ret = sceIoRead(fd, g_buf, ret);
	}

	if(ret < 0)
	{
		printf("\n- Error! Can't read index_04g.dat.\n");
		goto ERROR;
	}

	sceIoClose(fd);
	sceIoClose(fdw);

	printf("Ok\n- Checking index_04g.dat... ");
	fd = sceIoOpen("flash0:/vsh/etc/index_04g.dat", PSP_O_RDONLY, 777);
	if(fd >= 0)
		printf("Ok\n");
	else
		printf("Error! Not founded index_04g.dat.\n");

	return 0;

ERROR:
	sceIoClose(fd);
	sceIoClose(fdw);
	
	printf("\n");

	return ret;
}

void InitFlash()
{
	int ret = sceIoUnassign("flash0:");

	while(ret < 0)
	{
		ret = sceIoUnassign("flash0:");
		sceKernelDelayThread(500000);
	}

	sceIoAssign("flash0:", "lflash0:0,0", "flashfat0:", 0, NULL, 0);
}

void ErrorExit(char *fmt, ...)
{
	char msg[256];

	va_list list;
	va_start(list, fmt);
	vsprintf(msg, fmt, list);
	va_end(list);
	printf(msg);
	sceKernelDelayThread(3 * 1000 * 1000);
	sceKernelExitGame(); 
}

void LoadStartModule()
{
	sceIoRemove("ids.prx");
	SceUID out = sceIoOpen("ids.prx", PSP_O_WRONLY | PSP_O_CREAT, 0777);
	sceIoWrite(out, ids_prx, size_ids_prx);
	sceIoClose(out);

	SceUID mod = pspSdkLoadStartModule("ids.prx", PSP_MEMORY_PARTITION_KERNEL);
	sceIoRemove("ids.prx");

	if(mod < 0)
		ErrorExit(" Error 0x%08X loading/starting ids.prx from buffer.\n", mod);
}

int main(void)
{
	pspDebugScreenInit();
	LoadStartModule();

#ifndef DEBUG
	int devkit = sceKernelDevkitVersion();
	char *devkit_chr = (char *)&devkit;
	if(devkit > FW(0x660) || devkit < FW(0x631))
		ErrorExit(" Firmware error! Sorry, your FW is not supported: %i.%i%i.\n", devkit_chr[3], devkit_chr[2], devkit_chr[1]);
	
	if(kuKernelGetModel() != 8)
		ErrorExit(" Model error! It's program only for PSP with 9g module.\n");
#endif

	printf(
	" 6.3x/6.60 -> 6.20 TA-095v2 PRE-Downgrader by frostegater\n"
	"  Press X to start.\n\n");

	for(;;)
	{
		unsigned int b = wait_press(0xFFFF);
		wait_release(0xFFFF);
		if(b & PSP_CTRL_CROSS)
		{
			InitFlash();
			CopyIndex();
			Fix51Key();
			printf("- Press X to exit.\n");

			for(;;)
			{
				int b = wait_press(0xFFFF);
				wait_release(0xFFFF);
				if(b & PSP_CTRL_CROSS)
					break;
			}

			sceKernelExitGame();
		}
	}

	return 0;
}
