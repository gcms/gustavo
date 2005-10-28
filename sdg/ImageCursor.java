import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Point;
import java.awt.Toolkit;

public class ImageCursor implements Cursor {

    private Image image;

    private Point hotSpot;

    private static Image loadImage(String filename) {
        Image result = null;
        Toolkit toolkit = Toolkit.getDefaultToolkit();

        result = toolkit.getImage(filename);

        return result;
    }

    public ImageCursor(String filename, Point hotSpot) {
        this(loadImage(filename), hotSpot);
    }

    public ImageCursor(Image image, Point hotSpot) {
        this.image = image;
        this.hotSpot = hotSpot;
    }

    public ImageCursor(Image image) {
        this(image, 0, 0);
    }

    public ImageCursor(String filename) {
        this(loadImage(filename));
    }

    public ImageCursor(Image image, int x, int y) {
        this(image, new Point(x, y));
    }

    public ImageCursor(String filename, int x, int y) {
        this(loadImage(filename), x, y);
    }

    public Image getImage() {
        return image;
    }

    public void setImage(Image image) {
        this.image = image;
    }

    public void drawCursor(Graphics g, int x, int y) {
        g.drawImage(image, x - hotSpot.x, y - hotSpot.y, null);
    }
}
