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
