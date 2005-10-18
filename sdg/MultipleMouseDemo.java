import java.awt.BorderLayout;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JButton;
import javax.swing.JFrame;
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
        JFrame frame = new JFrameEx();

        JButton button = new JButton("HELLO");
        button.addMouseListener(new MouseAdapter() {
            public String getMouseId(MouseEvent ev) {
                if (ev instanceof MultipleMouseEvent) {
                    MultipleMouseEvent e = (MultipleMouseEvent) ev;
                    return String.valueOf(e.getMouseId());
                }
                return "";
            }

            public void mouseClicked(MouseEvent ev) {
                System.out.print(getMouseId(ev) + " ");
                System.out.print("CLICKED");
                System.out.println("\tx = " + ev.getX() + ", y = " + ev.getY());
            }

            public void mousePressed(MouseEvent ev) {
                System.out.print(getMouseId(ev) + " ");
                System.out.print("PRESSED");
                System.out.println("\tx = " + ev.getX() + ", y = " + ev.getY());
            }

            public void mouseReleased(MouseEvent ev) {
                System.out.print(getMouseId(ev) + " ");
                System.out.print("RELEASED");
                System.out.println("\tx = " + ev.getX() + ", y = " + ev.getY());
            }

            public void mouseEntered(MouseEvent ev) {
                System.out.print(getMouseId(ev) + " ");
                System.out.print("ENTERED");
                System.out.println("\tx = " + ev.getX() + ", y = " + ev.getY());
            }

            public void mouseExited(MouseEvent ev) {
                System.out.print(getMouseId(ev) + " ");
                System.out.print("EXITED");
                System.out.println("\tx = " + ev.getX() + ", y = " + ev.getY());
            }
        });

        frame.getContentPane().add(button, BorderLayout.PAGE_END);

        frame.setSize(400, 400);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }

}
