import javax.microedition.lcdui.Graphics;

public class TetrisCanvasDrawer implements TetrisDrawer {
    private Graphics g;

    private int blockSize;

    private int baseX, baseY;

    private int background;

    public TetrisCanvasDrawer(Graphics g, int x, int y, int blockSize,
            int background) {
        this.g = g;
        baseX = x;
        baseY = y;
        this.background = background;
        this.blockSize = blockSize;
    }

    public TetrisCanvasDrawer(Graphics g, int x, int y, int blockSize) {
        this(g, x, y, blockSize, 0xe0e0e0);
    }

    public void draw(int x, int y, int color) {

        if (color != 0) {
            g.setColor(0xc0c0c0);
            g.fillRect(baseX + blockSize * x + 1, baseY + blockSize * y + 1,
                    blockSize - 2, blockSize - 2);
        } else {
            g.setColor(0xffffff);
            g.fillRect(baseX + blockSize * x, baseY + blockSize * y, blockSize,
                    blockSize);
        }

        g.setColor(color == 0 ? background : color);
        g.fillRect(baseX + blockSize * x, baseY + blockSize * y, blockSize - 3,
                blockSize - 3);
    }
}
