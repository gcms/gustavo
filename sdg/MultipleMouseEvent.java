import java.awt.Component;
import java.awt.event.MouseEvent;

public class MultipleMouseEvent extends MouseEvent {

    /**
     * Generated serialVersionUID.
     */
    private static final long serialVersionUID = 3258417244043162936L;

    /**
     * Number of the mouse.
     */
    private int mouseId;

    /**
     * @see java.awt.event.MouseEvent
     */
    public MultipleMouseEvent(Component source, int mouseId, int id, long when,
            int modifiers, int x, int y, int clickCount, boolean popupTrigger,
            int button) {

        super(source, id, when, modifiers, x, y, clickCount, popupTrigger,
                button);
        this.mouseId = mouseId;
    }

    /**
     * @see java.awt.event.MouseEvent
     */
    public MultipleMouseEvent(Component source, int mouseId, int id, long when,
            int modifiers, int x, int y, int clickCount, boolean popupTrigger) {
        super(source, id, when, modifiers, x, y, clickCount, popupTrigger);
        this.mouseId = mouseId;
    }

    public int getMouseId() {
        return mouseId;
    }

}
