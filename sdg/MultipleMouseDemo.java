import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

public class MultipleMouseDemo {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
    }

    public static void createAndShowGUI() {
        final JFrame frame = new JFrameEx();

        ColoredCircle circle = new ColoredCircle(10000);
        circle.addColor(Color.BLUE);
        circle.addColor(Color.RED);
        circle.addColor(Color.GREEN);
        circle.addColor(Color.MAGENTA);

        frame.add(circle, BorderLayout.CENTER);

        JButton button = new JButton("Click Me");
        button.addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent arg0) {
                new Exception().printStackTrace();
                if (arg0 instanceof MultipleMouseEvent) {
                    MultipleMouseEvent me = (MultipleMouseEvent) arg0;
                    System.out.println(me.getMouseId() + " clicked");
                }
            }
        });

        frame.add(button, BorderLayout.PAGE_END);

        frame.setSize(600, 600);

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        // frame.setExtendedState(frame.getExtendedState() |
        // JFrame.MAXIMIZED_BOTH);
        frame.setVisible(true);

    }
}

class ColoredCircle extends JPanel implements MouseListener {
    private List colors;

    private int colorNumber;

    public void addColor(Color c) {
        colors.add(c);
    }

    public Color getColor() {
        return (Color) colors.get(colorNumber);
    }

    public void paint(Graphics g) {
        g.setColor(getColor());
        g.fillOval(200, 200, 200, 200);
    }

    public ColoredCircle(int interval) {
        colors = new ArrayList();

        this.addMouseListener(this);

        Timer timer = new Timer();

        timer.schedule(new TimerTask() {
            public void run() {
                colorNumber = (int) (Math.random() * colors.size());
                try {
                    SwingUtilities.invokeAndWait(new Runnable() {
                        public void run() {
                            paint(getGraphics());
                        }
                    });
                } catch (InvocationTargetException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

            }
        }, 0, interval);
    }

    public void mouseClicked(MouseEvent arg0) {
        System.out.println(arg0);
    }

    public void mousePressed(MouseEvent arg0) {
    }

    public void mouseReleased(MouseEvent arg0) {
    }

    public void mouseEntered(MouseEvent arg0) {
    }

    public void mouseExited(MouseEvent arg0) {
    }
}