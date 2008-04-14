import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Enumeration;

public class TetrisBoard {
    private int[][] cells;

    private int width;

    private int height;

    private int currentX;

    private int currentY;

    private PieceModel current;

    private PieceModel next;

    private boolean finished;

    public TetrisBoard(int width, int height) {
        this.width = width;
        this.height = height;

        cells = new int[width][height];

        next = newPieceModel();
        nextPiece();
    }

    public void tick() {
        if (isFinished())
            throw new IllegalStateException();

        if (conflict(current, currentX, currentY + 1)) {
            if (currentY < 0) {
                finished = true;
            }
            place(current, currentX, currentY);
            score();

            nextPiece();
        } else {
            currentY++;
        }
    }

    private void place(PieceModel model, int dx, int dy) {
        for (Enumeration e = model.getPoints().elements(); e.hasMoreElements();) {
            Point p = (Point) e.nextElement();

            int x = p.getX() + dx;
            int y = p.getY() + dy;

            if (y >= 0)
                cells[x][y] = current.getColor();
        }
    }

    private void score() {
        int score = 0;
        // verificar linhas
        outer: for (int i = height - 1; i >= 0; i--) {
            // cada linha...

            for (int j = 0; j < width; j++) {
                // se não está completa, passa pra próxima linhas
                if (cells[j][i] == 0)
                    continue outer;
            }
            score++; // +1 linha

            // desloca as linhas
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < width; j++) {
                    cells[j][k] = cells[j][k - 1];
                }
            }

            for (int j = 0; j < width; j++) {
                cells[j][0] = 0;
            }

            // as linhas foram desloacadas,
            // então é necessário reajustar o indice
            i++;
        }

        if (score > 0) {
            score(score);
        }
    }

    private static final int[] scoreMultipliers = { 0, 10, 15, 20, 30 };

    private int totalScore;

    private void score(int score) {
        totalScore += score * scoreMultipliers[score] / 10;
    }

    public int getScore() {
        return totalScore;
    }

    public void left() {
        if (isFinished())
            throw new IllegalStateException();

        if (!conflict(current, currentX - 1, currentY)) {
            currentX--;
        }
    }

    public void right() {
        if (isFinished())
            throw new IllegalStateException();

        if (!conflict(current, currentX + 1, currentY)) {
            currentX++;
        }
    }

    public void rotate() {
        if (isFinished())
            throw new IllegalStateException();

        PieceModel rotated = current.rotate();
        if (!conflict(rotated, currentX, currentY)) {
            current = rotated;
        }
    }

    public void down() {
        if (isFinished())
            throw new IllegalStateException();

        if (!conflict(current, currentX, currentY + 1)) {
            currentY++;
        }
    }

    private PieceModel newPieceModel() {
        return PieceModel.getRandomModel();
    }

    private void nextPiece() {
        current = next;
        currentX = width / 2 - current.getWidth() / 2;
        currentY = -current.getHeight();

        next = newPieceModel();
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    private boolean conflict(PieceModel piece, int x, int y) {
        if (piece.getHeight() + y > height // || y < 0
                || piece.getWidth() + x > width || x < 0)
            return true;

        for (Enumeration e = piece.getPoints().elements(); e.hasMoreElements();) {
            Point p = (Point) e.nextElement();

            if (p.getY() + y < 0)
                continue;

            if (cells[p.getX() + x][p.getY() + y] != 0) {
                return true;
            }
        }

        return false;
    }

    public void draw(TetrisDrawer drawer) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                drawer.draw(x, y, cells[x][y]);
            }
        }

        for (Enumeration e = current.getPoints().elements(); e
                .hasMoreElements();) {
            Point p = (Point) e.nextElement();

            int x = p.getX() + currentX;
            int y = p.getY() + currentY;
            drawer.draw(x, y, current.getColor());
        }
    }

    public TetrisBoard(byte[] data) throws IOException {
        DataInputStream input = new DataInputStream(new ByteArrayInputStream(
                data));

        this.width = input.readInt();
        this.height = input.readInt();

        cells = new int[width][height];

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                cells[i][j] = input.readInt();
            }
        }

        currentX = input.readInt();
        currentY = input.readInt();

        current = PieceModel.readModel(input);
        next = PieceModel.readModel(input);
    }

    public byte[] getState() throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream output = new DataOutputStream(baos);

        output.writeInt(width);
        output.writeInt(height);

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                output.writeInt(cells[i][j]);
            }
        }

        output.writeInt(currentX);
        output.writeInt(currentY);

        current.writeData(output);
        next.writeData(output);

        return baos.toByteArray();
    }

    public boolean isFinished() {
        return finished;
    }

    public void drawNext(TetrisCanvasDrawer drawer) {
        for (Enumeration e = next.getPoints().elements(); e.hasMoreElements();) {
            Point p = (Point) e.nextElement();

            drawer.draw(p.getX(), p.getY(), next.getColor());
        }

    }
}
