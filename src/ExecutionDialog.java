import javax.swing.*;
import javax.swing.border.TitledBorder;
import java.awt.*;

/**
 * Created by gopia on 16/02/2017.
 */
public class ExecutionDialog {
    private JButton doneSaveFileButton;
    private JRadioButton drawLinesRadioButton;
    private JRadioButton placeRobotRadioButton;

    {
// GUI initializer generated by IntelliJ IDEA GUI Designer
// >>> IMPORTANT!! <<<
// DO NOT EDIT OR ADD ANY CODE HERE!
        $$$setupUI$$$();
    }

    /**
     * Method generated by IntelliJ IDEA GUI Designer
     * >>> IMPORTANT!! <<<
     * DO NOT edit this method OR call it in your code!
     *
     * @noinspection ALL
     */
    private void $$$setupUI$$$() {
        final JPanel panel1 = new JPanel();
        panel1.setLayout(new GridBagLayout());
        panel1.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(new Color(-16777216)), "Kiran Gopinathan's Map Builder", TitledBorder.CENTER, TitledBorder.DEFAULT_POSITION));
        final JPanel spacer1 = new JPanel();
        GridBagConstraints gbc;
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 0;
        gbc.weightx = 100.0;
        gbc.fill = GridBagConstraints.BOTH;
        panel1.add(spacer1, gbc);
        final JPanel spacer2 = new JPanel();
        gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 4;
        gbc.weightx = -1000.0;
        gbc.weighty = 10000.0;
        panel1.add(spacer2, gbc);
        doneSaveFileButton = new JButton();
        doneSaveFileButton.setText("Done(Save File)");
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 4;
        gbc.fill = GridBagConstraints.BOTH;
        panel1.add(doneSaveFileButton, gbc);
        drawLinesRadioButton = new JRadioButton();
        drawLinesRadioButton.setSelected(true);
        drawLinesRadioButton.setText("Draw Lines");
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 3;
        gbc.anchor = GridBagConstraints.WEST;
        panel1.add(drawLinesRadioButton, gbc);
        placeRobotRadioButton = new JRadioButton();
        placeRobotRadioButton.setSelected(false);
        placeRobotRadioButton.setText("Place Robot");
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 2;
        gbc.anchor = GridBagConstraints.WEST;
        panel1.add(placeRobotRadioButton, gbc);
        final JLabel label1 = new JLabel();
        label1.setText("Options Dialog");
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 1;
        panel1.add(label1, gbc);
    }
}