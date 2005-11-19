public class RegularExit implements Exit {
    private Room room;

    public RegularExit(Room nextRoom) {
        this.room = nextRoom;
    }

    public Room getRoom() {
        return room;
    }

}
