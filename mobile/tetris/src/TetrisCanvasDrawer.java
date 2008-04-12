import javax.microedition.lcdui.Graphics;

public class TetrisCanvasDrawer implements TetrisDrawer {
    private Graphics g;

    private int blockSize;

    public TetrisCanvasDrawer(Graphics g, int blockSize) {
        this.g = g;
        this.blockSize = blockSize;
    }

    public void draw(int x, int y, int color) {

        if (color != 0) {
            g.setColor(0xc0c0c0);
            g.fillRect(blockSize * x + 1, blockSize * y + 1, blockSize - 2,
                    blockSize - 2);
        } else {
            g.setColor(0xffffff);
            g.fillRect(blockSize * x, blockSize * y, blockSize, blockSize);
        }

        g.setColor(color == 0 ? 0xe0e0e0 : color);
        g.fillRect(blockSize * x, blockSize * y, blockSize - 3, blockSize - 3);
    }
}
