package ui;


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

    public void log(Request req, long start, long end) {
        model.addRow(new Object[] { req, new Long(req.getSize()),
                new Long(start == -1 ? -1 : (end - start)) });
    }
}
