import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

public class JFrameEx extends JFrame {
    /*
     * Loads JFrameEx.dll.
     */
    static {
        System.loadLibrary("JFrameEx");
    }

    /**
     * Gets the window handle.
     * 
     * @return window handle id.
     */
    public native int getHWND();

    /**
     * Native entry point for subclassing the JFrame window.
     * 
     * @param hwnd
     *            window handle id
     */
    public native void setHook(int hwnd);

    /**
     * Native entry point for removing the hook.
     * 
     * @param hwnd
     *            window handle id
     */
    public native void resetHook(int hwnd);

    private class PointerGlassPane extends Component {

        public void paint(Graphics g) {
            getParent().paintComponents(g);

            for (Iterator it = mouseStates.keySet().iterator(); it.hasNext();) {
                Integer mouseId = (Integer) it.next();
                MouseState state = (MouseState) mouseStates.get(mouseId);

                // System.out.println("PAINTING");
                // System.out.println("x = " + x + ", y = " + y);

                g.setColor(state.getCursorColor());
                g.fillOval(state.getX() - 5, state.getY() - 5, 10, 10);
            }
        }

    }

    private Map mouseStates = new HashMap();

    private MouseState getMouseState(int hDevice) {
        Integer key = new Integer(hDevice);

        MouseState state = (MouseState) mouseStates.get(key);

        if (state == null) {
            state = new MouseState(0, 0, hDevice, mouseContext());
            mouseStates.put(key, state);
        }

        return state;
    }

    /**
     * @see javax.swing.JFrame
     */
    public JFrameEx(String title) {
        super(title);
        setGlassPane(new PointerGlassPane());
    }

    /**
     * @see javax.swing.JFrame
     */
    public JFrameEx() {
        this("");
    }

    /**
     * Subclass the window once it has been created.
     */
    public void addNotify() {
        super.addNotify();
        setHook(getHWND());
    }

    /**
     * Remove the subclass-ing when the window is about to be destroyed.
     */
    public void removeNotify() {
        resetHook(getHWND());
        super.removeNotify();
    }

    private Component mouseContext() {
        return getRootPane();
    }

    private Dimension getFrameSize() {
        Rectangle rect = getWindowRectangle();

        return rect.getSize();
    }

    private int systemMouseId;

    public MouseState getSystemMouseState() {
        return (MouseState) mouseStates.get(new Integer(systemMouseId));
    }

    public int getSystemMouseId() {
        return systemMouseId;
    }

    public void setSystemMouseId(int systemMouseId) {
        this.systemMouseId = systemMouseId;
    }

    private Point systemCursorPosition;

    public Point getSystemCursorPosition() {
        if (systemCursorPosition == null) {
            systemCursorPosition = getCursorPos();
        }

        return systemCursorPosition;
    }

    public void setSystemCursorPosition(Point p) {
        systemCursorPosition = p;
    }

    public void moveSystemCursorPosition(int iLastX, int iLastY) {
        systemCursorPosition.x += iLastX;
        systemCursorPosition.y += iLastY;
    }

    public void mouseActionPerformed(int hDevice, int usFlags, long ulButtons,
            int usButtonFlags, int usButtonData, long ulRawButtons,
            long iLastX, long iLastY, long ulExtraInformation) {
        // System.out.println("hDevice = " + hDevice);
        // System.out.println("x = " + iLastX + ", " + "y = " + iLastY);
        // System.out.println("ulButtons = 0x" + Long.toHexString(ulButtons));

        MouseState state = getMouseState(hDevice);

        // setup a system mouse
        if (getSystemMouseState() == null) {
            setSystemMouseId(hDevice);
        }

        // assert getSystemMouseState() != null;

        if (hDevice == getSystemMouseId()) {
            // assert state.equals(getSystemMouseState());
            moveSystemCursorPosition((int) iLastX, (int) iLastY);
            if (mouseContext().getBounds().contains(getSystemCursorPosition())) {
                state.move((int) iLastX, (int) iLastY);
                setSystemCursorPosition(state.getLocationOnScreen());
            } else {
                moveSystemCursorPosition((int) iLastX, (int) iLastY);
            }
        } else {
            state.move((int) iLastX, (int) iLastY);
        }

        setCursorPos(getSystemCursorPosition());

        if (ulButtons == 0x1) {
            state.pressButton(MouseState.BUTTON1);
        } else if (ulButtons == 0x2) {
            state.releaseButton(MouseState.BUTTON1);
        } else if (ulButtons == 0x4) {
            state.pressButton(MouseState.BUTTON3);
        } else if (ulButtons == 0x8) {
            state.releaseButton(MouseState.BUTTON3);
        } else if (ulButtons == 0x10) {
            state.pressButton(MouseState.BUTTON2);
        } else if (ulButtons == 0x20) {
            state.releaseButton(MouseState.BUTTON2);
        }

        getGlassPane().paint(getGlassPane().getGraphics());

    }

    public native int showCursor(boolean show);

    public void moveSystemCursor(Point p) {
        Rectangle rect = getWindowRectangle();

        Point newP = SwingUtilities.convertPoint(mouseContext(), p, this);

        setCursorPos(newP.x + rect.x, newP.y + rect.y);
    }

    public boolean setCursorPos(Point p) {
        return setCursorPos(p.x, p.y);
    }

    public native boolean setCursorPos(int x, int y);

    public native long getLastError();

    public native Rectangle getWindowRectangle();

    public native Point getCursorPos();
}
