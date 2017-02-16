/**
 * Created by gopia on 16/02/2017.
 */
public class Robot {
    public int id;
    public String type;
    public int[] startingPose;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public int[] getStartingPose() {
        return startingPose;
    }

    public void setStartingPose(int[] startingPose) {
        this.startingPose = startingPose;
    }
}
