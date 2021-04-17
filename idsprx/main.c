#include <pspsdk.h>
#include <pspkernel.h>
#include <pspidstorage.h>
#include <string.h>

PSP_MODULE_INFO("IdStorage", 0x1006, 1, 0);
PSP_MAIN_THREAD_ATTR(0);

int sceIdStorageCreateLeaf(int key);
int sceIdStorageDeleteLeaf(int key);

int ReadKey(int key, char *buffer)
{
	int err;
	u32 k1;

	k1 = pspSdkSetK1(0);

	memset(buffer, 0, 512);
	err = sceIdStorageReadLeaf(key, buffer);

	pspSdkSetK1(k1);

	return err;
}

int WriteKey(int key, char *buffer)
{
	int err;
	u32 k1;
	char buffer2[512];

	k1 = pspSdkSetK1(0);

	err = sceIdStorageReadLeaf(key, buffer2);
	if(err < 0)
		sceIdStorageCreateLeaf(key);

	err = sceIdStorageWriteLeaf(key, buffer);
	sceIdStorageFlush();

	pspSdkSetK1(k1);

	return err;
}

int CreateKey(int key)
{
	int err;
	u32 k1;

	k1 = pspSdkSetK1(0);

	err = sceIdStorageCreateLeaf(key);
	sceIdStorageFlush();

	pspSdkSetK1(k1);

	return err;
}

int DeleteKey(int key)
{
	int err;
	u32 k1;

	k1 = pspSdkSetK1(0);

	err = sceIdStorageDeleteLeaf(key);
	sceIdStorageFlush();

	pspSdkSetK1(k1);

	return err;
}

int module_start(SceSize args, void *argp)
{
	return 0;
}

int module_stop()
{
	return 0;
}
