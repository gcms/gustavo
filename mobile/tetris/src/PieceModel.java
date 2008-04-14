import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.Enumeration;
import java.util.Random;
import java.util.Vector;

public class PieceModel {
    private Vector points = new Vector();

    private int maxX = 0;

    private int maxY = 0;

    private int color;

    protected PieceModel(int color) {
        this.color = color;
    }

    protected void addPoint(Point p) {
        maxX = Math.max(maxX, p.getX());
        maxY = Math.max(maxY, p.getY());

        points.addElement(p);
    }

    protected void addPoint(int x, int y) {
        addPoint(new Point(x, y));
    }

    public static final PieceModel SQUARE_PIECE_MODEL = new PieceModel(
            0x0000ff00);

    static {
        SQUARE_PIECE_MODEL.addPoint(0, 0);
        SQUARE_PIECE_MODEL.addPoint(0, 1);
        SQUARE_PIECE_MODEL.addPoint(1, 0);
        SQUARE_PIECE_MODEL.addPoint(1, 1);
    }

    public static final PieceModel T_PIECE_MODEL = new PieceModel(0x00ff0000);

    static {
        T_PIECE_MODEL.addPoint(0, 1);
        T_PIECE_MODEL.addPoint(1, 1);
        T_PIECE_MODEL.addPoint(2, 1);
        T_PIECE_MODEL.addPoint(1, 0);
    }

    public static final PieceModel I_PIECE_MODEL = new PieceModel(0x000000FF);

    static {
        I_PIECE_MODEL.addPoint(0, 0);
        I_PIECE_MODEL.addPoint(0, 1);
        I_PIECE_MODEL.addPoint(0, 2);
        I_PIECE_MODEL.addPoint(0, 3);
    }

    public static final PieceModel L_PIECE_MODEL = new PieceModel(0x0000FFFF);

    static {
        L_PIECE_MODEL.addPoint(0, 0);
        L_PIECE_MODEL.addPoint(1, 0);
        L_PIECE_MODEL.addPoint(1, 1);
        L_PIECE_MODEL.addPoint(1, 2);
    }

    public static final PieceModel RIGHT_PIECE_MODEL = new PieceModel(
            0x00FF00FF);

    static {
        RIGHT_PIECE_MODEL.addPoint(0, 1);
        RIGHT_PIECE_MODEL.addPoint(1, 1);
        RIGHT_PIECE_MODEL.addPoint(1, 0);
        RIGHT_PIECE_MODEL.addPoint(2, 0);
    }

    public Vector getPoints() {
        return points;
    }

    public PieceModel rotate() {
        PieceModel rotated = new PieceModel(getColor());

        int centerX = (maxX + 1) / 2;
        int centerY = (maxY + 1) / 2;

        for (Enumeration e = points.elements(); e.hasMoreElements();) {
            Point p = (Point) e.nextElement();

            rotated.addPoint(-(p.getY() - centerY) + centerX,
                    (p.getX() - centerX) + centerY);
        }

        rotated.setOrigin(0, 0);

        return rotated;
    }

    private void setOrigin(int x, int y) {
        int minX = Integer.MAX_VALUE;
        int minY = Integer.MAX_VALUE;

        for (Enumeration e = points.elements(); e.hasMoreElements();) {
            Point p = (Point) e.nextElement();

            minX = Math.min(minX, p.getX());
            minY = Math.min(minY, p.getY());
        }

        maxX = 0;
        maxY = 0;
        for (Enumeration e = points.elements(); e.hasMoreElements();) {
            Point p = (Point) e.nextElement();
            p.setX(p.getX() + x - minX);
            p.setY(p.getY() + y - minY);

            maxX = Math.max(maxX, p.getX());
            maxY = Math.max(maxY, p.getY());
        }

    }

    public int getWidth() {
        return maxX + 1;
    }

    public int getColor() {
        return color;
    }

    public int getHeight() {
        return maxY + 1;
    }

    private static final PieceModel[] pieces = { SQUARE_PIECE_MODEL,
            T_PIECE_MODEL, I_PIECE_MODEL, L_PIECE_MODEL, RIGHT_PIECE_MODEL };

    public static PieceModel getRandomModel() {
        return pieces[Math
                .abs(new Random(System.currentTimeMillis()).nextInt())
                % pieces.length];
    }

    public static PieceModel readModel(DataInput input) throws IOException {
        PieceModel pm = new PieceModel(input.readInt());
        pm.maxX = input.readInt();
        pm.maxY = input.readInt();
        
        
        int nPoints = input.readInt();
        for (int i = 0; i < nPoints; i++) {
            Point p = new Point(0, 0);
            p.setX(input.readInt());
            p.setY(input.readInt());
            
            pm.points.addElement(p);
        }

        return pm;

    }

    public void writeData(DataOutput output) throws IOException {
        output.writeInt(color);
        output.writeInt(maxX);
        output.writeInt(maxY);
        
        int nPoints = points.size();
        output.writeInt(nPoints);
        for (int i = 0; i < nPoints; i++) {
            Point p = (Point) points.elementAt(i);
            
            output.writeInt(p.getX());
            output.writeInt(p.getY());
        }
    }
}
