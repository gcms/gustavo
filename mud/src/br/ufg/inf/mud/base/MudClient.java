package br.ufg.inf.mud.base;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

import br.ufg.inf.mud.command.LookCommand;


public class MudClient implements Runnable {
    private Socket socket;

    private MudServer server;

    private Player player;

    public MudClient(Socket socket, MudServer server) throws IOException {
        this.socket = socket;
        this.server = server;
        player = new Player(socket.getOutputStream(), server.getStartRoom());
        server.dispatchCommand(player, new LookCommand(), null);
    }

    public void run() {
        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(
                    socket.getInputStream()));

            String line;

            while ((line = reader.readLine()) != null) {
                server.dispatchCommand(this, line);
            }

        } catch (IOException e) {
            e.printStackTrace();
            return;
        }
    }

    public Player getPlayer() {
        return player;
    }
}
