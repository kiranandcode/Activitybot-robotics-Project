/**
 * Created by gopia on 16/02/2017.
 */
public class RobotSimulator {
    public String title;
    public int[] arenaSize;
    public Robot[] robots;

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public int[] getArenaSize() {
        return arenaSize;
    }

    public void setArenaSize(int[] arenaSize) {
        this.arenaSize = arenaSize;
    }

    public Robot[] getRobots() {
        return robots;
    }

    public void setRobots(Robot[] robots) {
        this.robots = robots;
    }

    public Wall[] getWalls() {
        return walls;
    }

    public void setWalls(Wall[] walls) {
        this.walls = walls;
    }

    public Wall[] walls;
}
