package ui;

import java.awt.FlowLayout;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.JTextField;

import base.ErrorHandler;
import base.Request;
import base.RequestFactory;

public class JPanelRequestFactory extends JPanel implements RequestFactory,
        ObjectFactory {
    private JComboBox optionsJComboBox;

    private JTextField sizeJTextField;

    private RequestFactory reqFactory;

    private ErrorHandler handler;

    public JPanelRequestFactory(RequestFactory reqFactory, ErrorHandler handler) {
        setErrorHandler(handler);
        setRequestFactory(reqFactory);
        setLayout(new FlowLayout(FlowLayout.LEFT));

        add(getOptionsJComboBox());

        add(getSizeJTextField());
    }

    public void setErrorHandler(ErrorHandler handler) {
        this.handler = handler;
    }

    public ErrorHandler getErrorHandler() {
        return handler;
    }

    private void setRequestFactory(RequestFactory reqFactory) {
        this.reqFactory = reqFactory;
    }

    public Request getRequest() {
        if (sizeJTextField.getText().length() == 0) {
            return null;
        }

        try {
            long l = Long.valueOf(sizeJTextField.getText()).longValue();

            return getRequest((String) getOptionsJComboBox().getSelectedItem(),
                    l);
        } catch (NumberFormatException e) {
            handler.debug(e);
            return null;
        }
    }

    public Request getRequest(String param) {
        if (sizeJTextField.getText().length() == 0) {
            return null;
        }

        long l = Long.valueOf(sizeJTextField.getText()).longValue();
        return getRequest(param, l);
    }

    public Object createObject() {
        return getRequest();
    }

    public Request getRequest(String param, long size) {
        return reqFactory.getRequest(param, size);
    }

    private void setOptions(String[] options) {
        setOptions(options);
    }

    public String[] getOptions() {
        return reqFactory.getOptions();
    }

    private JComboBox getOptionsJComboBox() {
        if (optionsJComboBox == null) {
            optionsJComboBox = new JComboBox(getOptions());
        }

        return optionsJComboBox;
    }

    private JTextField getSizeJTextField() {
        if (sizeJTextField == null) {
            sizeJTextField = new JTextField(10);
            sizeJTextField.addKeyListener(new KeyAdapter() {
                public void keyTyped(KeyEvent e) {
                    if (!Character.isDigit(e.getKeyChar())
                            && e.getKeyChar() != KeyEvent.VK_BACK_SPACE) {
                        e.consume();
                    }
                }
            });
        }

        return sizeJTextField;
    }
}