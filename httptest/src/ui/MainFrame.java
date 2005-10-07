package ui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
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

	private ModuleLoader moduleLoader;

	private JMenu menuModule;

	private static final String confFile = ".httptestrc";

	public MainFrame() {

		setTitle("HttpTest");

		loadProperties();

		/* TODO: separate module loading logic from it */
		moduleLoader = new ModuleLoader();

		String modulesPathString = configuration.getProperty("modulesPath");
		if (modulesPathString != null) {
			String[] modulesPath = modulesPathString.split("\\|");

			for (int i = 1; i < modulesPath.length; i++) {
				try {
					moduleLoader.loadPath(new File(modulesPath[i]));
				} catch (InvalidModuleException e) {
					e.printStackTrace();
					getErrorHandler().debug(e);
				}
			}
		}

		try {
			moduleLoader.loadModule("http",
					"http.HttpConnectionConnectionFactory",
					"http.HttpStateStateFactory", "http.HttpRequestFactory");
			moduleLoader.loadModule("hessian",
					"hessian.HessianConnectionFactory",
					"hessian.HessianStateFactory",
					"hessian.HessianRequestFactory");

		} catch (InvalidModuleException e) {
			// e.printStackTrace();
			getErrorHandler().debug(e);
		}

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

		menuModule = new JMenu("Módulo");
		menuModule.setMnemonic(KeyEvent.VK_M);

		updateModulesMenu();

		menuOption.add(menuModule);

		JMenuItem menuCarregar = new JMenuItem("Carregar módulos");
		menuCarregar.setMnemonic(KeyEvent.VK_C);
		menuCarregar.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ev) {
				String lastDir = configuration.getProperty("lastdir");

				JFileChooser fc = lastDir != null ? new JFileChooser(lastDir)
						: new JFileChooser();

				fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

				if (fc.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
					configuration.setProperty("lastdir", fc.getSelectedFile()
							.getAbsolutePath());

					try {
						moduleLoader.loadPath(fc.getSelectedFile());

						updateModulesMenu();

						/*
						 * TODO: abstract modules paths, and remove redundancy
						 * on paths, the way it's currently implemented a path
						 * may appear more than once in the configuration file
						 */
						String modulesPath = configuration
								.getProperty("modulesPath");
						modulesPath = modulesPath == null ? "" : modulesPath;

						StringBuffer buffer = new StringBuffer(modulesPath);
						buffer.append("|");
						buffer.append(fc.getSelectedFile().getAbsolutePath());
						configuration.setProperty("modulesPath", buffer
								.toString());

					} catch (InvalidModuleException e) {
						getErrorHandler().debug(e);
					}

				}
			}
		});

		menuOption.add(menuCarregar);

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

	private void updateModulesMenu() {
		menuModule.removeAll();

		String[] modules = moduleLoader.getAvailableModulesNames();

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