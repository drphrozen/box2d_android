/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code
#include <jni.h>
#include <android/log.h>

#include <GLES/gl.h>

#include <Box2D/Box2D.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Framework/Test.h"

#define LOG_TAG   "libgl2jni"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static void checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		LOGI("after %s() glError (0x%x)\n", op, error);
	}
}

bool setupGraphics(int w, int h) {
	glViewport(0, 0, w, h);
	checkGlError("glViewport");
	return true;
}

int32 testIndex = 0;
int32 testSelection = 0;
int32 testCount = 0;
TestEntry* entry;
Test* test;

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

extern "C" {
	JNIEXPORT void JNICALL Java_org_box2d_testbed_GL2JNILib_init(JNIEnv * env, jobject obj, jint width, jint height);
	JNIEXPORT void JNICALL Java_org_box2d_testbed_GL2JNILib_resize(JNIEnv * env, jobject obj, jint width, jint height);
	JNIEXPORT void JNICALL Java_org_box2d_testbed_GL2JNILib_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_org_box2d_testbed_GL2JNILib_init(JNIEnv * env, jobject obj, jint width, jint height) {
	LOGI("setupGraphics");
	setupGraphics(width, height);
	LOGI("setupTests");
	setupTests();
}

JNIEXPORT void JNICALL Java_org_box2d_testbed_GL2JNILib_resize(JNIEnv * env, jobject obj, jint width, jint height) {
	glViewport(0, 0, width, height);
	checkGlError("glViewport");

	glMatrixMode(GL_PROJECTION);
	checkGlError("glMatrixMode");
	glLoadIdentity();
	checkGlError("glLoadIdentity");
	float32 ratio = float32(width) / float32(height);

	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= 1.0f; // viewZoom;

	b2Vec2 lower = settings.viewCenter - extents;
	b2Vec2 upper = settings.viewCenter + extents;

	// L/R/B/T
	//gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
	glOrthof(lower.x, upper.x, lower.y, upper.y, -1.0f, 1.0f);
	checkGlError("glOrthof");
}

static void SimulationLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	checkGlError("glClear");

	glMatrixMode(GL_MODELVIEW);
	checkGlError("glMatrixMode");
	glLoadIdentity();
	checkGlError("glLoadIdentity");

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

JNIEXPORT void JNICALL Java_org_box2d_testbed_GL2JNILib_step(JNIEnv * env, jobject obj) {
	LOGI("SimulationLoop");
	SimulationLoop();
}
