import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.util.Calendar;
import java.util.Date;

import javax.microedition.lcdui.Choice;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Gauge;
import javax.microedition.lcdui.List;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;
import javax.microedition.rms.InvalidRecordIDException;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreFullException;
import javax.microedition.rms.RecordStoreNotFoundException;
import javax.microedition.rms.RecordStoreNotOpenException;

import com.sun.j2me.global.DateFormatSymbols;
import com.sun.j2me.global.DateTimeFormat;

public class TetrisMIDlet extends MIDlet implements CommandListener {
    private List mainMenu;

    private Command selectCommand;

    private TetrisCanvas canvas;

    private TetrisGame currentGame;

    private Form settingsForm;

    private Gauge levelGauge;

    private Gauge fpsGauge;

    private Form scoresForm;

    public TetrisMIDlet() {
    }

    private TetrisCanvas getCanvas() {
        if (canvas == null) {
            canvas = new TetrisCanvas();

            canvas.addCommand(new Command("Back", Command.BACK, 2));
            canvas.setCommandListener(this);
        }

        canvas.setFrameFrequency(TetrisSettings.getInstance()
                .getFrameFrequency());

        return canvas;
    }

    private List getMainMenu() {

        if (mainMenu == null) {
            mainMenu = new List("Tetris", Choice.IMPLICIT);
            mainMenu.append("New game", null);
            mainMenu.append("Settings", null);
            mainMenu.append("Scores", null);
            mainMenu.addCommand(new Command("Exit", Command.EXIT, 0));
            selectCommand = new Command("Select", Command.ITEM, 1);
            mainMenu.setSelectCommand(selectCommand);
            mainMenu.setCommandListener(this);
        }

        if (currentGame != null) {
            if (!mainMenu.getString(0).equals("Continue")) {
                mainMenu.insert(0, "Continue", null);
            }
        } else {
            if (mainMenu.getString(0).equals("Continue")) {
                mainMenu.delete(0);
            }
        }

        return mainMenu;

    }

    private Form getSettingsForm() {
        if (settingsForm == null) {
            settingsForm = new Form("Settings");
            settingsForm.append(levelGauge = new Gauge("Level", true, 8, 4));
            settingsForm.append(fpsGauge = new Gauge("Frames per second", true,
                    14, 10));

            settingsForm.addCommand(new Command("Back", Command.BACK, 1));
            settingsForm.setCommandListener(this);
        }

        return settingsForm;
    }

    private Form getScoresForm() {
        if (scoresForm == null) {
            scoresForm = new Form("Scores");
            scoresForm.addCommand(new Command("Back", Command.BACK, 1));
            scoresForm.setCommandListener(this);
        }

        scoresForm.deleteAll();

        try {
            RecordStore store = RecordStore.openRecordStore("tetris", true);

            byte[] data = store.getRecord(2);
            DataInputStream in = new DataInputStream(new ByteArrayInputStream(
                    data));

            try {
                int index = 0;
                while (in.available() > 0) {
                    Date date = new Date(in.readLong());
                    int score = in.readInt();

                    Calendar cal = Calendar.getInstance();
                    cal.setTime(date);

                    String dateStr = cal.get(Calendar.DAY_OF_MONTH) + "/"
                            + cal.get(Calendar.MONTH) + "/"
                            + cal.get(Calendar.YEAR);

                    scoresForm.append("" + ++index + ". " + dateStr + ": "
                            + score + "\n");
                }
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }

            store.closeRecordStore();
        } catch (RecordStoreFullException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (RecordStoreNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (RecordStoreException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        if (scoresForm.size() == 0) {
            scoresForm.append("No point scored");
        }

        return scoresForm;
    }

    protected void destroyApp(boolean arg0) throws MIDletStateChangeException {
        stop();
    }

    protected void pauseApp() {
        stop();
    }

    private void stop() {
        if (canvas != null)
            canvas.stop();

        if (currentGame != null && !currentGame.getBoard().isFinished()) {
            try {
                saveCurrent();
            } catch (RecordStoreNotOpenException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (InvalidRecordIDException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (RecordStoreException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }

    private void saveCurrent() throws RecordStoreNotOpenException,
            InvalidRecordIDException, IOException, RecordStoreException {
        RecordStore store = RecordStore.openRecordStore("tetris", true);

        byte[] state = currentGame.getState();

        if (store.getNumRecords() > 0) {
            store.setRecord(1, state, 0, state.length);
        } else {
            store.addRecord(state, 0, state.length);
        }

        store.closeRecordStore();

    }

    private void loadCurrent() throws RecordStoreFullException,
            RecordStoreNotFoundException, RecordStoreException, IOException {
        RecordStore store = RecordStore.openRecordStore("tetris", true);

        if (store.getNumRecords() < 1)
            return;

        currentGame = new TetrisGame(store.getRecord(1));

        store.closeRecordStore();
    }

    protected void startApp() throws MIDletStateChangeException {
        try {
            loadCurrent();
        } catch (RecordStoreFullException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (RecordStoreNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (RecordStoreException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        Display.getDisplay(this).setCurrent(getMainMenu());
    }

    public void commandAction(Command c, Displayable d) {
        if (c.getCommandType() == Command.EXIT) {
            stop();
            notifyDestroyed();
        } else if (d == mainMenu && c == selectCommand) {
            String option = mainMenu.getString(mainMenu.getSelectedIndex());
            if (option.equals("New game")) {
                currentGame = TetrisSettings.getInstance().createGame();
                getCanvas().setGame(currentGame);
                canvas.show();
                Display.getDisplay(this).setCurrent(canvas);
            } else if (option.equals("Continue")) {
                getCanvas().setGame(currentGame);
                canvas.show();
                Display.getDisplay(this).setCurrent(canvas);
            } else if (option.equals("Settings")) {
                Display.getDisplay(this).setCurrent(getSettingsForm());
            } else if (option.equals("Scores")) {
                Display.getDisplay(this).setCurrent(getScoresForm());
            }
        } else if (d == canvas && c.getCommandType() == Command.BACK) {
            canvas.hide();
            Display.getDisplay(this).setCurrent(getMainMenu());
        } else if (d == settingsForm) {
            TetrisSettings settings = TetrisSettings.getInstance();
            settings.setFrameFrequency(fpsGauge.getValue() + 1);
            settings.setLevel(levelGauge.getValue() + 1);
            Display.getDisplay(this).setCurrent(getMainMenu());
        } else if (d == scoresForm) {
            Display.getDisplay(this).setCurrent(getMainMenu());
        }

    }

}
