#include <windows.h> 
#include <mmdeviceapi.h> 
#include <endpointvolume.h>
#include <audioclient.h>
#include "Util_SoundAdminUtil.h"
#include <iostream>
#include <stdio.h>



GUID g_guidMyContext = GUID_NULL;
static IAudioEndpointVolume *g_pEndptVol = NULL;
IMMDeviceEnumerator *pEnumerator = NULL;
IMMDevice *pDevice = NULL;
float MAX_VOL = 100.0;

JNIEXPORT void JNICALL Java_Util_SoundAdminUtil_init
(JNIEnv *env, jobject)
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	jclass cls;
	cls = env->FindClass("Util/OperationFailedException");
	if (cls == NULL) {
		return;
	}
	HRESULT hr = S_OK;
	//CAudioEndpointVolumeCallback EPVolEvents;

	//得到g_guidMyContext hr只是结果
	hr = CoCreateGuid(&g_guidMyContext);
	if (FAILED(hr)) {
		env->ThrowNew(cls, "CoCreateGuid error");
	}
	// Get enumerator for audio endpoint devices. pEnumerator中
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&pEnumerator);
	if (FAILED(hr)) {
		printf("2");
		env->ThrowNew(cls, "CoCreateInstance error");
		printf("3");
	}
	// Get default audio-rendering device. pDevice中
	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	if (FAILED(hr)) {
		env->ThrowNew(cls, "pEnumerator->GetDefaultAudioEndpoint error");
	}
	hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_ALL, NULL, (void**)&g_pEndptVol);
	if (FAILED(hr)) {
		env->ThrowNew(cls, "pDevice->Activate error");
	}
}

JNIEXPORT void JNICALL Java_Util_SoundAdminUtil_setMasterVolume(JNIEnv *env,
	jobject obj,jint nVolume)
{
	jclass cls;
	cls = env->FindClass("Util/OperationFailedException");
	if (cls == NULL) {
		return;
	}
	float fVolume = (float)nVolume / MAX_VOL;//设置音量
	HRESULT hr = g_pEndptVol->SetMasterVolumeLevelScalar(fVolume, &g_guidMyContext);
	if (FAILED(hr)) {
		env->ThrowNew(cls, "setMasterVolume error");
	}
}

JNIEXPORT jint JNICALL Java_Util_SoundAdminUtil_getMasterVolume(JNIEnv *env,
	jobject obj)
{
	jclass cls;
	cls = env->FindClass("Util/OperationFailedException");
	if (cls == NULL) {
		return -1;
	}
	int ret;
	float fVolume;
	HRESULT hr = g_pEndptVol->GetMasterVolumeLevelScalar(&fVolume);//得到主音量的大小浮点数（0.0~1.0）
	if (FAILED(hr)) {
		env->ThrowNew(cls, "getMasterVolume error");
	}
	ret = (int)(MAX_VOL*fVolume + 0.5);//将音量转化为整数（0到100） MAX_VOL为100
	return ret;
}

JNIEXPORT void JNICALL Java_Util_SoundAdminUtil_setMute(JNIEnv *env,
	jobject obj,jboolean bMute)
{
	jclass cls;
	cls = env->FindClass("Util/OperationFailedException");
	if (cls == NULL) {
		return;
	}
	HRESULT hr = g_pEndptVol->SetMute(bMute, &g_guidMyContext);//设置静音
	if (FAILED(hr)) {
		env->ThrowNew(cls, "setMute error");
	}
}


JNIEXPORT jboolean JNICALL Java_Util_SoundAdminUtil_getMute(JNIEnv *env,
	jobject obj)
{
	jclass cls;
	cls = env->FindClass("Util/OperationFailedException");
	if (cls == NULL) {
		return false;
	}
	BOOL bMute;//是否静音
	HRESULT hr = g_pEndptVol->GetMute(&bMute);//得到静音状态
	if (FAILED(hr)) {
		env->ThrowNew(cls, "getMute error");
	}
	return bMute;
}

JNIEXPORT void JNICALL Java_Util_SoundAdminUtil_finished(JNIEnv *env,
	jobject obj)
{
	//释放指针
	//SAFE_RELEASE(pEnumerator);
	pEnumerator->Release();
	//SAFE_RELEASE(pDevice);
	pDevice->Release();
	//SAFE_RELEASE(g_pEndptVol);
	g_pEndptVol->Release();
	CoUninitialize();
}
