import java.io.DataInputStream;
import java.io.IOException;
import java.util.Random;

import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.List;
import javax.microedition.lcdui.TextBox;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDlet;
import javax.microedition.rms.InvalidRecordIDException;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreFullException;
import javax.microedition.rms.RecordStoreNotFoundException;
import javax.microedition.rms.RecordStoreNotOpenException;

public class TipOfTheDay extends MIDlet implements CommandListener {

    private static final String[] optionsStringList = { "Ver dica do dia",
            "Adicionar dica" };

    private Display display;

    private List optionsList;

    private Form displayForm;

    private TextBox addBox;

    private Command exitCommand;

    private Command okCommand;

    private Command backCommand;

    private Command saveCommand;

    private Command nextCommand;

    private Displayable currentScreen;

    private Random random;

    private RecordStore store;

    public TipOfTheDay() {

        try {
            store = RecordStore.openRecordStore("totd", true);
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

        random = new Random();
        random.setSeed(System.currentTimeMillis());

        display = Display.getDisplay(this);

        exitCommand = new Command("Sair", Command.EXIT, 2);
        okCommand = new Command("OK", Command.OK, 1);
        backCommand = new Command("Voltar", Command.BACK, 2);
        saveCommand = new Command("Salvar", Command.OK, 1);
        nextCommand = new Command("Próximo", Command.OK, 1);

        optionsList = new List("Menu", List.IMPLICIT, optionsStringList, null);
        optionsList.addCommand(exitCommand);
        optionsList.addCommand(okCommand);

        optionsList.setCommandListener(this);

        displayForm = new Form("Dica do dia");
        displayForm.append("");
        displayForm.addCommand(backCommand);
        displayForm.addCommand(nextCommand);
        displayForm.setCommandListener(this);

        addBox = new TextBox("Nova dica", "", 256, TextField.ANY);
        addBox.addCommand(backCommand);
        addBox.addCommand(saveCommand);
        addBox.setCommandListener(this);

        currentScreen = optionsList;
    }

    private DataInputStream getInputStream() {
        return new DataInputStream(getClass().getResourceAsStream(
                "/bin_tips.txt"));
    }

    public void startApp() {
        display.setCurrent(currentScreen);
    }

    public void pauseApp() {
        currentScreen = display.getCurrent();
    }

    public void destroyApp(boolean unconditional) {
    }

    public void commandAction(Command c, Displayable d) {
        if (c == okCommand || c == List.SELECT_COMMAND) {
            switch (optionsList.getSelectedIndex()) {
            case 0:
                loadRecord();
                currentScreen = displayForm;
                break;
            case 1:
                currentScreen = addBox;
                break;
            }
        } else if (c == backCommand) {
            currentScreen = optionsList;
        } else if (c == nextCommand) {
            loadRecord();
        } else if (c == saveCommand) {
            if (addBox.getString().trim().equals("")) {
                showAlert("Dica invalida", "Dica vazia", AlertType.ERROR,
                        addBox);
            } else {
                saveRecord();
                currentScreen = optionsList;
            }
        } else {
            notifyDestroyed();
        }

        display.setCurrent(currentScreen);
    }

    private void loadRecord() {
        displayForm.delete(0);

        String dica = getRandomRecord();

        if (dica == null) {
            displayForm.append("Não há nenhuma dica armazenada");
        } else {
            displayForm.append(dica);
        }
    }

    private void saveRecord() {
        if (!saveRecord(addBox.getString())) {
            showAlert("Erro",
                    "Ocorreu um erro ao tentar armazenar a nova dica",
                    AlertType.ERROR, addBox);
        } else {
            addBox.setString("");
        }
    }

    private void showAlert(String title, String message, AlertType type,
            Displayable next) {
        Alert alert = new Alert(title, message, null, type);
        display.setCurrent(alert, next);
    }

    private boolean saveRecord(String str) {
        byte[] data = str.getBytes();

        try {
            store.addRecord(data, 0, data.length);
        } catch (RecordStoreNotOpenException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return false;
        } catch (RecordStoreFullException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return false;
        } catch (RecordStoreException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return false;
        }

        return true;
    }

    private String getRandomRecord() {
        DataInputStream stream = getInputStream();
        int streamSize;
        try {
            streamSize = stream.readInt();
        } catch (IOException e1) {
            e1.printStackTrace();
            return null;
        }

        int totalSize = streamSize;
        try {
            totalSize = streamSize + store.getNumRecords();
        } catch (RecordStoreNotOpenException e1) {
            // TODO Auto-generated catch block
            e1.printStackTrace();
        }

        int index;

        try {
            index = Math.abs(random.nextInt()) % totalSize;
        } catch (ArithmeticException e) {
            e.printStackTrace();
            return null;
        }

        if (index >= streamSize) {
            return getStoreEntry(index - streamSize + 1);
        } else {
            return getResourceEntry(stream, index);
        }
    }

    private String getStoreEntry(int index) {
        try {
            byte[] data = store.getRecord(index);
            return new String(data);
        } catch (RecordStoreNotOpenException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return null;
        } catch (InvalidRecordIDException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return null;
        } catch (RecordStoreException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return null;
        }
    }

    private String getResourceEntry(DataInputStream stream, int index) {
        if (index < 0) {
            return null;
        }
        try {
            while (index-- > 0) {
                int lineSize = stream.readInt();

                if (stream.skipBytes(lineSize) != lineSize) {
                    return null;
                }
            }

            int lineSize = stream.readInt();
            byte[] data = new byte[lineSize];
            stream.read(data, 0, lineSize);
            return new String(data);
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }
}