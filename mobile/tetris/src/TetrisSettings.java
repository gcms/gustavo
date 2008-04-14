public class TetrisSettings {

    private int level;

    private int frameFrequency;

    private TetrisSettings() {
        level = 4;
        frameFrequency = 10;
    }

    public void setLevel(int level) {
        this.level = level;
    }

    public int getLevel() {
        return level;
    }

    public void setFrameFrequency(int frameFrequency) {
        this.frameFrequency = frameFrequency;
    }

    public int getFrameFrequency() {
        return frameFrequency;
    }

    public TetrisGame createGame() {
        return new TetrisGame(level);
    }

    private static TetrisSettings singleton;

    public static TetrisSettings getInstance() {
        if (singleton == null)
            singleton = new TetrisSettings();

        return singleton;
    }
}
