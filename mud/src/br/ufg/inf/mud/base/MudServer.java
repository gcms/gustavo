package br.ufg.inf.mud.base;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import br.ufg.inf.mud.command.Command;
import br.ufg.inf.mud.command.NoSuchCommand;



public class MudServer {
    private int port;

    private boolean running;

    private ServerSocket socket;

    private Map<String, Command> commands;

    private Queue<Event> events;

    private Room startRoom;

    public MudServer(int port) {
        commands = Collections.synchronizedMap(new HashMap<String, Command>());
        events = new ConcurrentLinkedQueue<Event>();
        this.port = port;
    }

    public void start() {
        new Thread() {
            public void run() {
                runServer();
            }
        }.start();

        new Thread() {
            public void run() {
                runMainLoop();
            }
        }.start();

        while (running)
            ;
    }

    public void runServer() {
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

    public void runMainLoop() {
        while (running) {
            Event ev = events.poll();
            if (ev == null) {
                continue;
            }

            System.out.println(ev.getCommand());
            ev.getCommand().execute(this, ev.getPlayer(), null,
                    ev.getCommandLine());

        }
    }

    public void stop() {
        running = false;
    }

    public Room getStartRoom() {
        return startRoom;
    }

    public void setStartRoom(Room room) {
        startRoom = room;
    }

    public void handleConnection(Socket s) {
        System.out.println(new Exception().getStackTrace()[0]);
        try {
            MudClient c = new MudClient(s, this);
            new Thread(c).start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static class Event {
        private Player player;

        private Command command;

        private String commandLine;

        public Event(Player player, Command command, String commandLine) {
            this.player = player;
            this.command = command;
            this.commandLine = commandLine;
        }

        public Player getPlayer() {
            return player;
        }

        public Command getCommand() {
            return command;
        }

        public String getCommandLine() {
            return commandLine;
        }
    }

    public void setCommand(String commandLine, Command command) {
        commands.put(commandLine, command);
    }

    public Command getCommand(String command) {
        int index = command.indexOf(' ');

        command = index < 0 ? command : command.substring(0, command
                .indexOf(' '));

        Command cmd = commands.get(command);

        if (cmd == null) {
            return new NoSuchCommand();
        } else {
            return commands.get(command);
        }
    }

    public void dispatchCommand(MudClient client, String command) {
        dispatchCommand(client.getPlayer(), getCommand(command), command);
    }

    public void dispatchCommand(Player player, String command) {
        dispatchCommand(player, getCommand(command), command);
    }

    public void dispatchCommand(Player player, Command command,
            String commandLine) {
        events.offer(new Event(player, command, commandLine));
    }
}
