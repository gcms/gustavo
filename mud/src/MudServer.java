import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class MudServer implements Runnable {
    private int port;

    private boolean running;

    private ServerSocket socket;

    private Set<MudClient> clients;

    private Map<String, Command> commands;

    public MudServer(int port) {
        clients = new HashSet<MudClient>();
        commands = new HashMap<String, Command>();
        this.port = port;
    }

    public void run() {
        running = true;

        try {
            socket = new ServerSocket(port);
        } catch (IOException e) {
            e.printStackTrace();
        }

        while (running) {
            try {
                Socket s = socket.accept();
                handleConnection(s);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void stop() {
        running = false;
    }

    public void handleConnection(Socket s) {
        MudClient c = new MudClient(s, this);
        clients.add(c);

        new Thread(c).start();
    }

    public Command getCommand(String command) {
        command = command.substring(0, command.indexOf(' '));
        return commands.get(command);
    }
}
