package ui;

import javax.swing.JOptionPane;

import base.ErrorHandler;

public class JOptionPaneErrorHandler implements ErrorHandler {
    public void debug(Object o) {
        if (o != null && o instanceof Exception) {
            JOptionPane.showMessageDialog(null, ((Exception) o).getMessage());
        } else {
            JOptionPane.showMessageDialog(null, o);
        }
    }
}