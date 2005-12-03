import java.awt.Graphics;
import java.awt.image.ImageObserver;

public interface Cursor {
    public void drawCursor(Graphics g, int x, int y, ImageObserver observer);
}
