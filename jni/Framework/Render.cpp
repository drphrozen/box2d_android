/*
* Copyright (c) 2006-2007 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "Render.h"

#include <GLES/gl.h>

#include <cstdio>
#include <cstdarg>
#include <cstring>
using namespace std;

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	glColor4f(color.r, color.g, color.b, 1);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
//	glBegin(GL_LINE_LOOP);
//	for (int32 i = 0; i < vertexCount; ++i)
//	{
//		glVertex2f(vertices[i].x, vertices[i].y);
//	}
//	glEnd();
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

//	glBegin(GL_TRIANGLE_FAN);
//	for (int32 i = 0; i < vertexCount; ++i)
//	{
//		glVertex2f(vertices[i].x, vertices[i].y);
//	}
//	glEnd();
	glDisable(GL_BLEND);

	glColor4f(color.r, color.g, color.b, 1.0f);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINE_LOOP, 0, vertexCount);

//	glBegin(GL_LINE_LOOP);
//	for (int32 i = 0; i < vertexCount; ++i)
//	{
//		glVertex2f(vertices[i].x, vertices[i].y);
//	}
//	glEnd();
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const int32 k_segments = 16;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	b2Vec2 vertices[k_segments];

	for (int32 i = 0; i < k_segments; ++i)
	{
		vertices[i] = center + radius * b2Vec2(cosf(theta), sinf(theta));
		theta += k_increment;
	}

	glColor4f(color.r, color.g, color.b, 1);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINE_LOOP, 0, k_segments);


//	glBegin(GL_LINE_LOOP);
//	for (int32 i = 0; i < k_segments; ++i)
//	{
//		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
//		glVertex2f(v.x, v.y);
//		theta += k_increment;
//	}
//	glEnd();
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const int32 k_segments = 16;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	b2Vec2 vertices[k_segments];

	for (int32 i = 0; i < k_segments; ++i)
	{
		vertices[i] = center + radius * b2Vec2(cosf(theta), sinf(theta));
		theta += k_increment;
	}

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, k_segments);

	glDisable(GL_BLEND);

	theta = 0.0f;
	glColor4f(color.r, color.g, color.b, 1.0f);
	for (int32 i = 0; i < k_segments; ++i)
	{
		vertices[i] = center + radius * b2Vec2(cosf(theta), sinf(theta));
		theta += k_increment;
	}
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, k_segments);


	vertices[0] = b2Vec2(center.x, center.y);
	vertices[1] = center + radius * axis;
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINES, 0, 2);

//	const int32 k_segments = 16;
//	const float32 k_increment = 2.0f * b2_pi / k_segments;
//	float32 theta = 0.0f;
//	glEnable(GL_BLEND);
//	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
//	glBegin(GL_TRIANGLE_FAN);
//	for (int32 i = 0; i < k_segments; ++i)
//	{
//		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
//		glVertex2f(v.x, v.y);
//		theta += k_increment;
//	}
//	glEnd();
//	glDisable(GL_BLEND);
//
//	theta = 0.0f;
//	glColor4f(color.r, color.g, color.b, 1.0f);
//	glBegin(GL_LINE_LOOP);
//	for (int32 i = 0; i < k_segments; ++i)
//	{
//		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
//		glVertex2f(v.x, v.y);
//		theta += k_increment;
//	}
//	glEnd();
//
//	b2Vec2 p = center + radius * axis;
//	glBegin(GL_LINES);
//	glVertex2f(center.x, center.y);
//	glVertex2f(p.x, p.y);
//	glEnd();
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glColor4f(color.r, color.g, color.b, 1);
	b2Vec2 vertices[2];
	vertices[0] = p1;
	vertices[1] = p2;
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINES, 0, 2);

//	glColor3f(color.r, color.g, color.b);
//	glBegin(GL_LINES);
//	glVertex2f(p1.x, p1.y);
//	glVertex2f(p2.x, p2.y);
//	glEnd();
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	const float32 k_axisScale = 0.4f;
	b2Vec2 vertices[2];
	vertices[0] = xf.p;
	vertices[1] = xf.p + k_axisScale * xf.q.GetXAxis();

	glColor4f(1.0f, 0.0f, 0.0f, 1);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINES, 0, 2);

	vertices[0] = xf.p;
	vertices[1] = xf.p + k_axisScale * xf.q.GetYAxis();

	glColor4f(0.0f, 1.0f, 0.0f, 1);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINES, 0, 2);

//	b2Vec2 p1 = xf.p, p2;
//	const float32 k_axisScale = 0.4f;
//	glBegin(GL_LINES);
//
//	glColor3f(1.0f, 0.0f, 0.0f);
//	glVertex2f(p1.x, p1.y);
//	p2 = p1 + k_axisScale * xf.q.GetXAxis();
//	glVertex2f(p2.x, p2.y);
//
//	glColor3f(0.0f, 1.0f, 0.0f);
//	glVertex2f(p1.x, p1.y);
//	p2 = p1 + k_axisScale * xf.q.GetYAxis();
//	glVertex2f(p2.x, p2.y);
//
//	glEnd();
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	glPointSize(size);

	glColor4f(color.r, color.g, color.b, 1);
	glVertexPointer(2, GL_POINTS, 0, &p);
	glDrawArrays(GL_LINES, 0, 1);

	glPointSize(1.0f);
//	glPointSize(size);
//	glBegin(GL_POINTS);
//	glColor3f(color.r, color.g, color.b);
//	glVertex2f(p.x, p.y);
//	glEnd();
//	glPointSize(1.0f);
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
	const int vertexCount = 4;
	b2Vec2 vertices[vertexCount];

	glColor4f(c.r, c.g, c.b, 1);
	vertices[0] = b2Vec2(aabb->lowerBound.x, aabb->lowerBound.y);
	vertices[1] = b2Vec2(aabb->upperBound.x, aabb->lowerBound.y);
	vertices[2] = b2Vec2(aabb->upperBound.x, aabb->upperBound.y);
	vertices[3] = b2Vec2(aabb->lowerBound.x, aabb->upperBound.y);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
//	glColor3f(c.r, c.g, c.b);
//	glBegin(GL_LINE_LOOP);
//	glVertex2f(aabb->lowerBound.x, aabb->lowerBound.y);
//	glVertex2f(aabb->upperBound.x, aabb->lowerBound.y);
//	glVertex2f(aabb->upperBound.x, aabb->upperBound.y);
//	glVertex2f(aabb->lowerBound.x, aabb->upperBound.y);
//	glEnd();
}
