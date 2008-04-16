import java.io.IOException;

public class TetrisGame {
    private TetrisBoard board;

    private int level;

    public TetrisGame(int level) {
        board = new TetrisBoard(10, 20);
        this.level = level;
    }

    public int getLevel() {
        return level;
    }

    public TetrisBoard getBoard() {
        return board;
    }

    public int getScore() {
        return getBoard().getScore() * (level + 10) / 10;
    }

    public TetrisGame(byte[] data) throws IOException {
        this.level = 4; // should be in data
        board = new TetrisBoard(data);
    }

    public byte[] getState() throws IOException {
        return board.getState();
    }
}
