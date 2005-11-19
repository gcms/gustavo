package br.ufg.inf.graph;

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

public class GUIMain {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
    }

    private static Thread t;

    private static ColorGraph g;

    private static void createAndShowGUI() {
        final ColorGraphLogger logger = new ColorGraphLogger() {
            public void prepare(ColorGraph graph) {
                synchronized (graph) {
                    try {
                        graph.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }

            public void okColor(Vertex v, Color color) {
                System.out.println(ColorGraph.getName(v) + " = " + color);
            }

            public void failColor(Vertex v, Color color, Vertex fail) {
                System.out.println(ColorGraph.getName(v) + " = " + color
                        + " FALHA c/ " + ColorGraph.getName(fail));
            }

            public void backtrack(Vertex v) {
                System.out.println(ColorGraph.getName(v) + " BACKTRACKING");
            }
        };

        final String[] names = { "A", "B", "C", "D", "E", "F" };
        final byte[][] ajacencias = { { 0, 1, 1, 0, 1, 0 },
                { 1, 0, 0, 0, 1, 1 }, { 1, 0, 0, 0, 1, 1 },
                { 0, 0, 0, 0, 0, 1 }, { 1, 1, 1, 0, 0, 1 },
                { 0, 1, 1, 1, 1, 0 } };

        JFrame frame = new JFrame();
        frame.addKeyListener(new KeyAdapter() {
            public void keyTyped(KeyEvent ev) {
                if (t == null || !t.isAlive()) {
                    t = new Thread() {
                        public void run() {
                            g = new ColorGraph(names, ajacencias);
                            g.setLogger(logger);
                            g.color(new Color[] { Color.GREEN, Color.RED,
                                    Color.BLUE });
                            System.out.println("TERMINOU\n");
                        }
                    };
                    t.start();
                } else {
                    synchronized (g) {
                        g.notify();
                    }
                }
            }
        });

        frame.setSize(400, 400);

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        frame.setVisible(true);

    }
}
