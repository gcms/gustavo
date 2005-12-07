import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.image.ImageObserver;

public class ColorCursor implements Cursor {
    private Color c;

    public ColorCursor(Color c) {
        this.c = c;
    }

    public void drawCursor(Graphics g, int x, int y, ImageObserver observer) {
        g.setColor(Color.WHITE);
        g.fillOval(x - 6, y - 6, 13, 13);

        g.setColor(c);
        g.fillOval(x - 5, y - 5, 11, 11);

        g.setColor(Color.WHITE);
        g.drawLine(x, y - 3, x, y + 3);
        g.drawLine(x - 3, y, x + 3, y);
    }

    public Color getColor() {
        return c;
    }
}
