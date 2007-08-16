package base;

import java.util.Date;
import java.util.Calendar;
import java.util.GregorianCalendar;

import java.util.Iterator;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

import java.io.IOException;
import java.io.FileInputStream;
import java.io.Reader;
import java.io.InputStreamReader;
import java.io.BufferedReader;

public class OutputStreamLog {
    private Map requests;

    public OutputStreamLog() {
        requests = new HashMap();
    }

    public void loadFile(String filename) throws IOException {
        loadData(new InputStreamReader(new FileInputStream(filename)));
    }

    public void loadData(Reader reader) throws IOException {
        BufferedReader buffReader = new BufferedReader(reader);

        String line;
        while ((line = buffReader.readLine()) != null) {
            String data[] = line.split("[\\s)(]+");
            String type = data[0];
            long size = Long.valueOf(data[1]).longValue();

            long start = Long.valueOf(data[2]).longValue();
            long end = Long.valueOf(data[3]).longValue();
            long delay = end - start;

            Map sizes = (Map) requests.get(data[0]);
            if (sizes == null) {
                sizes = new HashMap();
                requests.put(data[0], sizes);
            }

            Date date = new Date(start + delay / 2);

            Calendar cal = new GregorianCalendar();
            cal.setTime(date);

            int position = cal.get(Calendar.HOUR_OF_DAY) * 6
                    + cal.get(Calendar.MINUTE) / 10;

            List[] time = (List[]) sizes.get(new Long(size));
            if (time == null) {
                time = new List[6 * 24];
                sizes.put(new Long(size), time);
            }

            if (time[position] == null) {
                time[position] = new ArrayList();
            }

            time[position].add(new Long(delay));
        }
    }

    public long getAverageDelay(String type, int pos, long size) {
        if (pos < 0 || pos >= 6 * 24) {
            return 0;
        }

        Map sizes = (Map) requests.get(type);

        if (sizes == null) {
            return 0;
        }

        List[] delays = (List[]) sizes.get(new Long(size));

        if (delays == null) {
            return 0;
        }

        if (delays[pos] == null) {
            return getAverageDelay(type, pos - 1, size);
        }

        long sum = 0;
        for (Iterator it = delays[pos].iterator(); it.hasNext(); /* */) {
            Long l = (Long) it.next();
            sum += l.longValue();
        }

        return sum / delays[pos].size();
    }

    public long[] getAverageDelaysTimeBySize(String type, long size, int start,
            int number) {
        long[] delays = new long[number];

        for (int i = 0; i < number; i++) {
            delays[i] = getAverageDelay(type, i + start, size);
        }

        return delays;
    }

    public long[] getAverageDelaysTimeBySize(String type, long size) {
        return getAverageDelaysTimeBySize(type, size, 0, 6 * 24);
    }

    public double getAverageDelayBySize(String type, long size) {
        Map sizes = (Map) requests.get(type);
        if (sizes == null) {
            return -1;
        }

        List[] time = (List[]) sizes.get(new Long(size));

        if (time == null) {
            return -1;
        }

        long result = 0;
        long nelem = 0;
        for (int i = 0; i < time.length; i++) {
            if (time[i] == null) {
                continue;
            }
            for (Iterator it = time[i].iterator(); it.hasNext(); /* */) {
                result += ((Long) it.next()).longValue();
                nelem++;
            }
        }

        return (double) result / (double) nelem;
    }

    public static void main(String[] args) throws IOException {
        OutputStreamLog log = new OutputStreamLog();
        log.loadFile("home.log");
        long[] delays = log.getAverageDelaysTimeBySize("GET", 1000);

        for (int i = 0; i < delays.length; i++) {
            System.out.println(delays[i]);
        }
    }
}