# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
MY_DIR := $(call my-dir)
LOCAL_PATH := $(MY_DIR)/../../..

include $(CLEAR_VARS)

LOCAL_MODULE     := box2d
LOCAL_SRC_FILES  := Box2D/Collision/b2BroadPhase.cpp Box2D/Collision/b2CollideCircle.cpp Box2D/Collision/b2CollideEdge.cpp Box2D/Collision/b2CollidePolygon.cpp Box2D/Collision/b2Collision.cpp Box2D/Collision/b2Distance.cpp Box2D/Collision/b2DynamicTree.cpp Box2D/Collision/b2TimeOfImpact.cpp Box2D/Collision/Shapes/b2ChainShape.cpp Box2D/Collision/Shapes/b2CircleShape.cpp Box2D/Collision/Shapes/b2EdgeShape.cpp Box2D/Collision/Shapes/b2PolygonShape.cpp Box2D/Common/b2BlockAllocator.cpp Box2D/Common/b2Draw.cpp Box2D/Common/b2Math.cpp Box2D/Common/b2Settings.cpp Box2D/Common/b2StackAllocator.cpp Box2D/Common/b2Timer.cpp Box2D/Dynamics/b2Body.cpp Box2D/Dynamics/b2ContactManager.cpp Box2D/Dynamics/b2Fixture.cpp Box2D/Dynamics/b2Island.cpp Box2D/Dynamics/b2World.cpp Box2D/Dynamics/b2WorldCallbacks.cpp Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp Box2D/Dynamics/Contacts/b2CircleContact.cpp Box2D/Dynamics/Contacts/b2Contact.cpp Box2D/Dynamics/Contacts/b2ContactSolver.cpp Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp Box2D/Dynamics/Contacts/b2PolygonContact.cpp Box2D/Dynamics/Joints/b2DistanceJoint.cpp Box2D/Dynamics/Joints/b2FrictionJoint.cpp Box2D/Dynamics/Joints/b2GearJoint.cpp Box2D/Dynamics/Joints/b2Joint.cpp Box2D/Dynamics/Joints/b2MouseJoint.cpp Box2D/Dynamics/Joints/b2PrismaticJoint.cpp Box2D/Dynamics/Joints/b2PulleyJoint.cpp Box2D/Dynamics/Joints/b2RevoluteJoint.cpp Box2D/Dynamics/Joints/b2RopeJoint.cpp Box2D/Dynamics/Joints/b2WeldJoint.cpp Box2D/Dynamics/Joints/b2WheelJoint.cpp Box2D/Rope/b2Rope.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(MY_DIR)

include $(CLEAR_VARS)

LOCAL_MODULE           := testbed-activity
LOCAL_SRC_FILES        := Framework/Main.cpp Framework/Render.cpp Framework/Test.cpp Tests/TestEntries.cpp
LOCAL_LDLIBS           := -llog -landroid -lEGL -lGLESv1_CM
LOCAL_STATIC_LIBRARIES := android_native_app_glue box2d
LOCAL_C_INCLUDES       := $(MY_DIR)/../../..

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
