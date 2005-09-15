
import java.awt.Rectangle;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

import de.progra.charting.ChartEncoder;
import de.progra.charting.DefaultChart;
import de.progra.charting.EncodingException;
import de.progra.charting.model.ObjectChartDataModel;
import de.progra.charting.render.LineChartRenderer;

public class Graphic {
    private List models;

    private List rows;

    private List cols;

    private String title;

    private int interval;

    public int getInterval() {
        return interval;
    }

    public void setInterval(int interval) {
        this.interval = interval;
    }

    public Graphic(String title, int interval) {
        this.title = title;
        this.interval = interval;

        this.models = new ArrayList();
        this.rows = new ArrayList();
    }

    public void loadFile(File f, String name) throws NumberFormatException,
            IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(
                new FileInputStream(f)));

        List colList = new ArrayList();
        List valList = new ArrayList();

        String line;

        while ((line = reader.readLine()) != null) {
            String[] data = line.split("\\s+");
            colList.add(data[0]);
            valList.add(Double.valueOf(data[1]));
        }

        if (cols == null) {
            cols = colList;
        } else if (!colList.equals(cols)) {
            throw new IllegalArgumentException();
        }

        models.add(valList);
        rows.add(name);
    }

    public void loadFile(String filename, String name)
            throws NumberFormatException, IOException {
        loadFile(new File(filename), name);
    }

    public void saveGraphic(String filename) throws FileNotFoundException,
            EncodingException {
        double[][] model = new double[rows.size()][cols.size()];

        int i = 0;
        for (Iterator it = models.iterator(); it.hasNext(); i++) {
            List row = (List) it.next();

            int j = 0;
            for (Iterator jt = row.iterator(); jt.hasNext(); j++) {
                model[i][j] = ((Double) jt.next()).doubleValue();
            }
        }

        String[] intervalCols = new String[cols.size()];

        i = 0;
        for (Iterator it = cols.iterator(); it.hasNext(); i++) {
            if (interval <= 1 || (i + 1) % interval == 0) {
                intervalCols[i] = (String) it.next();
            } else {
                it.next();
                intervalCols[i] = "";
            }
        }

        ObjectChartDataModel data = new ObjectChartDataModel(model,
                intervalCols, (String[]) rows.toArray(new String[] {}));
        DefaultChart c = new DefaultChart(data, title,
                DefaultChart.LINEAR_X_LINEAR_Y);
        c.addChartRenderer(new LineChartRenderer(c.getCoordSystem(), data), 1);
        c.setBounds(new Rectangle(0, 0, 600, 400));
        ChartEncoder.createEncodedImage(new FileOutputStream(filename), c,
                "png");

    }
}