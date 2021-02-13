import java.util.ArrayList;

public class CengTree
{
    public CengTreeNode root;
    // Any extra attributes...

    public CengTree(Integer order)
    {
        CengTreeNode.order = order;
        // TODO: Initialize the class

        root = new CengTreeNodeLeaf(null);
    }

    public void addVideo(CengVideo video)
    {
        // TODO: Insert Video to Tree

        root.addVideo(video);

        if (root.getSize() > 2*CengTreeNode.order) {
            root = makeParent(root, root.split());
        }
    }

    public ArrayList<CengTreeNode> searchVideo(Integer key)
    {
        // TODO: Search within whole Tree, return visited nodes.
        // Return null if not found.

        ArrayList<CengTreeNode> array = root.search(key);

        if (array != null) {
            int i = 0;
            for (CengTreeNode node: array)
                node.print(i++, key);
        } else
            System.out.println("Could not find " + key + ".");

        return array;
    }

    public void printTree()
    {
        // TODO: Print the whole tree to console

        root.printAll(0);
    }

    // Any extra functions...

    public CengTreeNode makeParent(CengTreeNode n1, ArrayList<Object> arr) {
        CengTreeNodeInternal newNode = new CengTreeNodeInternal(null);
        newNode.addNode(n1, arr);

        return newNode;
    }
}
