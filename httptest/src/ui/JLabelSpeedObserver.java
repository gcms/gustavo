package ui;

import javax.swing.JLabel;

import base.SpeedRequestLogger;

import java.util.Observer;
import java.util.Observable;


public class JLabelSpeedObserver extends JLabel implements Observer {

    public void update(Observable o, Object arg) {
        /* TODO: separate speed metering from request logging */
        SpeedRequestLogger s = (SpeedRequestLogger) o;
        setHorizontalAlignment(RIGHT);
        setText("Taxa média: " + o.toString());
    }
}
