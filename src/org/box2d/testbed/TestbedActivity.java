package org.box2d.testbed;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MenuItem.OnMenuItemClickListener;
import android.view.MotionEvent;

public class TestbedActivity extends Activity {

  protected static native String[] nativeTests();
  protected static native void nativeChangeTest(int test);

  static {
    System.loadLibrary("box2d_testbed");
  }

  @Override protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    mGLView = new TestbedView(this);
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

  @Override public boolean onCreateOptionsMenu(Menu menu) {
    final String[] tests = nativeTests();

    AlertDialog.Builder builder = new AlertDialog.Builder(this);
    builder.setTitle("Pick a test");
    builder.setItems(tests, new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface dialog, int item) {
        nativeChangeTest(item);
      }
    });
    final AlertDialog alert = builder.create();

    MenuItem selectTest = menu.add(R.string.change_test);
    selectTest.setIcon(android.R.drawable.ic_menu_more);
    selectTest.setOnMenuItemClickListener(new OnMenuItemClickListener() {
      @Override public boolean onMenuItemClick(MenuItem item) {
        alert.show();
        return true;
      }
    });
    return true;
  }
  
  private GLSurfaceView mGLView;
}

class TestbedView extends GLSurfaceView {
  
  protected static final int MOUSE_STATE_DOWN = 0;
  protected static final int MOUSE_STATE_UP = 1;
  protected static final int MOUSE_STATE_MOVE = 2;

  protected static native void nativeMouse(int state, int x, int y);

  public TestbedView(Context context) {
    super(context);
    mRenderer = new TestbedRenderer();
    setRenderer(mRenderer);
  }

  public boolean onTouchEvent(final MotionEvent event) {
    int action = event.getAction();
    switch(action) {
    case MotionEvent.ACTION_DOWN:
      nativeMouse(MOUSE_STATE_DOWN, (int)event.getX(), (int)event.getY());
      break;
    case MotionEvent.ACTION_UP:
//      String out = String.format("%.2fx%.2f (%.2fx%.2f)", event.getX(), event.getY(), event.getRawX(), event.getRawY());
//      Toast.makeText(getContext(),  out, Toast.LENGTH_SHORT).show();
      nativeMouse(MOUSE_STATE_UP, (int)event.getX(), (int)event.getY());
      break;
    case MotionEvent.ACTION_MOVE:
      nativeMouse(MOUSE_STATE_MOVE, (int)event.getX(), (int)event.getY());
      break;
    }
    
    return true;
  }

  TestbedRenderer mRenderer;
}

class TestbedRenderer implements GLSurfaceView.Renderer {
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
