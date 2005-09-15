package ui;

import http.HttpConnectionConnectionFactory;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URI;
import java.util.Properties;

import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.table.DefaultTableModel;

import base.BufferRequestLogger;
import base.ErrorHandler;
import base.NotifyEvent;
import base.NotifyListener;
import base.OutputStreamRequestLogger;
import base.Request;
import base.RequestRunner;

public class HttpTestMainFrame extends JFrame {
    private JPanelRequestFactory requestFactory;

    private RequestRunnerListModel listModel;

    private RequestJPanel requestJPanel;

    private DefaultTableModel tableModel;

    private ErrorHandler errorHandler;

    private BufferRequestLogger logger;

    /* Right pane widgets, need to find a way to move this details elsewhere */
    private JTextField urlJTextField;

    private JButton executeJButton;

    private JButton cancelJButton;

    private JTable resultJTable;

    private RequestRunner currentRunner;

    private Properties configuration;

    private static final String confFile = ".httptestrc";

    public HttpTestMainFrame() {
        listModel = new RequestRunnerListModel();

        logger = new BufferRequestLogger();

        /* -- start packing left pane -- */
        JPanel leftPane = new JPanel(new BorderLayout());

        requestFactory = new JPanelRequestFactory();
        requestJPanel = new RequestJPanel(listModel, requestFactory, listModel);

        leftPane.add(requestFactory, BorderLayout.PAGE_START);
        leftPane.add(requestJPanel, BorderLayout.CENTER);
        /* -- end packing left pane -- */

        /* -- start packing right pane -- */
        JPanel rightPane = new JPanel(new BorderLayout());

        JPanel topPane = new JPanel(new FlowLayout());
        topPane.add(new JLabel("URL:"));
        topPane.add(getUrlJTextField());
        topPane.add(getExecuteJButton());
        topPane.add(getCancelJButton());

        rightPane.add(topPane, BorderLayout.PAGE_START);

        JPanel bottomPane = new JPanel(new BorderLayout());
        bottomPane.add(new JScrollPane(getResultJTable()), BorderLayout.CENTER);
        rightPane.add(bottomPane, BorderLayout.CENTER);

        /* -- end packing right pane -- */

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
                leftPane, rightPane);

        getContentPane().setLayout(new BorderLayout(20, 20));
        getContentPane().add(Box.createRigidArea(new Dimension(1, 1)),
                BorderLayout.PAGE_START);
        getContentPane().add(Box.createRigidArea(new Dimension(1, 1)),
                BorderLayout.PAGE_END);
        getContentPane().add(Box.createRigidArea(new Dimension(1, 1)),
                BorderLayout.LINE_START);
        getContentPane().add(Box.createRigidArea(new Dimension(1, 1)),
                BorderLayout.LINE_END);

        getContentPane().add(splitPane);

        setTitle("HttpTest");

        /* load configuration */
        loadProperties();
        getUrlJTextField().setText(configuration.getProperty("lasturl"));
        String lastRequest = configuration.getProperty("lastrequest");
        if (lastRequest != null) {
            String[] requests = lastRequest.split(":");
            for (int i = 0; i < requests.length; i++) {
                String[] data = requests[i].split("[)(]");

                long size;
                try {
                    size = Long.valueOf(data[1]).longValue();
                } catch (NumberFormatException e) {
                    continue;
                }

                Request req = requestFactory.getRequest(data[0], size);

                if (req == null) {
                    continue;
                }

                listModel.add(req);
            }
        }

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
                        newLogger.loadFromFile(fc.getSelectedFile(),
                                requestFactory);
                        resetLogger();
                        newLogger.logTo(logger);

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
                        logger.logTo(new OutputStreamRequestLogger(
                                new FileOutputStream(fc.getSelectedFile())));
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

    private DefaultTableModel createDefaultTableModel() {
        DefaultTableModel model = new DefaultTableModel();
        model.addColumn("Método");
        model.addColumn("Tamanho (bytes)");
        model.addColumn("Atraso (ms)");

        return model;
    }

    private ErrorHandler getErrorHandler() {
        if (errorHandler == null) {
            errorHandler = new JOptionPaneErrorHandler();
        }

        return errorHandler;
    }

    private DefaultTableModel getTableModel() {
        if (tableModel == null) {
            tableModel = createDefaultTableModel();
        }

        return tableModel;
    }

    private void setTableModel(DefaultTableModel model) {
        this.tableModel = model;
        getResultJTable().setModel(model);
    }

    private JTextField getUrlJTextField() {
        if (urlJTextField == null) {
            urlJTextField = new JTextField(30);
        }

        return urlJTextField;
    }

    private JButton getExecuteJButton() {
        if (executeJButton == null) {
            executeJButton = new JButton("Executar");
            executeJButton.setMnemonic(KeyEvent.VK_X);
            executeJButton.addActionListener(new ActionListener() {
                public void actionPerformed(java.awt.event.ActionEvent e) {
                    execute();
                }
            });
        }

        return executeJButton;
    }

    private void resetLogger() {
        /* clear the current TableModel by creating a new one */
        setTableModel(createDefaultTableModel());

        /* clear the logger */
        logger.clear();
        logger.clearLoggers();

        logger.addLogger(new DefaultTableModelRequestLogger(getTableModel()));
    }

    private void execute() {
        currentRunner = (RequestRunnerListModel) requestJPanel.getListModel();
        RequestRunnerListModel newListModel = (RequestRunnerListModel) currentRunner
                .clone();
        requestJPanel.setListModel(newListModel);
        requestJPanel.setSimpleList(newListModel);

        try {
            currentRunner
                    .setConnectionFactory(new HttpConnectionConnectionFactory(
                            new URI(getUrlJTextField().getText())));
            //            currentRunner.setUri(new URI(getUrlJTextField().getText()));
        } catch (java.net.URISyntaxException e) {
            errorHandler.debug(e.getMessage());
            return;
        }

        resetLogger();

        currentRunner.setRequestLogger(logger);

        currentRunner.setErrorHandler(getErrorHandler());

        currentRunner.addNotifyListener(new NotifyListener() {
            public void notificationPerformed(NotifyEvent ev) {
                if (ev.getSource() == currentRunner) {
                    getExecuteJButton().setEnabled(true);
                    getCancelJButton().setEnabled(false);
                }
            }
        });

        getExecuteJButton().setEnabled(false);
        getCancelJButton().setEnabled(true);
        new Thread(currentRunner).start();

        /* updating configuration */
        configuration.setProperty("lasturl", getUrlJTextField().getText());

        StringBuffer buffer = new StringBuffer();

        for (int i = 0; i < listModel.size(); i++) {
            buffer.append(listModel.get(i));
            if (i < listModel.size() - 1) {
                buffer.append(':');
            }
        }
        configuration.setProperty("lastrequest", buffer.toString());
    }

    private JButton getCancelJButton() {
        if (cancelJButton == null) {
            cancelJButton = new JButton("Cancel");
            cancelJButton.setMnemonic(KeyEvent.VK_C);
            cancelJButton.setEnabled(false);
            cancelJButton.addActionListener(new ActionListener() {
                public void actionPerformed(java.awt.event.ActionEvent ev) {
                    cancel();
                }
            });
        }

        return cancelJButton;
    }

    private void cancel() {
        currentRunner.stop();
        currentRunner = null;
        getExecuteJButton().setEnabled(true);
        getCancelJButton().setEnabled(false);
    }

    private JTable getResultJTable() {
        if (resultJTable == null) {
            resultJTable = new JTable(getTableModel());
        }

        return resultJTable;
    }

}