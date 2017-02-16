import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import org.omg.CORBA.BooleanHolder;
import org.omg.CORBA.StringHolder;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.swing.border.MatteBorder;
import javax.swing.event.MouseInputAdapter;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Line2D;
import java.awt.geom.Path2D;
import java.awt.geom.Rectangle2D;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

import static java.lang.Math.atan;
import static java.lang.Math.tan;
import static java.lang.Math.toDegrees;

/**
 * Created by gopia on 16/02/2017.
 */

class drawingScreen extends JPanel{

}

public class MapBuilderGui {
    public static MapBuilderGui g_map;
    private int map_width, map_height;
    private String map_title;
    private int screen_width = Toolkit.getDefaultToolkit().getScreenSize().width, screen_height = Toolkit.getDefaultToolkit().getScreenSize().height;

    private class MapSizeSelector extends JFrame {
        public JTextField height = new JTextField("5200"), width = new JTextField("6400"), title = new JTextField("Custom Map");
        public JButton button = new JButton("Okay");

        public MapSizeSelector() {
            JPanel panel =  new JPanel();
            JPanel subPanel = new JPanel();
            subPanel.setLayout(new GridLayout(4,3));
            JLabel label = new JLabel("Height");
            subPanel.add(label);
            subPanel.setVisible(true);
            subPanel.add(height);
            height.setVisible(true);
            label = new JLabel("Width");
            subPanel.add(label);
            label.setVisible(true);
            subPanel.add(width);
            width.setVisible(true);
            label = new JLabel("Title");
            subPanel.add(label);
            label.setVisible(true);
            subPanel.add(title);
            title.setVisible(true);

            subPanel.setVisible(true);
            panel.add(subPanel);
            panel.add(button);
            panel.setLayout(new GridLayout(2,1));
            this.add(panel);
            this.pack();
        }
    }

    private class MapDrawFrame extends JFrame {
        private int frame_height, frame_width;
        private double map_width_ratio,map_height_ratio;
        public Map map;
        public MapDrawFrame() {
            setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
            frame_width = screen_width/2;
            frame_height = screen_height/2;
            setSize(frame_width+30, frame_height+30);
            map_width_ratio = (float)map_width/frame_width;
            map_height_ratio = (float)map_height/frame_height;
            map = new Map();
            map.setBorder(new MatteBorder(10,10,10,10, Color.black));
            map.setVisible(true);
            JPanel superPanel = new JPanel(new GridBagLayout());
            superPanel.setBorder(new EmptyBorder(20,20,20,20));
            //this.setContentPane(superPanel);
            this.add(map);

        }

        public class Map extends JPanel implements MouseMotionListener{
            public Rectangle2D robot;
            Double robotAngle = 0.0;
            MouseInputAdapter currentAdapter;
            public LineDrawingMouseListener lineListener;
            public RobotPlacingMouseListener robotPlacer;
            Line2D currentLine;
            private int x,y;
            private ArrayList<Line2D> lines = new ArrayList<>();
            public Map() {
                this.setSize(frame_width,frame_height);
                currentAdapter = new LineDrawingMouseListener();
                lineListener = (LineDrawingMouseListener) currentAdapter;
                robotPlacer = new RobotPlacingMouseListener();
                addMouseListener(currentAdapter);
                addMouseMotionListener(currentAdapter);
            }

            public void updateListener(MouseInputAdapter adapter) {
                removeMouseListener(currentAdapter);
                removeMouseMotionListener(currentAdapter);
                currentAdapter = adapter;
                addMouseMotionListener(currentAdapter);
                addMouseListener(currentAdapter);
            }

            public void paintComponent(Graphics g) {
                g.clearRect(0,0,frame_width, frame_height);
                g.setColor(Color.black);
                System.out.println("Printing");
                for(int i = 0; i< map_width; i+= map_width/20) {
                    g.drawLine((int)(i/map_width_ratio), 0, (int)(i/map_width_ratio), frame_height);
                }
                for(int j = 0; j<map_height; j+=map_height/20) {
                    g.drawLine(0, (int)(j/map_height_ratio), frame_width, (int)(j/map_height_ratio));
                }

                for(Line2D line : lines) g.drawLine((int)line.getX1(), (int)line.getY1(), (int)line.getX2(), (int)line.getY2());
                if(currentLine != null) g.drawLine((int) currentLine.getX1(), (int)currentLine.getY1(), (int) currentLine.getX2(), (int) currentLine.getY2());

                if(robot != null) {



                    Path2D path = new Path2D.Double();
                    path.append(robot, false);
                    AffineTransform transform = new AffineTransform();
                    transform.rotate(Math.toRadians(robotAngle), robot.getX() + robot.getWidth()/2, robot.getY() + robot.getHeight()/2);
                    path.transform(transform);
                    Graphics2D g2 = (Graphics2D) g;
                    g2.draw(path);
                }

            }


            @Override
            public void mouseDragged(MouseEvent e) {

            }

            @Override
            public void mouseMoved(MouseEvent e) {

            }

            public class LineDrawingMouseListener extends MouseInputAdapter {

                @Override
                public void mousePressed(MouseEvent e) {
                    int x = e.getX();
                    int y = e.getY();
                    currentLine = new Line2D.Double(x, y, x,y);
                    repaint();
                }

                @Override
                public void mouseDragged(MouseEvent e) {
                    updateSize(e);
                }

                @Override
                public void mouseReleased(MouseEvent e) {
                    lines.add(currentLine);
                }

                private void updateSize(MouseEvent e) {
                    int x = e.getX();
                    int y = e.getY();
                    currentLine.setLine(currentLine.getX1(), currentLine.getY1(), x, y);
                    repaint();
                }
            }
            public final double RobotWidth = 100;
            public final double RobotHeight = 70;
            public class RobotPlacingMouseListener extends MouseInputAdapter {

                private State state = State.PLACING;
                public RobotPlacingMouseListener() {
                    if(robot != null) state = State.NOT_PLACING;
                }

                @Override
                public void mousePressed(MouseEvent e) {
                    System.out.println(state.toString());
                    if(state == State.PLACING) {
                        System.out.println("Placing Robot");
                        robot = new Rectangle2D.Double(e.getX()-RobotHeight/2,e.getY()-RobotWidth/2, RobotHeight, RobotWidth);
                        state = State.NOT_PLACING;
                    }
                    else if(state == State.NOT_PLACING) {
                        state = State.ROTATING;
                    }
                    else if(state == State.ROTATING) {
                        if(robot != null) if(robot.contains(e.getX(), e.getY())) {robot = null; state = State.PLACING;}
                        else state = State.NOT_PLACING;
                    }
                    repaint();
                }

                @Override
                public void mouseDragged(MouseEvent e) {
                    if( state == State.ROTATING)
                        updateSize(e);
                }

                @Override
                public void mouseReleased(MouseEvent e) {
                    repaint();
                }

                private void updateSize(MouseEvent e) {
                    double x = e.getX();
                    double y = e.getY();
                    if(robot != null) {
                        double dx = robot.getCenterX() - x;
                        double dy = robot.getCenterX() - y;
                        robotAngle = Math.toDegrees(Math.atan2(dy,dx));
                    }

                    repaint();
                }
            }

        }

    }
    class holder {
        private String end_state;
        public holder(String state) {this.end_state = state;}
        public void setState(String truth ){this.end_state = truth;}
        public String getState(){return end_state;}
    }
    public Thread end_thread;
    public MapBuilderGui() {
        MapSizeSelector selector = new MapSizeSelector();
        selector.button.addActionListener(event -> {
            map_height = Integer.parseInt(selector.height.getText());
            map_width = Integer.parseInt(selector.width.getText());
            map_title = selector.title.getText();
            selector.setVisible(false);
            selector.dispose();
            MapDrawFrame map = new MapDrawFrame();
            ExecutionDialog executionDialog = new ExecutionDialog(
                    (action) -> {map.map.updateListener(map.map.lineListener);},
                    (action)->{map.map.updateListener(map.map.robotPlacer);},
                    (action) -> {

                                StringBuilder output = new StringBuilder();
                                OutputMap final_map = new OutputMap();
                                RobotSimulator robotSimulator = new RobotSimulator();
                                int[] arenaSize = new int[]{map_width, map_height};
                                robotSimulator.setArenaSize(arenaSize);
                                Robot[] robots = new Robot[] {
                                        new Robot()
                                };
                                robots[0].setId(0);
                                robots[0].setType("ActivityBot");
                        System.out.println(map == null);
                        System.out.println(map.map == null);
                        System.out.println(map.map.robot == null);
                                robots[0].setStartingPose(new int[] {
                                        (int) ((map.map.robot.getX())*map.map_width_ratio),
                                        (int) ((map.getHeight()-map.map.robot.getY())*map.map_height_ratio),
                                        (int) map.map.robotAngle.doubleValue()
                                });
                                robotSimulator.setRobots(robots);
                                Wall[] walls = new Wall[map.map.lines.size()];
                                for(int i = 0; i<map.map.lines.size(); i++) {
                                    Line2D line = map.map.lines.get(i);
                                    walls[i] = new Wall();
                                    walls[i].setStartPoint(new int[]{
                                            (int) ((line.getX1())*map.map_width_ratio), (int) ((map.getHeight()-line.getY1())*map.map_height_ratio),
                                    });
                                    walls[i].setEndPoint(new int[] {
                                            (int) ((line.getX2())*map.map_width_ratio), (int) ((map.getHeight()-line.getY2())*map.map_height_ratio)
                                    });
                                }
                                robotSimulator.setWalls(walls);
                                final_map.setRobotsimulator(robotSimulator);

                        robotSimulator.setTitle(map_title);





                                JFileChooser fileChooser = new JFileChooser();
                                fileChooser.setDialogTitle("Specify the output file - name it .json, ya dingus!");
                                JFrame frame = new JFrame();
                                frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                                int userSelection;
                                do {
                                    userSelection = fileChooser.showSaveDialog(frame);
                                } while (userSelection != JFileChooser.APPROVE_OPTION);

                                File outputMap = fileChooser.getSelectedFile();
                                Gson gson = new Gson();
                                FileWriter writer = null;
                                try {
                                    writer = new FileWriter(outputMap);
                                    gson.toJson(final_map, writer);
                                } catch (IOException e) {
                                    frame = new JFrame();
                                    frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                                    JPanel panel = new JPanel(new GridLayout(2,1));
                                    panel.add(new JLabel("Unknown Error occured"));
                                    panel.add(new JLabel(e.toString()));
                                    frame.setVisible(true);
                                }
                                finally {
                                    try {
                                        if (writer != null) writer.close();
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                        System.exit(0);
                                    }
                                }

                                System.exit(1);

                    });

            map.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

            executionDialog.setVisible(true);
            map.setVisible(true);
        });
        EventQueue.invokeLater(()->selector.setVisible(true));

    }


    public static void main(String[] args) {
        new MapBuilderGui();

    }

    public class ExecutionDialog extends JFrame {

                public JButton doneSaveFileButton;
                public ButtonGroup options = new ButtonGroup();
                public JRadioButton drawLinesRadioButton;
                public JRadioButton placeRobotRadioButton;

                public ExecutionDialog(ActionListener drawLines, ActionListener placeRobot, ActionListener save) {
                    setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                        setSize(1000,1000);
                        JPanel panel2 = new JPanel();
                        panel2.setLayout(new BoxLayout(panel2, BoxLayout.PAGE_AXIS));
                        JLabel label = new JLabel("Kiran Gopinathan's Map Creator");
                        panel2.add(label);
                        label.setVisible(true);
                        label = new JLabel("Place open files in simulator using load map");
                        panel2.add(label);
                        label.setVisible(true);
                        panel2.add(Box.createRigidArea(new Dimension(0,5)));
                        doneSaveFileButton = new JButton("Done - Save file");
                        panel2.add(doneSaveFileButton);
                        drawLinesRadioButton = new JRadioButton("Draw Walls(Click and Drag)");
                        placeRobotRadioButton = new JRadioButton("Place Robot in map");
                        options.add(drawLinesRadioButton);
                        drawLinesRadioButton.setSelected(true);
                        options.add(placeRobotRadioButton);

                        panel2.add(drawLinesRadioButton);
                        panel2.add(placeRobotRadioButton);
                        //panel2.add();
                        doneSaveFileButton.setVisible(true);
                        drawLinesRadioButton.setVisible(true);
                        placeRobotRadioButton.setVisible(true);
                        //options.setVisible(true);

                        doneSaveFileButton.addActionListener(save);
                        placeRobotRadioButton.addActionListener(placeRobot);
                        drawLinesRadioButton.addActionListener(drawLines);

                        add(panel2);
                        this.pack();

                    panel2.setVisible(true);

                }

                public class CustomPanel extends JPanel {

                }
    }

}





