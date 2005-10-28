import java.awt.Graphics;
import java.awt.Image;

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

    public void drawCursor(Graphics g, int x, int y) {
        super.drawCursor(g, x, y);
        g.drawChars(name.toCharArray(), 0, name.length(), x, y);
    }
}
