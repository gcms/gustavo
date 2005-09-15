package ui;

import java.util.Properties;


import javax.swing.JApplet;

import base.ErrorHandler;

public class MainApplet extends JApplet {
    public MainApplet() {
        add(new MainPanel(new Properties(), new ErrorHandler() {

            public void debug(Object o) {
            }
        }));
    }
}
