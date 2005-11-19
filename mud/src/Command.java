public interface Command {
    public boolean execute(MudServer server, Player player, Room room,
            String command);
}
