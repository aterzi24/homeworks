package ceng.ceng351.cengtubedb;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class CengTubeDB implements ICengTubeDB{
	private Connection connection = null;

	@Override
	public void initialize() {
		final String url = "jdbc:mysql://144.122.71.168:3306/db216735?useSSL=false";
		final String user = "";
		final String password = "";
		try {
			connection = DriverManager.getConnection(url, user, password);
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
	}

	@Override
	public int createTables() {
		int count = 0;
		final String userTable = "create table User (" +
				"userID int, " +
				"userName varchar(30), " +
				"email varchar(30), " +
				"password varchar(30), " +
				"status varchar(15), " +
				"primary key (userID) )";

		final String videoTable = "create table Video (" +
				"videoID int, " +
				"userID int, " +
				"videoTitle varchar(60), " +
				"likeCount int, " +
				"dislikeCount int, " +
				"datePublished date, " +
				"primary key (videoID), " +
				"foreign key (userID) references User(userID) on delete cascade )";

		final String commentTable = "create table Comment (" +
				"commentID int, " +
				"userID int, " +
				"videoID int, " +
				"commentText varchar(1000), " +
				"dateCommented date, " +
				"primary key (commentID), " +
				"foreign key (userID) references User(userID) on delete set null, " +
				"foreign key (videoID) references Video(videoID) on delete cascade )";

		final String watchTable = "create table Watch (" +
				"userID int, " +
				"videoID int, " +
				"dateWatched date, " +
				"primary key (userID, videoID), " +
				"foreign key (userID) references User(userID) on delete cascade, " +
				"foreign key (videoID) references Video(videoID) on delete cascade )";

		try (Statement statement = connection.createStatement()){
			try {
				statement.executeUpdate(userTable);
				count++;
			} catch (SQLException throwables) {
				throwables.printStackTrace();
			}
			try {
				statement.executeUpdate(videoTable);
				count++;
			} catch (SQLException throwables) {
				throwables.printStackTrace();
			}
			try {
				statement.executeUpdate(commentTable);
				count++;
			} catch (SQLException throwables) {
				throwables.printStackTrace();
			}
			try {
				statement.executeUpdate(watchTable);
				count++;
			} catch (SQLException throwables) {
				throwables.printStackTrace();
			}
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		return count;
	}

	@Override
	public int dropTables() {
		int count = 0;
		final String str1 = "drop table Watch";
		final String str2 = "drop table Comment";
		final String str3 = "drop table Video";
		final String str4 = "drop table User";
		try (Statement statement = connection.createStatement()){
			try {
				statement.executeUpdate(str1);
				count++;
			} catch (SQLException throwables) {
				throwables.printStackTrace();
			}
			try {
				statement.executeUpdate(str2);
				count++;
			} catch (SQLException throwables) {
				throwables.printStackTrace();
			}
			try {
				statement.executeUpdate(str3);
				count++;
			} catch (SQLException throwables) {
				throwables.printStackTrace();
			}
			try {
				statement.executeUpdate(str4);
				count++;
			} catch (SQLException throwables) {
				throwables.printStackTrace();
			}
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		} finally {
			try {
				connection.close();
			} catch (SQLException throwables) {
				throwables.printStackTrace();
			}
		}
		return count;
	}

	@Override
	public int insertUser(User[] users) {
		int count = 0;
		final String str = "insert into User values(?, ?, ?, ?, ?)";
		try (PreparedStatement preparedStatement = connection.prepareStatement(str)) {
			for (User user : users) {
				preparedStatement.setInt(1, user.getUserID());
				preparedStatement.setString(2, user.getUserName());
				preparedStatement.setString(3, user.getEmail());
				preparedStatement.setString(4, user.getPassword());
				preparedStatement.setString(5, user.getStatus());
				try {
					count += preparedStatement.executeUpdate();
				} catch (SQLException throwables) {
					throwables.printStackTrace();
				}
			}
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		return count;
	}

	@Override
	public int insertVideo(Video[] videos) {
		int count = 0;
		final String str = "insert into Video values(?, ?, ?, ?, ?, ?)";
		try (PreparedStatement preparedStatement = connection.prepareStatement(str)) {
			for (Video video : videos) {
				preparedStatement.setInt(1, video.getVideoID());
				preparedStatement.setInt(2, video.getUserID());
				preparedStatement.setString(3, video.getVideoTitle());
				preparedStatement.setInt(4, video.getLikeCount());
				preparedStatement.setInt(5, video.getDislikeCount());
				preparedStatement.setString(6, video.getDatePublished());
				try {
					count += preparedStatement.executeUpdate();
				} catch (SQLException throwables) {
					throwables.printStackTrace();
				}
			}
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		return count;
	}

	@Override
	public int insertComment(Comment[] comments) {
		int count = 0;
		final String str = "insert into Comment values(?, ?, ?, ?, ?)";
		try (PreparedStatement preparedStatement = connection.prepareStatement(str)) {
			for (Comment comment : comments) {
				preparedStatement.setInt(1, comment.getCommentID());
				preparedStatement.setInt(2, comment.getUserID());
				preparedStatement.setInt(3, comment.getVideoID());
				preparedStatement.setString(4, comment.getCommentText());
				preparedStatement.setString(5, comment.getDateCommented());
				try {
					count += preparedStatement.executeUpdate();
				} catch (SQLException throwables) {
					throwables.printStackTrace();
				}
			}
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		return count;
	}

	@Override
	public int insertWatch(Watch[] watchEntries) {
		int count = 0;
		final String str = "insert into Watch values(?, ?, ?)";
		try (PreparedStatement preparedStatement = connection.prepareStatement(str)) {
			for (Watch watch : watchEntries) {
				preparedStatement.setInt(1, watch.getUserID());
				preparedStatement.setInt(2, watch.getVideoID());
				preparedStatement.setString(3, watch.getDateWatched());
				try {
					count += preparedStatement.executeUpdate();
				} catch (SQLException throwables) {
					throwables.printStackTrace();
				}
			}
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		return count;
	}

	@Override
	public QueryResult.VideoTitleLikeCountDislikeCountResult[] question3() {
		final String sql = "select videoTitle, likeCount, dislikeCount from Video where likeCount > dislikeCount " +
				"order by videoTitle asc";
		List<QueryResult.VideoTitleLikeCountDislikeCountResult> list = new ArrayList<>();
		try (Statement statement = connection.createStatement()) {
			ResultSet resultSet = statement.executeQuery(sql);
			while (resultSet.next()) {
				String videoTitle = resultSet.getString("videoTitle");
				int likeCount = resultSet.getInt("likeCount");
				int dislikeCount = resultSet.getInt("dislikeCount");
				list.add(new QueryResult.VideoTitleLikeCountDislikeCountResult(videoTitle, likeCount, dislikeCount));
			}
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		QueryResult.VideoTitleLikeCountDislikeCountResult[] array = new
				QueryResult.VideoTitleLikeCountDislikeCountResult[list.size()];
		array = list.toArray(array);
		return array;
	}

	@Override
	public QueryResult.VideoTitleUserNameCommentTextResult[] question4(Integer userID) {
		final String sql = "select V.videoTitle, U.userName, C.commentText from Video V, User U, Comment C " +
				"where U.userID = ? and C.userID = U.userID and V.videoID = C.videoID order by V.videoTitle asc";
		List<QueryResult.VideoTitleUserNameCommentTextResult> list = new ArrayList<>();
		try (PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
			preparedStatement.setInt(1, userID);
			ResultSet resultSet = preparedStatement.executeQuery();
			while (resultSet.next()) {
				String videoTitle = resultSet.getString("videoTitle");
				String userName = resultSet.getString("userName");
				String commentText = resultSet.getString("commentText");
				list.add(new QueryResult.VideoTitleUserNameCommentTextResult(videoTitle, userName, commentText));
			}
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		QueryResult.VideoTitleUserNameCommentTextResult[] array = new
				QueryResult.VideoTitleUserNameCommentTextResult[list.size()];
		array = list.toArray(array);
		return array;
	}

	@Override
	public QueryResult.VideoTitleUserNameDatePublishedResult[] question5(Integer userID) {
		final String sql = "select Temp.videoTitle, U.userName, Temp.datePublished from User U, " +
				"(select * from Video where videoTitle not like '%VLOG%') as Temp " +
				"where U.userID = ? and U.userID = Temp.userID and Temp.datePublished = " +
				"(select min(Temp.datePublished) from (select * from Video where videoTitle not like '%VLOG%') " +
				"as Temp where Temp.userID = U.userID)" +
				"order by Temp.videoTitle asc";
		List<QueryResult.VideoTitleUserNameDatePublishedResult> list = new ArrayList<>();
		try (PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
			preparedStatement.setInt(1, userID);
			ResultSet resultSet = preparedStatement.executeQuery();
			resultSet.next();
			String videoTitle = resultSet.getString("videoTitle");
			String userName = resultSet.getString("userName");
			String datePublished = resultSet.getDate("datePublished").toString();
			list.add(new QueryResult.VideoTitleUserNameDatePublishedResult(videoTitle, userName, datePublished));
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		QueryResult.VideoTitleUserNameDatePublishedResult[] array = new
				QueryResult.VideoTitleUserNameDatePublishedResult[list.size()];
		array = list.toArray(array);
		return array;
	}

	@Override
	public QueryResult.VideoTitleUserNameNumOfWatchResult[] question6(String dateStart, String dateEnd) {
		final String sql = "select V.videoTitle, U.userName, count(*) as numOfWatch from User U, Video V, Watch W " +
				"where V.userID = U.userID and V.videoID = W.videoID and ? <= W.dateWatched and W.dateWatched <= ? " +
				"group by V.videoID, V.videoTitle, U.userName " +
				"order by numOfWatch desc limit 3";
		List<QueryResult.VideoTitleUserNameNumOfWatchResult> list = new ArrayList<>();
		try (PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
			preparedStatement.setDate(1, Date.valueOf(dateStart));
			preparedStatement.setDate(2, Date.valueOf(dateEnd));
			ResultSet resultSet = preparedStatement.executeQuery();
			while (resultSet.next()) {
				String videoTitle = resultSet.getString("videoTitle");
				String userName = resultSet.getString("userName");
				int numOfWatch = resultSet.getInt("numOfWatch");
				list.add(new QueryResult.VideoTitleUserNameNumOfWatchResult(videoTitle, userName, numOfWatch));
			}
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		QueryResult.VideoTitleUserNameNumOfWatchResult[] array = new
				QueryResult.VideoTitleUserNameNumOfWatchResult[list.size()];
		array = list.toArray(array);
		return array;
	}

	@Override
	public QueryResult.UserIDUserNameNumOfVideosWatchedResult[] question7() {
		final String sql = "select U.userID, U.userName, count(*) as numOfVideosWatched from User U, Watch W, " +
				"(select videoID from Watch group by videoID having count(*) = 1) as Temp " +
				"where U.userID = W.userID and W.videoID = Temp.videoID group by U.userID, U.userName " +
				"order by U.userID asc";
		List<QueryResult.UserIDUserNameNumOfVideosWatchedResult> list = new ArrayList<>();
		try (Statement statement = connection.createStatement()) {
			ResultSet resultSet = statement.executeQuery(sql);
			while (resultSet.next()) {
				int userID = resultSet.getInt("userID");
				String userName = resultSet.getString("userName");
				int numOfVideosWatched = resultSet.getInt("numOfVideosWatched");
				list.add(new QueryResult.UserIDUserNameNumOfVideosWatchedResult(userID, userName, numOfVideosWatched));
			}
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		QueryResult.UserIDUserNameNumOfVideosWatchedResult[] array = new
				QueryResult.UserIDUserNameNumOfVideosWatchedResult[list.size()];
		array = list.toArray(array);
		return array;
	}

	@Override
	public QueryResult.UserIDUserNameEmailResult[] question8() {
		final String sql = "select U.userID, U.userName, U.email from User U " +
				"where not exists (select * from Video V where U.userID = V.userID and V.videoID not in " +
				"(select C.videoID from Comment C where U.userID = C.userID)) and not exists (select * " +
				"from Video V where U.userID = V.userID and V.videoID not in (select W.videoID from Watch W " +
				"where U.userID = W.userID)) and exists (select * from Video V where U.userID = V.userID) " +
				"order by U.userID asc";
		List<QueryResult.UserIDUserNameEmailResult> list = new ArrayList<>();
		try (Statement statement = connection.createStatement()) {
			ResultSet resultSet = statement.executeQuery(sql);
			while (resultSet.next()) {
				int userID = resultSet.getInt("userID");
				String userName = resultSet.getString("userName");
				String email = resultSet.getString("email");
				list.add(new QueryResult.UserIDUserNameEmailResult(userID, userName, email));
			}
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		QueryResult.UserIDUserNameEmailResult[] array = new
				QueryResult.UserIDUserNameEmailResult[list.size()];
		array = list.toArray(array);
		return array;
	}

	@Override
	public QueryResult.UserIDUserNameEmailResult[] question9() {
		final String sql = "select U.userID, U.userName, U.email from User U " +
				"where exists (select * from Watch W where U.userID = W.userID) and not exists (select * from " +
				"Comment C where U.userID = C.userID) order by U.userID asc";
		List<QueryResult.UserIDUserNameEmailResult> list = new ArrayList<>();
		try (Statement statement = connection.createStatement()) {
			ResultSet resultSet = statement.executeQuery(sql);
			while (resultSet.next()) {
				int userID = resultSet.getInt("userID");
				String userName = resultSet.getString("userName");
				String email = resultSet.getString("email");
				list.add(new QueryResult.UserIDUserNameEmailResult(userID, userName, email));
			}
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		QueryResult.UserIDUserNameEmailResult[] array = new
				QueryResult.UserIDUserNameEmailResult[list.size()];
		array = list.toArray(array);
		return array;
	}

	@Override
	public int question10(int givenViewCount) {
		int count = 0;
		final String str = "update User U set U.status = 'verified' where U.userID in (select U.userID from Video V," +
				"Watch W where U.userID = V.userID and V.videoID = W.videoID group by U.userID having count(*) > ?)";
		try (PreparedStatement preparedStatement = connection.prepareStatement(str)) {
			preparedStatement.setInt(1, givenViewCount);
			count = preparedStatement.executeUpdate();
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		return count;
	}

	@Override
	public int question11(Integer videoID, String newTitle) {
		int count = 0;
		final String str = "update Video set videoTitle = ? where videoID = ?";
		try (PreparedStatement preparedStatement = connection.prepareStatement(str)) {
			preparedStatement.setString(1, newTitle);
			preparedStatement.setInt(2, videoID);
			count = preparedStatement.executeUpdate();
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		return count;
	}

	@Override
	public int question12(String videoTitle) {
		int count = 0;
		final String str = "delete from Video where videoTitle = ?";
		final String str2 = "select count(*) as count from Video";
		try (PreparedStatement preparedStatement = connection.prepareStatement(str)) {
			preparedStatement.setString(1, videoTitle);
			preparedStatement.executeUpdate();
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		try (Statement statement = connection.createStatement()) {
			ResultSet resultSet = statement.executeQuery(str2);
			resultSet.next();
			count = resultSet.getInt("count");
		} catch (SQLException throwables) {
			throwables.printStackTrace();
		}
		return count;
	}
}
