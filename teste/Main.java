
import java.awt.Rectangle;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

import de.progra.charting.ChartEncoder;
import de.progra.charting.DefaultChart;
import de.progra.charting.EncodingException;
import de.progra.charting.model.ObjectChartDataModel;
import de.progra.charting.render.LineChartRenderer;

public class Main {

    public static void main(String[] args) throws IOException,
            EncodingException {
        if (args.length < 4) {
            System.err
                    .println("usage: java Main <file> <title> <interval> <output>");
            System.exit(1);
        }

        BufferedReader reader = new BufferedReader(new InputStreamReader(
                new FileInputStream(args[0])));

        List colList = new ArrayList();
        List valList = new ArrayList();

        String line;

        while ((line = reader.readLine()) != null) {
            String[] data = line.split("\\s+");
            colList.add(data[0]);
            valList.add(Double.valueOf(data[1]));
        }

        int interval = Integer.valueOf(args[2]).intValue();

        String[] cols = new String[colList.size()];
        double[][] model = new double[1][valList.size()];

        for (int i = 0; i < valList.size(); i++) {
            if (interval == 0 || (i + 1) % interval == 0) {
                cols[i] = (String) colList.get(i);
            } else {
                cols[i] = "";
            }
            model[0][i] = ((Double) valList.get(i)).doubleValue();
        }

        String[] rows = { " " };

        String title = args[1];

        createGraphic(cols, rows, model, title, args[3]);
    }

    public static void createGraphic(Object[] cols, String[] rows,
            double[][] model, String title, String filename)
            throws FileNotFoundException, EncodingException {

        ObjectChartDataModel data = new ObjectChartDataModel(model, cols, rows);
        DefaultChart c = new DefaultChart(data, title,
                DefaultChart.LINEAR_X_LINEAR_Y);
        c.addChartRenderer(new LineChartRenderer(c.getCoordSystem(), data), 1);
        c.setBounds(new Rectangle(0, 0, 600, 400));
        ChartEncoder.createEncodedImage(new FileOutputStream(filename), c,
                "png");
    }
}