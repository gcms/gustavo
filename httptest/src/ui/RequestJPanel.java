package ui;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.ListModel;

public class RequestJPanel extends JPanel {
    private JList requestJList;

    private JButton addJButton;

    private JButton removeJButton;

    private JButton downJButton;

    private JButton upJButton;

    private SimpleList simpleList;

    private ListModel listModel;

    private ObjectFactory objectFactory;

    public RequestJPanel(SimpleList list, ObjectFactory factory, ListModel model) {
        requestJList = new JList();

        setSimpleList(list);
        setListModel(model);
        setObjectFactory(factory);

        JScrollPane scrolledPane = new JScrollPane(requestJList);

        /* -- packing buttons start -- */
        JPanel buttonPane = new JPanel(new GridLayout(4, 1, 0, 5));

        addJButton = new JButton("Adicionar");
        addJButton.setMnemonic(KeyEvent.VK_D);
        addJButton.addActionListener(new AddItemActionListener());
        buttonPane.add(addJButton);

        removeJButton = new JButton("Remover");
        removeJButton.setMnemonic(KeyEvent.VK_R);
        removeJButton.addActionListener(new RemoveItemActionListener());
        buttonPane.add(removeJButton);

        upJButton = new JButton("Cima");
        upJButton.setMnemonic(KeyEvent.VK_I);
        upJButton.addActionListener(new MoveItemActionListener(-1));
        buttonPane.add(upJButton);

        downJButton = new JButton("Baixo");
        downJButton.setMnemonic(KeyEvent.VK_B);
        downJButton.addActionListener(new MoveItemActionListener(1));
        buttonPane.add(downJButton);
        /* -- packing buttons end -- */

        setLayout(new BorderLayout());

        JPanel buttonPaneContainer = new JPanel();
        buttonPaneContainer.add(buttonPane);

        add(scrolledPane, BorderLayout.CENTER);
        add(buttonPaneContainer, BorderLayout.LINE_END);
    }

    public void setListModel(ListModel listModel) {
        this.listModel = listModel;
        this.requestJList.setModel(listModel);
    }

    public ListModel getListModel() {
        return listModel;
    }

    public void setObjectFactory(ObjectFactory requestFactory) {
        this.objectFactory = requestFactory;
    }

    public ObjectFactory getObjectFactory() {
        return objectFactory;
    }

    public void add(Object req) {
        int index = requestJList.getSelectedIndex();
        if (index == -1) {
            simpleList.add(req);
        } else {
            simpleList.add(index, req);
        }

    }

    public void remove() {
        Object[] values = requestJList.getSelectedValues();
        for (int i = 0; i < values.length; i++) {
            simpleList.remove(values[i]);
        }
    }

    public void move(int modifier) {
        int[] indices = requestJList.getSelectedIndices();
        for (int i = 0; i < indices.length; i++) {
            int index = indices[i] + modifier;
            if (index >= 0 && index < simpleList.size()) {
                Object o = simpleList.get(indices[i]);
                simpleList.remove(indices[i]);
                simpleList.add(index, o);
                indices[i] = index;
            } else {
                indices[i] = -1;
            }
        }
        requestJList.setSelectedIndices(indices);
    }

    private class AddItemActionListener implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            Object o = objectFactory.createObject();
            if (o != null) {
                add(o);
            }
        }
    }

    private class RemoveItemActionListener implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            remove();
        }
    }

    private class MoveItemActionListener implements ActionListener {
        int modifier;

        public MoveItemActionListener(int modifier) {
            this.modifier = modifier;
        }

        public void actionPerformed(ActionEvent e) {
            move(modifier);
        }
    }

    public SimpleList getSimpleList() {
        return simpleList;
    }

    public void setSimpleList(SimpleList list) {
        this.simpleList = list;
    }
}