import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.game.GameCanvas;

public class TetrisCanvas extends GameCanvas implements Runnable {

    private boolean running;
    private TetrisGame game;

    public TetrisCanvas() {
        super(true);

        game = new TetrisGame(10, 20);
        game.start();
    }

    public void start() {
        running = true;
        new Thread(this).start();
    }

    public void stop() {
        running = false;
    }

    public void run() {
        Graphics g = getGraphics();

        int frame = 0;

        while (running) {

            if (frame == 0)
                game.tick();
            
            frame = (frame + 1) % 8;

            int states = getKeyStates();
            if ((states & LEFT_PRESSED) != 0) {
                game.left();
            } else if ((states & RIGHT_PRESSED) != 0) {
                game.right();
            } else if ((states & UP_PRESSED) != 0) {
                game.rotate();
            } else if ((states & DOWN_PRESSED) != 0) {
                game.down();
            }

            long start = System.currentTimeMillis();
            render(g);
            flushGraphics();
            long duration = System.currentTimeMillis() - start;
            

            try {
                Thread.sleep(Math.max(100 - duration, 0));
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }

    }

    protected void render(Graphics g) {

        g.setColor(0xffffff);
        g.fillRect(0, 0, getWidth(), getHeight());

        game.draw(new TetrisCanvasDrawer(g, Math.min(getWidth() / 10,
                getHeight() / 20)));
        
        g.drawString("" + game.getScore(), getWidth(), getHeight(),
                Graphics.BOTTOM | Graphics.RIGHT);

    }
}
