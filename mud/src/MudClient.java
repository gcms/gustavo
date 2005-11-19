import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

public class MudClient implements Runnable {
    private Socket socket;

    private MudServer server;
    
    private Player player;

    public MudClient(Socket socket, MudServer server) {
        this.socket = socket;
        this.server = server;
    }

    public void run() {
        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(
                    socket.getInputStream()));

            String line;

            while ((line = reader.readLine()) != null) {
                parseCommand(line);
            }

        } catch (IOException e) {
            e.printStackTrace();
            return;
        }
    }

    public void parseCommand(String command) {
        Command cmd = server.getCommand(command);

        cmd.execute(player, command);
    }

}
