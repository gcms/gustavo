package ui;

import java.lang.reflect.InvocationTargetException;

import javax.swing.SwingUtilities;
import javax.swing.table.DefaultTableModel;

import base.Request;
import base.RequestLogger;

public class DefaultTableModelRequestLogger implements RequestLogger {
	private DefaultTableModel model;

	public DefaultTableModelRequestLogger(DefaultTableModel model) {
		this.model = model;
	}

	public boolean isEditable(int row, int col) {
		return false;
	}

	public void log(final Request req, final long start, final long end) {
		Runnable runnable = new Runnable() {
			public void run() {
				model.addRow(new Object[] { req, new Long(req.getSize()),
						new Long(start == -1 ? -1 : (end - start)) });
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
