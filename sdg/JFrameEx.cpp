#define _WIN32_WINNT 0x0501
#define WM_INPUT 0x00FF

#include <windows.h>
#include <windowsx.h>
#include <process.h>

#include <stdlib.h>
#include <string.h>


#include "jawt.h"
#include "jawt_md.h"

#include "JFrameEx.h"


#define MY_ASSERT(X,S) if (!X) { fprintf(stderr,"%s\n",S); return 0L;}

static void InitRawInput() {
	RAWINPUTDEVICE rid[50]; // allocate storage for 50 device (not going to need this many :) )

	rid[0].usUsagePage = 0x01; 
	rid[0].usUsage = 0x02; 
	rid[0].dwFlags = 0; //RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
	rid[0].hwndTarget = NULL;

	if (RegisterRawInputDevices(rid, 1, sizeof(rid[0])) == FALSE) {
//		wsprintf(mousemessage,"RawInput init failed: %s\n", GetLastError());
		//registration failed. 
	}
}

typedef jint (JNICALL *pJNI_GetCreatedJavaVMs)(JavaVM **, jsize, jsize *);

/* TODO: Move to another file of JNI utilities */
pJNI_GetCreatedJavaVMs getJavaVM()
{
	// Use the entrypoint of the current VM.
	HINSTANCE hMod = ::GetModuleHandle("msjava");
	if(hMod == NULL)
		hMod = ::GetModuleHandle("jvm");
	if(hMod == NULL)
		hMod = ::GetModuleHandle("javai");
	pJNI_GetCreatedJavaVMs pFunc = (pJNI_GetCreatedJavaVMs)::GetProcAddress(hMod,"JNI_GetCreatedJavaVMs");
	return pFunc;
}


static void DebugString(jobject frame, char *string) {
	JavaVM *vmBuf = NULL;
	JNIEnv *pEnv = NULL;
	jsize bufLen = 1;
	jint nVMs = 0;
	
	pJNI_GetCreatedJavaVMs pFunc = getJavaVM();
	(*pFunc)(&vmBuf, bufLen, &nVMs);
	
	vmBuf->AttachCurrentThread((void **) &pEnv, NULL);
	
	jclass cls = pEnv->GetObjectClass(frame);
	
	jmethodID mid = pEnv->GetMethodID(cls, "debug", "(Ljava/lang/String;)V");
	
	if (mid == 0) {
		return;
	}
		
	pEnv->CallVoidMethod(frame, mid, pEnv->NewStringUTF(string));
		
	vmBuf->DetachCurrentThread();
}

static void MouseActionPerformed(jobject frame, HANDLE hDevice, USHORT usFlags,
		ULONG ulButtons, USHORT usButtonFlags, USHORT usButtonData,
		ULONG ulRawButtons, LONG ILastX, LONG ILastY,
		ULONG ulExtraInformation) {
	JavaVM *vmBuf = NULL;
	JNIEnv *pEnv = NULL;
	jsize bufLen = 1;
	jint nVMs = 0;
	
	pJNI_GetCreatedJavaVMs pFunc = getJavaVM();
	(*pFunc)(&vmBuf, bufLen, &nVMs);
	
	vmBuf->AttachCurrentThread((void **) &pEnv, NULL);
	
	jclass cls = pEnv->GetObjectClass(frame);
	
	jmethodID mid = pEnv->GetMethodID(cls, "mouseActionPerformed", "(IIJIIJJJJ)V");
	
	if (mid == 0) {
		return;
	}
		
	pEnv->CallVoidMethod(frame, mid, (jint) hDevice, (jint) usFlags,
			(jlong) ulButtons, (jint) usButtonFlags, (jint) usButtonData,
			(jlong) ulRawButtons, (jlong) ILastX, (jlong) ILastY,
			(jlong) ulExtraInformation);
		
	vmBuf->DetachCurrentThread();
}

/*
 * Class:     JFrameEx
 * Method:    getHWND
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_JFrameEx_getHWND
  (JNIEnv *env, jobject obj_this) {
    jboolean result;
    jint lock;
    JAWT awt;
    JAWT_DrawingSurface *ds;
    JAWT_DrawingSurfaceInfo *dsi;
    JAWT_Win32DrawingSurfaceInfo *dsi_win;
    HDC hdc;
    HWND hWnd;

	/* Get the AWT */
	awt.version = JAWT_VERSION_1_3;
    result = JAWT_GetAWT(env, &awt);
    MY_ASSERT(result != JNI_FALSE, "wrong jawt version");

    /* Get the drawing surface */
	if ((ds = awt.GetDrawingSurface(env, obj_this)) == NULL)
		return 0L;

	/* Lock the drawing surface */
	lock = ds->Lock(ds);
    MY_ASSERT((lock & JAWT_LOCK_ERROR) == 0, "can't lock the drawing surface");
    
	/* Get the drawing surface info */
	dsi = ds->GetDrawingSurfaceInfo(ds);

	/* Get the platform-specific drawing info */
	dsi_win = (JAWT_Win32DrawingSurfaceInfo *)dsi->platformInfo;
    hdc = dsi_win->hdc;
    hWnd = dsi_win->hwnd;

	/* Free the drawing surface info */
	ds->FreeDrawingSurfaceInfo(dsi);
	/* Unlock the drawing surface */
	ds->Unlock(ds);
	/* Free the drawing surface */
	awt.FreeDrawingSurface(ds);


    return ((jint) hWnd);
}


// Using STL for storing the callback information.
#include <map>
using namespace std;
typedef map<HWND, jobject, less<HWND> > HWND2OBJECT;
typedef map<HWND, WNDPROC, less<HWND> > HWND2PROC;

// Shared Data
static HWND2PROC theProcMap;
static HWND2OBJECT theFrameMap;

// Helpers for getting and storing the JFrameEx object
static void setFrame(HWND hwnd, jobject object)
{
	theFrameMap.insert(HWND2OBJECT::value_type(hwnd,object));
}
static jobject getFrame(HWND hwnd)
{
	HWND2OBJECT::iterator iterator; 
	iterator = theFrameMap.find(hwnd);
	if(iterator != theFrameMap.end())    
		return (*iterator).second;
	return 0;
}

// Helpers for getting and storing the initial window proc's
static void setProc(HWND hwnd, WNDPROC proc)
{
	theProcMap.insert(HWND2PROC::value_type(hwnd,proc));
}
static WNDPROC getProc(HWND hwnd)
{
	HWND2PROC::iterator iterator; 
	iterator = theProcMap.find(hwnd);
	if(iterator != theProcMap.end())    
		return (*iterator).second;
	return 0;
}



LRESULT CALLBACK
FrameWindowProc (HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC oldProc = getProc((HWND) hwnd);
	jobject frame = getFrame((HWND) hwnd);

	LPBYTE lpb;// = new BYTE[dwSize];//LPBYTE lpb = new BYTE[dwSize];
	UINT dwSize;
	RAWINPUT *raw;

    if (nMsg == WM_INPUT) {
		GetRawInputData((HRAWINPUT) lParam, RID_INPUT, NULL, &dwSize, 
				sizeof(RAWINPUTHEADER));

		lpb = (BYTE *) malloc(sizeof(LPBYTE) * dwSize);
		if (lpb == NULL)  {
			return 0;
		} 

		if (GetRawInputData((HRAWINPUT) lParam, RID_INPUT, lpb, &dwSize, 
				sizeof(RAWINPUTHEADER)) != dwSize )
			DebugString (frame, "GetRawInputData doesn't return correct size !\n"); 

		raw = (RAWINPUT *) lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE) {
			MouseActionPerformed(
					frame,
					raw->header.hDevice,
					raw->data.mouse.usFlags, 
					raw->data.mouse.ulButtons, 
					raw->data.mouse.usButtonFlags, 
					raw->data.mouse.usButtonData, 
					raw->data.mouse.ulRawButtons, 
					raw->data.mouse.lLastX,
					raw->data.mouse.lLastY,
					raw->data.mouse.ulExtraInformation);
		} 

		/*
		InvalidateRect(hwnd, 0, TRUE);
		SendMessage(hwnd, WM_PAINT, 0, 0);
		*/
						
		free(lpb); 
		return 0;
	} 

    return CallWindowProc (oldProc, hwnd, nMsg, wParam, lParam);
}

// JNI native entry point for subclassing the window
JNIEXPORT void JNICALL Java_JFrameEx_setHook
  (JNIEnv *pEnv, jobject f, jint hwnd)
{
	InitRawInput();
	// ensure that the java object can be called from any thread.
	jobject frame = pEnv->NewGlobalRef(f);

	WNDPROC oldProc = (WNDPROC)::SetWindowLong((HWND)hwnd, GWL_WNDPROC, (LONG)FrameWindowProc);

	// store the java object
	setFrame((HWND)hwnd, frame);
	// store the old window proc
	setProc ((HWND)hwnd, oldProc);
}

// JNI native entry point remving subclassing from the window
JNIEXPORT void JNICALL Java_JFrameEx_resetHook
  (JNIEnv *pEnv, jobject f, jint hwnd)
{
	WNDPROC oldProc = getProc((HWND)hwnd);
	jobject frame = getFrame((HWND)hwnd);
	::SetWindowLong((HWND)hwnd, GWL_WNDPROC, (LONG)oldProc);
	pEnv->DeleteGlobalRef(frame);
}


/*
 * Class:     JFrameEx
 * Method:    showCursor
 * Signature: (Z)I
 */
JNIEXPORT jint JNICALL Java_JFrameEx_showCursor
  (JNIEnv * env, jobject this_obj, jboolean show) {
  	
  	return (jint) ShowCursor((BOOL) show);
}

/*
 * Class:     JFrameEx
 * Method:    setCursorPost
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_JFrameEx_setCursorPos
  (JNIEnv *env, jobject obj_this, jint x, jint y) {
    HWND hwnd = (HWND) Java_JFrameEx_getHWND(env, obj_this);
    
    RECT rect;

    if (!GetWindowRect(hwnd, &rect)) {
  	    return (jboolean) false;
    }

    POINT point;

    if (!GetCursorPos(&point)) {
        return (jboolean) false;
    }

    if (point.x > rect.left && point.x < rect.right
            && point.y > rect.top && point.y < rect.bottom) {
        return (jboolean) SetCursorPos((int) (rect.left + x + 3),
                (int) (rect.top + y + 22));
    }

    return true;
}
/*
 * Class:     JFrameEx
 * Method:    getLastError
 * Signature: ()I
 */
JNIEXPORT jlong JNICALL Java_JFrameEx_getLastError
  (JNIEnv *env, jobject obj_this) {
    return (jlong) GetLastError();
}
