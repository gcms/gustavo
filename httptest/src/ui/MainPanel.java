package ui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.net.URI;
import java.util.Properties;

import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.table.DefaultTableModel;

import base.BufferRequestLogger;
import base.ErrorHandler;
import base.NotifyEvent;
import base.NotifyListener;
import base.NullErrorHandler;
import base.Request;
import base.RequestFactory;
import base.RequestRunner;
import base.SpeedRequestLogger;

public class MainPanel extends JPanel {
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

    private JLabelSpeedObserver speedLabel;

    private RequestRunner currentRunner;

    private Properties configuration;

    private ModuleLoader.Module module;

    public MainPanel(Properties configuration, ErrorHandler errorHandler,
            ModuleLoader.Module module) {
        this.module = module;
        this.configuration = configuration;
        this.errorHandler = errorHandler;

        listModel = new RequestRunnerListModel();

        logger = new BufferRequestLogger();

        /* -- start packing left pane -- */
        JPanel leftPane = new JPanel(new BorderLayout());

        requestFactory = new JPanelRequestFactory(module.getRequestFactory(),
                errorHandler);
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
        bottomPane.add(getSpeedLabel(), BorderLayout.PAGE_END);

        rightPane.add(bottomPane, BorderLayout.CENTER);

        /* -- end packing right pane -- */

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
                leftPane, rightPane);

        setLayout(new BorderLayout(20, 20));
        add(Box.createRigidArea(new Dimension(1, 1)), BorderLayout.PAGE_START);
        add(Box.createRigidArea(new Dimension(1, 1)), BorderLayout.PAGE_END);
        add(Box.createRigidArea(new Dimension(1, 1)), BorderLayout.LINE_START);
        add(Box.createRigidArea(new Dimension(1, 1)), BorderLayout.LINE_END);

        add(splitPane);

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

    }

    public void setModule(ModuleLoader.Module module) {
        if (!this.module.equals(module)) {

            RequestFactory reqFactory = module.getRequestFactory();

            if (reqFactory == null) {
                return;
            }

            /* clearing older requests */
            listModel = new RequestRunnerListModel();
            requestJPanel.setListModel(listModel);
            requestJPanel.setSimpleList(listModel);

            requestFactory.setRequestFactory(reqFactory);

            this.module = module;
        }
    }

    public ModuleLoader.Module getModule() {
        return module;
    }

    private JLabelSpeedObserver getSpeedLabel() {
        if (speedLabel == null) {
            speedLabel = new JLabelSpeedObserver();
        }

        return speedLabel;
    }

    private DefaultTableModel createDefaultTableModel() {
        DefaultTableModel model = new DefaultTableModel();
        model.addColumn("Método");
        model.addColumn("Tamanho (bytes)");
        model.addColumn("Atraso (ms)");

        return model;
    }

    private ErrorHandler getErrorHandler() {

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

        SpeedRequestLogger sl = new SpeedRequestLogger();
        sl.addObserver(getSpeedLabel());
        logger.addLogger(sl);
    }

    private void execute() {
        currentRunner = (RequestRunnerListModel) requestJPanel.getListModel();
        RequestRunnerListModel newListModel = (RequestRunnerListModel) currentRunner
                .clone();
        requestJPanel.setListModel(newListModel);
        requestJPanel.setSimpleList(newListModel);

        try {
            currentRunner
                    .setConnectionFactory(module.getConnectionFactory(new URI(
                            getUrlJTextField().getText())));
            // currentRunner.setUri(new URI(getUrlJTextField().getText()));
        } catch (java.net.URISyntaxException e) {
            errorHandler.debug(e.getMessage());
            return;
        }

        resetLogger();

        currentRunner.setStateFactory(module.getStateFactory());

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
        currentRunner.setErrorHandler(new NullErrorHandler());
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

    public RequestFactory getRequestFactory() {
        return requestFactory;
    }

    public void loadLog(BufferRequestLogger newLogger) {
        resetLogger();
        newLogger.logTo(logger);

    }

    public BufferRequestLogger getBufferRequestLogger() {
        return logger;
    }

}