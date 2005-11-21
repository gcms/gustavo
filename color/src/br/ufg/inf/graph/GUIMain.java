package br.ufg.inf.graph;

import java.awt.BorderLayout;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.JFrame;
import javax.swing.JTextArea;
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
        JFrame frame = new JFrame();

        final JTextArea area = new JTextArea();
        area.setEditable(false);

        frame.add(area, BorderLayout.CENTER);

        final ColorGraphLogger logger = new ColorGraphLogger() {
            public void prepare(ColorGraph graph, Vertex v) {
                synchronized (graph) {
                    try {
                        graph.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }

            public void okColor(final Vertex v, final Color color) {
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        area.append(ColorGraph.getName(v) + " = " + color
                                + "\n");
                    }
                });
            }

            public void failColor(final Vertex v, final Color color,
                    final Vertex fail) {
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        area.append(ColorGraph.getName(v) + " = " + color
                                + " FALHA c/ " + ColorGraph.getName(fail)
                                + "\n");
                    }
                });
            }

            public void backtrack(final Vertex v) {
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        area.append(ColorGraph.getName(v) + " BACKTRACKING\n");
                    }
                });
            }
        };

        final String[] names = { "A", "B", "C", "D", "E", "F" };
        final byte[][] ajacencias = { { 0, 1, 1, 0, 1, 0 },
                { 1, 0, 0, 0, 1, 1 }, { 1, 0, 0, 0, 1, 1 },
                { 0, 0, 0, 0, 0, 1 }, { 1, 1, 1, 0, 0, 1 },
                { 0, 1, 1, 1, 1, 0 } };

        area.addKeyListener(new KeyAdapter() {
            public void keyTyped(KeyEvent ev) {
                if (t == null || !t.isAlive()) {
                    t = new Thread() {
                        public void run() {
                            area.setText("");
                            g = new ColorGraph(names, ajacencias);
                            g.setLogger(logger);
                            g.color(new Color[] { Color.GREEN, Color.RED,
                                    Color.BLUE });
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
