import javax.swing.*;
import java.awt.*;
import java.lang.reflect.InvocationTargetException;

/**
 * Created by gopia on 16/02/2017.
 */
public class GetInput {
    class holder {
        private boolean truth;
        public void setTruth(boolean truth ){this.truth = truth;}
        public boolean getTruth(){return truth;}
    }

    StringBuilder output = new StringBuilder();
    volatile holder h = new holder();
    public void getInput() {

    h.setTruth(false);
    JFrame frame = new JFrame();
    JPanel panel = new JPanel();
    JLabel label = new JLabel("Enter the file name:");
    JTextField textField = new JTextField("Kiran-Custom-Map");
    JButton button = new JButton("Okay");
        button.addActionListener((press) -> {
        output.append(textField.getText());
        h.setTruth(true);
            frame.setVisible(false);
    });

                        panel.add(textField);
                        panel.add(label);
                        panel.add(button);
                        frame.add(panel);
                        frame.pack();
                        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

    final JFrame altFrame = frame;
                        label.setVisible(true);
                        textField.setVisible(true);
                        button.setVisible(true);
                        panel.setVisible(true);

            altFrame.setVisible(true);

    }

    public boolean retrievedValues() {return h.getTruth();}

    public String getString() {while(!this.retrievedValues());return output.toString();}


}
