import java.awt.Color;
import java.awt.Component;
import java.awt.Point;
import java.awt.event.MouseEvent;
import java.util.HashMap;
import java.util.Map;

import javax.swing.SwingUtilities;

public class MouseState {
    public static final int BUTTON1 = 0;

    public static final int BUTTON2 = 1;

    public static final int BUTTON3 = 2;

    /**
     * Stores the current mouse pointer position.
     */
    private Point currentPoint;

    private int mouseId;

    /**
     * Represents button pressing state.
     */
    private static class Button {
        private Map clicked = new HashMap();

        private boolean pressed;

        private Point pressStartPoint = null;

        public void press(Point p) {
            pressed = true;
            pressStartPoint = p;
        }

        public void press(int x, int y) {
            press(new Point(x, y));
        }

        public boolean isPressed() {
            return pressed;
        }

        public Point getPressStartPoint() {
            return pressed ? pressStartPoint : null;
        }

        public int click(Component c) {
            Integer count = (Integer) clicked.get(c);
            if (count == null) {
                count = new Integer(1);
            } else {
                count = new Integer(count.intValue() + 1);
            }

            clicked.put(c, count);
            return count.intValue();
        }

        public int clickCount(Component c) {
            Integer count = (Integer) clicked.get(c);

            return count == null ? 0 : count.intValue();
        }

        public void release() {
            pressed = false;
        }

        public int getPressStartY() {
            return getPressStartPoint().y;
        }

        public int getPressStartX() {
            return getPressStartPoint().x;
        }
    }

    private Button[] buttons;

    private Component mouseContext;

    private static final Color[] colors = { Color.BLUE, Color.RED, Color.GREEN,
            Color.MAGENTA };

    private Color cursorColor;

    public MouseState(int x, int y, int mouseId, Component mouseContext) {
        currentPoint = new Point(x, y);
        this.mouseId = mouseId;
        this.mouseContext = mouseContext;

        this.cursorColor = colors[(int) (Math.random() * 4)];

        buttons = new Button[3];

        for (int i = 0; i < 3; i++) {
            buttons[i] = new Button();
        }

    }

    public Color getCursorColor() {
        return cursorColor;
    }

    /**
     * Current x cursor position.
     * 
     * @return x coordinate of cursor position
     */
    public int getX() {
        return currentPoint.x;
    }

    /**
     * Current y cursor position.
     * 
     * @return y coordinate of cursor position
     */
    public int getY() {
        return currentPoint.y;
    }

    /**
     * Current cursor position.
     * 
     * @return coordinate of cursor position
     */
    public Point getLocation() {
        return currentPoint;
    }

    public Point getLocationOnScreen() {
        Point scP = mouseContext.getLocationOnScreen();
        Point curP = getLocation();
        return new Point(scP.x + curP.x, scP.y + curP.y);
    }

    private int getButton(int buttonNo) {
        switch (buttonNo) {
        case BUTTON1:
            return MouseEvent.BUTTON1;
        case BUTTON2:
            return MouseEvent.BUTTON2;
        case BUTTON3:
            return MouseEvent.BUTTON3;
        default:
            break;
        }

        return buttonNo;
    }

    public boolean isButtonPressed(int buttonNo) {
        return buttons.length > buttonNo && buttons[buttonNo].pressed;
    }

    public void pressButton(int buttonNo) {
        if (buttons.length > buttonNo) {
            Button b = buttons[buttonNo];

            Component comp = SwingUtilities.getDeepestComponentAt(mouseContext,
                    getX(), getY());
            Point p = relativeTo(comp);

            comp.dispatchEvent(new MultipleMouseEvent(comp, mouseId,
                    MouseEvent.MOUSE_PRESSED, System.currentTimeMillis(),
                    getModifiers(), p.x, p.y, b.clickCount(comp), false,
                    getButton(buttonNo)));

            b.press(getLocation());
        }
    }

    public void releaseButton(int buttonNo) {
        if (buttons.length > buttonNo) {
            Button b = buttons[buttonNo];

            if (b.isPressed()) {
                Component oldComp = SwingUtilities.getDeepestComponentAt(
                        mouseContext, b.getPressStartX(), b.getPressStartY());
                Component newComp = SwingUtilities.getDeepestComponentAt(
                        mouseContext, getX(), getY());

                // System.out.println(oldComp);
                // System.out.println(newComp);

                Point p = relativeTo(newComp);

                if (oldComp.equals(newComp)) {
                    newComp.dispatchEvent(new MultipleMouseEvent(newComp,
                            mouseId, MouseEvent.MOUSE_CLICKED, System
                                    .currentTimeMillis(), getModifiers(), p.x,
                            p.y, b.click(newComp), false, getButton(buttonNo)));
                }

                newComp.dispatchEvent(new MultipleMouseEvent(newComp, mouseId,
                        MouseEvent.MOUSE_RELEASED, System.currentTimeMillis(),
                        getModifiers(), p.x, p.y, b.clickCount(newComp), false,
                        getButton(buttonNo)));

                b.release();
            }
        }
    }

    public void move(int iLastX, int iLastY) {
        Point newP = new Point(getX() + iLastX, getY() + iLastY);

        newP.x = newP.x < 0 ? 0
                : newP.x > mouseContext.getWidth() ? mouseContext.getWidth()
                        : newP.x;
        newP.y = newP.y < 0 ? 0
                : newP.y > mouseContext.getHeight() ? mouseContext.getHeight()
                        : newP.y;

        move(newP);
    }

    public void move(Point p) throws IllegalArgumentException {

        if (p.x > mouseContext.getWidth() || p.y > mouseContext.getHeight()) {
            throw new IllegalArgumentException(
                    "trying to move pointer outside bounds");
        }

        Component oldComp = SwingUtilities.getDeepestComponentAt(mouseContext,
                getX(), getY());
        Component newComp = SwingUtilities.getDeepestComponentAt(mouseContext,
                p.x, p.y);

        long when = System.currentTimeMillis();

        if (!oldComp.equals(newComp)) {
            oldComp.dispatchEvent(new MultipleMouseEvent(oldComp, mouseId,
                    MouseEvent.MOUSE_EXITED, when, getModifiers(), p.x, p.y, 0,
                    false));

            newComp.dispatchEvent(new MultipleMouseEvent(newComp, mouseId,
                    MouseEvent.MOUSE_ENTERED, when, getModifiers(), p.x, p.y,
                    0, false));

            // System.out.println("OLDCOMP = " + oldComp);
            // System.out.println("NEWCOMP = " + newComp);
        }

        mouseContext.dispatchEvent(new MultipleMouseEvent(mouseContext,
                mouseId, MouseEvent.MOUSE_MOVED, when, getModifiers(), p.x,
                p.y, 0, false));

        this.currentPoint = p;
    }

    private int modifiers;

    public void setModifiers(int modifiers) {
        this.modifiers = modifiers;
    }

    public int getModifiers() {
        return modifiers;
    }

    private Point relativeTo(Component c) {
        return SwingUtilities.convertPoint(mouseContext, getX(), getY(), c);
    }

    public String toString() {
        return "Pointer at: " + currentPoint;
    }
}
