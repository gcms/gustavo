import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.net.MalformedURLException;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;

import com.caucho.hessian.client.HessianProxyFactory;
import com.caucho.hessian.io.HessianProtocolException;

public class DictionaryJFrame extends JFrame {

    private JButton button;

    private JTextPane textPane;

    private JTextField textField;

    private Dictionary dic;

    public static void main(String args[]) {
        try {
            DictionaryJFrame frame = new DictionaryJFrame();
            frame.setVisible(true);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void search() {
        button.setEnabled(false);
        textField.setEnabled(false);

        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                String word = textField.getText();

                try {

                    if (word != null && word.length() > 0) {
                        String translation = dic.translate(word);
                        if (translation.matches("^\\s*$")) {
                            textPane.setText("Translation unavailable");
                        } else {
                            textPane.setText(translation);
                        }
                    }
                } finally {
                    button.setEnabled(true);
                    textField.setEnabled(true);
                    textField.grabFocus();
                    textField.setSelectionStart(0);
                    textField.setSelectionEnd(word.length());
                }
            }
        });
    }

    public DictionaryJFrame() throws MalformedURLException {
        super();
        // setBounds(100, 100, 500, 375);

        setTitle("Tradutor");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        {
            HessianProxyFactory factory = new HessianProxyFactory();
            factory.setOverloadEnabled(true);
            dic = (Dictionary) factory.create(Dictionary.class,
                    "http://software.inf.ufg.br:8080/hessiantest/dictionary");
        }
        {
            final JPanel panel = new JPanel();
            panel.setLayout(new FlowLayout());
            getContentPane().add(panel, BorderLayout.NORTH);
            {
                textField = new JTextField();
                textField.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        search();
                    }
                });
                textField.setColumns(20);
                panel.add(textField);
            }
            {
                button = new JButton();
                button.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        search();
                    }
                });
                button.setText("Traduzir");
                button.setMnemonic(KeyEvent.VK_T);
                panel.add(button);
            }
        }
        {
            final JScrollPane scrollPane = new JScrollPane();
            getContentPane().add(scrollPane, BorderLayout.CENTER);
            {
                textPane = new JTextPane();
                textPane.setPreferredSize(new Dimension(200, 200));
                textPane.setEditable(false);
                scrollPane.setViewportView(textPane);
            }
        }
        //
        pack();
    }

}
interface Dictionary {
    public String[] words();
    public String[] words(int num);
    public String translate(String word);
}

