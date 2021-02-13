import java.util.ArrayList;

public class CengTreeNodeInternal extends CengTreeNode
{
    private ArrayList<Integer> keys;
    private ArrayList<CengTreeNode> children;

    public CengTreeNodeInternal(CengTreeNode parent)
    {
        super(parent);

        // TODO: Extra initializations, if necessary.
        type = CengNodeType.Internal;
        keys = new ArrayList<>();
        children = new ArrayList<>();
    }

    // GUI Methods - Do not modify
    public ArrayList<CengTreeNode> getAllChildren()
    {
        return this.children;
    }
    public Integer keyCount()
    {
        return this.keys.size();
    }
    public Integer keyAtIndex(Integer index)
    {
        if(index >= this.keyCount() || index < 0)
        {
            return -1;
        }
        else
        {
            return this.keys.get(index);
        }
    }

    // Extra Functions

    public void addNode(CengTreeNode n1, ArrayList<Object> arr) {
        children.add(n1);
        children.add((CengTreeNode) arr.get(0));

        keys.add((int)arr.get(1));
    }

    @Override
    public void addVideo(CengVideo video) {
        int i;
        int key = video.getKey();
        int size = keys.size();

        for (i = 0; i < size; i++)
            if (key < keys.get(i))
                break;

        CengTreeNode node = children.get(i);
        node.addVideo(video);

        if (node.getSize() > 2*CengTreeNode.order) {
            ArrayList<Object> arr = node.split();
            children.add(i+1, (CengTreeNode) arr.get(0));
            keys.add(i, (int)arr.get(1));
        }
    }

    @Override
    public int getSize() { return keyCount(); }

    @Override
    public ArrayList<Object> split() {
        CengTreeNodeInternal newNode = new CengTreeNodeInternal(getParent());
        int size = keys.size();

        for (int i = order+1; i < size; i++) {
            newNode.keys.add(keys.get(i));
            newNode.children.add(children.get(i));
        }
        newNode.children.add(children.get(size));

        ArrayList<Object> arr = new ArrayList<>();
        arr.add(newNode);
        arr.add(keys.get(order));

        for (int i = 0; i <= order; i++) {
            keys.remove((int)order);
            children.remove(order+1);
        }

        return arr;
    }

    @Override
    public void printAll(int n) {
        print(n, 0);

        for (CengTreeNode node: children)
            node.printAll(n+1);
    }

    @Override
    public ArrayList<CengTreeNode> search(int key) {
        int i;
        int size = keys.size();

        for (i = 0; i < size; i++)
            if (key < keys.get(i))
                break;

        ArrayList<CengTreeNode> arr = children.get(i).search(key);
        if (arr == null)
            return null;

        arr.add(0, this);
        return arr;
    }

    @Override
    public void print(int n, int key) {
        printTab(n);
        System.out.println("<index>");
        for (int i: keys) {
            printTab(n);
            System.out.println(i);
        }
        printTab(n);
        System.out.println("</index>");
    }
}
