import java.util.Timer;
import java.util.TimerTask;

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Graphics;

public class TetrisGame extends Canvas {
    private TetrisBoard board;

    public TetrisGame() {
        board = new TetrisBoard(10, 20);

        board.start();

        new Timer().schedule(new TimerTask() {
            public void run() {
                System.out.println("run()");
                synchronized (board) {
                    board.tick();
                }
                System.out.println("ticked()");
                repaint();

            }
        }, 0, 800);
    }

    protected void paint(Graphics g) {
        board.draw(new TetrisCanvasDrawer(g, Math.min(getWidth() / 10,
                getHeight() / 20)));
    }

    protected void keyPressed(int keyCode) {
        doKey(keyCode);
    }

    protected void keyRepeated(int keyCode) {
        doKey(keyCode);
    }

    private void doKey(int keyCode) {
        int gc = getGameAction(keyCode);

        if (gc != 0) {
            synchronized (board) {
                doAction(gc);
            }
            repaint();
        }
    }

    private void doAction(int action) {
        switch (action) {
            case UP:
                board.rotate();
                break;
            case LEFT:
                board.left();
                break;
            case RIGHT:
                board.right();
                break;
            case DOWN:
                board.down();
                break;
        }
    }
}
