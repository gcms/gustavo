import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.image.ImageObserver;

public class ImageCursor implements Cursor {

    public static final ImageCursor ARROW = new ImageCursor("arrow.png");

    private Image image;

    private Point hotSpot;

    private double rotation;

    private static Image loadImage(String filename) {
        Image result = null;
        Toolkit toolkit = Toolkit.getDefaultToolkit();

        result = toolkit.getImage(filename);

        return result;
    }

    public ImageCursor(String filename, Point hotSpot) {
        this(loadImage(filename), hotSpot, 0);
    }

    public ImageCursor(ImageCursor cursor, double rotation) {
        this(cursor.image, cursor.hotSpot, rotation);
    }

    public ImageCursor(Image image, Point hotSpot, double rotation) {
        this.rotation = rotation;
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
        this(image, new Point(x, y), 0);
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

    public void drawCursor(Graphics g, int x, int y, ImageObserver observer) {
        Graphics2D g2d = (Graphics2D) g;

        g2d.translate(x - hotSpot.x, y - hotSpot.y);

        g2d.rotate(rotation);

        // g.drawImage(image, x - hotSpot.x, y - hotSpot.y, observer);

        g2d.drawImage(image, 0, 0, observer);

        g2d.rotate(-rotation);

        g2d.translate(-x + hotSpot.x, -y + hotSpot.y);
    }

    public double getRotation() {
        return rotation;
    }

    public void setRotation(double rotation) {
        this.rotation = rotation;
    }
}
