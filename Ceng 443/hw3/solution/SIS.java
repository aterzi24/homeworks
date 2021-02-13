import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

public class SIS {
    private static String fileSep = File.separator;
    private static String lineSep = System.lineSeparator();
    private static String space   = " ";

    private List<Student> studentList = new ArrayList<>();

    public SIS(){ processOptics(); }

    private void processOptics(){
        // TODO
        try {
            // get all files under input folder as stream
            Stream<Path> paths = Files.list(Paths.get("." + fileSep + "input"));

            paths.forEach(path -> {
                try {
                    // for each file, read them line by line
                    Scanner in = new Scanner(new File(String.valueOf(path)));
                    String[] line1 = in.nextLine().split(space);
                    String[] line2 = in.nextLine().split(space);
                    String line3 = in.nextLine();
                    String line4 = in.nextLine();

                    int size = line1.length;
                    // get studentID from first line
                    int studentID = Integer.parseInt(line1[size-1]);

                    // get necessary information from second line
                    int year = Integer.parseInt(line2[0]);
                    int courseCode = Integer.parseInt(line2[1]);
                    int credit = Integer.parseInt(line2[2]);

                    // calculate grade by using the information on last line
                    IntStream AnswerStream = line4.chars();
                    long trueCount = AnswerStream.filter(e -> e == 'T').count();
                    AnswerStream = line4.chars();
                    long totalCount = AnswerStream.count();
                    double grade = (100.0 * trueCount) / totalCount;

                    // create new course
                    Course course = new Course(courseCode, year, line3, credit, grade);

                    // check whether student with studentID was added to studentList before
                    Student student = studentList.stream().filter(s -> s.getStudentID() == studentID)
                            .findFirst().orElse(null);

                    // if not found, then create new student with given course and add to the studentList
                    if (student == null) {
                        String[] names = Arrays.copyOfRange(line1, 0, size-2);
                        String surname = line1[size-2];

                        student = new Student(names, surname, studentID);
                        student.getTakenCourses().add(course);
                        studentList.add(student);
                    }
                    // if found, then add course to takenCourse list
                    else {
                        student.getTakenCourses().add(course);
                    }

                } catch (FileNotFoundException fileNotFoundException) {
                    fileNotFoundException.printStackTrace();
                }
            });
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public double getGrade(int studentID, int courseCode, int year){
        // TODO
        // check if student with studentID is present
        Student student = studentList.stream().filter(s -> s.getStudentID() == studentID).findFirst().orElse(null);
        // if not return -1
        if (student == null)
            return -1;

        // only get appropriate courses from the student's takenCourse list
        List<Course> course = student.getTakenCourses().stream()
                .filter(c -> c.getCourseCode() == courseCode && c.getYear() == year).collect(Collectors.toList());

        // get course objects with exam grades
        Course m1C = course.stream().filter(c -> c.getExamType().equals("Midterm1")).findFirst().orElse(null);
        Course m2C = course.stream().filter(c -> c.getExamType().equals("Midterm2")).findFirst().orElse(null);
        Course finalC = course.stream().filter(c -> c.getExamType().equals("Final")).findFirst().orElse(null);

        // calculate average grade and return
        double m1G = 0.0;
        double m2G = 0.0;
        double finalG = 0.0;

        if (m1C != null)
            m1G = m1C.getGrade();
        if (m2C != null)
            m2G = m2C.getGrade();
        if (finalC != null)
            finalG = finalC.getGrade();
        return 0.25 * (m1G + m2G) + 0.5 * finalG;
    }

    public void updateExam(int studentID, int courseCode, String examType, double newGrade){
        // TODO
        // get student
        Student student = studentList.stream().filter(s -> s.getStudentID() == studentID).findFirst().orElse(null);
        if (student == null)
            return;

        // find correct course and update the grade of the most recent year
        student.getTakenCourses().stream()
                .filter(c -> c.getCourseCode() == courseCode && c.getExamType().equals(examType))
                .max(Comparator.comparingInt(Course::getYear)).ifPresent(c -> c.setGrade(newGrade));
    }

    public void createTranscript(int studentID){
        // TODO
        // get student
        Student student = studentList.stream().filter(s -> s.getStudentID() == studentID).findFirst().orElse(null);
        if (student == null)
            return;

        // group courses according to their years
        Map<Integer, List<Course>> map = student.getTakenCourses().stream()
                .collect(Collectors.groupingBy(Course::getYear));

        // sort the keySet list in ascendant order
        map.keySet().stream().sorted().forEach(year -> {
            System.out.println(year);
            // get all distinct sorted courses for the specific year
            map.get(year).stream().map(Course::getCourseCode).distinct().sorted().forEach(cc -> {
                // calculate the grade
                double grade = getGrade(studentID, cc, year);
                System.out.print(cc);
                String letterGrade;
                // determine letter grade and print
                if (grade >= 89.5)
                    letterGrade = "AA";
                else if (grade >= 84.5)
                    letterGrade = "BA";
                else if (grade >= 79.5)
                    letterGrade = "BB";
                else if (grade >= 74.5)
                    letterGrade = "CB";
                else if (grade >= 69.5)
                    letterGrade = "CC";
                else if (grade >= 64.5)
                    letterGrade = "DC";
                else if (grade >= 59.5)
                    letterGrade = "DD";
                else if (grade >= 49.5)
                    letterGrade = "FD";
                else
                    letterGrade = "FF";
                System.out.println(space + letterGrade);
            });
        });

        List<Integer> calculatedCourses = new ArrayList<>();
        List<Double> grades = new ArrayList<>();
        List<Integer> credits = new ArrayList<>();

        // sort keySet list in descendant order
        map.keySet().stream().sorted((y1,y2) -> y2 - y1).forEach(year ->
                // check whether this course calculated before by searching in calculatedCourses list
                map.get(year).stream().map(Course::getCourseCode).distinct().forEach(cc -> {
                    Integer courseCode = calculatedCourses.stream().filter(s -> s.equals(cc))
                            .findFirst().orElse(null);
                    // if not added before, then
                    if (courseCode == null) {
                        // add courseCode to list
                        calculatedCourses.add(cc);
                        // find course information with the courseCode
                        Course course = student.getTakenCourses().stream()
                                .filter(c -> c.getCourseCode() == cc).findFirst().orElse(null);
                        // calculate grade
                        double grade = getGrade(studentID, cc, year);
                        double point;
                        if (grade >= 89.5)
                            point = 4.0;
                        else if (grade >= 84.5)
                            point = 3.5;
                        else if (grade >= 79.5)
                            point = 3.0;
                        else if (grade >= 74.5)
                            point = 2.5;
                        else if (grade >= 69.5)
                            point = 2.0;
                        else if (grade >= 64.5)
                            point = 1.5;
                        else if (grade >= 59.5)
                            point = 1.0;
                        else if (grade >= 49.5)
                            point = 0.5;
                        else
                            point = 0.0;

                        // add weighted grade to grades list
                        grades.add(point * course.getCredit());
                        // add credit to credits list
                        credits.add(course.getCredit());
                    }
                })
        );

        // calculate gpa
        double totalGrade = grades.stream().reduce(0.0, Double::sum);
        double totalCredit = credits.stream().reduce(0, Integer::sum);
        System.out.printf("CGPA: %.2f%n", (totalGrade/totalCredit));
    }

    public void findCourse(int courseCode){
        // TODO
        List<Integer> years = new ArrayList<>();
        // add all years when this course was given to the list
        studentList.forEach(s ->
                s.getTakenCourses().forEach(c -> {
                    if (c.getCourseCode() == courseCode)
                        years.add(c.getYear());
            })
        );

        List<Integer> studentIDs = new ArrayList<>();

        // for each year, find all students taken this course
        years.stream().distinct().sorted().forEach(year -> {
            studentList.forEach(s -> s.getTakenCourses().stream()
                    .filter(c -> c.getCourseCode() == courseCode && c.getYear() == year)
                    .findFirst().ifPresent(c -> studentIDs.add(s.getStudentID())));
            // print how many student took this course, then clear list for the next iteration
            System.out.println(year + space + (long) studentIDs.size());
            studentIDs.clear();
        });
    }

    public void createHistogram(int courseCode, int year){
        // TODO
        List<Student> students = new ArrayList<>();
        // find all students taken this course at given year
        studentList.forEach(s -> s.getTakenCourses().stream()
                .filter(c -> c.getCourseCode() == courseCode && c.getYear() == year)
                .findFirst().ifPresent(c -> students.add(s)));

        List<Integer> num = Arrays.asList(0, 10, 20, 30, 40, 50, 60, 70, 80);
        List<Student> count = new ArrayList<>();

        // this is a loop i = 0 to 80
        num.forEach(i -> {
            // find how many student get grade between [i and i+10)
            // if one did not take course at that year then it return -1, so it is never counted
            students.forEach(s -> {
                double grade = getGrade(s.getStudentID(), courseCode, year);
                if ((double)i <= grade && grade < (i+10.0))
                    count.add(s);
            });
            // print and clear list
            System.out.println(i + "-" + (i+10) + space + (long) count.size());
            count.clear();
        });

        // this one is special because 100 is included
        students.forEach(s -> {
            double grade = getGrade(s.getStudentID(), courseCode, year);
            if (90.0 <= grade && grade <= 100.0)
                count.add(s);
        });
        System.out.println("90-100 " + (long) count.size());
    }
}