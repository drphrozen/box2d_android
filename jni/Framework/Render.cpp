#include "Render.h"

#include <android/log.h>

#include <GLES/gl.h>

#include <cstdio>
#include <cstdarg>
#include <cstring>
using namespace std;

#define LOG_TAG   "libbox2d_testbed_render"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static void checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		LOGI("after %s() glError (0x%x)\n", op, error);
	}
}

DebugDraw::~DebugDraw() {}

void DebugDraw::DrawArray(const GLvoid *pointer, GLsizei count, GLenum mode) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, pointer);
	checkGlError("glVertexPointer");
	glDrawArrays(mode, 0, count);
	checkGlError("glDrawArrays");
}

void DebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color& color) {
	glColor4f(color.r, color.g, color.b, 1);
	DrawArray(vertices, vertexCount, GL_LINE_LOOP);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	DrawArray(vertices, vertexCount, GL_TRIANGLE_FAN);

	glDisable(GL_BLEND);

	glColor4f(color.r, color.g, color.b, 1.0f);
	DrawArray(vertices, vertexCount, GL_LINE_LOOP);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const int32 k_segments = 16;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;

	for (int32 i = 0; i < k_segments; ++i)
	{
		mVertices[i] = center + radius * b2Vec2(cosf(theta), sinf(theta));
		theta += k_increment;
	}

	glColor4f(color.r, color.g, color.b, 1.0f);
	DrawArray(mVertices, k_segments, GL_LINE_LOOP);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const int32 k_segments = 16;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;

	for (int32 i = 0; i < k_segments; ++i)
	{
		mVertices[i] = center + radius * b2Vec2(cosf(theta), sinf(theta));
		theta += k_increment;
	}

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

	DrawArray(mVertices, k_segments, GL_TRIANGLE_FAN);

	glDisable(GL_BLEND);

	theta = 0.0f;
	glColor4f(color.r, color.g, color.b, 1.0f);
	for (int32 i = 0; i < k_segments; ++i)
	{
		mVertices[i] = center + radius * b2Vec2(cosf(theta), sinf(theta));
		theta += k_increment;
	}
	DrawArray(mVertices, k_segments, GL_LINE_LOOP);

	mVertices[0] = b2Vec2(center.x, center.y);
	mVertices[1] = center + radius * axis;
	DrawArray(mVertices, 2, GL_LINES);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glColor4f(color.r, color.g, color.b, 1.0f);
	mVertices[0] = p1;
	mVertices[1] = p2;
	DrawArray(mVertices, 2, GL_LINES);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	const float32 k_axisScale = 0.4f;
	mVertices[0] = xf.p;
	mVertices[1] = xf.p + k_axisScale * xf.q.GetXAxis();

	glColor4f(1.0f, 0.0f, 0.0f, 1);
	DrawArray(mVertices, 2, GL_LINES);

	mVertices[0] = xf.p;
	mVertices[1] = xf.p + k_axisScale * xf.q.GetYAxis();

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	DrawArray(mVertices, 2, GL_LINES);
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	glPointSize(size);

	glColor4f(color.r, color.g, color.b, 1.0f);
	DrawArray(&p, 1, GL_POINTS);

	glPointSize(1.0f);
}

void DebugDraw::DrawString(int x, int y, const char *string, ...)
{
//	char buffer[128];
//
//	va_list arg;
//	va_start(arg, string);
//	vsprintf(buffer, string, arg);
//	va_end(arg);
//
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
///// TODO: glutGet
////	int w = glutGet(GLUT_WINDOW_WIDTH);
////	int h = glutGet(GLUT_WINDOW_HEIGHT);
////	gluOrtho2D(0, w, h, 0);
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//
//	glColor3f(0.9f, 0.6f, 0.6f);
//	glRasterPos2i(x, y);
//	int32 length = (int32)strlen(buffer);
///// TODO: glutBitmapCharacter
////	for (int32 i = 0; i < length; ++i)
////	{
////		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
////	}
//
//	glPopMatrix();
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	glColor4f(c.r, c.g, c.b, 1.0f);
	mVertices[0] = b2Vec2(aabb->lowerBound.x, aabb->lowerBound.y);
	mVertices[1] = b2Vec2(aabb->upperBound.x, aabb->lowerBound.y);
	mVertices[2] = b2Vec2(aabb->upperBound.x, aabb->upperBound.y);
	mVertices[3] = b2Vec2(aabb->lowerBound.x, aabb->upperBound.y);
	DrawArray(mVertices, 4, GL_LINE_LOOP);
}
