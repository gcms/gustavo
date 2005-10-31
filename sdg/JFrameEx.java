import java.awt.Component;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import javax.swing.JComponent;
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
    private native int getHWND();

    /**
     * Native entry point for subclassing the JFrame window.
     * 
     * @param hwnd
     *            window handle id
     */
    private native void setHook(int hwnd);

    /**
     * Native entry point for removing the hook.
     * 
     * @param hwnd
     *            window handle id
     */
    private native void resetHook(int hwnd);

    private PointerGlassPane glassPane;

    private boolean initialized;

    private class PointerGlassPane extends JComponent {
        public void paintComponent(Graphics g) {
            super.paintComponent(g);

            for (Iterator it = mouseStates.keySet().iterator(); it.hasNext();) {
                Integer mouseId = (Integer) it.next();
                MouseState state = (MouseState) mouseStates.get(mouseId);

                // System.out.println("PAINTING");
                // System.out.println("x = " + x + ", y = " + y);

                state.getCursor().drawCursor(g, state.getX(), state.getY());
            }
        }

    }

    private Map mouseStates = new HashMap();

    public MouseState getMouseState(int hDevice) {
        Integer key = new Integer(hDevice);

        MouseState state = (MouseState) mouseStates.get(key);

        if (state == null) {
            state = new MouseState(0, 0, hDevice, mouseContext(),
                    componentContext());
            mouseStates.put(key, state);
        }

        return state;
    }

    private int checkMouseIds() {
        if (!initialized) {
            initialized = initRawInput();
        }

        int[] ids = getMouseIds();
        if (ids == null) {
            return mouseStates.size();
        }

        for (int i = 0; i < ids.length; i++) {
            getMouseState(ids[i]);
        }

        return ids.length;
    }

    public MouseState[] getMouseStates() {
        checkMouseIds();
        return (MouseState[]) mouseStates.values().toArray(
                new MouseState[mouseStates.size()]);
    }

    public int getMouseCount() {
        return checkMouseIds();
    }

    /**
     * @see javax.swing.JFrame
     */

    public JFrameEx(String title) {
        super(title);
        setGlassPane(glassPane = new PointerGlassPane());
        glassPane.setVisible(true);
        glassPane.setOpaque(false);

        initRawInput();
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
        return getLayeredPane();
    }

    private Component[] componentContext() {
        return new Component[] { getLayeredPane() };
        // if (getJMenuBar() != null) {
        // return new Component[] { getContentPane(), getJMenuBar() };
        // } else {
        // return new Component[] { getContentPane() };
        // }
    }

    /* system mouse id */
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

    /* System mouse cursor */
    private Point systemCursorPosition;

    private Point getSystemCursorPosition() {
        if (systemCursorPosition == null) {
            systemCursorPosition = getCursorPos();
        }

        return systemCursorPosition;
    }

    private void setSystemCursorPosition(Point p) {
        systemCursorPosition = p;
    }

    private void moveSystemCursorPosition(int iLastX, int iLastY) {
        systemCursorPosition.x += iLastX;
        systemCursorPosition.y += iLastY;
    }

    public static Rectangle getBoundsOnScreen(Component comp) {
        Component parent = comp.getParent();
        if (parent == null) {
            return comp.getBounds();
        } else {
            Rectangle pb = getBoundsOnScreen(parent);
            Rectangle cb = comp.getBounds();

            cb.x += pb.x;
            cb.y += pb.y;

            return cb;
        }
    }

    public void mouseActionPerformed(final int hDevice, int usFlags,
            final long ulButtons, int usButtonFlags, int usButtonData,
            long ulRawButtons, final long iLastX, final long iLastY,
            long ulExtraInformation) {
        // System.out.println("hDevice = " + hDevice);
        // System.out.println("x = " + iLastX + ", " + "y = " + iLastY);
        // System.out.println("ulButtons = 0x" + Long.toHexString(ulButtons));

        final MouseState state = getMouseState(hDevice);

        // setup a system mouse
        if (getSystemMouseState() == null) {
            setSystemMouseId(hDevice);
            setSystemCursorPosition(getCursorPos());
        }

        // assert getSystemMouseState() != null;

        // RIDEV_NOLEGACY
        if (hDevice == getSystemMouseId()) {
            // assert state.equals(getSystemMouseState());
            moveSystemCursorPosition((int) iLastX, (int) iLastY);

            if (getBoundsOnScreen(mouseContext()).contains(
                    getSystemCursorPosition())) {
                while (showCursor(false) >= 0)
                    ;
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        state.move((int) iLastX, (int) iLastY);
                    }
                });
                setSystemCursorPosition(state.getLocationOnScreen());

            } else {
                while (showCursor(true) < 0)
                    ;
                // setSystemCursorPosition(getCursorPos());
                moveSystemCursorPosition((int) iLastX, (int) iLastY);// RIDEV_NOLEGACY
            }
        } else {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    state.move((int) iLastX, (int) iLastY);
                }
            });
        }

        setCursorPos(getSystemCursorPosition());

        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                glassPane.repaint();

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

            }
        });

        // SwingUtilities.invokeLater(new Runnable() {
        // public void run() {
        // mouseActionPerformed(hDevice, ulButtons, (int) iLastX,
        // (int) iLastY);
        // }
        // });

        // SwingUtilities.invokeLater(new Runnable() {
        // public void run() {
        // mouseActionPerformed(hDevice, ulButtons, (int) iLastX,
        // (int) iLastY);
        // }
        // });

    }

    public void mouseActionPerformed(int hDevice, long ulButtons, int iLastX,
            int iLastY) {

        MouseState state = getMouseState(hDevice);

        // setup a system mouse
        if (getSystemMouseState() == null) {
            setSystemMouseId(hDevice);
            setSystemCursorPosition(getCursorPos());
        }

        // assert getSystemMouseState() != null;

        // RIDEV_NOLEGACY
        if (hDevice == getSystemMouseId()) {
            // assert state.equals(getSystemMouseState());
            moveSystemCursorPosition((int) iLastX, (int) iLastY);

            if (getBoundsOnScreen(mouseContext()).contains(
                    getSystemCursorPosition())) {
                while (showCursor(false) >= 0)
                    ;
                state.move(iLastX, iLastY);
                setSystemCursorPosition(state.getLocationOnScreen());
            } else {
                while (showCursor(true) < 0)
                    ;
                // setSystemCursorPosition(getCursorPos());
                moveSystemCursorPosition(iLastX, iLastY);// RIDEV_NOLEGACY
            }
        } else {
            state.move(iLastX, iLastY);
        }

        setCursorPos(getSystemCursorPosition());

        // 0
        // if (hDevice == getSystemMouseId()) {
        // Point curPosition = getCursorPos();
        //            
        // if (mouseContext().getBounds().contains(curPosition)) {
        // while (showCursor(false) >= 0);
        // Point p = state.getMouseContext().getLocationOnScreen();
        // p.x = curPosition.x - p.x;
        // p.y = curPosition.y - p.y;
        // state.move(p);
        // } else {
        // while (showCursor(true) < 0);
        // }
        // setSystemCursorPosition(curPosition);
        // } else {
        // state.move((int) iLastX, (int) iLastY);
        // setCursorPos(getSystemCursorPosition());
        // }

        glassPane.repaint();

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

    }

    private native boolean initRawInput();

    private native int showCursor(boolean show);

    private boolean setCursorPos(Point p) {
        return setCursorPos(p.x, p.y);
    }

    private native boolean setCursorPos(int x, int y);

    private native long getLastError();

    private native Point getCursorPos();

    private native int[] getMouseIds();
}
