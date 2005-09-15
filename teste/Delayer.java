import java.util.EventObject;

import javax.swing.JComponent;

public abstract class Delayer implements Runnable {
	private EventObject event;

	private DelayConf conf;
	
	public Delayer(EventObject event, DelayConf conf) {
		this.event = event;
		this.conf = conf;
	}

	public void run() {
		((JComponent) event.getSource()).setEnabled(false);

		try {
			Thread.sleep(conf.getDelay());
		} catch (InterruptedException e) {
		}
		doEvent(event);
		((JComponent) event.getSource()).setEnabled(true);
	}

	public abstract void doEvent(EventObject ev);
}
