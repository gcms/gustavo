package ui;

import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextField;
import javax.swing.ButtonGroup;

import base.Request;
import base.RequestFactory;

import java.awt.FlowLayout;
import java.awt.event.KeyEvent;
import java.awt.event.KeyAdapter;

import http.RequestBase;
import http.GetRequest;
import http.PostRequest;

public class JPanelRequestFactory extends JPanel implements RequestFactory,
        ObjectFactory {
    public static final String GET = "GET";

    public static final String POST = "POST";

    private JRadioButton getJRadioButton;

    private JRadioButton postJRadioButton;

    private JTextField sizeJTextField;

    public JPanelRequestFactory() {
        setLayout(new FlowLayout(FlowLayout.LEFT));

        getJRadioButton = new JRadioButton("GET");
        getJRadioButton.setMnemonic(KeyEvent.VK_G);
        getJRadioButton.setSelected(true);
        add(getJRadioButton);

        postJRadioButton = new JRadioButton("POST");
        postJRadioButton.setMnemonic(KeyEvent.VK_P);
        add(postJRadioButton);

        ButtonGroup group = new ButtonGroup();
        group.add(getJRadioButton);
        group.add(postJRadioButton);

        sizeJTextField = new JTextField(10);
        sizeJTextField.addKeyListener(new KeyAdapter() {
            public void keyTyped(KeyEvent e) {
                if (!Character.isDigit(e.getKeyChar())
                        && e.getKeyChar() != KeyEvent.VK_BACK_SPACE) {
                    e.consume();
                }
            }
        });
        add(sizeJTextField);
    }

    public Request getRequest() {
        if (sizeJTextField.getText().length() == 0) {
            return null;
        }

        long l = Long.valueOf(sizeJTextField.getText()).longValue();
        return getJRadioButton.isSelected() ? (RequestBase) new GetRequest(l)
                : (RequestBase) new PostRequest(l);
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
        return param.equals(GET) ? (RequestBase) new GetRequest(size) : param
                .equals(POST) ? (RequestBase) new PostRequest(size) : null;

    }
}