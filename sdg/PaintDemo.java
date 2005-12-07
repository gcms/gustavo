import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

public class PaintDemo {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
    }

    private static final ColorCursor[] cursors = { new ColorCursor(Color.BLUE),
            new ColorCursor(Color.GREEN), new ColorCursor(Color.MAGENTA) };

    private static class Coloring {
        public Coloring(Color c, Point p) {
            this.c = c;
            this.p = p;
        }

        private Color c;

        public Color getColor() {
            return c;
        }

        private Point p;

        public Point getPoint() {
            return p;
        }
    }

    private static class DrawingPane extends JPanel implements
            MouseMotionListener {
        private List painted = new LinkedList();

        public DrawingPane() {
            addMouseMotionListener(this);
        }

        public void mouseDragged(MouseEvent e) {
            System.out.println(e);
            if (e instanceof MultipleMouseEvent) {
                MultipleMouseEvent me = (MultipleMouseEvent) e;
                me.getMouseId();
                ColorCursor c = (ColorCursor) me.getMouseState().getCursor();
                painted.add(new Coloring(c.getColor(), me.getPoint()));
            }
        }

        public void mouseMoved(MouseEvent e) {
            System.out.println(e);
        }

        public void paint(Graphics g) {
            for (Iterator it = painted.iterator(); it.hasNext(); /* */) {
                Coloring c = (Coloring) it.next();
                g.setColor(c.c);
                g.fillOval(c.p.x - 5, c.p.y - 5, 10, 10);
            }
        }

    }

    public static void createAndShowGUI() {
        final JFrameEx frameEx = new JFrameEx();

        JMenuBar menuBar = new JMenuBar();
        frameEx.setJMenuBar(menuBar);
        
        JMenu menu = new JMenu("Menu");
        menuBar.add(menu);

        JMenuItem menuItemCursor = new JMenuItem("Tornar cursor do sistema");
        menuItemCursor.addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                if (e instanceof MultipleMouseEvent) {
                    MultipleMouseEvent me = (MultipleMouseEvent) e;
                    frameEx.setSystemMouseId(me.getMouseId());
                }
            }
        });
        menu.add(menuItemCursor);

        MouseState[] states = frameEx.getMouseStates();

        for (int i = 0; i < states.length; i++) {
            states[i].setCursor(cursors[i]);
        }

        frameEx.getContentPane().add(new DrawingPane(), BorderLayout.CENTER);

        frameEx.setDefaultCloseOperation(JFrameEx.EXIT_ON_CLOSE);
        frameEx.setSize(600, 600);
        frameEx.setVisible(true);
    }
}
