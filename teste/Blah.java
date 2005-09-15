import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.EventObject;

import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.SpringLayout;
import javax.swing.border.EtchedBorder;

public class Blah extends JFrame implements DelayConf {

	private JTextPane textPane;

	private JList list;

	private DefaultListModel model;

	private JTextField number;

	private SpringLayout springLayout;

	private JTextField delay;

	private static class Element {
		public String line;

		public String text;

		public Element(String line, String text) {
			this.line = line;
			this.text = text;
		}

		public String toString() {
			return line;
		}
	}

	public static void main(String args[]) {
		try {
			Blah frame = new Blah();
			frame.setVisible(true);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private DefaultListModel getModel() {
		if (model == null) {
			model = new DefaultListModel();
		}

		return model;
	}

	public Blah() {
		super();
		setBounds(100, 100, 500, 375);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		getContentPane().setLayout(new BorderLayout());
		{
			final JPanel panel = new JPanel();
			panel.setLayout(new FlowLayout());
			getContentPane().add(panel, BorderLayout.NORTH);
			{
				final JLabel label = new JLabel();
				label.setText("Atraso");
				panel.add(label);
			}
			{
				delay = new JTextField();
				delay.setColumns(10);
				panel.add(delay);
			}
		}
		{
			final JPanel panel = new JPanel();
			springLayout = new SpringLayout();
			panel.setLayout(springLayout);
			panel.setBorder(new EtchedBorder(EtchedBorder.LOWERED));
			getContentPane().add(panel, BorderLayout.CENTER);
			JScrollPane scrollPane;
			{
				scrollPane = new JScrollPane();
				panel.add(scrollPane);
				springLayout.putConstraint(SpringLayout.SOUTH, scrollPane, -11,
						SpringLayout.SOUTH, panel);
				springLayout.putConstraint(SpringLayout.EAST, scrollPane, 195,
						SpringLayout.WEST, panel);
				springLayout.putConstraint(SpringLayout.NORTH, scrollPane, 50,
						SpringLayout.NORTH, panel);
				springLayout.putConstraint(SpringLayout.WEST, scrollPane, 10,
						SpringLayout.WEST, panel);
				{
					list = new JList(getModel());
					list.addMouseListener(new MouseAdapter() {
						public void mouseClicked(MouseEvent event) {
							new Thread(new Delayer(event, Blah.this) {
								public void doEvent(EventObject ev) {
									int index = list.getSelectedIndex();
									if (index >= 0 && index < getModel().size()) {
										textPane.setText(((Element) getModel()
												.get(index)).text);
									}
								}
							}).start();
						}
					});
					scrollPane.setViewportView(list);
				}
			}
			{
				number = new JTextField();
				panel.add(number);
				springLayout.putConstraint(SpringLayout.SOUTH, number, -5,
						SpringLayout.NORTH, scrollPane);
				springLayout.putConstraint(SpringLayout.EAST, number, 110,
						SpringLayout.WEST, scrollPane);
				springLayout.putConstraint(SpringLayout.NORTH, number, -30,
						SpringLayout.NORTH, scrollPane);
				springLayout.putConstraint(SpringLayout.WEST, number, 0,
						SpringLayout.WEST, scrollPane);
			}
			JButton button;
			{
				button = new JButton();

				button.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						new Thread(new Delayer(e, Blah.this) {
							public void doEvent(EventObject ev) {
								long num;
								try {
									num = Integer.valueOf(number.getText())
											.intValue();
								} catch (NumberFormatException e) {
									num = 0;
								}
								getModel().clear();

								for (int i = 0; i < num; i++) {
									getModel().addElement(
											new Element(getString((int) (Math
													.random() * 10 + 10)),
													getString(100)));
								}

							}
						}).start();
					}
				});

				button.setText("OK");
				panel.add(button);
				springLayout.putConstraint(SpringLayout.SOUTH, button, 0,
						SpringLayout.SOUTH, number);
				springLayout.putConstraint(SpringLayout.EAST, button, 0,
						SpringLayout.EAST, scrollPane);
				springLayout.putConstraint(SpringLayout.NORTH, button, 0,
						SpringLayout.NORTH, number);
				springLayout.putConstraint(SpringLayout.WEST, button, 5,
						SpringLayout.EAST, number);
			}
			{
				final JPanel panel_1 = new JPanel();
				panel_1.setLayout(new BorderLayout());
				panel_1.setBorder(new EtchedBorder(EtchedBorder.LOWERED));
				panel.add(panel_1);
				springLayout.putConstraint(SpringLayout.SOUTH, panel_1, 255,
						SpringLayout.NORTH, scrollPane);
				springLayout.putConstraint(SpringLayout.EAST, panel_1, 330,
						SpringLayout.WEST, panel);
				springLayout.putConstraint(SpringLayout.NORTH, panel_1, 0,
						SpringLayout.NORTH, scrollPane);
				springLayout.putConstraint(SpringLayout.WEST, panel_1, 5,
						SpringLayout.EAST, button);
				{
					textPane = new JTextPane();
					textPane.setFocusable(false);
					textPane.setEditable(false);
					panel_1.add(textPane);
				}
			}
		}
		//

	}

	public int getDelay() {
		try {
			return Integer.valueOf(delay.getText()).intValue();
		} catch (NumberFormatException e) {
			return 0;
		}
	}

	private static String getString(int len) {
		StringBuffer buffer = new StringBuffer();
		while (buffer.length() < len) {
			buffer.append((char) (Math.random() * ('z' - 'a') + 'a'));
		}
		return buffer.toString();

	}

}