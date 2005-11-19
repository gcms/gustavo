import java.io.IOException;

public class NoSuchCommand implements Command {

    public boolean execute(MudServer server, Player player, Room room, String command) {
        try {
            player.println("Comando nao existente.");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return true;
    }
}
