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

import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
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

        ModuleLoader moduleLoader = new ModuleLoader(
                "C:\\Documents and Settings\\warnoleto\\Desktop\\modules\\");
        

        ModuleLoader.Module module = moduleLoader.getModule("http");

        mainPanel = new MainPanel(configuration, getErrorHandler(), module);
        setContentPane(mainPanel);

        /* save configuration before closing */
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent ev) {
                storeProperties();
            }
        });

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

        JMenu menuHelp = new JMenu("Ajuda");
        menuHelp.setMnemonic(KeyEvent.VK_J);

        JMenuItem menuAbout = new JMenuItem("Sobre");
        menuAbout.setMnemonic(KeyEvent.VK_S);

        menuHelp.add(menuAbout);

        menuBar.add(menuHelp);

        setJMenuBar(menuBar);

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