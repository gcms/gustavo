import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class Player {
    private OutputStream out;

    private Room currentRoom;

    public Player(OutputStream out) {
        this(out, null);
    }

    public Player(OutputStream out, Room room) {
        this.out = out;
        currentRoom = room;
    }

    public Room getRoom() {
        return currentRoom;
    }

    public void setRoom(Room room) {
        currentRoom = room;
    }

    public void println(String str) throws IOException {
        DataOutputStream dout = new DataOutputStream(out);

        dout.writeBytes(str);
        dout.writeBytes("\r\n");
    }

    public void joinRoom(Room newRoom) {
        currentRoom.removePlayer(this);
        newRoom.addPlayer(this);
        setRoom(newRoom);
    }

}
