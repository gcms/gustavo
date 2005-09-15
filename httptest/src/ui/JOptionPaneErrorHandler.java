package ui;

import javax.swing.JOptionPane;

import base.ErrorHandler;


public class JOptionPaneErrorHandler implements ErrorHandler {
    public void debug(Object o) {
        JOptionPane.showMessageDialog(null, o);
    }

}