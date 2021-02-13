import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public class CengTreeParser
{
    public static ArrayList<CengVideo> parseVideosFromFile(String filename)
    {
        ArrayList<CengVideo> videoList = new ArrayList<CengVideo>();

        // You need to parse the input file in order to use GUI tables.
        // TODO: Parse the input file, and convert them into CengVideos

        File file = new File(filename);

        Scanner in = null;
        try {
            in = new Scanner(file);
            while (in.hasNextLine()) {
                String data = in.nextLine();
                String[] tokens = data.split("[|]");
                videoList.add(new CengVideo(Integer.parseInt(tokens[0]), tokens[1], tokens[2], tokens[3]));
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        return videoList;
    }

    public static void startParsingCommandLine()
    {
        // TODO: Start listening and parsing command line -System.in-.
        // There are 4 commands:
        // 1) quit : End the app, gracefully. Print nothing, call nothing, just break off your command line loop.
        // 2) add : Parse and create the video, and call CengVideoRunner.addVideo(newlyCreatedVideo).
        // 3) search : Parse the key, and call CengVideoRunner.searchVideo(parsedKey).
        // 4) print : Print the whole tree, call CengVideoRunner.printTree().

        // Commands (quit, add, search, print) are case-insensitive.

        Scanner in = new Scanner(System.in);

        while (true) {
            String input = in.nextLine();
            String[] tokens = input.split("[|]");

            if (tokens[0].equals("quit"))
                break;

            switch (tokens[0]) {
                case "add":
                    CengVideoRunner.addVideo(
                            new CengVideo(Integer.parseInt(tokens[1]), tokens[2], tokens[3], tokens[4]) );
                    break;
                case "search":
                    CengVideoRunner.searchVideo(Integer.parseInt(tokens[1]));
                    break;
                case "print":
                    CengVideoRunner.printTree();
                    break;
            }
        }
    }
}
