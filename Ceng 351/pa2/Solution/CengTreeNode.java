import java.awt.Color;
import java.util.ArrayList;

public abstract class CengTreeNode
{
    static protected Integer order;
    private CengTreeNode parent;
    protected CengNodeType type; // Type needs to be set for proper GUI. Check CengNodeType.java.

    // GUI Accessors - do not modify
    public Integer level;
    public Color color;

    // Any extra attributes, if necessary

    public CengTreeNode(CengTreeNode parent)
    {
        this.parent = parent;

        this.color = CengGUI.getRandomBorderColor();

        // TODO: Extra initializations, if necessary.
    }

    // Getters-Setters - Do not modify
    public CengTreeNode getParent()
    {
        return parent;
    }

    public void setParent(CengTreeNode parent)
    {
        this.parent = parent;
    }

    public CengNodeType getType()
    {
        return type;
    }

    // GUI Methods - Do not modify
    public Color getColor()
    {
        return this.color;
    }

    // Extra Functions

    public void printTab(int n) {
        for (int i = 0; i < n; i++)
            System.out.print("\t");
    }

    public abstract void addVideo(CengVideo video);
    public abstract int getSize();
    // this arraylist of objects behaves like Pair<CengTreeNode, Integer>
    public abstract ArrayList<Object> split();

    public abstract void printAll(int n);

    public abstract ArrayList<CengTreeNode> search(int key);
    public abstract void print(int n, int key);
}
