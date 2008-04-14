import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.game.GameCanvas;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreFullException;
import javax.microedition.rms.RecordStoreNotFoundException;

public class TetrisCanvas extends GameCanvas implements Runnable,
        CommandListener {

    private TetrisGame game;

    /* Looping fields */
    private int frameTime;

    private int tickTime;

    private int tickFrameNum;

    private int currentFrame = 0;

    /* State fields */
    private boolean paused;

    private Thread workingThread;

    /* Rendering fields */
    private Graphics g;

    private int blockSize;

    private int contentX, contentY, contentWidth, contentHeight;

    private int boardX, boardY, boardWidth, boardHeight;

    private int infoX, infoY, infoWidth, infoHeight;

    private int nextX, nextY, nextWidth, nextHeight, nextBlockSize;

    private TetrisCanvasDrawer drawer;

    private TetrisCanvasDrawer nextDrawer;

    /* Command fields */
    private Command pauseCommand;

    private Command resumeCommand;

    private CommandListener listener;

    public TetrisCanvas() {
        super(true);

        g = getGraphics();

        pauseCommand = new Command("Pause", Command.STOP, 1);
        resumeCommand = new Command("Resume", Command.OK, 1);

        addCommand(pauseCommand);
        super.setCommandListener(this);
    }

    public void commandAction(Command c, Displayable d) {
        if (listener != null)
            listener.commandAction(c, d);

        if (c == pauseCommand && !paused) {
            pause();
        } else if (c == resumeCommand && paused) {
            resume();
        }
    }

    public void setCommandListener(CommandListener listener) {
        this.listener = listener;
    }

    public void setFrameFrequency(int frameFrequency) {
        // System.out.println("frameFrequency set to " + frameFrequency);
        frameTime = 1000 / Math.max(1, Math.min(frameFrequency, 20));
        setTickTime(tickTime);
    }

    private void setTickTime(int tickTime) {
        this.tickTime = tickTime;
        tickFrameNum = Math.max(tickTime, frameTime) / frameTime;

        // System.out.println("tick Time is " + tickTime);
        // System.out.println("tickFrameNum is " + tickFrameNum + " at " +
        // frameTime);
    }

    private void setLevel(int speed) {
        if (speed < 1 || speed > 9) {
            throw new IllegalArgumentException(
                    "Speed should be between 1 and 9");
        }

        setTickTime(1000 / speed);
    }

    public void setGame(TetrisGame game) {
        if (this.game != null && this.game != game) {
            this.game = game;
            resume();
        }

        this.game = game;

        setLevel(game.getLevel());

        blockSize = Math.min(getWidth() / game.getBoard().getWidth(),
                getHeight() / game.getBoard().getHeight()) - 1;

        boardWidth = blockSize * game.getBoard().getWidth();
        boardHeight = blockSize * game.getBoard().getHeight();

        boardX = boardY = (getHeight() - boardHeight) / 2;

        contentHeight = boardHeight + 2;
        contentX = contentY = (getHeight() - contentHeight) / 2;
        contentWidth = Math.max(boardWidth, (getWidth() - 2 * contentX));

        drawer = new TetrisCanvasDrawer(g, boardX + 2, boardY + 2, blockSize);

        infoX = boardX + boardWidth + 2;
        infoY = boardY;

        infoWidth = contentWidth - boardWidth - 4;
        infoHeight = boardHeight;

        // 40 is the height of the score string
        nextBlockSize = Math.min(blockSize, Math.min(infoWidth / 3,
                infoHeight - 40 / 4));
        nextWidth = nextBlockSize * 3;
        nextHeight = nextBlockSize * 4;

        nextX = infoX + (infoWidth - nextWidth) / 2;
        nextY = infoY + 20;

        nextDrawer = new TetrisCanvasDrawer(g, nextX, nextY, nextBlockSize,
                0xffffff);
    }

    public void show() {
        g.setColor(0xffffff);
        g.fillRect(0, 0, getWidth(), getHeight());

        if (game.getBoard().isFinished()) {
            gameDraw();
            // } else if (workingThread != null) {
            // resume();
        } else if (workingThread == null) {
            start();
        }
    }

    public void hide() {
        stop();
    }

    public void start() {
        if (game == null)
            throw new IllegalStateException("Game not set");

        if (workingThread != null)
            throw new IllegalStateException("Already started");

        if (game.getBoard().isFinished())
            throw new IllegalStateException(
                    "Trying to start game already finished");

        workingThread = new Thread(this);
        workingThread.start();

        gameDraw();
        flushGraphics();
    }

    public void stop() {
        workingThread = null;
        System.out.println("Stopped!");
    }

    public void pause() {
        if (game.getBoard().isFinished())
            throw new IllegalStateException();

        paused = true;

        removeCommand(pauseCommand);
        addCommand(resumeCommand);
    }

    public void resume() {
        if (game.getBoard().isFinished())
            throw new IllegalStateException();

        paused = false;

        removeCommand(resumeCommand);
        addCommand(pauseCommand);
    }

    public void run() {
        while (Thread.currentThread() == workingThread
                && !game.getBoard().isFinished()) {

            long start = System.currentTimeMillis();
            if (!paused) {
                gameLoop();
            } else {
                // paused
            }

            long duration = System.currentTimeMillis() - start;

            try {
                Thread.sleep(Math.max(frameTime - duration, 0));
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }

        }

        if (game.getBoard().isFinished()) {
            removeCommand(pauseCommand);
            removeCommand(resumeCommand);

            saveScore();
        }

        stop();
    }

    private void gameLoop() {

        if ((currentFrame % tickFrameNum) == 0)
            game.getBoard().tick();

        currentFrame++;

        if (game.getBoard().isFinished())
            return;

        int states = getKeyStates();
        if ((states & LEFT_PRESSED) != 0) {
            game.getBoard().left();
        } else if ((states & RIGHT_PRESSED) != 0) {
            game.getBoard().right();
        } else if ((states & UP_PRESSED) != 0) {
            game.getBoard().rotate();
        } else if ((states & DOWN_PRESSED) != 0) {
            game.getBoard().down();
        }

        gameDraw();

    }

    private void gameDraw() {
        render(g);
        flushGraphics();
    }

    protected void render(Graphics g) {

        g.setColor(0xffffff);
        g.fillRect(0, 0, getWidth(), getHeight());

        g.setClip(boardX, boardY, boardWidth + 1, boardHeight + 1);
        game.getBoard().draw(drawer);

        g.setClip(nextX, nextY, nextWidth + 1, nextHeight + 1);
        game.getBoard().drawNext(nextDrawer);

        g.setClip(contentX, contentY, contentWidth + 1, contentHeight + 1);

        g.setColor(0);
        g.drawRect(contentX, contentY, contentWidth, contentHeight);

        // g.drawRect(infoX, infoY, infoWidth, infoHeight);

        // g.setColor(0xffff00);
        // g.drawRect(boardX, boardY, boardWidth, boardHeight);

        Font f = Font.getFont(Font.FACE_SYSTEM, Font.STYLE_BOLD,
                Font.SIZE_LARGE);
        g.setFont(f);

        g.drawString("Score: " + game.getScore(), infoX + infoWidth - 5, infoY
                + infoHeight, Graphics.BOTTOM | Graphics.RIGHT);

        g.drawString("Level: " + game.getLevel(), infoX + infoWidth - 5, infoY
                + infoHeight - f.getHeight() - 2, Graphics.BOTTOM
                | Graphics.RIGHT);
    }

    // should not be in this class
    private void saveScore() {

        try {
            RecordStore store = RecordStore.openRecordStore("tetris", true);

            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            DataOutputStream dos = new DataOutputStream(baos);

            if (store.getNumRecords() > 1) {
                byte[] input = store.getRecord(2);
                dos.write(input);
            }

            dos.writeLong(System.currentTimeMillis());
            dos.writeInt(game.getScore());
            dos.flush();

            byte[] output = baos.toByteArray();
            if (store.getNumRecords() > 1) {
                store.setRecord(2, output, 0, output.length);
            } else {
                int recordId = 0;
                do {
                    recordId = store.addRecord(output, 0, output.length);
                } while (recordId < 2);
            }

            store.closeRecordStore();
        } catch (RecordStoreFullException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (RecordStoreNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (RecordStoreException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

    }
}
