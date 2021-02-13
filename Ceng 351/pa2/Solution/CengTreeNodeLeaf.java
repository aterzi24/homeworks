import java.util.ArrayList;
import java.util.Arrays;

public class CengTreeNodeLeaf extends CengTreeNode
{
    private ArrayList<CengVideo> videos;
    // TODO: Any extra attributes

    public CengTreeNodeLeaf(CengTreeNode parent)
    {
        super(parent);

        // TODO: Extra initializations
        type = CengNodeType.Leaf;
        videos = new ArrayList<>();
    }

    // GUI Methods - Do not modify
    public int videoCount()
    {
        return videos.size();
    }
    public Integer videoKeyAtIndex(Integer index)
    {
        if(index >= this.videoCount()) {
            return -1;
        } else {
            CengVideo video = this.videos.get(index);

            return video.getKey();
        }
    }

    // Extra Functions

    @Override
    public void addVideo(CengVideo video) {
        int i;
        int key = video.getKey();
        int size = videos.size();

        for (i = 0; i < size; i++)
            if (key < videos.get(i).getKey())
                break;

        videos.add(i, video);
    }

    @Override
    public int getSize() { return videoCount(); }

    @Override
    public ArrayList<Object> split() {
        CengTreeNodeLeaf newLeaf = new CengTreeNodeLeaf(getParent());
        int size = videos.size();

        for (int i = order; i < size; i++)
            newLeaf.videos.add(videos.get(i));

        for (int i = 0; i <= order; i++) {
            size--;
            videos.remove(size);
        }

        ArrayList<Object> arr = new ArrayList<>();
        arr.add(newLeaf);
        arr.add(newLeaf.videos.get(0).getKey());
        return arr;
    }

    @Override
    public void printAll(int n) {
        printTab(n);
        System.out.println("<data>");
        for (CengVideo video: videos) {
            printTab(n);
            System.out.println("<record>"+video.fullName()+"</record>");
        }
        printTab(n);
        System.out.println("</data>");
    }

    @Override
    public ArrayList<CengTreeNode> search(int key) {
        int i;
        int size = videos.size();

        for (i = 0; i < size; i++)
            if (key == videos.get(i).getKey()) {
                ArrayList<CengTreeNode> arr = new ArrayList<>();
                arr.add(this);
                return arr;
            }
        return null;
    }

    @Override
    public void print(int n, int key) {
        printTab(n);
        for (CengVideo video: videos)
            if (video.getKey() == key) {
                System.out.println("<record>" + video.fullName() + "</record>");
                break;
            }
    }
}
