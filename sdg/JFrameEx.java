import java.awt.Component;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import javax.swing.JFrame;

public class JFrameEx extends JFrame {
    /*
     * Loads JFrameEx.dll.
     */
    static {
        System.loadLibrary("JFrameEx");
    }

    /**
     * Automatically generated serialVersionUID.
     */
    private static final long serialVersionUID = 3617858581639540788L;

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

        /**
         * Generated serialVersionUID.
         */
        private static final long serialVersionUID = 3258134673799526197L;

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

    private PointerGlassPane glassPane;

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
        setGlassPane((glassPane = new PointerGlassPane()));
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
        return getContentPane();
    }

    private Point getNewPoint(Point p, int lastX, int lastY) {
        Point newP = new Point(p.x + lastX, p.y + lastY);
        newP.x = newP.x < 0 ? 0
                : newP.x > mouseContext().getWidth() ? mouseContext()
                        .getWidth() : newP.x;
        newP.y = newP.y < 0 ? 0
                : newP.y > mouseContext().getHeight() ? mouseContext()
                        .getHeight() : newP.y;

        return newP;
    }

    /**
     * Notifies the window of a mouseEvent.
     */
    public void mouseActionPerformed(int hDevice, int usFlags, long ulButtons,
            int usButtonFlags, int usButtonData, long ulRawButtons,
            long iLastX, long iLastY, long ulExtraInformation) {
        // System.out.println("hDevice = " + hDevice);
        // System.out.println("x = " + iLastX + ", " + "y = " + iLastY);
        // System.out.println("ulButtons = 0x" + Long.toHexString(ulButtons));

        MouseState state = getMouseState(hDevice);

        Point oldP = state.getPosition();
        Point newP = getNewPoint(state.getPosition(), (int) iLastX,
                (int) iLastY);

        if (!oldP.equals(newP)) {
            state.move(newP);
        }

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

    public native boolean setCursorPos(int x, int y);
}
