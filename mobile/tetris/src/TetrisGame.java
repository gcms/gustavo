import java.util.Enumeration;

public class TetrisGame {
    private int[][] cells;

    private int width;

    private int height;

    private int currentX;

    private int currentY;

    private PieceModel current;

    public TetrisGame(int width, int height) {
        this.width = width;
        this.height = height;

        cells = new int[width][height];
    }

    public void tick() {
        if (conflict(current, currentX, currentY + 1)) {
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
    
    private int scoreGeral;
    
    private void score(int score) {
        scoreGeral += (width * score) * (score + 10) / 10;
    }
    
    public int getScore() {
        return scoreGeral;
    }

    public void left() {
        if (!conflict(current, currentX - 1, currentY)) {
            currentX--;
        }
    }

    public void right() {
        if (!conflict(current, currentX + 1, currentY)) {
            currentX++;
        }
    }

    public void rotate() {
        PieceModel rotated = current.rotate();
        if (!conflict(rotated, currentX, currentY)) {
            current = rotated;
        }
    }

    public void down() {
        if (!conflict(current, currentX, currentY + 1)) {
            currentY++;
        }
    }

    public PieceModel nextModel() {
        return PieceModel.getRandomModel();
    }

    public void start() {
        nextPiece();
    }

    private void nextPiece() {
        current = nextModel();

        currentX = width / 2 - current.getWidth() / 2;
        currentY = 0;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    public boolean conflict(PieceModel piece, int x, int y) {
        for (Enumeration e = piece.getPoints().elements(); e.hasMoreElements();) {
            Point p = (Point) e.nextElement();

            if (piece.getHeight() + y > height || y < 0
                    || piece.getWidth() + x > width || x < 0
                    || cells[p.getX() + x][p.getY() + y] != 0) {
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
}
