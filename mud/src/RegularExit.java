public class RegularExit implements Exit {
    private Room room;

    public RegularExit(Room nextRoom) {
        this.room = nextRoom;
    }

    public Room getRoom(Room r) {
        return room;
    }

    public String leaveMessage() {
        return "saiu da sala.";
    }

    public String joinMessage() {
        return "entrou na sala.";
    }

    public String ownMessage() {
        return null;
    }

}
