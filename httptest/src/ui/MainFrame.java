package ui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;

import javax.swing.ButtonGroup;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.KeyStroke;

import base.BufferRequestLogger;
import base.ErrorHandler;
import base.OutputStreamRequestLogger;

public class MainFrame extends JFrame {
	private Properties configuration;

	private MainPanel mainPanel;

	private ErrorHandler errorHandler;

	private static final String confFile = ".httptestrc";

	public MainFrame() {

		setTitle("HttpTest");

		loadProperties();

		final ModuleLoader moduleLoader = new ModuleLoader(
				"C:\\Documents and Settings\\gustavo\\Desktop\\modules\\");

		String[] modules = moduleLoader.getAvailableModulesNames();

		/* set content */
		ModuleLoader.Module module = moduleLoader.getModule(modules[0]);
		mainPanel = new MainPanel(configuration, getErrorHandler(), module);
		setContentPane(mainPanel);

		/* add menus */
		JMenuBar menuBar = new JMenuBar();

		JMenu menuFile = new JMenu("Arquivo");
		menuFile.setMnemonic(KeyEvent.VK_A);

		JMenuItem menuViewLog = new JMenuItem("Ver log");
		menuViewLog.setMnemonic(KeyEvent.VK_V);
		menuViewLog.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ev) {
				String lastDir = configuration.getProperty("lastdir");

				JFileChooser fc = lastDir != null ? new JFileChooser(lastDir)
						: new JFileChooser();

				if (fc.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
					try {
						BufferRequestLogger newLogger = new BufferRequestLogger();

						newLogger.loadFromFile(fc.getSelectedFile(), mainPanel
								.getRequestFactory());
						mainPanel.loadLog(newLogger);
					} catch (FileNotFoundException e) {
						getErrorHandler().debug(e);
					} catch (IllegalArgumentException e) {
						getErrorHandler().debug(e);
					} catch (IOException e) {
						getErrorHandler().debug(e);
					}
				}

				configuration.setProperty("lastdir", fc.getSelectedFile()
						.getParent());

			}
		});

		menuFile.add(menuViewLog);

		JMenuItem menuSaveLog = new JMenuItem("Salvar log");
		menuSaveLog.setMnemonic(KeyEvent.VK_S);
		menuSaveLog.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S,
				KeyEvent.CTRL_DOWN_MASK));
		menuSaveLog.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ev) {
				String lastDir = configuration.getProperty("lastdir");

				JFileChooser fc = lastDir != null ? new JFileChooser(lastDir)
						: new JFileChooser();

				if (fc.showSaveDialog(null) == JFileChooser.APPROVE_OPTION) {
					try {
						mainPanel.getBufferRequestLogger().logTo(
								new OutputStreamRequestLogger(
										new FileOutputStream(fc
												.getSelectedFile())));
					} catch (FileNotFoundException e) {
						getErrorHandler().debug(e);
					}
				}
				configuration.setProperty("lastdir", fc.getSelectedFile()
						.getParent());

			}
		});

		menuFile.add(menuSaveLog);
		menuFile.addSeparator();

		JMenuItem menuExit = new JMenuItem("Sair");
		menuExit.setMnemonic(KeyEvent.VK_R);
		menuExit.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X,
				KeyEvent.CTRL_DOWN_MASK));
		menuExit.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				storeProperties();
				System.exit(0);
			}

		});

		menuFile.add(menuExit);

		menuBar.add(menuFile);

		JMenu menuOption = new JMenu("Opções");
		menuOption.setMnemonic(KeyEvent.VK_O);

		JMenu menuModule = new JMenu("Módulo");
		menuModule.setMnemonic(KeyEvent.VK_M);

		ButtonGroup group = new ButtonGroup();
		for (int i = 0; i < modules.length; i++) {
			JRadioButtonMenuItem item = new JRadioButtonMenuItem(modules[i],
					i == 0);
			item.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent ev) {
					mainPanel.setModule(moduleLoader.getModule(((JMenuItem) ev
							.getSource()).getText()));
				}
			});
			menuModule.add(item);
			group.add(item);
		}

		menuOption.add(menuModule);

		menuBar.add(menuOption);

		JMenu menuHelp = new JMenu("Ajuda");
		menuHelp.setMnemonic(KeyEvent.VK_J);

		JMenuItem menuAbout = new JMenuItem("Sobre");
		menuAbout.setMnemonic(KeyEvent.VK_S);

		menuHelp.add(menuAbout);

		menuBar.add(menuHelp);

		setJMenuBar(menuBar);

		/* save configuration before closing */
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent ev) {
				storeProperties();
			}
		});

		pack();
	}

	private ErrorHandler getErrorHandler() {
		if (errorHandler == null) {
			errorHandler = new JOptionPaneErrorHandler();
		}

		return errorHandler;
	}

	private void storeProperties() {
		try {
			configuration.store(new FileOutputStream(System
					.getProperty("user.home")
					+ "/" + confFile), null);
		} catch (IOException e) {
		}
	}

	private void loadProperties() {
		configuration = new Properties();
		try {
			configuration.load(new FileInputStream(System
					.getProperty("user.home")
					+ "/" + confFile));
		} catch (IOException e) {
		}
	}
}