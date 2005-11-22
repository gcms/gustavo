package br.ufg.inf.mud.base;
import br.ufg.inf.mud.command.ExitCommand;
import br.ufg.inf.mud.command.LookCommand;
import br.ufg.inf.mud.command.NameCommand;
import br.ufg.inf.mud.command.SayCommand;

public class Main {
    public static Room getRoom() {
        Exit e = new NoExit();
        Room r = new Room();

        r.setNorth(e);
        r.setSouth(e);
        r.setWest(e);
        r.setEast(e);

        return r;
    }

    public static void main(String[] args) {
        Room r1 = getRoom();
        r1.setDescription("Esta e a sala 1, as saidas sao mostradas\r\n"
                + "abaixo, e podem ser usadas para se mover.");

        Room r2 = getRoom();
        r2.setDescription("Esta e a sala 2");

        Room r3 = getRoom();
        r3.setDescription("Esta e a sala 3");

        r1.setEast(new RegularExit(r2));
        r2.setWest(new RegularExit(r1));

        r1.setNorth(new RegularExit(r3));
        r3.setSouth(new RegularExit(r1));

        MudServer server = new MudServer(9090);
        server.setStartRoom(r1);

        ExitCommand cmd = new ExitCommand();

        server.setCommand("n", cmd);
        server.setCommand("s", cmd);
        server.setCommand("e", cmd);
        server.setCommand("w", cmd);

        server.setCommand("l", new LookCommand());
        
        server.setCommand("say", new SayCommand());
        
        server.setCommand("name", new NameCommand());

        server.start();
    }

}
