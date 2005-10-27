import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import javax.swing.JFrame;
import javax.swing.JPanel;
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

    private PointerGlassPane glassPane;

    private class PointerGlassPane extends JPanel implements MouseListener,
            MouseMotionListener, MouseWheelListener {
        private Container contentPane;

        public void setContentPane(Container contentPane) {
            this.contentPane = contentPane;
        }

        public PointerGlassPane(Container contentPane) {
            this.contentPane = contentPane;

            addMouseListener(this);
            addMouseMotionListener(this);
            addMouseWheelListener(this);
            setOpaque(false);
        }

        public void mouseMoved(MouseEvent e) {
            redispatchMouseEvent(e, false);
        }

        public void mouseDragged(MouseEvent e) {
            redispatchMouseEvent(e, false);
        }

        public void mouseClicked(MouseEvent e) {
            redispatchMouseEvent(e, false);
        }

        public void mouseEntered(MouseEvent e) {
            redispatchMouseEvent(e, false);
        }

        public void mouseExited(MouseEvent e) {
            redispatchMouseEvent(e, false);
        }

        public void mousePressed(MouseEvent e) {
            redispatchMouseEvent(e, false);
        }

        public void mouseReleased(MouseEvent e) {
            redispatchMouseEvent(e, true);
        }

        public void mouseWheelMoved(MouseWheelEvent e) {
            redispatchMouseEvent(e, true);
        }

        private void redispatchMouseEvent(MouseEvent e, boolean repaint) {
            Point glassPanePoint = e.getPoint();

            Container container = contentPane;
            Point containerPoint = SwingUtilities.convertPoint(this,
                    glassPanePoint, contentPane);

            Component component = SwingUtilities.getDeepestComponentAt(
                    container, containerPoint.x, containerPoint.y);

            Point componentPoint = SwingUtilities.convertPoint(glassPane,
                    glassPanePoint, component);

            if (e instanceof MultipleMouseEvent) {
                MultipleMouseEvent me = (MultipleMouseEvent) e;

                component.dispatchEvent(new MultipleMouseEvent(component, me
                        .getMouseId(), e.getID(), e.getWhen(),
                        e.getModifiers(), componentPoint.x, componentPoint.y, e
                                .getClickCount(), e.isPopupTrigger()));
            } else if (!(e instanceof MouseEvent)) {
                component
                        .dispatchEvent(new MouseEvent(component, e.getID(), e
                                .getWhen(), e.getModifiers(), componentPoint.x,
                                componentPoint.y, e.getClickCount(), e
                                        .isPopupTrigger()));

            }

        }

        public void paintComponent(Graphics g) {
            super.paintComponent(g);

            // getParent().paint(g); //

            for (Iterator it = mouseStates.keySet().iterator(); it.hasNext();) {
                Integer mouseId = (Integer) it.next();
                MouseState state = (MouseState) mouseStates.get(mouseId);

                // System.out.println("PAINTING");
                // System.out.println("x = " + x + ", y = " + y);

                Color c = g.getColor();
                state.getCursor().drawCursor(g, state.getX(), state.getY());
                g.setColor(c);
            }
        }

    }

    private Map mouseStates = new HashMap();

    private MouseState getMouseState(int hDevice) {
        Integer key = new Integer(hDevice);

        MouseState state = (MouseState) mouseStates.get(key);

        if (state == null) {
            state = new MouseState(0, 0, hDevice, mouseContext(), componentContext());
            mouseStates.put(key, state);
        }

        return state;
    }

    /**
     * @see javax.swing.JFrame
     */

    public JFrameEx(String title) {
        super(title);
        setGlassPane(glassPane = new PointerGlassPane(getContentPane()));
        glassPane.setVisible(true);
        glassPane.setOpaque(false);

    }

    /**
     * @see javax.swing.JFrame
     */
    public JFrameEx() {
        this("");
    }

    public void setContentPane(Container contentPane) {
        super.setContentPane(contentPane);
        glassPane.setContentPane(contentPane);
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
    
    private Component componentContext() {
        return getContentPane();
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
            setSystemCursorPosition(getCursorPos());
        }

        // assert getSystemMouseState() != null;

        // RIDEV_NOLEGACY
        if (hDevice == getSystemMouseId()) {
            // assert state.equals(getSystemMouseState());
            moveSystemCursorPosition((int) iLastX, (int) iLastY);
            if (mouseContext().getBounds().contains(getSystemCursorPosition())) {
                while (showCursor(false) >= 0)
                    ;
                state.move((int) iLastX, (int) iLastY);
                setSystemCursorPosition(state.getLocationOnScreen());
            } else {
                while (showCursor(true) < 0)
                    ;
                //setSystemCursorPosition(getCursorPos());
                moveSystemCursorPosition((int) iLastX, (int) iLastY);// RIDEV_NOLEGACY
            }
        } else {
            state.move((int) iLastX, (int) iLastY);
        }
        
        setCursorPos(getSystemCursorPosition());
      
        
        // 0
//        if (hDevice == getSystemMouseId()) {
//            Point curPosition = getCursorPos();
//            
//            if (mouseContext().getBounds().contains(curPosition)) {
//                while (showCursor(false) >= 0);
//                Point p = state.getMouseContext().getLocationOnScreen();
//                p.x = curPosition.x - p.x;
//                p.y = curPosition.y - p.y;
//                state.move(p);
//            } else {
//                while (showCursor(true) < 0);
//            }
//            setSystemCursorPosition(curPosition);
//        } else {
//            state.move((int) iLastX, (int) iLastY);
//            setCursorPos(getSystemCursorPosition());
//        }


        glassPane.repaint();

        if (mouseContext().getBounds().contains(getSystemCursorPosition())) {
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
    }

    public native int showCursor(boolean show);

    public boolean setCursorPos(Point p) {
        return setCursorPos(p.x, p.y);
    }

    public native boolean setCursorPos(int x, int y);

    public native long getLastError();

    public native Rectangle getWindowRectangle();

    public native Point getCursorPos();
}
