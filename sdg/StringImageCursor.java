import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.ImageObserver;

public class StringImageCursor extends ImageCursor {

    private String name;

    public StringImageCursor(String name, Image image, int x, int y) {
        super(image, x, y);
        this.name = name;
    }

    public StringImageCursor(String name, String image, int x, int y) {
        super(image, x, y);
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void drawCursor(Graphics g, int x, int y, ImageObserver observer) {
        super.drawCursor(g, x, y, observer);
        g.drawChars(name.toCharArray(), 0, name.length(), x, y);
    }
}
