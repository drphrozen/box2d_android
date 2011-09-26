package org.box2d.testbed;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;

public class TestbedActivity extends Activity {
  @Override protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    mGLView = new TestbedGLSurfaceView(this);
    setContentView(mGLView);
  }

  @Override protected void onPause() {
    super.onPause();
    mGLView.onPause();
  }

  @Override protected void onResume() {
    super.onResume();
    mGLView.onResume();
  }

  private GLSurfaceView mGLView;

}

class TestbedGLSurfaceView extends GLSurfaceView {
  public TestbedGLSurfaceView(Context context) {
    super(context);
    mRenderer = new TestbedRenderer();
    setRenderer(mRenderer);
  }

  public boolean onTouchEvent(final MotionEvent event) {
    if (event.getAction() == MotionEvent.ACTION_DOWN) {
//      nativePause();
    }
    return true;
  }

  TestbedRenderer mRenderer;
}

class TestbedRenderer implements GLSurfaceView.Renderer {
  static {
    System.loadLibrary("box2d_testbed");
  }
  /**
   * @param width
   *          the current view width
   * @param height
   *          the current view height
   */
  public static native void nativeInit();
  public static native void nativeResize(int width, int height);
  public static native void nativeRender();

  public void onSurfaceCreated(GL10 gl, EGLConfig config) {
    nativeInit();
  }

  public void onSurfaceChanged(GL10 gl, int w, int h) {
    // gl.glViewport(0, 0, w, h);
    nativeResize(w, h);
  }

  public void onDrawFrame(GL10 gl) {
    nativeRender();
  }
}
