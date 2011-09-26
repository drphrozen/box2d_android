#include <jni.h>
#include <android/log.h>

#include <GLES/gl.h>

#include <Box2D/Box2D.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Framework/Test.h"

#define LOG_TAG   "libbox2d_testbed"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static void checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		LOGI("after %s() glError (0x%x)\n", op, error);
	}
}

int32 testIndex = 0;
int32 testSelection = 0;
int32 testCount = 0;
TestEntry* entry;
Test* test;
int32 mWidth;
int32 mHeight;

bool setupTests() {
	testCount = 0;
	while (g_testEntries[testCount].createFcn != NULL) {
		++testCount;
	}

	testIndex = b2Clamp(testIndex, 0, testCount - 1);
	testSelection = testIndex;

	entry = g_testEntries + testIndex;
	test = entry->createFcn();
	return true;
}

Settings settings;

#define MOUSE_STATE_DOWN (0)
#define MOUSE_STATE_UP   (1)
#define MOUSE_STATE_MOVE (2)

extern "C" {
JNIEXPORT void JNICALL Java_org_box2d_testbed_TestbedRenderer_nativeInit(JNIEnv * env);
JNIEXPORT void JNICALL Java_org_box2d_testbed_TestbedRenderer_nativeResize(JNIEnv * env, jobject obj, jint width, jint height);
JNIEXPORT void JNICALL Java_org_box2d_testbed_TestbedRenderer_nativeRender(JNIEnv * env);

JNIEXPORT jobjectArray JNICALL Java_org_box2d_testbed_TestbedActivity_nativeTests(JNIEnv *env);
JNIEXPORT void JNICALL Java_org_box2d_testbed_TestbedActivity_nativeChangeTest(JNIEnv *env, jobject obj, jint test);
JNIEXPORT void JNICALL Java_org_box2d_testbed_TestbedView_nativeMouse(JNIEnv *env, jobject obj, jint state, jint x, jint y);
};

static b2Vec2 ConvertScreenToWorld(int32 x, int32 y)
{
	float32 u = x / float32(mWidth);
	float32 v = (mHeight - y) / float32(mHeight);

	float32 ratio = float32(mWidth) / float32(mHeight);
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= 1.0f; // viewZoom;

	b2Vec2 lower = settings.viewCenter - extents;
	b2Vec2 upper = settings.viewCenter + extents;

	b2Vec2 p;
	p.x = (1.0f - u) * lower.x + u * upper.x;
	p.y = (1.0f - v) * lower.y + v * upper.y;
	return p;
}


JNIEXPORT void JNICALL Java_org_box2d_testbed_TestbedView_nativeMouse(JNIEnv *env, jobject obj, jint state, jint x, jint y) {
	b2Vec2 p = ConvertScreenToWorld(x, y);
	switch(state) {
	case MOUSE_STATE_DOWN:
		test->MouseDown(p);
		break;
	case MOUSE_STATE_UP:
		test->MouseUp(p);
		break;
	case MOUSE_STATE_MOVE:
		test->MouseMove(p);
		break;
	}
}

JNIEXPORT void JNICALL Java_org_box2d_testbed_TestbedActivity_nativeChangeTest(JNIEnv *env, jobject obj, jint test) {
	testSelection = test;
}

JNIEXPORT jobjectArray JNICALL Java_org_box2d_testbed_TestbedActivity_nativeTests(JNIEnv *env) {

	// count elements
	int testCount = 0;
	while(g_testEntries[testCount].createFcn != NULL) {
		++testCount;
	}

	// create string array
  jobjectArray ret = (jobjectArray)env->NewObjectArray(testCount, env->FindClass("java/lang/String"), env->NewStringUTF(""));

  // copy each element to array
  for(int i=0;i<testCount;i++) {
      env->SetObjectArrayElement(ret, i, env->NewStringUTF(g_testEntries[i].name));
  }
  return(ret);
}


JNIEXPORT void JNICALL Java_org_box2d_testbed_TestbedRenderer_nativeInit(JNIEnv * env) {
	glShadeModel(GL_FLAT);
	checkGlError("glShadeModel");

	//    glEnable(GL_LIGHTING);
	//    glEnable(GL_LIGHT0);
	//    glEnable(GL_LIGHT1);
	//    glEnable(GL_LIGHT2);

	glEnableClientState(GL_VERTEX_ARRAY);
	checkGlError("glEnableClientState");
	//    glEnableClientState(GL_COLOR_ARRAY);

	//	glViewport(0, 0, w, h);
	//	checkGlError("glViewport");

	LOGI("setupTests");
	setupTests();
}

static void prepareFrame() {
	// TODO: add to resize instead
	glViewport(0, 0, mWidth, mHeight);
	checkGlError("glViewport");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float32 ratio = float32(mWidth) / float32(mHeight);

	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= 1.0f; // viewZoom;

	b2Vec2 lower = settings.viewCenter - extents;
	b2Vec2 upper = settings.viewCenter + extents;

	// L/R/B/T
	//gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
	glOrthof(lower.x, upper.x, lower.y, upper.y, -1.0f, 1.0f);
	checkGlError("glOrthof");
	// end add to resize

	glMatrixMode(GL_MODELVIEW);
	checkGlError("glMatrixMode");
	glLoadIdentity();
	checkGlError("glLoadIdentity");
}

JNIEXPORT void JNICALL Java_org_box2d_testbed_TestbedRenderer_nativeResize(JNIEnv * env, jobject obj, jint width, jint height) {
	mWidth = width;
	mHeight = height;
}

static void SimulationLoop() {
	prepareFrame();

	test->SetTextLine(30);
	b2Vec2 oldCenter = settings.viewCenter;
	settings.hz = 60; //settingsHz;
	test->Step(&settings);
//	if (oldCenter.x != settings.viewCenter.x || oldCenter.y != settings.viewCenter.y)
//	{
//		Resize(width, height);
//	}

	test->DrawTitle(5, 15, entry->name);

//	glutSwapBuffers();

	if (testSelection != testIndex) {
		testIndex = testSelection;
		delete test;
		entry = g_testEntries + testIndex;
		test = entry->createFcn();
//		viewZoom = 1.0f;
		settings.viewCenter.Set(0.0f, 20.0f);
//		Resize(width, height);
	}
}

JNIEXPORT void JNICALL Java_org_box2d_testbed_TestbedRenderer_nativeRender(JNIEnv * env) {
	SimulationLoop();
}
