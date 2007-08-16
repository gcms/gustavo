package ui;

import javax.swing.JLabel;
import javax.swing.SwingUtilities;

import base.SpeedRequestLogger;

import java.lang.reflect.InvocationTargetException;
import java.util.Observer;
import java.util.Observable;

public class JLabelSpeedObserver extends JLabel implements Observer {

    public void update(Observable o, Object arg) {

        /* TODO: separate speed metering from request logging */
        final SpeedRequestLogger s = (SpeedRequestLogger) o;
        
        Runnable runnable = new Runnable() {
            public void run() {
                setHorizontalAlignment(RIGHT);

                setText("Taxa média: " + s.toString());
            }
        };
		
		try {
			SwingUtilities.invokeAndWait(runnable);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
}