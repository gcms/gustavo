import java.awt.Component;
import java.awt.event.MouseEvent;

public class MultipleMouseEvent extends MouseEvent {

    /**
     * Generated serialVersionUID.
     */
    private static final long serialVersionUID = 3258417244043162936L;

    /**
     * State of the mouse.
     */
    private MouseState mouseState;

    /**
     * @see java.awt.event.MouseEvent
     */
    public MultipleMouseEvent(Component source, MouseState state, int id,
            long when, int modifiers, int x, int y, int clickCount,
            boolean popupTrigger, int button) {

        super(source, id, when, modifiers, x, y, clickCount, popupTrigger,
                button);
        this.mouseState = state;
    }

    /**
     * @see java.awt.event.MouseEvent
     */
    public MultipleMouseEvent(Component source, MouseState state, int id,
            long when, int modifiers, int x, int y, int clickCount,
            boolean popupTrigger) {
        super(source, id, when, modifiers, x, y, clickCount, popupTrigger);
        this.mouseState = state;
    }

    public int getMouseId() {
        return getMouseState().getMouseId();
    }

    public MouseState getMouseState() {
        return mouseState;
    }

    public String toString() {
        return "mouseId=" + getMouseId() + " " + super.toString();
    }

}
